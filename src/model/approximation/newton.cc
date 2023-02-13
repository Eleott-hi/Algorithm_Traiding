#include "newton.h"

#include <algorithm>

namespace Interpolation {

Newton::Newton(std::vector<double> const& x,  //
               std::vector<double> const& y,  //
               std::size_t degree) {
  if (x.size() != y.size() || x.size() < 2) return;

  std::size_t polinoms = (x.size() - 1) / degree;
  newtons_.clear();
  newtons_.reserve(polinoms);

  for (std::size_t i = 0, step = degree; i < polinoms; i++) {
    int start = i * step;
    int end = (i == polinoms - 1) ? x.size() : (i + 1) * step + 1;

    newtons_.push_back({{x.begin() + start, x.begin() + end},
                        {y.begin() + start, y.begin() + end}});
  }
}

double Newton::GetValue(double x) const {
  auto itr = std::find_if(newtons_.begin(), newtons_.end(),
                          [&](auto& newton) { return newton.IsBelongX(x); });

  if (itr == newtons_.end()) return 0;
  return itr->GetValue(x);
}

Newton::MiniNewton::MiniNewton(std::vector<double>&& x,
                               std::vector<double>&& y) {
  if (x.size() != y.size() || x.size() < 2) return;
  x_ = std::move(x), y_ = std::move(y);

  coefs_.reserve(x_.size());
  coefs_.push_back(y_.at(0));

  for (std::size_t i = 1; i < x_.size(); i++) {
    double res = (y_.at(i) - GetSumP(i)) / GetP(i, i);
    coefs_.push_back(res);
  }
}

long double Newton::MiniNewton::GetSumP(std::size_t ind) const {
  long double res = 0;
  for (std::size_t i = 0; i < coefs_.size(); i++)
    res += coefs_[i] * GetP(i, ind);
  return res;
}

long double Newton::MiniNewton::GetP(std::size_t until, std::size_t ind) const {
  long double res = 1;
  for (std::size_t i = 0; i < until; i++) res *= x_.at(ind) - x_.at(i);
  return res;
}

long double Newton::MiniNewton::GetFreeP(std::size_t ind, double x) const {
  long double res = 1;
  for (std::size_t i = 0; i < ind; i++) res *= x - x_.at(i);
  return res;
}

long double Newton::MiniNewton::GetFreeSumP(double x) const {
  long double res = 0;
  for (std::size_t i = 0; i < coefs_.size(); i++)
    res += coefs_[i] * GetFreeP(i, x);
  return res;
}

double Newton::MiniNewton::GetValue(double x) const {
  return static_cast<double>(GetFreeSumP(x));
}

bool Newton::MiniNewton::IsBelongX(double x) const {
  if (x_.empty()) return false;
  return x_.front() <= x && x <= x_.back();
}

}  // namespace s21::Interpolation
