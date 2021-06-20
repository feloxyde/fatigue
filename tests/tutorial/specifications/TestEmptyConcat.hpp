// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_TESTEMPTYCONCAT_HPP
#define FATIGUE_TUTORIAL_TESTEMPTYCONCAT_HPP

#include "Concat.hpp"
#include <fatigue/Test.hpp>
#include <memory>


/* F is a factory building the type we want to test, in the form of a unique pointer */
template<typename Factory>
class Empty : public ftg::Test {
public:
  Empty() : ftg::Test("Empty") {}
  virtual ~Empty() {}

  virtual void run()
  {
    std::shared_ptr<Concat> c = Factory()();
    check_equal(c->concatenated(), "");
  }
};


#endif