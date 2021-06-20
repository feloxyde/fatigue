// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_FATIGUEEX_HPP
#define FATIGUE_TUTORIAL_FATIGUEEX_HPP

#include "TrivialRunner.hpp"
#include "customOptions.hpp"
#include <fatigue/fatigue.hpp>

class fatigue_ex : public ftg::fatigue {
public:
  fatigue_ex(int argc, char** argv) : ftg::fatigue({customOptions}, argc, argv)
  {

    bool werror = false;
    if (m_parsedOpts["werror"].as<bool>()) {
      werror = true;
      //is the option is set, we register it as a runner option so it can be supported by checking
      this->ftg::fatigue::m_config.foundRunnerOption("werror");
    }

    //registering runner in the list of available runners
    this->ftg::fatigue::m_runners.emplace("trivial", std::make_unique<TrivialRunner>(werror));

    //overriding config "runner" field when fast option is set.
    if (m_parsedOpts["fast"].as<bool>()) {
      this->ftg::fatigue::m_config.runner = "trivial";
    }
  }
};

#endif