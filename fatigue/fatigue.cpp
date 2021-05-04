#include "fatigue.hpp"
#include "Runner.hpp"
#include "runners/DefaultRunner.hpp"
#include <memory>

namespace ftg {

fatigue::fatigue(int argc, char** argv) : config()
{
  config.loadFromCLI(argc, argv);
  runners.emplace("default", std::make_unique<DefaultRunner>(std::cout, config));
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
  if(runners.find(config.runner) == runners.end()){
    std::cout << "Could not find runner " << config.runner << std::endl;
    std::cout <<"Available runners are : " << std::endl;
    for(auto const& r : runners){
      std::cout << r.first << std::endl;
    }
    return -1;
  } else {
    return runners.at(config.runner)->run(tests);
  }
}


} // namespace ftg