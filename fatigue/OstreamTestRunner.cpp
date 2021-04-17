#include "OstreamTestRunner.hpp"
#include "fatigue/Config.hpp"


namespace ftg {

OstreamTestLogger::OstreamTestLogger(std::ostream& ostream) : m_ostream(ostream),  m_passed(true), m_checkPassed(0), m_checkFailed(0){}
OstreamTestLogger::~OstreamTestLogger(){}

void OstreamTestLogger::checkFailed(MessageMode mode, std::string const& description, std::vector<ParamInfo> const& params, bool expected, bool result, bool important)
  {
    m_checkFailed++;
    
    if(important){
      m_ostream << "!!! ";
    }

    m_ostream << "(" << m_checkFailed + m_checkPassed << ") ";

    if(mode == MESSAGE_CHECK){
      m_ostream << "[ERROR] ";
      m_passed = false;
    } else if (mode == MESSAGE_FATAL){
      m_passed = false;
      m_ostream << "[FATAL] ";
    } else if (mode == MESSAGE_WARN){
      m_ostream << "[WARN] ";
    }
    m_ostream << "expected ";
    m_ostream << description;
    
    if(params.size() > 0){
      m_ostream << "( ";
      size_t i = 0;
      for(auto const& p : params){
        if(ftg::config().showParamNames){
          m_ostream << p.name << ": ";
        } 
        m_ostream << p.value ;
        if(ftg::config().showParamTypes){
          m_ostream << " [" << p.type << "]";
        }
        if(i < params.size() - 1 ){
          m_ostream << ", ";
        }
        i++;
      }
      m_ostream << " )";
    }

    if(expected && !result){
      m_ostream << " to succeed, but failed." << std::endl;
    } else if (!expected && result) {
      m_ostream << " to fail, but succeeded." << std::endl;
    }
}

void OstreamTestLogger::checkPassed()
{
    m_checkPassed++;
}

bool OstreamTestLogger::passed() const {return m_passed;}


  OstreamTestRunner::OstreamTestRunner(std::ostream& ostream) : m_ostream(ostream){}
 
 OstreamTestRunner::~OstreamTestRunner(){}

unsigned OstreamTestRunner::run(std::vector<std::unique_ptr<Suite>> const& suites)
  {
    m_ostream << "---------------------------" << std::endl;
    m_ostream << "------ RUNNING TESTS ------" << std::endl;
    m_ostream << "---------------------------";
    size_t totalPass = 0;
    size_t totalFailed = 0;
    size_t totalSkipped = 0;
    
    //running suites 
    for(auto& s : suites){
      //if entire suite is skipped, we dont display individual tests
      size_t countSkip = 0;
      auto tests = s->tests();
      for (auto& t : tests){
        if(!ftg::config().filter.shouldRun(s->name(), t->name())){
          countSkip++;
        }
      }
      if(countSkip == tests.size()){
        m_ostream << std::endl << std::endl << std::endl;
        m_ostream << "##### "  << s->name() << " ##### (skipped)";
        totalSkipped += countSkip;
        continue;
      }

      m_ostream << std::endl << std::endl << std::endl;
      m_ostream << "##### "  << s->name() << " #####";
      
      for (auto& t : tests){
        if(!ftg::config().filter.shouldRun(s->name(), t->name())){
          m_ostream  << std::endl << std::endl << "-- " << t->name() << " -- (skipped)"  << std::endl;
          totalSkipped++;
          continue;
        }
        m_ostream  << std::endl << std::endl << "-- " << t->name() << " --"  << std::endl;
        
        OstreamTestLogger otl(m_ostream);
        t->setLogger(&otl);
        
        if(t->load()){
          if (this->runLoadedTest(t, otl)){
            totalPass++;
          } else {
            totalFailed++;
          }
        } else {
          m_ostream << "-- failed : error during load phase --";
          totalFailed++;
        }
      }
    } 
    
    m_ostream << std::endl << std::endl << std::endl << "---------------------------" << std::endl;
    if(totalFailed == 0){
      m_ostream << "---------- PASSED ---------" << std::endl;
    } else {
      m_ostream << "---------- FAILED ---------" << std::endl;
    }
      m_ostream << "ran : "<< totalPass + totalFailed <<  std::endl; 
    if(totalFailed != 0){
      m_ostream << "failed : "<< totalFailed << std::endl; 
    }
    if(totalSkipped != 0){
      m_ostream << "skipped : " << totalSkipped << std::endl;
    }
    m_ostream << "---------------------------" << std::endl;
  
    return totalFailed;
  }

bool OstreamTestRunner::runLoadedTest(std::unique_ptr<Test> & t, OstreamTestLogger& otl)
  {
      bool exceptPass = true;
      bool passed = true;
      try {
        t->run();
        t->unload();
      }
      catch (ftg::FatalCheckFailure & e){
        m_ostream << "Test ended due to fatal check failing." << std::endl;
        t->unload();            
      }
      catch (...) {
        m_ostream << "[EXCEPTION] uncaught exception detected, test ending." << std::endl;
        exceptPass = false;
        t->unload();
      }

      if(exceptPass && otl.passed()){
        m_ostream << "-- passed : ";
        passed = true;
      } else {
        m_ostream << "-- failed : ";
        passed = false;
      }

      m_ostream << "out of " << otl.m_checkPassed + otl.m_checkFailed << " checks, " << otl.m_checkFailed << " failed. --";
      return passed;
  }



}
