#ifndef FATIGUE_TUTORIAL_TESTAVGONE_HPP
#define FATIGUE_TUTORIAL_TESTAVGONE_HPP

#include "avg.hpp"
#include <fatigue/Test.hpp>

class One : public ftg::Test {
public:
  One() : ftg::Test("One") {}
  virtual ~One() {}

  virtual void run()
  {
    std::vector<float> f; //empty vector
    f.push_back(1.0);
    check_equal(avg(f), 1.0); //checking our function
  }
};

#endif