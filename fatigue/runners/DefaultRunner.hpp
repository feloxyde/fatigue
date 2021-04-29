#ifndef FATIGUE_DEFAULTRUNNER_HPP
#define FATIGUE_DEFAULTRUNNER_HPP

#include "../Config.hpp"
#include "../Runner.hpp"
#include "../Suite.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <ostream>

namespace ftg {

struct DefaultLogger : public Logger {

  DefaultLogger(std::ostream& ostream, Config const& config);
  virtual ~DefaultLogger();

  virtual void checkFailed(MessageMode mode,
			   std::string const& description,
			   std::vector<ParamInfo> const& params,
			   bool expected,
			   bool result,
			   bool important);

  virtual void checkPassed();

  bool passed() const;

public:
  Config const& m_config;
  std::ostream& m_ostream;
  bool m_passed;
  size_t m_checkPassed;
  size_t m_checkFailed;
};

struct DefaultRunner : public Runner {
  DefaultRunner(std::ostream& ostream, Config const& config);
  virtual ~DefaultRunner();
  virtual unsigned run(TestList const& tests);


private:
  void dispatchTestList(TestList const& tests, std::vector<std::string> const& prefixes);
  void runSuite(std::unique_ptr<Suite> const& suite, std::vector<std::string> const& prefixes);
  void runTest(std::unique_ptr<Test> const& test, std::vector<std::string> const& prefixes);
  bool runLoadedTest(std::unique_ptr<Test> const& t);

private:
  Config const& m_config;
  std::ostream& m_ostream;
  size_t totalPass;
  size_t totalFailed;
  size_t totalSkipped;
};

} // namespace ftg

#endif