#ifndef FATIGUE_TUTORIAL_TENTEST_HPP
#define FATIGUE_TUTORIAL_TENTEST_HPP

#include <fatigue/Test.hpp>
#include <string>

class TenTest : public ftg::Test {
public:
  TenTest(std::string name) : ftg::Test(name) {}

protected:
  //our check. Better be it protected scope since we will only access it
  //in run() method of tests deriving from TenTest.

  template<ftg::ToStringable T>
  ftg::Check check_equals_ten(T const& value)
  {
    std::vector<ftg::ParamInfo> params;
    params.emplace_back("value", ftg::to_string(value), ftg::type_to_string<T>());
    return raw_check("check_near_equal", params, value == 10);
  }
};

#endif