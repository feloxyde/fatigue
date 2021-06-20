// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "SuiteAvg.hpp"
#include <fatigue/fatigue.hpp>
#include <memory>

int main(int argc, char** argv)
{

  auto f = ftg::fatigue(argc, argv);
  f.declare(std::make_unique<Avg>());
  return f.run();
}