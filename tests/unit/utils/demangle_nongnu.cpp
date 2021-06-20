// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include <cassert>

#undef __GNUG__
#include <fatigue/utils.hpp>
#include <string>


int main()
{
  std::string mangled("_Z1hi");

  assert(ftg::demangle(mangled.c_str()) == mangled);

  return 0;
}