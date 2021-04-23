#include "fatigue.hpp"
#include "fatigue/TestRunner.hpp"

namespace ftg {

TestList FatigueDriver::tests = TestList();

FatigueDriver FatigueDriver::declare(std::unique_ptr<Suite> suite) const
{
  tests.push_back(std::move(suite));
  return FatigueDriver();
}

FatigueDriver FatigueDriver::declare(std::unique_ptr<Test> test) const
{
  tests.push_back(std::move(test));
  return FatigueDriver();
}

unsigned FatigueDriver::run() const
{
  assert(config().runner);
  return config().runner->run(tests);
};

FatigueDriver fatigue(int argc, char** argv)
{
  config().loadFromCLI(argc, argv);
  return FatigueDriver();
}

} // namespace ftg