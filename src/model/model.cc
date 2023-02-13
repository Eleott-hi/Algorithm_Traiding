#include "model.h"

#include <QDate>
#include <fstream>

#include "approximation/least_squares.h"
#include "approximation/newton.h"
#include "approximation/spline.h"
#include "csv.h"
#include "thread_pool.h"
#include "timer.h"
#include "utils.h"

Model::GraphData Model::OpenFile(const QString &filename) {
  io::CSVReader<3> parser(filename.toStdString());
  parser.read_header(io::ignore_missing_column, "Date", "Close", "Weight");

  keys_.clear();
  dates_.clear();
  values_.clear();
  weights_.clear();

  QDate first_day;
  double close = 0;
  std::string date, weight;
  bool first = true;

  while (parser.read_row(date, close, weight)) {
    if (weight.empty()) weight = "1";
    QDate day = QDate::fromString(date.c_str(), "yyyy-MM-dd");

    if (first) {
      first_day = day;
      first = !first;
    }

    dates_.push_back(QDateTime(day, {}).toSecsSinceEpoch());
    keys_.push_back(first_day.daysTo(day));
    weights_.push_back(std::stod(weight));
    values_.push_back(close);
  }

  return {QVector<double>(dates_.begin(), dates_.end()),
          QVector<double>(values_.begin(), values_.end())};
}

Model::GraphData Model::Newton(size_t points, size_t degree) const {
  if (IsDataEmpty()) return {};

  Interpolation::Newton newton(keys_, values_, degree);
  return CalcGraph(&newton, points);
}

Model::GraphData Model::Spline(size_t points) const {
  if (IsDataEmpty()) return {};

  Interpolation::Spline spline(keys_, values_);
  return CalcGraph(&spline, points);
}

Model::GraphData Model::Approximate(size_t points,
                                    size_t degree,  //
                                    size_t days) const {
  if (IsDataEmpty()) return {};

  Approximation::LeastSquares approximation(keys_, values_, weights_, degree);
  return CalcGraph(&approximation, points, days);
}

std::tuple<double, double>  //
Model::FindInterpolationValue(double x, size_t degree) const {
  if (IsDataEmpty() || !(dates_.front() <= x && x <= dates_.back()))
    return {qQNaN(), qQNaN()};

  x = DateToKey(x);

  Interpolation::Newton newton(keys_, values_, degree);
  Interpolation::Spline spline(keys_, values_);

  return {newton.GetValue(x), spline.GetValue(x)};
}

double Model::FindApproximationValue(double x, size_t degree) const {
  if (IsDataEmpty()) return qQNaN();

  x = DateToKey(x);

  Approximation::LeastSquares approximation(keys_, values_, weights_, degree);
  return approximation.GetValue(x);
}

std::tuple<QVector<double>, QVector<double>, QVector<double>>
Model::InterpolationResearch(size_t points, size_t partitions,
                             size_t degree) const {
  if (IsDataEmpty()) return {};

  size_t step = Utils::CalcStep(points - keys_.size(), partitions - 1);

  if (step == 0) partitions = 2;
  if (points == keys_.size()) partitions = 1;

  QVector<double> keys(partitions), newton(partitions), spline(partitions);

  for (size_t i = 0; i < partitions; ++i)
    keys[i] = (i == partitions - 1) ? points : (keys_.size() + i * step);

  ThreadPool pool;
  const size_t k_count = 10;
  Matrix time1(partitions, k_count), time2(partitions, k_count);

  auto research = [&](std::function<void()> func) -> double {
    Timer timer;
    func();
    return timer.Finish().count();
  };

  for (size_t i = 0; i < partitions; ++i) {
    for (size_t j = 0; j < k_count; ++j) {
      pool.AddTask([&, i, j]() {
        time1(i, j) = research([&, degree, x = keys[i]] { Newton(x, degree); });
      });

      pool.AddTask([&, i, j]() {
        time2(i, j) = research([&, x = keys[i]] { Spline(x); });
      });
    }
  }

  pool.WaitAll();

  for (size_t i = 0; i < partitions; ++i) {
    for (size_t j = 0; j < k_count; ++j) {
      newton[i] += time1(i, j);
      spline[i] += time2(i, j);
    }
    newton[i] /= k_count;
    spline[i] /= k_count;
  }

  return {std::move(keys), std::move(newton), std::move(spline)};
}

Model::ApproximationResearchData  //
Model::ApproximationResearch(size_t points, size_t days) {
  if (IsDataEmpty()) return {};

  std::vector<double> tmp_weights(weights_.size(), 1);

  Approximation::LeastSquares app_1(keys_, values_, weights_, 1);
  Approximation::LeastSquares app_2(keys_, values_, weights_, 2);
  Approximation::LeastSquares app_3(keys_, values_, tmp_weights, 1);
  Approximation::LeastSquares app_4(keys_, values_, tmp_weights, 2);

  auto [keys_1, values_1] = CalcGraph(&app_1, points, days);
  auto [keys_2, values_2] = CalcGraph(&app_2, points, days);
  auto [keys_3, values_3] = CalcGraph(&app_3, points, days);
  auto [keys_4, values_4] = CalcGraph(&app_4, points, days);

  return {std::move(keys_1),    //
          std::move(values_1),  //
          std::move(values_2),  //
          std::move(values_3),  //
          std::move(values_4)};
}

std::tuple<QVector<double>, QVector<double>>  //
Model::CalcGraph(BaseApproximation *method, size_t points, size_t days) const {
  double first_key = keys_.front();
  double last_key = keys_.back() + days;
  double first_date = dates_.front();
  double last_date = QDateTime::fromSecsSinceEpoch(dates_.back())
                         .addDays(days)
                         .toSecsSinceEpoch();
  double step_key = Utils::CalcStep(last_key - first_key, points);
  double step_date = Utils::CalcStep(last_date - first_date, points);

  QVector<double> keys, values;
  while (first_key < last_key) {
    keys.push_back(first_date);
    values.push_back(method->GetValue(first_key));
    first_key += step_key, first_date += step_date;
  }

  return {std::move(keys), std::move(values)};
}

double Model::DateToKey(double date) const {
  QDate first_day = QDateTime::fromSecsSinceEpoch(dates_.front()).date();
  QDate day = QDateTime::fromSecsSinceEpoch(date).date();
  return first_day.daysTo(day);
}

bool Model::IsDataEmpty() const noexcept {
  return keys_.empty() || values_.empty() || dates_.empty();
}
