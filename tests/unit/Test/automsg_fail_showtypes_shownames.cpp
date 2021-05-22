// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "fatigue/runners/DefaultRunner.hpp"
#include <fatigue/Test.hpp>

#include <cassert>
#include <sstream>

using namespace ftg;

struct TestAutoMsgFail : public Test {
  TestAutoMsgFail(DefaultLogger const& logger, std::stringstream const& ss) :
      Test("TestAutoMsgFail"),
      dl(logger),
      ss(ss)
  {
  }

  virtual void run()
  {
    std::stringstream res;

    check_equal(10, 9);

    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 1);
    res << "(1) [ERROR] check_equal( l: 10 [int], r: 9 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_near_equal(20, 12, 2);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 2);
    res << "(2) [ERROR] check_near_equal( l: 20 [int], r: 12 [int], tolerance: 2 [int] ) -> true : failed."
	<< std::endl;
    assert(ss.str() == res.str());

    check_near_equal(7, 10, 2);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 3);
    res << "(3) [ERROR] check_near_equal( l: 7 [int], r: 10 [int], tolerance: 2 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_not_equal(12, 12);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 4);
    res << "(4) [ERROR] check_not_equal( l: 12 [int], r: 12 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_than(12, 10);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 5);
    res << "(5) [ERROR] check_less_than( l: 12 [int], r: 10 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_than(12, 12);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 6);
    res << "(6) [ERROR] check_less_than( l: 12 [int], r: 12 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_equal(12, 10);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 7);
    res << "(7) [ERROR] check_less_equal( l: 12 [int], r: 10 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_than(8, 9);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 8);
    res << "(8) [ERROR] check_greater_than( l: 8 [int], r: 9 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_than(10, 10);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 9);
    res << "(9) [ERROR] check_greater_than( l: 10 [int], r: 10 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_equal(7, 8);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 10);
    res << "(10) [ERROR] check_greater_equal( l: 7 [int], r: 8 [int] ) -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_throw<int>([]() {});
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 11);
    res << "(11) [ERROR] check_throw<int> -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_nothrow([]() { throw 1; });
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 12);
    res << "(12) [ERROR] check_nothrow -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_true(false);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 13);
    res << "(13) [ERROR] check_true -> true : failed." << std::endl;
    assert(ss.str() == res.str());

    check_false(true);
    assert(!dl.passed());
    assert(dl.m_checkPassed == 0);
    assert(dl.m_checkFailed == 14);
    res << "(14) [ERROR] check_false -> true : failed." << std::endl;
    assert(ss.str() == res.str());
  }

  DefaultLogger const& dl;
  std::stringstream const& ss;
};

int main()
{

  std::stringstream ss;

  Config conf;
  conf.showParamTypes = true;
  conf.showParamNames = true;
  DefaultLogger dl(ss, conf);
  TestAutoMsgFail t(dl, ss);
  t.setLogger(&dl);
  t.run();

  return 0;
}