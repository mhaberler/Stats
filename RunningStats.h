#ifndef RUNNINGSTATS_H
#define RUNNINGSTATS_H
// from: https://www.johndcook.com/blog/skewness_kurtosis/

typedef enum {
  CI90=0,
  CI95,
  CI99,
} ci_t;

class RunningStats {
public:
  RunningStats();
  void Clear();
  void Push(float x);
  int64_t NumDataValues() const;
  float Mean() const;
  float Variance() const;
  float StandardDeviation() const;
  float Skewness() const;
  float Kurtosis() const;
  float ConfidenceInterval(ci_t ci);

  friend RunningStats operator+( RunningStats const &a,  RunningStats const &b);
  RunningStats &operator+=(const RunningStats &rhs);

private:
  int64_t n;
  float M1, M2, M3, M4;
};

#endif
