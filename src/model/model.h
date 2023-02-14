#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <QVector>
#include <vector>

#include "approximation/base_approximation.h"

class Model {
 public:
  using GraphData = std::tuple<QVector<double>, QVector<double>>;
  using InterpolationResearchData =
      std::tuple<QVector<double>, QVector<double>, QVector<double>>;
  using ApproximationResearchData =
      std::tuple<QVector<double>, QVector<double>, QVector<double>,
                 QVector<double>, QVector<double>>;

  [[nodiscard]] GraphData OpenFile(const QString& filename);
  [[maybe_unused]] GraphData Newton(size_t points, size_t degree) const;
  [[maybe_unused]] GraphData Spline(size_t points) const;
  [[nodiscard]] GraphData Approximate(size_t points,
                                      size_t degree,  //
                                      size_t days) const;

  [[nodiscard]] InterpolationResearchData  //
  InterpolationResearch(size_t points, size_t partitions, size_t degree) const;

  [[nodiscard]] ApproximationResearchData  //
  ApproximationResearch(size_t points, size_t days);

  [[nodiscard]] std::tuple<double, double>  //
  FindInterpolationValue(double x, size_t degree) const;

  [[nodiscard]] double FindApproximationValue(double x, size_t degree) const;

 private:
  std::vector<double> keys_, values_, weights_, dates_;

  double DateToKey(double date) const;
  bool IsDataEmpty() const noexcept;

  [[nodiscard]] GraphData CalcGraph(BaseApproximation* method,
                                    size_t points,  //
                                    size_t days = 0) const;
};

#endif  // SRC_MODEL_MODEL_H_
