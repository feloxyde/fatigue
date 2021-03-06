// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_CUSTOMOPTIONS_HPP
#define FATIGUE_TUTORIAL_CUSTOMOPTIONS_HPP

/* PLEASE NOTE that here we use cxxopts.hpp 
provided by fatigue itself, to ensure compatibility.
*/
#include <fatigue/external/cxxopts.hpp>

inline void customOptions(cxxopts::Options& opts)
{
  //adding options
  opts.add_options()("f,fast",
		     "selects fastest runner available, overrides any runner selected by -r option",
		     cxxopts::value<bool>()->default_value("false"));
  opts.add_options()("w,werror",
		     "if set, warning messages fails the test",
		     cxxopts::value<bool>()->default_value("false"));
}

#endif