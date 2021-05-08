#include "fatigue.hpp"
#include "Runner.hpp"
#include "runners/DefaultRunner.hpp"
#include <memory>

namespace ftg {

fatigue::fatigue(int argc, char** argv) : fatigue(std::vector<fatigue::OptionsDeclarer>(), argc, argv)
{
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
  if (m_runners.find(m_config.runner) == m_runners.end()) {
    std::cout << "Could not find runner " << m_config.runner << std::endl;
    std::cout << "Available runners are : " << std::endl;
    for (auto const& r : m_runners) {
      std::cout << r.first << std::endl;
    }
    return -1;
  } else {
    return m_runners.at(m_config.runner)->run(tests);
  }
}

/** @brief helper function to register default options */
static void defaultOptions(cxxopts::Options& opts)
{
  // clang-format off
 opts.add_options()
  ("t,showtypes", "show parameter types when displaying checks results", cxxopts::value<bool>()->default_value("false"))
  ("n,shownames", "show parameter names when displaying checks results", cxxopts::value<bool>()->default_value("false"))
  ("r,runner","selects which runner to use to conduct tests", cxxopts::value<std::string>()->default_value("default"))
  ("s,select", "runs tests matching a regular expression", cxxopts::value<std::string>())
  ("e,exclude", "excludes tests matching a regular expression", cxxopts::value<std::string>());
  // clang-format on
}


fatigue::fatigue(std::vector<fatigue::OptionsDeclarer> const& newOpts, int argc, char** argv) : m_config()
{
  std::vector<fatigue::OptionsDeclarer> opts;
  opts.push_back(defaultOptions);
  opts.insert(opts.end(), newOpts.begin(), newOpts.end());

  cxxopts::Options options("Fatigue built test software", "You are supposed to know this btw.");

  for (auto const& o : opts) {
    o(options);
  }

  m_parsedOpts = options.parse(argc, argv);

  m_config.loadFromOpts(m_parsedOpts);

  m_runners.emplace("default", std::make_unique<DefaultRunner>(std::cout, m_config));
}


} // namespace ftg