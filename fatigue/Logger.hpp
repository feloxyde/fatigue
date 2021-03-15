#ifndef FATIGUE_LOGGER_HPP
#define FATIGUE_LOGGER_HPP

#include <memory>

namespace ftg {

enum MessageMode
{
  MESSAGE_CHECK,
  MESSAGE_WARN,
  MESSAGE_FATAL,
  MESSAGE_INFO
};

struct TestMessage final
{
  TestMessage(size_t sindex,
              size_t tindex,
              MessageMode mode,
              std::string const& description,
              bool important)
    : m_description(description)
    , m_mode(mode)
    , m_important(important)
    , m_sindex(sindex)
    , m_tindex(tindex)
  {}

  bool isImportant() const { return m_important; }

  std::string const& description() const { return m_description; }
  MessageMode mode() const { return m_mode; }

  size_t sindex() const { return m_sindex; }

  size_t tindex() const { return m_tindex; }

private:
  std::string m_description;
  MessageMode m_mode;
  bool m_important;
  size_t m_sindex;
  size_t m_tindex;
};

struct Logger
{
  // virtual bool supportsParallelism() const { return false; }
};

struct TestLogger : public logger
{
  virtual ~TestLogger() = 0;
  virtual void name(std::string const& name) = 0;
  virtual void message(TestMessage const& log) = 0;
  virtual void uncaughtException(size_t sindex, size_t tindex) = 0;
  virtual void skipped() = 0;
  virtual void loadFailure() = 0;
  virtual void checkPassed() = 0;

  virtual void commit();
};

struct SuiteLogger : public Logger
{
  virtual ~SuiteLogger() = 0;
  virtual void name(std::string const& name) = 0;
  virtual std::unique_ptr<TestLogger> testLogger() = 0;
  virtual void commit() = 0;
};

struct GlobalLogger : public Logger
{
  virtual ~GlobalLogger() = 0;
  virtual std::unique_ptr<SuiteLogger> suiteLogger() = 0;
  virtual bool passed() = 0;
  virtual void commit() = 0;
};

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
  virtual void message(TestMessage const& log)
  {
    std::string msg;
    if (log.important) {
      msg += "!!! ";
    }

    if (log.mode == MESSAGE_CHECK) {
      msg += "[ERROR] ";
      m_failed = true;
      m_checkFailed++;
      m_checkRan++;
    } else if (log.mode == MESSAGE_WARN) {
      msg += "[WARNING] ";
      m_checkFailed++;
      m_checkRan++;
    } else if (log.mode == MESSAGE_FATAL) {
      m_failed = true;
      m_checkFailed++ msg += "[FATAL] ";
      m_checkRan++;
    }
    msg += "<";
    msg += log.sindex;
    msg += ";";
    msg += log.tindex;
    msg += "> ";
    msg += log.description;
    m_ostream << msg << std::endl;
  }

  virtual void uncaughtException(size_t sindex, size_t tindex)
  {
    std::string msg;
    msg += "Test failed due to an uncaught exception at or after <";
    msg += sindex;
    msg += ";";
    msg += tindex;
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
  virtual void checkPassed() { m_checkRan++ }

private:
  OstreamSuiteLogger& m_parent;
  std::vector<std::string> m_messages;
  bool m_failed;
  size_t m_checkRan;
  size_t m_checkFailed;
  std::ostream& m_ostream;
}

struct OstreamSuiteLogger : public SuiteLogger
{
  OstreamSuiteLogger(OstreamGlobalLogger& parent);
  virtual ~SuiteLogger(){};
  virtual void name(std::string const& name)
  {
    m_ostream << std::endl << std::endl << std::endl;
    m_ostream << "###### " << name << " ######" << std::endl;
  };

  virtual std::unique_ptr<TestLogger> testLogger()
  {
      return std::make_unique<TestLogger)(*this, m_ostream);
  };

  void testFailed()
  {
    m_testRan++;
    m_testFailed++;
    m_failed = true;
  }
  void testPassed() { m_testRan++; }

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
    , m_ran(0);
  {}

  virtual ~GlobalLogger() = 0;
  virtual std::unique_ptr<SuiteLogger> suiteLogger()
  {
    return std::make_unique<suiteLogger>(*this, m_ostream);
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