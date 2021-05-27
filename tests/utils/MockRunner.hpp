#ifndef FATIGUE_TESTS_UTILS_MOCKRUNNER_HPP
#define FATIGUE_TESTS_UTILS_MOCKRUNNER_HPP

#include "fatigue/runners/DefaultRunner.hpp"
#include <fatigue/Runner.hpp>


using namespace ftg;


struct LogItem {
  LogItem();
  virtual ~LogItem();
};

struct MockLogger : public Logger {
  MockLogger() {}
  virtual ~MockLogger() {}
  virtual void report(Message const& report) { log.push_back(report); }

  size_t countCheckPassed()
  {
    size_t count = 0;
    for (auto const& l : log) {
      if (l.result == l.expected) {
	count++;
      }
    }
    return count;
  }

  size_t countCheckFailed()
  {
    size_t count = 0;
    for (auto const& l : log) {
      if (l.result != l.expected) {
	count++;
      }
    }
    return count;
  }

  std::vector<Logger::Message> log;
};

inline std::ostream& operator<<(std::ostream& out, Logger::Message const& message)
{
  if (message.mode == ftg::Logger::Message::MESSAGE_CHECK) {
    out << "[CHECK]";
  } else if (message.mode == ftg::Logger::Message::MESSAGE_WARN) {
    out << "[WARN]";
  }

  out << " " << message.description << " ";

  out << "(";
  bool first = true;
  for (auto const& p : message.params) {
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }
    out << p.name << " [" << p.type << "] : " << p.value;
  }
  out << ")";

  out << " result:" << message.result;
  out << ", expected:" << message.expected;
  out << ", important:" << message.important;
  out << std::endl;

  return out;
}

inline std::ostream& operator<<(std::ostream& out, MockLogger const& logger)
{
  for (auto const& m : logger.log) {
    out << m;
  }
  return out;
}

/* return empty string if pass, error string otherwise */
inline std::string compareMockLoggers(MockLogger const& result, MockLogger const& expected)
{
  std::stringstream out;

  size_t i = 0;
  while (i < result.log.size() && i < expected.log.size() && result.log[i] == expected.log[i]) {
    i++;
  }

  if (i < result.log.size() || i < expected.log.size()) {
    out << "elements at position " << i << " mismatch : " << std::endl;
    out << "found    : ";
    if (i < result.log.size()) {
      out << result.log[i];
    } else {
      out << "nothing (log ended)";
    }
    out << std::endl;
    out << "expected : ";
    if (i < expected.log.size()) {
      out << expected.log[i];
    } else {
      out << "nothing (log ended)";
    }
    out << std::endl;
  }

  return out.str();
}

#endif