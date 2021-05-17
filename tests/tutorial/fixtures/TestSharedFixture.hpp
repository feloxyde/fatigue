#ifndef FATIGUE_TUTORIAL_TESTSHAREDFIXTURE_HPP
#define FATIGUE_TUTORIAL_TESTSHAREDFIXTURE_HPP

#include <fatigue/Runner.hpp>
#include <fatigue/Suite.hpp>
#include <fatigue/Test.hpp>
#include <string>

struct SharedFixture {
  int a = 10;
};

class TestSharedFixture : public ftg::Test {
public:
  TestSharedFixture(std::shared_ptr<SharedFixture> fix, int index) :
      ftg::Test(std::string("TestSharedFixture:") + std::to_string(index)),
      fixture(fix)
  {
  }
  virtual ~TestSharedFixture() {}

  virtual void run() { check_equal(10, fixture->a); }

protected:
  std::shared_ptr<SharedFixture> fixture;
};


class SuiteSharedFixture : public ftg::Suite {
public:
  /* we use type_to_string to get name of parameters for better display */
  SuiteSharedFixture() : ftg::Suite("SuiteSharedFixture") {}
  virtual ~SuiteSharedFixture() {}

  virtual ftg::TestList tests() const
  {
    ftg::TestList tl;
    auto fixture = std::make_shared<SharedFixture>();
    tl.push_back(std::make_unique<TestSharedFixture>(fixture, 1));
    tl.push_back(std::make_unique<TestSharedFixture>(fixture, 2));
    tl.push_back(std::make_unique<TestSharedFixture>(fixture, 3));
    tl.push_back(std::make_unique<TestSharedFixture>(fixture, 4));
    return tl;
  }
};

#endif