#ifndef FATIGUE_OSTREAMLOGGER_HPP
#define FATIGUE_OSTREAMLOGGER_HPP 

#include "Logger.hpp"

namespace ftg {

struct OstreamSuiteLogger;
struct OstreamGlobalLogger;
struct OstreamTestLogger : public TestLogger
{
  OstreamTestLogger(OstreamSuiteLogger& parent, std::ostream& stream)
    : m_parent(parent)
    , m_ostream(stream)
    , m_failed(false)
    , m_checkRan(0)
    , m_checkFailed(0)
  {}
  virtual ~OstreamTestLogger();

  virtual void name(std::string const& name)
  {
    std::string msg;
    m_ostream << std::endl;
    msg += "-- ";
    msg += name;
    msg += " :";
    m_ostream << msg << std::endl;
  }
  
    virtual void message(MessageMode mode, std::string const& description, bool important)
  {
    std::string msg;
    if (important) {
      msg += "!!! ";
    }

    if (mode == MESSAGE_CHECK) {
      msg += "[ERROR] ";
      m_failed = true;
      m_checkFailed++;
      m_checkRan++;
    } else if (mode == MESSAGE_WARN) {
      msg += "[WARNING] ";
      m_checkFailed++;
      m_checkRan++;
    } else if (mode == MESSAGE_FATAL) {
      msg += "[FATAL] ";
      m_failed = true;
      m_checkFailed++; 
      m_checkRan++;
    }
    msg += "<";
    msg += m_checkRan;
    msg += ";";
    msg += m_checkRan;
    msg += "> ";
    msg += description;
    m_ostream << msg << std::endl;
  }

  virtual void uncaughtException()
  {
    std::string msg;
    msg += "Test failed due to an uncaught exception at or after <";
    msg += m_checkRan;
    msg += ";";
    msg += m_checkRan;
    msg += ">";
    m_ostream << msg << std::endl;
  }

  virtual void skipped()
  {
    std::string msg;
    msg += "skipped.";
  }

  virtual void loadFailure()
  {
    std::string msg;
    msg += "Unable to load test";
    m_ostream << msg << std::endl;
  }

  virtual void checkPassed() { m_checkRan++; }

  virtual void commit();

private:
  OstreamSuiteLogger& m_parent;
  std::vector<std::string> m_messages;
  bool m_failed;
  size_t m_checkRan;
  size_t m_checkFailed;
  std::ostream& m_ostream;
};

struct OstreamSuiteLogger : public SuiteLogger
{
  OstreamSuiteLogger(OstreamGlobalLogger& parent, std::ostream& ostream) : m_parent(parent), m_ostream(ostream), m_failed(false), m_testFailed(0), m_testRan(0){}
  virtual ~OstreamSuiteLogger(){}
  virtual void name(std::string const& name)
  {
    m_ostream << std::endl << std::endl << std::endl;
    m_ostream << "###### " << name << " ######" << std::endl;
  }

  virtual std::unique_ptr<TestLogger> testLogger()
  {
      return std::make_unique<OstreamTestLogger>(*this, m_ostream);
  }

  void testFailed()
  {
    m_testRan++;
    m_testFailed++;
    m_failed = true;
  }

  void testPassed() { m_testRan++; }

  virtual void commit();

private:
  OstreamGlobalLogger& m_parent;
  bool m_failed;
  size_t m_testRan;
  size_t m_testFailed;
  std::ostream& m_ostream;
};

struct OstreamGlobalLogger : public GlobalLogger
{
  OstreamGlobalLogger(std::ostream& ostream)
    : m_ostream(ostream)
    , m_failed(0)
    , m_ran(0)
  {}

  virtual ~OstreamGlobalLogger(){};
  virtual std::unique_ptr<SuiteLogger> suiteLogger()
  {
    return std::make_unique<OstreamSuiteLogger>(*this, m_ostream);
  };

  virtual bool passed() { return m_failed > 0; };
  virtual void commit()
  {
    m_ostream << std::endl << std::endl << std::endl;
    if (m_failed > 0) {
      m_ostream << m_failed << " TESTS FAILED OUT OF " << m_ran << std::endl;
    } else {
      m_ostream << m_ran << " TESTS SUCCESSFULLY PASSED " << std::endl;
    }
  };

  void reportTestRuns(size_t failed, size_t ran)
  {
    m_failed += failed;
    m_ran += ran;
  }

private:
  std::ostream& m_ostream;
  size_t m_failed;
  size_t m_ran;
};

inline void
OstreamTestLogger::commit()
{
  std::string msg;
  if (m_failed) {
    msg += "FAIL ";
    m_parent.testFailed();
  } else {
    msg += "PASS ";
    m_parent.testPassed();
  }
  msg += m_checkFailed;
  msg += "checks failed out of ";
  msg += m_checkRan;
  msg += " ran";
  m_ostream << msg << std::endl;
}

inline void
OstreamSuiteLogger::commit()
{
  m_parent.reportTestRuns(m_testFailed, m_testRan);
}

}

#endif 