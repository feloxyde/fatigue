// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_AVG_HPP
#define FATIGUE_TUTORIAL_AVG_HPP

#include <vector>

inline float avg(std::vector<float> const& v)
{
  if (v.empty()) {
    return 0.0;
  }

  float sum = 0.0;
  for (auto const& f : v) {
    sum += f;
  }

  return sum / v.size();
}

#endif