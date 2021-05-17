#ifndef FATIGUE_TUTORIAL_TESTAVGMANY_HPP
#define FATIGUE_TUTORIAL_TESTAVGMANY_HPP

#include "avg.hpp"
#include <fatigue/Test.hpp>
#include <vector>


class Many : public ftg::Test {
public:
  Many() : ftg::Test("Many") {}
  virtual ~Many() {}

  virtual void run()
  {
    std::vector<float> f; //empty vector
    f.push_back(1.0);
    f.push_back(2.0);
    f.push_back(3.0);
    f.push_back(4.0);
    check_equal(avg(f), 2.5); //checking our function
  }
};

#endif