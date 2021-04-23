#include "fatigue/Suite.hpp"
#include "fatigue/TestRunner.hpp"
#include <fatigue/fatigue.hpp>
#include <memory>
#include <string>

using namespace ftg;

template<int num, bool pass>
struct MockTest : public ftg::Test {
  MockTest() : Test(std::string("MockTest") + std::to_string(num) + ":" + std::to_string(pass)) {}
  virtual ~MockTest() {}
  virtual void run()
  {
    if constexpr (pass) {
      check_equal(10, 10);
    } else {
      check_equal(10, 20.0);
    }
  }
};

struct Suite1 : ftg::Suite {
  Suite1() : Suite("suite1") {}
  virtual ~Suite1() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::move(std::make_unique<MockTest<1, false>>()));
    tl.push_back(std::move(std::make_unique<MockTest<4, true>>()));

    return tl;
  }
};

struct Suite3 : ftg::Suite {
  Suite3() : Suite("suite3") {}
  virtual ~Suite3() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::move(std::make_unique<MockTest<1, true>>()));
    tl.push_back(std::move(std::make_unique<MockTest<4, true>>()));

    return tl;
  }
};

template<int num>
struct Suite2 : ftg::Suite {
  Suite2() : Suite("suite2:" + std::to_string(num)) {}
  virtual ~Suite2() {}

  virtual TestList tests() const
  {
    TestList tl;
    tl.push_back(std::move(std::make_unique<MockTest<1 + num, true>>()));
    tl.push_back(std::move(std::make_unique<MockTest<2 + num, false>>()));
    tl.push_back(std::move(std::make_unique<MockTest<3 + num, false>>()));
    return tl;
  }
};

TestList buildSuites()
{
  TestList tl;
  tl.push_back(std::make_unique<Suite2<0>>());
  tl.push_back(std::make_unique<Suite2<10>>());
  tl.push_back(std::make_unique<Suite2<20>>());
  return tl;
}

int main(int argc, char** argv)
{
  return fatigue(argc, argv)
      .declare(std::make_unique<Suite1>())
      .declare(buildSuites)
      .declare(std::make_unique<Suite3>())
      .run();
}