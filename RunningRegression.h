#ifndef RUNNINGREGRESSION
#define RUNNINGREGRESSION

// from: https://www.johndcook.com/blog/running_regression/

#include "RunningStats.h"

class RunningRegression {
public:
  RunningRegression();
  void Clear();
  void Push(float x, float y);
  int64_t NumDataValues() const;
  float Slope() const;
  float Intercept() const;
  float Correlation() const;

  friend RunningRegression operator+(const RunningRegression a,
                                     const RunningRegression b);
  RunningRegression &operator+=(const RunningRegression &rhs);

private:
  RunningStats x_stats;
  RunningStats y_stats;
  float S_xy;
  int64_t n;
};

#endif
