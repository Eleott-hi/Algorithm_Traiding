#include "main_window.h"

#include <QFileDialog>
#include <QMessageBox>

#include "ui_main_window.h"

MainWindow::MainWindow(Controller* controller)
    : QMainWindow(nullptr), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);

  plots_ = {ui_->interpolation_plot, ui_->approximation_plot,
            ui_->researchPlot};
  points_spin_boxes_ = {ui_->interpolation_points_spin_box,
                        ui_->approximation_points_spin_box,
                        ui_->research_points_spin_box};

  SetupPlots();
  
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::OnActionOpenTriggered() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "~/",
                                                  "Text files (*.csv)");

  if (filename.isEmpty()) return;

  QVector<double> keys, values;
  try {
    std::tie(keys, values) = controller_->OpenFile(filename);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not open file");
    return;
  }

  if (keys.empty() || values.empty()) return;

  for (auto spin_box : points_spin_boxes_) {
    spin_box->setMinimum(values.size());
    spin_box->setValue(values.size());
  }

  for (auto plot : plots_) {
    plot->Clear();
    if (plot == plots_.back()) break;
    plot->AddGraph("Origin", QCPScatterStyle::ssCircle);
    plot->graph()->setData(keys, values);
    plot->RescaleAndReplot();
  }

  ui_->interpolation_spline_plot_button->setEnabled(true);
  ui_->interpolation_newton_plot_button->setEnabled(true);
  ui_->approximation_plot_button->setEnabled(true);

  setWindowTitle(filename.section("/", -1) + " - " + "Algorithmic Trading");
}

void MainWindow::OnActionClearTriggered() {
  int current_tab = ui_->tabWidget->currentIndex();

  auto plot = plots_.at(current_tab);

  if (current_tab == 0) {
    plot->DeleteGraphsExceptFirst();
    ui_->interpolation_newton_plot_button->setEnabled(true);
    ui_->interpolation_spline_plot_button->setEnabled(true);

  } else if (current_tab == 1) {
    plot->DeleteGraphsExceptFirst();
    ui_->approximation_plot_button->setEnabled(true);

  } else {
    plot->Clear();
  }
}

void MainWindow::OnActionQuitTriggered() { QCoreApplication::quit(); }

void MainWindow::OnInterpolationNewtonPlotButtonClicked() {
  size_t points = ui_->interpolation_points_spin_box->value();
  size_t degree = ui_->interpolation_degree_spin_box->value();

  QVector<double> keys, values;
  try {
    std::tie(keys, values) = controller_->Newton(points, degree);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not proceed");
    return;
  }

  if (keys.empty() || values.empty()) return;

  auto& plot = ui_->interpolation_plot;
  plot->AddGraph("Newton, Degree: " + QString::number(degree));
  plot->graph()->setData(keys, values);

  if (plot->graphCount() >= 6) {
    ui_->interpolation_newton_plot_button->setEnabled(false);
    ui_->interpolation_spline_plot_button->setEnabled(false);
  }

  plot->RescaleAndReplot();
}

void MainWindow::OnInterpolationSplinePlotButtonClicked() {
  size_t points = ui_->interpolation_points_spin_box->value();
  QVector<double> keys, values;
  try {
    std::tie(keys, values) = controller_->Spline(points);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not proceed");
    return;
  }

  if (keys.empty() || values.empty()) return;

  auto& plot = ui_->interpolation_plot;
  plot->AddGraph("Spline");
  plot->graph()->setData(keys, values);

  if (plot->graphCount() >= 6) {
    ui_->interpolation_newton_plot_button->setEnabled(false);
    ui_->interpolation_spline_plot_button->setEnabled(false);
  }

  plot->RescaleAndReplot();
}

