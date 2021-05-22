// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../quick_example/SuiteAvg.hpp"
#include "TenTest.hpp"
#include "fatigue_ex.hpp"
#include <memory>

class EqTen : public TenTest {
public:
  EqTen() : TenTest("EqTen") {}
  virtual ~EqTen() {}

  virtual void run()
  {
    check_equals_ten(10); //checking our function
  }
};


int main(int argc, char** argv)
{

  // clang-format off
    return fatigue_ex(argc, argv)
        .declare(std::make_unique<Avg>()) //suite from previous tutorial
        .declare(std::make_unique<EqTen>())
        .run();
  // clang-format on
}