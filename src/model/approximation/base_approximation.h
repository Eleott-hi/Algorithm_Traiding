#ifndef SRC_MAIN_MODEL_APPROXIMATION_BASE_APPROXIMATION_H_
#define SRC_MAIN_MODEL_APPROXIMATION_BASE_APPROXIMATION_H_

class BaseApproximation {
 public:
  virtual ~BaseApproximation() = default;
  virtual double GetValue(double x) const = 0;
};

#endif  // SRC_MAIN_MODEL_APPROXIMATION_BASE_APPROXIMATION_H_