void MainWindow::OnApproximationPlotButtonClicked() {
  static size_t s_days = ui_->period_spin_box->value();

  size_t points = ui_->approximation_points_spin_box->value();
  size_t degree = ui_->approximation_degree_spin_box->value();
  size_t days = ui_->period_spin_box->value();

  QVector<double> keys, values;
  try {
    std::tie(keys, values) = controller_->Approximate(points, degree, days);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not proceed");
    return;
  }

  if (keys.empty() || values.empty()) return;

  auto& plot = ui_->approximation_plot;

  if (days != s_days) {
    plot->DeleteGraphsExceptFirst();
    s_days = days;
  }

  plot->AddGraph("Degree: " + QString::number(degree));
  plot->graph()->setData(keys, values);

  if (ui_->approximation_plot->graphCount() == 6) {
    ui_->approximation_plot_button->setEnabled(false);
  }

  plot->RescaleAndReplot();
}

void MainWindow::OnInterpolationSearchButtonClicked() {
  double date = ui_->interpolation_date_edit->dateTime().toSecsSinceEpoch();
  size_t degree = ui_->interpolation_degree_spin_box->value();

  double newton_value, spline_value;
  try {
    std::tie(newton_value, spline_value) =
        controller_->FindInterpolationValue(date, degree);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not proceed");
    return;
  }

  QString result = ui_->interpolation_date_edit->date().toString() +
                   "\nNewton polynome: " + QString::number(newton_value) +
                   "\nCube spline: " + QString::number(spline_value);
  QMessageBox::information(this, "Search result", result);
}

void MainWindow::OnApproximationSearchButtonClicked() {
  double date = ui_->approximation_date_edit->dateTime().toSecsSinceEpoch();
  size_t degree = ui_->approximation_degree_spin_box->value();

  double value = 0;
  try {
    value = controller_->FindApproximationValue(date, degree);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not proceed");
    return;
  }

  QString result = ui_->approximation_date_edit->date().toString() +
                   "\nValue: " + QString::number(value);
  QMessageBox::information(this, "Search result", result);
}

void MainWindow::OnInterpolationResearchButtonClicked() {
  size_t points = ui_->research_points_spin_box->value();
  size_t partitions = ui_->research_partitions_spin_box->value();

  QVector<double> keys, newton_values, spline_values;
  try {
    std::tie(keys, newton_values, spline_values) =
        controller_->InterpolationResearch(points, partitions);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not open file");
    return;
  }

  if (keys.empty() || newton_values.empty() || spline_values.empty()) return;

  auto& plot = ui_->researchPlot;
  plot->Clear();

  plot->AddGraph("Newton research");
  plot->graph()->setData(keys, newton_values);

  plot->AddGraph("Spline research");
  plot->graph()->setData(keys, spline_values);

  plot->RescaleAndReplot();
}

void MainWindow::OnApproximationResearchButtonClicked() {
  size_t points = ui_->approximation_points_spin_box->value();
  size_t days = ui_->period_spin_box->value();

  QVector<double> keys, values1, values2, values3, values4;
  try {
    std::tie(keys, values1, values2, values3, values4) =
        controller_->ApproximationResearch(points, days);
  } catch (...) {
    QMessageBox::critical(this, "Error occured", "Could not open file");
    return;
  }

  if (keys.empty() || values1.empty() || values2.empty() || values3.empty() ||
      values4.empty())
    return;

  auto& plot = ui_->approximation_plot;
  plot->DeleteGraphsExceptFirst();

  plot->AddGraph("Degree: 1, Weights: user-defined");
  plot->graph()->setData(keys, values1);

  plot->AddGraph("Degree: 2, Weights: user-defined");
  plot->graph()->setData(keys, values2);

  plot->AddGraph("Degree: 1, Weights: 1");
  plot->graph()->setData(keys, values3);

  plot->AddGraph("Degree: 2, Weights: 1");
  plot->graph()->setData(keys, values4);

  plot->RescaleAndReplot();

  ui_->approximation_plot_button->setDisabled(true);
}

void MainWindow::SetupPlots() {
  for (auto& plot : plots_) {
    connect(plot, &QCustomPlot::mouseMove, this, &MainWindow::PlotMouseMove);
    plot->SetAxis(plot == plots_.back() ? "Points" : "Date",
                  plot == plots_.back() ? "Time [ms]" : "Close",
                  plot != plots_.back());
  }
}

void MainWindow::PlotMouseMove(QMouseEvent* event) {
  for (auto plot : plots_) {
    plot->SetTracers(event->pos(), plot != plots_.back());
    plot->replot();
  }
}
