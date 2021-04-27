#include "fatigue/OstreamTestRunner.hpp"
#include "fatigue/TestDriver.hpp"
#include <fatigue/Test.hpp>

#include <cassert>
#include <sstream>

using namespace ftg;

struct TestAutoMsgFail : public Test {
  TestAutoMsgFail(OstreamTestLogger const& logger, std::stringstream const& ss) :
      Test("TestAutoMsgFail"),
      otl(logger),
      ss(ss)
  {
  }

  virtual void run()
  {
    std::stringstream res;

    check_equal(10, 9);

    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 1);
    res << "(1) [ERROR] expected check_equal( l: 10 [int], r: 9 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_near_equal(20, 12, 2);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 2);
    res << "(2) [ERROR] expected check_near_equal( l: 20 [int], r: 12 [int], tolerance: 2 [int] ) to succeed, but "
	   "failed."
	<< std::endl;
    assert(ss.str() == res.str());

    check_near_equal(7, 10, 2);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 3);
    res << "(3) [ERROR] expected check_near_equal( l: 7 [int], r: 10 [int], tolerance: 2 [int] ) to succeed, but "
	   "failed."
	<< std::endl;
    assert(ss.str() == res.str());

    check_not_equal(12, 12);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 4);
    res << "(4) [ERROR] expected check_not_equal( l: 12 [int], r: 12 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_than(12, 10);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 5);
    res << "(5) [ERROR] expected check_less_than( l: 12 [int], r: 10 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_than(12, 12);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 6);
    res << "(6) [ERROR] expected check_less_than( l: 12 [int], r: 12 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_less_equal(12, 10);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 7);
    res << "(7) [ERROR] expected check_less_equal( l: 12 [int], r: 10 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_than(8, 9);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 8);
    res << "(8) [ERROR] expected check_greater_than( l: 8 [int], r: 9 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_than(10, 10);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 9);
    res << "(9) [ERROR] expected check_greater_than( l: 10 [int], r: 10 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_greater_equal(7, 8);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 10);
    res << "(10) [ERROR] expected check_greater_equal( l: 7 [int], r: 8 [int] ) to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_throw<int>([]() {});
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 11);
    res << "(11) [ERROR] expected check_throw<int> to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_nothrow([]() { throw 1; });
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 12);
    res << "(12) [ERROR] expected check_nothrow to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_true(false);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 13);
    res << "(13) [ERROR] expected check_true to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());

    check_false(true);
    assert(!otl.passed());
    assert(otl.m_checkPassed == 0);
    assert(otl.m_checkFailed == 14);
    res << "(14) [ERROR] expected check_false to succeed, but failed." << std::endl;
    assert(ss.str() == res.str());
  }

  OstreamTestLogger const& otl;
  std::stringstream const& ss;
};

int main()
{

  std::stringstream ss;

  Config conf;
  conf.showParamTypes = true;
  conf.showParamNames = true;
  OstreamTestLogger otl(ss, conf);
  TestAutoMsgFail t(otl, ss);
  t.setLogger(&otl);
  t.run();

  return 0;
}