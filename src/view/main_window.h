#ifndef SRC_VIEW_MAIN_WINDOW_H_
#define SRC_VIEW_MAIN_WINDOW_H_

#include <QMainWindow>

#include "controller.h"
#include "plot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(Controller *controller);
  ~MainWindow();
  MainWindow(const MainWindow &) = delete;
  MainWindow(MainWindow &&) = delete;
  MainWindow &operator=(const MainWindow &) = delete;
  MainWindow &operator=(MainWindow &&) = delete;

 private slots:
  void OnActionOpenTriggered();
  void OnActionClearTriggered();
  void OnActionQuitTriggered();

  void OnInterpolationNewtonPlotButtonClicked();
  void OnInterpolationSplinePlotButtonClicked();
  void OnApproximationPlotButtonClicked();

  void OnInterpolationSearchButtonClicked();
  void OnApproximationSearchButtonClicked();

  void OnInterpolationResearchButtonClicked();
  void OnApproximationResearchButtonClicked();

 private:
  Ui::MainWindow *ui_;
  Controller *controller_;
  QList<Plot *> plots_;
  QList<QSpinBox *> points_spin_boxes_;

  void SetupPlots();
  void PlotMouseMove(QMouseEvent *event);
};

#endif  // SRC_VIEW_MAIN_WINDOW_H_
