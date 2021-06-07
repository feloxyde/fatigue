#ifndef FATIGUE_TESTS_UTILS_COMPARESTRINGS_HPP
#define FATIGUE_TESTS_UTILS_COMPARESTRINGS_HPP

#include <iostream>
#include <istream>
#include <sstream>
#include <string>

//this compares two strings, while maintaining a sliding frame to display sufficient informations
inline bool compareStrings(std::string const& res, std::string const& expected)
{
  std::istringstream rstream(res);
  std::istringstream estream(expected);


  size_t line = 1;
  bool diff = false;
  std::stringstream details;
  bool rfinished = false;
  bool efinished = false;
  while (!rfinished && !efinished) {
    std::string resLine, expLine;
    rfinished = !static_cast<bool>(std::getline(rstream, resLine));
    efinished = !static_cast<bool>(std::getline(estream, expLine));
    if (resLine != expLine || rfinished != efinished) {
      diff = true;
      details << std::endl << "lines " << line << " are different : " << std::endl;
      if (!rfinished) {
	details << "result   : " << resLine << std::endl;
      } else {
	details << "result string ended" << std::endl;
      }
      if (!efinished) {
	details << "expected : " << expLine << std::endl;
      } else {
	details << "expected string ended" << std::endl;
      }
    }
    line++;
  }


  if (diff) {
    std::cout << "The two strings are different : " << std::endl;
    std::cout << "### RESULT ###" << std::endl;
    std::cout << res << std::endl;
    std::cout << "##############" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "### EXPECTED ###" << std::endl;
    std::cout << expected << std::endl;
    std::cout << "################" << std::endl;
    std::cout << std::endl;
    std::cout << details.str() << std::endl;
  }

  return !diff;
}


#endif