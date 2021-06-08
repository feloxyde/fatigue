// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "../../utils/compareStrings.hpp"
#include <cassert>
#include <fatigue/Test.hpp>
#include <fatigue/fatigue.hpp>
#include <sstream>


int main()
{
  int argc = 2;
  char command[] = "cmd";
  char runner[] = "--runner=NotExisting";
  char* argv[] = {command, runner}; // we won't kill a beef for the test x)
  std::stringstream out;
  std::stringstream exp;
  ftg::fatigue f(argc, argv, out);
  int res = f.run();
  assert(res == ftg::fatigue::ExitCodes::runnerNotFound);

  exp << "Could not find runner NotExisting" << std::endl;
  exp << "Available runners are :" << std::endl;
  exp << "default" << std::endl;

  assert(compareStrings(out.str(), exp.str()));

  return 0;
}