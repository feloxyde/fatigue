#ifndef FATIGUE_TESTDRIVER_HPP
#define FATIGUE_TESTDRIVER_HPP
#include "utils.hpp"
#include <concepts>
#include <cstddef>
#include <exception>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace ftg {

//#FIXME add a decoupled "output" (ReportDisplay ?) class for message to allow
// user to
// customize its output !

enum MessageMode { MESSAGE_CHECK, MESSAGE_WARN, MESSAGE_FATAL, MESSAGE_INFO };

struct LogMessage {
  LogMessage(size_t sindex, size_t tindex, MessageMode mode,
                              std::string const &description, bool important) :
  m_description(description), m_mode(mode), m_important(important), m_sindex(sindex), m_tindex(tindex)
  {}

  bool isImportant() const {
    return m_important;
  }

  std::string const &description() const
  {
    return m_description;
  }
  MessageMode mode() const
  {
    return m_mode;
  }

  size_t sindex() const
  {
    return m_sindex;
  }
  
  size_t tindex() const{
  return m_tindex;
}

private:
  std::string m_description;
  MessageMode m_mode;
  bool m_important;
  size_t m_sindex;
  size_t m_tindex;
};

struct ReportDisplay {
  virtual void operator<<(LogMessage const &message) = 0;
};

//#FIXME count checks when they pass/fail, and report number in case of uncaught
// exception.
typedef std::string TestId;

struct FatalAssertionExit {
    FatalAssertionExit(){};
    virtual const char *what() {
      return "Fatal assertion exit : test execution stopped because a fatal "
             "check was not met";
    }
};

class TestDriver;

class CheckReporter {
  private:
    std::string m_description;
    MessageMode m_mode;
    bool m_res;
    bool m_expected;
    bool m_important;
    TestDriver &m_test;
    bool m_reported;

  public:
     CheckReporter(TestDriver &test, std::string const &description, bool res): 
      m_test(test), m_description(description), m_res(res),
      m_mode(MESSAGE_CHECK), m_expected(true), m_important(false),
      m_reported(false) {}
    
    ~CheckReporter() { report(); }

    void warn() { m_mode = MESSAGE_WARN; }

    void fatal() {
      m_mode = MESSAGE_FATAL;
      report();
      if(m_res != m_expected){
        throw FatalAssertionExit();
      }
    }
    
    CheckReporter &important() {
      m_important = true;
      return *this;
    }

    CheckReporter &fails() {
      m_expected = false;
      return *this;
    }

    CheckReporter &succeeds() {
      m_expected = true;
      return *this;
    }

  private:
    void report();
  };

class TestDriver {
public:
  TestDriver(): m_showTypes(false), m_directReport(false), m_log(), m_passed(true), m_checkCount(0),
  m_display(nullptr){}
 
public:
  std::vector<LogMessage> const &log() {return m_log;}
  bool passed(){return m_passed;}
  void setReportDisplay(ReportDisplay * rd) {m_display = rd;}
  void setShowTypes(bool show) {m_showTypes = show;}
  void setDirectReport(bool direct){m_directReport = direct;}

protected:
  bool showTypes() const {return m_showTypes;}

private:
  bool m_showTypes;
  bool m_directReport;
  std::vector<LogMessage> m_log;
  bool m_passed;
  size_t m_checkCount;
  ReportDisplay* m_display;

private:
  friend CheckReporter;
};

/* ### IMPLEMENTATION ### */

inline void CheckReporter::report() {
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

      LogMessage lm(m_test.m_checkCount, m_test.m_checkCount, m_mode,
                    msg, m_important);
      m_test.m_log.push_back(lm);

      if (m_test.m_directReport) {
        *(m_test.m_display) << lm;
      }

      if (m_mode == MESSAGE_CHECK || m_mode == MESSAGE_FATAL) {
        m_test.m_passed = false;
      }
      // unveil STACK of INFO (tbi latter)
    }
  }
}

} // namespace ftg

#endif