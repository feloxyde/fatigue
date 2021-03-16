#ifndef FATIGUE_LOGGER_HPP
#define FATIGUE_LOGGER_HPP

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>

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

struct TestLogger : public Logger
{
  virtual ~TestLogger() = 0;
  virtual void name(std::string const& name) = 0;
  virtual void message(MessageMode mode, std::string const& description, bool important) = 0;
  virtual void uncaughtException() = 0;
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



}

#endif