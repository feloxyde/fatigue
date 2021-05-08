#ifndef FATIGUE_TUTORIAL_CUSTOMOPTIONS_HPP
#define FATIGUE_TUTORIAL_CUSTOMOPTIONS_HPP

#include <fatigue/external/cxxopts.hpp>

inline void customOptions(cxxopts::Options& opts)
{
  //adding options
  opts.add_options()("f,fast",
		     "selects fastest runner available, overrides any runner selected by -r option",
		     cxxopts::value<bool>()->default_value("false"));
}

#endif