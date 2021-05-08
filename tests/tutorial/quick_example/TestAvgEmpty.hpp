#ifndef FATIGUE_TUTORIAL_TESTAVGEMPTY_HPP
#define FATIGUE_TUTORIAL_TESTAVGEMPTY_HPP


#include "avg.hpp"
#include <fatigue/Test.hpp> //inclusion of Test definition

class Empty : public ftg::Test {
public:
  Empty() : ftg::Test("Empty") {}
  virtual ~Empty() {}

  virtual void run()
  {
    std::vector<float> f;     //empty vector
    check_equal(avg(f), 0.0); //checking our function
  }
};


#endif