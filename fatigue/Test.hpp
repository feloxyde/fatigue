#ifndef FATIGUE_TEST_HPP
#define FATIGUE_TEST_HPP

#include "Checker.hpp"
#include "TestRunner.hpp"
#include "utils.hpp"
#include <cassert>
#include <concepts>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace ftg {
//#FIXME count checks when they pass/fail, and report number in case of uncaught
// exception.
typedef std::string TestId;

// clang-format off
template <typename T> concept OutStreamable = requires(std::ostream &a, T const& b) {
  { a << b } ->std::same_as<std::ostream &>;
};

template <typename T> std::string to_string(T const &a);

template <OutStreamable T> std::string to_string(T const &a)
{
  std::stringstream ss;
  ss << a;
  return ss.str();
}

template <typename T> concept ToStringable = requires(T const& a){
  { ftg::to_string(a) } -> std::same_as<std::string>;
};

template <typename T> concept Displayable = OutStreamable<T> && ToStringable<T>;

template <typename T, typename U> concept EQComparable = requires(T const& left, U const& right){
  { left == right } ->std::same_as<bool>; 
};

template <typename T, typename U> concept NEComparable = requires(T const& left, U const& right){
  { left != right } ->std::same_as<bool>; 
};

template <typename T, typename U> concept LTComparable = requires(T const& left, U const& right){
  { left < right } ->std::same_as<bool>; 
};

template <typename T, typename U> concept LEComparable = requires(T const& left, U const& right){
  { left <= right } ->std::same_as<bool>; 
};

template <typename T, typename U> concept GTComparable = requires(T const& left, U const& right){
  { left > right } ->std::same_as<bool>; 
};

template <typename T, typename U> concept GEComparable = requires(T const& left, U const& right){
  { left >= right } ->std::same_as<bool>; 
};

template<typename T, typename U> concept Addable = requires (T const& left, U const& right){   
  { left + right } ->std::same_as<T>;
};

template<typename T, typename U> concept Subbable = requires (T const& left, U const& right){   
  { left - right } ->std::same_as<T>;
};

template <typename T> concept LambdaProxy = requires (T const& a){
  {a()}->std::same_as<void>;
};
// clang-format on

class Test : public Checker {
public:
  Test(std::string const& name);
  virtual ~Test();

public:
  virtual void run() = 0;
  virtual bool load() noexcept { return true; }
  virtual void unload() noexcept {}

protected:
  // clang-format off

  Check check_true(bool b);

  Check check_true(bool b, std::string const& description);

  Check check_false(bool b);

  Check check_false(bool b, std::string const& description);

  template <Displayable T, Displayable U>
  requires EQComparable<T, U>
  Check check_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires EQComparable<T, U>
  Check check_equal(T const& left, U const& right, std::string const& description);
  
  template <Displayable T, Displayable U, Displayable V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  Check check_near_equal(T const& left, U const& right, V const& tolerance);

