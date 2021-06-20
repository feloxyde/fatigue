// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "SuiteAvg.hpp"
#include <fatigue/fatigue.hpp>
#include <memory>

int main(int argc, char** argv)
{
  // clang-format off
  return ftg::fatigue(argc, argv)
        .declare(std::make_unique<Avg>())
        .run();
  // clang-format on
}