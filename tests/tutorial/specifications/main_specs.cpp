// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "ConcreteConcat.hpp"
#include "SpecsConcat.hpp"
#include <fatigue/fatigue.hpp>
#include <memory>

struct factoryVectorConcat {
  std::unique_ptr<VectorConcat> operator()() { return std::make_unique<VectorConcat>(); }
};

struct factoryListConcat {
  std::unique_ptr<ListConcat> operator()() { return std::make_unique<ListConcat>(); }
};


int main(int argc, char** argv)
{
  // clang-format off
  return ftg::fatigue(argc, argv)
        .declare(std::make_unique<SpecsConcat<factoryListConcat>>())
        .declare(std::make_unique<SpecsConcat<factoryListConcat>>())
        .run();
  // clang-format on
}