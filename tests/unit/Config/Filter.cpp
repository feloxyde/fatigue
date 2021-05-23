// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include <cassert>
#include <fatigue/Config.hpp>
#include <initializer_list>

using namespace ftg;

typedef std::vector<std::string> sv;

int main()
{

  Filter f;
  assert(!f.select.has_value());
  assert(!f.exclude.has_value());
  assert(f.separator == "//");
  assert(f.shouldRun(sv({"some, name"}), "some other"));
  assert(f.shouldRun(sv({"XXX"}), "XXXXXXX"));

  Filter fs;
  fs.select = ".*test.*";
  assert(fs.shouldRun(sv({"somesuite"}), "sometestname"));
  assert(!fs.shouldRun(sv({"somesuite"}), "somename"));

  Filter fe;
  fe.exclude = ".*test.*";
  assert(!fe.shouldRun(sv({"somesuite"}), "sometestname"));
  assert(fe.shouldRun(sv({"somesuite"}), "somename"));

  Filter fes;
  fes.select = ".*test.*";
  fes.exclude = ".*name.*";
  assert(fes.shouldRun(sv({"somesuite"}), "sometest"));
  assert(!fes.shouldRun(sv({"somesuite"}), "sometestname"));
  assert(!fes.shouldRun(sv({"somesuite"}), "somename"));

  Filter fsep;
  fsep.select = "suite.*//test";
  assert(fsep.shouldRun(sv({"suite"}), "test"));
  assert(!fsep.shouldRun(sv({"tsuite"}), "suite"));
  assert(!fsep.shouldRun(sv({"suite"}), "sometest"));
  assert(fsep.shouldRun(sv({"suite222"}), "test"));

  Filter fsep2;
  fsep2.separator = "--";
  fsep2.select = "suite.*--test";
  assert(fsep2.shouldRun(sv({"suite"}), "test"));
  assert(!fsep2.shouldRun(sv({"tsuite"}), "suite"));
  assert(!fsep2.shouldRun(sv({"suite"}), "sometest"));
  assert(fsep2.shouldRun(sv({"suite222"}), "test"));

  return 0;
};