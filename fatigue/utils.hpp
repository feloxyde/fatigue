#ifndef FATIGUE_UTILS_HPP
#define FATIGUE_UTILS_HPP

namespace ftg {};

#include <iostream>
#include <typeinfo>

#ifdef __GNUG__
#include <cstdlib>
#include <cxxabi.h>
#include <memory>
inline std::string demangle(const char *name) {
  int status = -4; // some arbitrary value to eliminate the compiler warning

  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

  return (status == 0) ? res.get() : name;
}

#else
// does nothing if not g++
std::string demangle(const char *name) { return name; }

#endif

template <typename T> std::string type_to_string() {
  return demangle(typeid(T).name());
}

#endif