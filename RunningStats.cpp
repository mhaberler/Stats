#include "RunningStats.h"
#include <math.h>

// https://www.geeksforgeeks.org/confidence-interval/
// assumes standard distribution
// 90% 1.645
// 95% 1.960
// 99% 2.576
static float z_values[] = {1.645f, 1.960f, 2.576f};

RunningStats::RunningStats() { Clear(); }

void RunningStats::Clear() {
  n = 0;
  M1 = M2 = M3 = M4 = 0.0;
}

void RunningStats::Push(float x) {
  float delta, delta_n, delta_n2, term1;

  int64_t n1 = n;
  n++;
  delta = x - M1;
  delta_n = delta / n;
  delta_n2 = delta_n * delta_n;
  term1 = delta * delta_n * n1;
  M1 += delta_n;
  M4 += term1 * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * M2 -
        4 * delta_n * M3;
  M3 += term1 * delta_n * (n - 2) - 3 * delta_n * M2;
  M2 += term1;
}

int64_t RunningStats::NumDataValues() const { return n; }

float RunningStats::Mean() const { return M1; }

float RunningStats::Variance() const { return M2 / (n - 1.0f); }

float RunningStats::StandardDeviation() const { return sqrt(Variance()); }

float RunningStats::Skewness() const {
  return sqrt(float(n)) * M3 / pow(M2, 1.5f);
}

float RunningStats::Kurtosis() const {
  return float(n) * M4 / (M2 * M2) - 3.0f;
}

float RunningStats::ConfidenceInterval(ci_t ci) {
  if (n < 300)
    return NAN;
  if (ci < CI90 || ci > CI99)
    return NAN;
  return z_values[ci] * StandardDeviation() / sqrt((float)n);
}

RunningStats operator+( RunningStats const &a,  RunningStats const &b) {
  RunningStats combined;

  combined.n = a.n + b.n;

  float delta = b.M1 - a.M1;
  float delta2 = delta * delta;
  float delta3 = delta * delta2;
  float delta4 = delta2 * delta2;

  combined.M1 = (a.n * a.M1 + b.n * b.M1) / combined.n;

  combined.M2 = a.M2 + b.M2 + delta2 * a.n * b.n / combined.n;

  combined.M3 = a.M3 + b.M3 +
                delta3 * a.n * b.n * (a.n - b.n) / (combined.n * combined.n);
  combined.M3 += 3.0f * delta * (a.n * b.M2 - b.n * a.M2) / combined.n;

  combined.M4 = a.M4 + b.M4 +
                delta4 * a.n * b.n * (a.n * a.n - a.n * b.n + b.n * b.n) /
                    (combined.n * combined.n * combined.n);
  combined.M4 += 6.0f * delta2 * (a.n * a.n * b.M2 + b.n * b.n * a.M2) /
                     (combined.n * combined.n) +
                 4.0f * delta * (a.n * b.M3 - b.n * a.M3) / combined.n;

  return combined;
}

RunningStats &RunningStats::operator+=(const RunningStats &rhs) {
  RunningStats combined = *this + rhs;
  *this = combined;
  return *this;
}