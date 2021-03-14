#ifndef FATIGUE_TESTDRIVER_HPP
#define FATIGUE_TESTDRIVER_HPP
#include "utils.hpp"
#include <concepts>
#include <cstddef>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace ftg {

//#FIXME add a decoupled "output" class for message to allow user to
// customize its output !

enum MessageMode { MESSAGE_CHECK, MESSAGE_WARN, MESSAGE_FATAL, MESSAGE_INFO };

struct LogMessage {
  LogMessage(MessageMode mode, std::string const &description, bool important);
  bool isImportant() const;
  std::string const &description() const;
  MessageMode mode();
  friend std::ostream &operator<<(std::ostream &out, LogMessage);

private:
  std::string m_description;
  MessageMode m_mode;
  bool m_important;
};
//#FIXME count checks when they pass/fail, and report number in case of uncaught
// exception.
typedef std::string TestId;

class TestLogger {
public:
  TestLogger();

public:
  class CheckReporter {
  public:
    CheckReporter(TestLogger &test, std::string const &description, bool res);
    ~CheckReporter();

    // used to tell what to do with result
    // void check();  // reports and marks test as failed (default)
    void warn();   // reports and does not marks test as failed
    void fatal();  // reports and exit tests
    void assert(); // same as fatal

    //#FIXME should add an EMPHASE option or something like that to mark answer
    // as important ?
    CheckReporter &important(); // marks assertion fail as important
    // CheckReporter & notImportant(); // marks assertion as not especially
    // important (default)

    // used to tell that result should be inverted
    CheckReporter &
    fails(); // means that check should be considered as OK if res = false
    CheckReporter &succeeds(); // means that check shoube be considered as OK
    // if res = true (default)

  private:
    void report();

  private:
    std::string m_description;
    MessageMode m_mode;
    bool m_res;
    bool m_expected;
    bool m_important;
    TestLogger &m_test;
    bool m_reported;
  };

  struct FatalAssertionExit : public std::exception {
    FatalAssertionExit(){};
    virtual const char *what() {
      return "Fatal assertion exit : test execution stopped because a fatal "
             "check was not met";
    }
  };

public:
  std::vector<LogMessage> const &log();
  bool passed();

protected:
  bool showTypes();

private:
  bool m_showTypes;
  bool m_directReport;
  std::vector<LogMessage> m_log;
  bool m_passed;
  TestId m_name;
  size_t m_checkCount;
};

/** OTHER METHODS IMPLEMENTATION */

inline TestLogger::CheckReporter::CheckReporter(TestLogger &test,
                                                std::string const &description,
                                                bool res)
    : m_test(test), m_description(description), m_res(res),
      m_mode(MESSAGE_CHECK), m_expected(true), m_important(false),
      m_reported(false) {}

inline TestLogger::CheckReporter::~CheckReporter() { report(); }

inline void TestLogger::CheckReporter::report() {
  if (m_reported == false) {
    m_reported = true;
    m_test.m_checkCount++;
    if ((m_res && !m_expected) || (!m_res && m_expected)) {
      std::string msg = "expected ";
      msg += m_description;
      if (m_expected == true) {
        msg += " to succeed, but failed.";
      } else {
        msg += " to fail, but succeeded.";
      }

      LogMessage lm(m_mode, m_description, m_important);
      m_test.m_log.push_back(lm);

      if (m_test.m_directReport) {
        std::cout << lm << std::endl;
      }

      if (m_mode == MESSAGE_CHECK || m_mode == MESSAGE_FATAL) {
        m_test.m_passed = false;
      }
      // unveil STACK of INFO (tbi latter)
    }
  }
}

inline void TestLogger::CheckReporter::warn() { m_mode = MESSAGE_WARN; }

inline void TestLogger::CheckReporter::fatal() {
  m_mode = MESSAGE_FATAL;
  report();
  throw FatalAssertionExit();
}

inline void TestLogger::CheckReporter::assert() { fatal(); }

inline TestLogger::CheckReporter &TestLogger::CheckReporter::important() {
  m_important = true;
  return *this;
}

inline TestLogger::CheckReporter &TestLogger::CheckReporter::fails() {
  m_expected = false;
  return *this;
}

inline TestLogger::CheckReporter &TestLogger::CheckReporter::succeeds() {
  m_expected = true;
  return *this;
}

} // namespace ftg

#endif