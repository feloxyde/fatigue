#ifndef FATIGUE_VECTORLOGGER_HPP
#define FATIGUE_VECTORLOGGER_HPP 

#include "Logger.hpp"

namespace ftg {

enum TestStatus {TEST_FINISHED, TEST_SKIPPED, TEST_LOAD_FAILURE, TEST_EXCEPTION_EXIT};

struct VectorTestResult {
  std::vector<TestMessage> messages;
  size_t checkRan;
  size_t checkFailed;
  size_t exceptionSindex;
  size_t exceptionTindex;
  TestStatus status;
  std::string name;
};

struct VectorSuiteLogger;
struct VectorGlobalLogger;
struct VectorTestLogger : public TestLogger
{
  VectorTestLogger(VectorSuiteLogger& parent, std::ostream& stream)
    : m_parent(parent)
    , m_ostream(stream)
    , m_checkRan(0)
    , m_checkFailed(0)
  {}
  virtual ~VectorTestLogger();

  virtual void name(std::string const& name)
  {
    m_name = name;
  }
  
  virtual void message(TestMessage const& log)
  {
    m_messages.push_back(log);
  }

  virtual void uncaughtException(size_t sindex, size_t tindex)
  {
      m_exceptionSindex = sindex;
      m_exceptionTindex = tindex;
      m_status = TEST_EXCEPTION_EXIT;
  }

  virtual void skipped()
  {
    m_status = TEST_SKIPPED;
  }

  virtual void loadFailure()
  {
    m_status = TEST_LOAD_FAILURE;
  }

  virtual void checkPassed() { m_checkRan++; }

  virtual void commit();

  std::vector<TestMessage> const& messages(){return m_messages;}

private:
  VectorSuiteLogger& m_parent;
  std::vector<TestMessage> m_messages;
  size_t m_checkRan;
  size_t m_checkFailed;
  size_t m_exceptionSindex;
  size_t m_exceptionTindex;
  TestStatus m_status;
  std::string m_name;
  std::ostream& m_ostream;
};

struct VectorSuiteResult
{
  std::vector<VectorTestResult> results;
  std::string name;
};

struct VectorSuiteLogger : public SuiteLogger
{
  VectorSuiteLogger(VectorGlobalLogger& parent, std::ostream& ostream) : m_parent(parent), m_ostream(ostream) {}
  virtual ~VectorSuiteLogger(){}
  virtual void name(std::string const& name)
  {
    m_name = name;
  }

  virtual std::unique_ptr<TestLogger> testLogger()
  {
      return std::make_unique<VectorTestLogger>(*this, m_ostream);
  }

  void reportTest(VectorTestResult const& r){
    m_results.push_back(r);
  }

  virtual void commit();

private:
  VectorGlobalLogger& m_parent;
  std::vector<VectorTestResult> m_results;
  std::string m_name;
  std::ostream& m_ostream;
};

struct VectorGlobalLogger : public GlobalLogger
{
  VectorGlobalLogger(std::ostream& ostream)
    : m_ostream(ostream)
  {}

  virtual ~VectorGlobalLogger(){};
  virtual std::unique_ptr<SuiteLogger> suiteLogger()
  {
    return std::make_unique<VectorSuiteLogger>(*this, m_ostream);
  };

  virtual bool passed() {return false;}


  virtual void commit()
  {}

  std::vector<VectorSuiteResult> const& getResults() const;

  void reportSuite(VectorSuiteResult const& r) 
  {
    m_results.push_back(r);
  }

private:
  std::ostream& m_ostream;
  std::vector<VectorSuiteResult> m_results;
};

inline void
VectorTestLogger::commit()
{
  VectorTestResult res;
  res.messages = m_messages;
  res.checkRan = m_checkRan;
  res.checkFailed = m_checkFailed;
  res.exceptionSindex = m_exceptionSindex;
  res.exceptionTindex = m_exceptionTindex;
  res.status = m_status;
  res.name = m_name;
  m_parent.reportTest(res);
};


inline void
VectorSuiteLogger::commit()
{  
  VectorSuiteResult res;
  res.results = m_results;
  res.name = m_name;
  m_parent.reportSuite(res);
}

}

#endif 