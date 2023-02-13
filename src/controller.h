#ifndef SRC_MAIN_CONTROLLER_H_
#define SRC_MAIN_CONTROLLER_H_

#include "model.h"

class Controller {
 public:
  explicit Controller(Model *model) : model_(model) {}
  ~Controller() = default;
  Controller(Controller &&) = delete;
  Controller(const Controller &) = delete;
  Controller &operator=(Controller &&) = delete;
  Controller &operator=(const Controller &) = delete;

  [[nodiscard]] Model::GraphData OpenFile(QString const &filename) {
    return model_->OpenFile(filename);
  }

  [[nodiscard]] Model::GraphData Newton(size_t points, size_t degree) const {
    return model_->Newton(points, degree);
  }

  [[nodiscard]] Model::GraphData Spline(size_t points) const {
    return model_->Spline(points);
  }

  [[nodiscard]] Model::GraphData Approximate(size_t points,
                                             size_t degree,  //
                                             size_t days) const {
    return model_->Approximate(points, degree, days);
  }

  [[nodiscard]] std::tuple<double, double>  //
  FindInterpolationValue(double x, int degree) const {
    return model_->FindInterpolationValue(x, degree);
  }

  [[nodiscard]] double FindApproximationValue(double x, size_t degree) const {
    return model_->FindApproximationValue(x, degree);
  }

  [[nodiscard]] Model::InterpolationResearchData  //
  InterpolationResearch(size_t points, size_t partitions,
                        size_t degree = 5) const {
    return model_->InterpolationResearch(points, partitions, degree);
  }

  [[nodiscard]] Model::ApproximationResearchData  //
  ApproximationResearch(size_t points, size_t days) {
    return model_->ApproximationResearch(points, days);
  }

 private:
  Model *model_;
};

#endif  // SRC_MAIN_CONTROLLER_H_
