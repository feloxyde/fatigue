#ifndef FATIGUE_TUTORIAL_SPECSCONCAT_HPP
#define FATIGUE_TUTORIAL_SPECSCONCAT_HPP

#include "TestEmptyConcat.hpp"
#include "TestManyConcat.hpp"
#include <fatigue/Suite.hpp>
#include <fatigue/utils.hpp>
#include <memory>
#include <vector>

template<typename Factory>
class SpecsConcat : public ftg::Suite {
public:
  /* we use type_to_string to get name of parameters for better display */
  SpecsConcat() : ftg::Suite(std::string("SpecsConcat<" + ftg::type_to_string<Factory>() + std::string(">"))) {}
  virtual ~SpecsConcat() {}

  virtual ftg::TestList tests() const
  {
    ftg::TestList tl;
    tl.push_back(std::make_unique<Many<Factory>>());
    tl.push_back(std::make_unique<Empty<Factory>>());
    return tl;
  }
};

#endif