#ifndef SRC_MAIN_MODEL_APPROXIMATION_NEWTON_H_
#define SRC_MAIN_MODEL_APPROXIMATION_NEWTON_H_

#include <vector>

#include "base_approximation.h"

namespace Interpolation {

class Newton : public BaseApproximation {
 public:
  Newton(std::vector<double> const& x,  //
         std::vector<double> const& y,  //
         std::size_t degree);
  ~Newton() = default;
  Newton(Newton&&) = delete;
  Newton(const Newton&) = delete;
  Newton& operator=(Newton&&) = delete;
  Newton& operator=(const Newton&) = delete;

  virtual double GetValue(double x) const override;

 private:
  class MiniNewton;

  std::vector<MiniNewton> newtons_;
};

class Newton::MiniNewton {
 public:
  MiniNewton(std::vector<double>&& x,  //
             std::vector<double>&& y);
  ~MiniNewton() = default;
  MiniNewton(MiniNewton&&) = default;
  MiniNewton(const MiniNewton&) = default;
  MiniNewton& operator=(MiniNewton&&) = default;
  MiniNewton& operator=(const MiniNewton&) = default;

  double GetValue(double x) const;
  bool IsBelongX(double x) const;

 private:
  std::vector<long double> coefs_;
  std::vector<double> x_, y_;

  long double GetSumP(std::size_t ind) const;
  long double GetP(std::size_t until, std::size_t ind) const;
  long double GetFreeSumP(double x) const;
  long double GetFreeP(std::size_t ind, double xp) const;
};

}  // namespace s21::Interpolation

#endif  // SRC_MAIN_MODEL_APPROXIMATION_NEWTON_H_