  template <typename T, typename U, typename V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  Check check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& description);

  template <Displayable T, Displayable U>
  requires NEComparable<T, U>
  Check check_not_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires NEComparable<T, U>
  Check check_not_equal(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires LTComparable<T, U>
  Check check_less_than(T const& left, U const& right);

  template <typename T, typename U>
  requires LTComparable<T, U>
  Check check_less_than(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires LEComparable<T, U>
  Check check_less_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires LEComparable<T, U>
  Check check_less_equal(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires GTComparable<T, U>
  Check check_greater_than(T const& left, U const& right);

  template <typename T, typename U>
  requires GTComparable<T, U>
  Check check_greater_than(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires GEComparable<T, U>
  Check check_greater_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires GEComparable<T, U>
  Check check_greater_equal(T const& left, U const& right, std::string const& description);  

 
  template <typename Except, LambdaProxy T>
  Check check_throw(T const& expr);  

  template <typename Except, LambdaProxy T>
  Check check_throw(std::string const& description, T const& expr);  
  
  template <LambdaProxy T>
  Check check_nothrow(T const& expr);

  template <LambdaProxy T>
  Check check_nothrow(std::string const& description, T const& expr);
//clang-format on

private: 

  template<Displayable T, Displayable U>
  std::vector<ParamInfo> formatBinaryCheck(T const& left, U const& right);

  template<Displayable T, Displayable U, Displayable V>
  std::vector<ParamInfo> formatBinaryTolerantCheck(T const& left, U const& right, V const& tolerance);
};

/** OTHER METHODS IMPLEMENTATION */

inline Test::Test(std::string const& name) : Checker(name){}

inline Test::~Test(){}

template<Displayable T, Displayable U>
std::vector<ParamInfo>  Test::formatBinaryCheck(T const& left, U const& right){
  std::vector<ParamInfo> params;
  params.emplace_back("l", ftg::to_string(left), type_to_string<T>());
  params.emplace_back("r", ftg::to_string(right), type_to_string<U>());
  return params;
}

template<Displayable T, Displayable U, Displayable V>
std::vector<ParamInfo>  Test::formatBinaryTolerantCheck(T const& left, U const& right, V const& tolerance)
{
  std::vector<ParamInfo> params;
  params.emplace_back("l", ftg::to_string(left), type_to_string<T>());
  params.emplace_back("r", ftg::to_string(right), type_to_string<U>());
  params.emplace_back("tolerance", ftg::to_string(tolerance), type_to_string<V>());
  return params;
}


/* CHECK IMPLEMENTATIONS */
inline Check Test::check_true(bool b)
{
  return Check(*this, "check_true", std::vector<ParamInfo>(), b);
}

inline Check Test::check_true(bool b, std::string const& description)
{
  return Check(*this, description, std::vector<ParamInfo>(), b);
}

inline Check Test::check_false(bool b)
{
  return Check(*this, "check_false", std::vector<ParamInfo>(), !b);
}
  
inline Check Test::check_false(bool b, std::string const& description)
{
  return Check(*this, description, std::vector<ParamInfo>(), !b);
}

template <Displayable T, Displayable U>
requires EQComparable<T, U>
Check Test::check_equal(T const& left, U const& right)
{
  return Check(*this, "check_equal",  formatBinaryCheck(left, right), left == right);
}

template <typename T, typename U>
requires EQComparable<T, U>
Check Test::check_equal(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description,  std::vector<ParamInfo>(), left == right);
}
  
template <Displayable T, Displayable U, Displayable V>
requires LEComparable<T, U> && GEComparable<T, U>  && Addable<U, V> && Subbable<U, V>
Check Test::check_near_equal(T const& left, U const& right, V const& tolerance)
{
  return Check(*this, "check_near_equal", formatBinaryTolerantCheck(left, right, tolerance), left <= right + tolerance && left >= right - tolerance);
}

template <typename T, typename U, typename V>
requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
Check Test::check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& description)
{
  return Check(*this, description, std::vector<ParamInfo>(), left <= right + tolerance && left >= right - tolerance);
}

template <Displayable T, Displayable U>
requires NEComparable<T, U>
Check Test::check_not_equal(T const& left, U const& right)
{
  return Check(*this, "check_not_equal", formatBinaryCheck(left, right), left != right);
}

template <typename T, typename U>
requires NEComparable<T, U>
Check Test::check_not_equal(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description, std::vector<ParamInfo>(), left != right);
}

template <Displayable T, Displayable U>
requires LTComparable<T, U>
Check Test::check_less_than(T const& left, U const& right)
{
  return Check(*this, "check_less_than", formatBinaryCheck(left, right), left < right);
}

template <typename T, typename U>
requires LTComparable<T, U>
Check Test::check_less_than(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description, left < right);
}

template <Displayable T, Displayable U>
requires LEComparable<T, U>
Check Test::check_less_equal(T const& left, U const& right)
{
  return Check(*this, "check_less_equal", formatBinaryCheck(left, right), left <= right);
}

template <typename T, typename U>
requires LEComparable<T, U>
Check Test::check_less_equal(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description, left <= right);
}

template <Displayable T, Displayable U>
requires GTComparable<T, U>
Check Test::check_greater_than(T const& left, U const& right)
{
  return Check(*this, "check_greater_than", formatBinaryCheck(left, right), left > right);
}

template <typename T, typename U>
requires GTComparable<T, U>
Check Test::check_greater_than(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description, left > right);
}

template <Displayable T, Displayable U>
requires GEComparable<T, U>
Check Test::check_greater_equal(T const& left, U const& right)
{
  return Check(*this, "check_greater_equal", formatBinaryCheck(left, right), left >= right);
}

template <typename T, typename U>
requires GEComparable<T, U>
Check Test::check_greater_equal(T const& left, U const& right, std::string const& description)
{
  return Check(*this, description, left >= right);
}



template <typename Except, LambdaProxy T>
Check Test::check_throw(T const& expr)
{
  bool thrown = false;
  try {
    expr();
  } catch (Except &e) {
    thrown = true;
  }
  return Check(*this, std::string("check_throw<") + type_to_string<Except>() + std::string(">"), std::vector<ParamInfo>(), thrown);
}


template <typename Except, LambdaProxy T>
Check Test::check_throw(std::string const& description, T const& expr)
{
  bool thrown = false;
  try {
    expr();
  } catch (Except &e) {
    thrown = true;
  }
  return Check(*this, description, std::vector<ParamInfo>(), thrown);
}
  
template <LambdaProxy T>
Check Test::check_nothrow(T const& expr)
{
  bool nothrown = true;
  try{
    expr();
  } catch (...){
    nothrown = false;
  }
  return Check(*this, "check_nothrow", std::vector<ParamInfo>(), nothrown);
}


template <LambdaProxy T>
Check Test::check_nothrow(std::string const& description, T const& expr)
{
  bool nothrown = true;
  try{
    expr();
  } catch (...){
    nothrown = false;
  }
  return Check(*this, description, std::vector<ParamInfo>(), nothrown);
}

} // namespace ftg

#endif
