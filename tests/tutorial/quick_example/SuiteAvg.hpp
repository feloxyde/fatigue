// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_SUITEAVG_HPP
#define FATIGUE_TUTORIAL_SUITEAVG_HPP

#include "TestAvgEmpty.hpp"
#include "TestAvgMany.hpp"
#include "TestAvgOne.hpp"
#include <fatigue/Suite.hpp>
#include <memory>
#include <vector>

class Avg : public ftg::Suite {
public:
  Avg() : ftg::Suite("Avg") {}
  virtual ~Avg() {}

  virtual ftg::TestList tests() const
  {
    ftg::TestList tl;
    tl.push_back(std::make_unique<Empty>());
    tl.push_back(std::make_unique<One>());
    tl.push_back(std::make_unique<Many>());
    return tl;
  }
};

#endif