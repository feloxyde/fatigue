// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "compareStrings.hpp"
#include <cassert>
#include <sstream>


int main()
{
  std::stringstream a;
  std::stringstream b;

  a << "sometext" << std::endl;
  b << "sometext" << std::endl;

  assert(compareStrings(a.str(), b.str()));

  a << "somemoretest" << std::endl;
  b << "someothertext" << std::endl;

  assert(!compareStrings(a.str(), b.str()));

  std::stringstream c;
  std::stringstream d;

  assert(compareStrings(c.str(), d.str()));

  d << "somenicetext" << std::endl;

  assert(!compareStrings(c.str(), d.str()));
  return 0;
}