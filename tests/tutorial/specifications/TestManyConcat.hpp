// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_TESTMANYCONCAT_HPP
#define FATIGUE_TUTORIAL_TESTMANYCONCAT_HPP

#include "Concat.hpp"
#include <fatigue/Test.hpp>
#include <fatigue/utils.hpp>
#include <memory>


/* F is a factory building the type we want to test, in the form of a unique pointer */
template<typename Factory>
class Many : public ftg::Test {
public:
  Many() : ftg::Test("Many") {}
  virtual ~Many() {}

  virtual void run()
  {
    std::shared_ptr<Concat> c = Factory()();
    c->add("one");
    c->add("two");
    c->add("three");
    check_equal(c->concatenated(), "onetwothree");
  }
};


#endif