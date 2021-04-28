#include "fatigue.hpp"
#include "OstreamTestRunner.hpp"
#include "Runner.hpp"
#include <memory>

namespace ftg {

fatigue::fatigue(int argc, char** argv) : config()
{
  config.loadFromCLI(argc, argv);
}

fatigue& fatigue::declare(std::unique_ptr<Suite> suite)
{
  tests.push_back(std::move(suite));
  return *this;
}

fatigue& fatigue::declare(std::unique_ptr<Test> test)
{
  tests.push_back(std::move(test));
  return *this;
}

unsigned fatigue::run() const
{
  std::unique_ptr<Runner> runner;

  if (config.runner == "default") {
    runner = std::make_unique<OstreamTestRunner>(std::cout, config);
  } else {
    std::cout << "runner " << config.runner << "not found !" << std::endl;
    return -1; //#FIXME should be changed for proper error handling !
  }

  return runner->run(tests);
};


} // namespace ftg