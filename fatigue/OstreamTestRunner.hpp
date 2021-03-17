#ifndef FATIGUE_OSTREAMTESTRUNNER_HPP
#define FATIGUE_OSTREAMTESTRUNNER_HPP

#include "TestRunner.hpp"
#include "Suite.hpp"  
#include "fatigue/TestDriver.hpp"
#include <cstddef>
#include <exception>
#include <ostream>


namespace ftg {

struct OstreamTestLogger : public TestLogger {

  OstreamTestLogger(std::ostream& ostream) : m_ostream(ostream), m_checkFailed(0), m_passed(true), m_checkPassed(0){}
  virtual ~OstreamTestLogger(){}

  virtual void checkFailed(MessageMode mode, std::string const& description, bool important)
  {

    m_checkFailed++;
    
    if(important){
      m_ostream << "!!! ";
    }

    m_ostream << "<" << m_checkFailed + m_checkPassed << "> ";

    if(mode == MESSAGE_CHECK){
      m_ostream << "[ERROR] ";
      m_passed = false;
    } else if (mode == MESSAGE_FATAL){
      m_passed = false;
      m_ostream << "[FATAL] ";
    } else if (mode == MESSAGE_WARN){
      m_ostream << "[WARN] ";
    }

    m_ostream << description << std::endl;
  }

  virtual void checkPassed()
  {
    m_checkPassed++;
  }

  bool passed() const {return m_passed;}

public: 
  std::ostream& m_ostream;
  size_t m_checkPassed;
  size_t m_checkFailed;
  bool m_passed;
};

struct OstreamTestRunner : public TestRunner
{
  OstreamTestRunner(std::ostream& ostream) : m_ostream(ostream){}
  virtual ~OstreamTestRunner(){}
  virtual void configure(int argc, const char** argv){/* nothing to do here */}
  virtual void run(std::vector<std::unique_ptr<Suite>> const& suites)
  {
    m_ostream << "---------------------------" << std::endl;
    m_ostream << "------ RUNNING TESTS ------" << std::endl;
    m_ostream << "---------------------------" << std::endl; 
    size_t totalPass = 0;
    size_t totalFailed = 0;
    
    //running suites 
    for(auto& s : suites){
      m_ostream << std::endl << std::endl << std::endl;
      m_ostream << "##### "  << s->name() << " ##### " << std::endl;
      
      for (auto& t : s->tests()){
        m_ostream  << std::endl << "-- " << t->name() << " --";
        OstreamTestLogger otl(m_ostream);
        bool exceptPass = true;
        t->setLogger(&otl);
        if(t->load()){

          try {
          
          } 
          catch (ftg::FatalCheckFailure & e){
            m_ostream << "Test ended due to fatal check failing." << std::endl;
          }
          catch (...) {
            m_ostream << "[EXCEPTION] uncaught exception detected, test ending." << std::endl;
            exceptPass = false;
          }
           
          if(totalPass && otl.passed()){
            m_ostream << "-- passed : ";
            totalPass++;
          } else {
            m_ostream << "-- failed : ";
            totalFailed++;
          }

          m_ostream << "out of " << otl.m_checkPassed + otl.m_checkFailed << " checks, " << otl.m_checkFailed << "failed. --";

        } else {
          m_ostream << "-- failed : error during load phase --" << std::endl;
          totalFailed++;
        }
      }
    } 
    
    m_ostream << "---------------------------" << std::endl;
    if(totalFailed == 0){
      m_ostream << "---------- PASSED ---------" << std::endl;
    } else {
      m_ostream << "---------- FAILED ---------" << std::endl;
    }
      m_ostream << "ran : "<< totalPass + totalFailed <<  std::endl; 
    if(totalFailed != 0){
      m_ostream << "failed : "<< totalFailed << std::endl; 
    }
    m_ostream << "---------------------------" << std::endl;
  }

private:
  std::ostream& m_ostream;
};

}

#endif