// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include <cassert>
#include <fatigue/utils.hpp>
#include <string>


int main()
{
  //too many underscore, this is not a gnug mangled type
  std::string mangled("___Z1hidfoedsy");

  assert(ftg::demangle(mangled.c_str()) == mangled);

  return 0;
}