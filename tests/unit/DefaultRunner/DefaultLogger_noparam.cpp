#include <cassert>
#include <fatigue/Checker.hpp>
#include <fatigue/Test.hpp>
#include <fatigue/runners/DefaultRunner.hpp>
#include <sstream>

using namespace ftg;

int main()
{

  Checker td("name");
  std::stringstream ss;
  std::stringstream res;

  Config conf;
  DefaultLogger dr(ss, conf);

  td.setLogger(&dr);
  assert(dr.passed());
  assert(dr.m_checkPassed == 0);
  assert(dr.m_checkFailed == 0);
  assert(ss.str() == res.str());

  //passing reporter
  td.raw_check("pass", std::vector<ParamInfo>(), true);
  assert(dr.m_checkPassed == 1);
  assert(dr.m_checkFailed == 0);
  assert(ss.str() == res.str());

  td.raw_check("warn fail", std::vector<ParamInfo>(), false).warn();
  assert(dr.passed());
  assert(dr.m_checkPassed == 1);
  assert(dr.m_checkFailed == 1);
  res << "(2) [WARN] expected warn fail to succeed, but failed." << std::endl;
  assert(ss.str() == res.str());

  //failing reporter, default msg
  td.raw_check("fail", std::vector<ParamInfo>(), false);
  assert(!dr.passed());
  assert(dr.m_checkPassed == 1);
  assert(dr.m_checkFailed == 2);
  res << "(3) [ERROR] expected fail to succeed, but failed." << std::endl;
  assert(ss.str() == res.str());

  //passing reporter, fatal
  td.raw_check("fatal pass", std::vector<ParamInfo>(), true).fatal();
  assert(!dr.passed());
  assert(dr.m_checkPassed == 2);
  assert(dr.m_checkFailed == 2);
  assert(ss.str() == res.str());

  //failing reporter, fatal
  bool thrown = false;
  try {
    td.raw_check("fatal fail", std::vector<ParamInfo>(), false).fatal();
  } catch (FatalCheckFailure fe) {
    thrown = true;
  }
  assert(thrown);
  assert(!dr.passed());
  assert(dr.m_checkPassed == 2);
  assert(dr.m_checkFailed == 3);
  res << "(5) [FATAL] expected fatal fail to succeed, but failed." << std::endl;
  assert(ss.str() == res.str());

  //failing important
  td.raw_check("important fail", std::vector<ParamInfo>(), false).important();
  assert(!dr.passed());
  assert(dr.m_checkPassed == 2);
  assert(dr.m_checkFailed == 4);
  res << "!!! (6) [ERROR] expected important fail to succeed, but failed." << std::endl;
  assert(ss.str() == res.str());

  td.raw_check("success", std::vector<ParamInfo>(), true).fails();
  assert(!dr.passed());
  assert(dr.m_checkPassed == 2);
  assert(dr.m_checkFailed == 5);
  res << "(7) [ERROR] expected success to fail, but succeeded." << std::endl;
  assert(ss.str() == res.str());

  //failing important
  td.raw_check("important fail succeeds", std::vector<ParamInfo>(), false).important().succeeds();
  assert(!dr.passed());
  assert(dr.m_checkPassed == 2);
  assert(dr.m_checkFailed == 6);
  res << "!!! (8) [ERROR] expected important fail succeeds to succeed, but failed." << std::endl;
  assert(ss.str() == res.str());

  return 0;
}