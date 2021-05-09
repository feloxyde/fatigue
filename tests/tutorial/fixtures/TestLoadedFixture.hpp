#ifndef FATIGUE_TUTORIAL_TESTLOADEDFIXTURE_HPP
#define FATIGUE_TUTORIAL_TESTLOADEDFIXTURE_HPP

#include <fatigue/Test.hpp>

struct HeavyFixture {
  int a = 10;
};

class TestLoadedFixture : public ftg::Test {
public:
  TestLoadedFixture() : ftg::Test("TestLoadedFixture") {}
  virtual ~TestLoadedFixture() {}

  virtual bool load() noexcept
  {
    fixture = std::make_unique<HeavyFixture>();

    //we return true if the load succeeded
    return fixture != nullptr;
  }

  virtual void unload() noexcept
  {
    //directly deleting the fixture
    fixture.reset();
  }

  virtual void run() { check_equal(10, fixture->a); }

protected:
  std::unique_ptr<HeavyFixture> fixture;
};


#endif