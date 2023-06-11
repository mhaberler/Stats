#include "RunningRegression.h"

RunningRegression::RunningRegression() { Clear(); }

void RunningRegression::Clear() {
  x_stats.Clear();
  y_stats.Clear();
  S_xy = 0.0;
  n = 0;
}

void RunningRegression::Push(float x, float y) {
  S_xy +=
      (x_stats.Mean() - x) * (y_stats.Mean() - y) * float(n) / float(n + 1);

  x_stats.Push(x);
  y_stats.Push(y);
  n++;
}

int64_t RunningRegression::NumDataValues() const { return n; }

float RunningRegression::Slope() const {
  float S_xx = x_stats.Variance() * (n - 1.0);

  return S_xy / S_xx;
}

float RunningRegression::Intercept() const {
  return y_stats.Mean() - Slope() * x_stats.Mean();
}

float RunningRegression::Correlation() const {
  float t = x_stats.StandardDeviation() * y_stats.StandardDeviation();
  return S_xy / ((n - 1) * t);
}

RunningRegression operator+(const RunningRegression a,
                            const RunningRegression b) {
  RunningRegression combined;

  combined.x_stats = a.x_stats + b.x_stats;
  combined.y_stats = a.y_stats + b.y_stats;
  combined.n = a.n + b.n;

  float delta_x = b.x_stats.Mean() - a.x_stats.Mean();
  float delta_y = b.y_stats.Mean() - a.y_stats.Mean();
  combined.S_xy = a.S_xy + b.S_xy +
                  float(a.n * b.n) * delta_x * delta_y / float(combined.n);

  return combined;
}

RunningRegression &RunningRegression::operator+=(const RunningRegression &rhs) {
  RunningRegression combined = *this + rhs;
  *this = combined;
  return *this;
}
