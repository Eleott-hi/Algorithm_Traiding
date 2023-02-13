#include "plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent) {
  setInteractions({QCP::iRangeDrag, QCP::iRangeZoom, QCP::iSelectPlottables});
  axisRect()->setAutoMargins({QCP::msBottom, QCP::msLeft});
  axisRect()->setMargins(QMargins{50, 50, 50, 50});
  axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop);
  legend->setVisible(true);
}

Plot::~Plot() { Clear(); }

void Plot::SetAxis(QString const &x_axis, QString const &y_axis, bool to_date) {
  xAxis->setLabel(x_axis);
  yAxis->setLabel(y_axis);

  if (!to_date) return;

  QSharedPointer<QCPAxisTickerDateTime> date_ticker(new QCPAxisTickerDateTime);
  date_ticker->setDateTimeFormat("yyyy-MM-dd");
  xAxis->setTicker(date_ticker);
  xAxis->setTickLabelRotation(-45);
}

void Plot::SetTracers(QPoint const &pos, bool to_date) {
  double x = xAxis->pixelToCoord(pos.x());

  std::vector<size_t> y_coords;
  y_coords.reserve(6);

  auto GetShift = [&y_coords](double y) -> int {
    int shift = -30;
    for (auto &i : y_coords)
      while ((i - 30 <= y + shift) && (y + shift <= i + 30)) --shift;

    y_coords.push_back(y + shift);
    return shift;
  };

  for (auto &i : attributes_) {
    auto &[tracer, arrow, label] = i.second;
    tracer->setGraphKey(x);
    tracer->updatePosition();
    auto [x_coord, y_coord] = tracer->position->coords();
    QString x_info = to_date
                         ? QCPAxisTickerDateTime::keyToDateTime(x_coord).  //
                           toString("yyyy-MM-dd")
                         : QString::number(x_coord);

    label->setText(x_info + "\n" + QString::number(y_coord));
    label->setLayer("overlay");

    arrow->start->setCoords(0, GetShift(tracer->position->pixelPosition().y()));
  }
}

void Plot::AddGraph(QString const &name,
                    QCPScatterStyle::ScatterShape scatter_shape) {
  static QVector<QColor> colors = {Qt::blue,  Qt::red,  Qt::darkYellow,
                                   Qt::green, Qt::cyan, Qt::magenta};
  QColor color = colors.at(graphCount());

  addGraph();
  graph()->setName(name);
  graph()->setPen(QPen(color));
  SetGraph(graph(), color);
  graph()->setLineStyle(QCPGraph::lsLine);
  graph()->setScatterStyle(QCPScatterStyle(scatter_shape, 5));
}

void Plot::DeleteGraphsExceptFirst() {
  while (graphCount() > 1) DeleteGraph(graph());
  RescaleAndReplot();
}

void Plot::Clear() {
  for (auto &attribute : attributes_) {
    auto &[tracer, arrow, label] = attribute.second;

    removeItem(tracer);
    removeItem(arrow);
    removeItem(label);
  }

  attributes_.clear();
  clearGraphs();
}

void Plot::SetGraph(QCPGraph *graph, QColor const &color) {
  auto tracer = new QCPItemTracer(this);
  tracer->setVisible(false);
  tracer->setGraph(graph);

  auto arrow = new QCPItemLine(this);
  arrow->end->setParentAnchor(tracer->position);

  arrow->setHead(QCPLineEnding::esSpikeArrow);
  arrow->start->setParentAnchor(arrow->end);
  arrow->setClipToAxisRect(false);
  arrow->start->setCoords(0, -30);
  arrow->setPen(QPen(color));
  arrow->setLayer("overlay");

  auto label = new QCPItemText(this);
  label->position->setParentAnchor(arrow->start);
  label->setPositionAlignment(Qt::AlignCenter);
  label->setPadding(QMargins(3, 0, 3, 0));
  label->setBrush(QBrush(Qt::white));
  label->setClipToAxisRect(false);
  label->setPen(QPen(color));
  label->setLayer("overlay");

  attributes_.emplace(graph, std::make_tuple(tracer, arrow, label));
}

void Plot::DeleteGraph(QCPGraph *graph) {
  auto itr = attributes_.find(graph);
  if (itr == attributes_.end()) return;

  auto &[tracer, arrow, label] = itr->second;
  removeItem(tracer);
  removeItem(arrow);
  removeItem(label);

  attributes_.erase(itr);
  removeGraph(graph);
}
