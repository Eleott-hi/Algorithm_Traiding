#ifndef SRC_VIEW_PLOT_H_
#define SRC_VIEW_PLOT_H_

#include "shared/qcustomplot.h"

class Plot : public QCustomPlot {
 public:
  Plot(QWidget* parent = nullptr);
  ~Plot();
  Plot(Plot&&) = delete;
  Plot(const Plot&) = delete;
  Plot& operator=(Plot&&) = delete;
  Plot& operator=(const Plot&) = delete;

  void SetAxis(QString const& x_axis,  //
               QString const& y_axis,  //
               bool to_date = false);

  void AddGraph(QString const& name,
                QCPScatterStyle::ScatterShape = QCPScatterStyle::ssNone);

  void SetTracers(QPoint const& pos, bool to_date = false);
  void RescaleAndReplot() { rescaleAxes(), replot(); }
  void DeleteGraphsExceptFirst();
  void Clear();

 private:
  using Attributes = std::tuple<QCPItemTracer*, QCPItemLine*, QCPItemText*>;

  std::map<QCPGraph*, Attributes> attributes_;

  void SetGraph(QCPGraph* graph, QColor const& color);
  void DeleteGraph(QCPGraph* graph);
};

#endif  // SRC_VIEW_PLOT_H_
