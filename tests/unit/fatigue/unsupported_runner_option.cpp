// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/compareStrings.hpp"
#include <cassert>
#include <fatigue/Test.hpp>
#include <fatigue/fatigue.hpp>
#include <sstream>


class TestRunner : public ftg::Logger, public ftg::Runner {
public:
  virtual std::unordered_set<std::string> supportedOptions() const
  {
    return std::unordered_set<std::string>({"select"});
  }

  virtual void report(Logger::Message const&) noexcept {}

  virtual unsigned run(ftg::TestList const&) { return 1; }
};


class fatigue_test : public ftg::fatigue {
public:
  fatigue_test(int argc, char** argv, std::ostream& out) : ftg::fatigue(argc, argv, out)
  {
    //registering runner in the list of available runners
    this->ftg::fatigue::m_runners.emplace("test", std::make_unique<TestRunner>());
  }
};

int main()
{
  int argc = 4;
  char command[] = "cmd";
  char select[] = "--select=tests";
  char exclude[] = "--exclude=others";
  char runner[] = "--runner=test";
  char* argv[] = {command, select, exclude, runner}; // we won't kill a beef for the test x)
  std::stringstream out;
  std::stringstream exp;
  fatigue_test f(argc, argv, out);
  int res = f.run();
  assert(res == ftg::fatigue::ExitCodes::unsupportedOption);

  exp << "Error : runner ["
      << "test"
      << "] : ["
      << "exclude"
      << "] option not supported" << std::endl;
  exp << "Error : unsupported options were sent to runner. Aborting." << std::endl;

  assert(compareStrings(out.str(), exp.str()));

  return 0;
}