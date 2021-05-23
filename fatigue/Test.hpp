// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

/**

*/

/** @file */


#ifndef FATIGUE_TEST_HPP
#define FATIGUE_TEST_HPP

#include "Checker.hpp"
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

/** @addtogroup user_api
@{ 
*/

// clang-format off
template <typename T> concept OutStreamable = requires(std::ostream &a, T const& b) {
  { a << b } ->std::same_as<std::ostream &>;
};

/** @brief used to by checks to convert argument values into string. 

    By default, it will try to use ```std::stringstream::operator<<( const& T)```,
    however template can be specialized if you want to use a custom conversion. 
*/
template <typename T> std::string to_string(T const &value);

/** @brief  Specialization of ftg::to_string using ```std::stringstream::operator<<( const& T)```. */
template <OutStreamable T> std::string to_string(T const &value)
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}

template <typename T> concept ToStringable = requires(T const& a){
  { ftg::to_string(a) } -> std::same_as<std::string>;
};

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

/** @brief Test class, meant to be derived from to implement test, providing checks. */
class Test : public Checker {
public:
  /** @brief Constructor of a test */
  Test(std::string const& name);
  virtual ~Test();

public:
  /** @brief Method meant to be overriden, which is the test execution scenario. */
  virtual void run() = 0;

  /** @brief Method meant to be optionally overriden, which is run before execution of scenario. */
  virtual bool load() noexcept { return true; }

  /** @brief Method meant to be optionally overriden, which is run after execution of scenario. */
  virtual void unload() noexcept {}

protected:
  // clang-format off

  /**
  \defgroup checks Checks
  @{
  */

  /** @brief Checks ```b == true``` */
  Check check_true(bool b);

  /** @brief Checks ```b == true``` */
  Check check_true(bool b, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());
  
  /** @brief Checks ```b == false``` */
  Check check_false(bool b);

  /** @brief Checks ```b == false``` */
  Check check_false(bool b, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left == right``` */
  template <ToStringable T, ToStringable U>
  requires EQComparable<T, U>
  Check check_equal(T const& left, U const& right);

  /** @brief Checks ```left == right``` */
  template <typename T, typename U>
  requires EQComparable<T, U>
  Check check_equal(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());
  
  /** @brief Checks ```right - tolerance <= left && right + tolerance >= left``` */
  template <ToStringable T, ToStringable U, ToStringable V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  Check check_near_equal(T const& left, U const& right, V const& tolerance);

  /** @brief Checks ```right - tolerance <= left && right + tolerance >= left``` */
  template <typename T, typename U, typename V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  Check check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left != right``` */
  template <ToStringable T, ToStringable U>
  requires NEComparable<T, U>
  Check check_not_equal(T const& left, U const& right);
  
  /** @brief Checks ```left != right``` */
  template <typename T, typename U>
  requires NEComparable<T, U>
  Check check_not_equal(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left < right``` */
  template <ToStringable T, ToStringable U>
  requires LTComparable<T, U>
  Check check_less_than(T const& left, U const& right);

  /** @brief Checks ```left < right``` */
  template <typename T, typename U>
  requires LTComparable<T, U>
  Check check_less_than(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left <= right``` */
  template <ToStringable T, ToStringable U>
  requires LEComparable<T, U>
  Check check_less_equal(T const& left, U const& right);

  /** @brief Checks ```left <= right``` */
  template <typename T, typename U>
  requires LEComparable<T, U>
  Check check_less_equal(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left > right``` */
  template <ToStringable T, ToStringable U>
  requires GTComparable<T, U>
  Check check_greater_than(T const& left, U const& right);

  /** @brief Checks ```left > right``` */
  template <typename T, typename U>
  requires GTComparable<T, U>
  Check check_greater_than(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());

  /** @brief Checks ```left >= right``` */
  template <ToStringable T, ToStringable U>
  requires GEComparable<T, U>
  Check check_greater_equal(T const& left, U const& right);

  /** @brief Checks ```left >= right``` */
  template <typename T, typename U>
  requires GEComparable<T, U>
  Check check_greater_equal(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());  
  
  /** @brief Checks that expression throws 

      ```cpp
      try {
        T();
      } catch (Except &){
        //check passes
      }
      //check fails
      ```
  */
  template <typename Except, LambdaProxy T>
  Check check_throw(T const& expr);  
  
  /** @brief Checks that expression throws 

      ```cpp
      try {
        T();
      } catch (Except &){
        //check passes
      }
      //check fails
      ```
  */
  template <typename Except, LambdaProxy T>
  Check check_throw(T const& expr, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());  
  
  /** @brief Checks that expression doesn't throw 

      ```cpp
      try {
        T();
      } catch (...){
        //check fails
      }
      //check passes
      ```
  */
  template <LambdaProxy T>
  Check check_nothrow(T const& expr);

  /** @brief Checks that expression doesn't throw 
      ```cpp
      try {
        T();
      } catch (...){
        //check fails
      }
      //check passes
      ```
  */
  template <LambdaProxy T>
  Check check_nothrow(T const& expr, std::string const& description, std::vector<ParamInfo> const& params = std::vector<ParamInfo>());
  // clang-format on
  /**
  @}
  */


private:
  /** @brief Used for parameters report formatting */
  template<ToStringable T, ToStringable U>
  std::vector<ParamInfo> formatBinaryCheck(T const& left, U const& right);

  /** @brief Used for parameters report formatting */
  template<ToStringable T, ToStringable U, ToStringable V>
  std::vector<ParamInfo> formatBinaryTolerantCheck(T const& left, U const& right, V const& tolerance);
};

/**
@}
*/

} // namespace ftg


/** OTHER METHODS IMPLEMENTATION */

namespace ftg {


inline Test::Test(std::string const& name) : Checker(name)
{
}

inline Test::~Test()
{
}

template<ToStringable T, ToStringable U>
std::vector<ParamInfo> Test::formatBinaryCheck(T const& left, U const& right)
{
  std::vector<ParamInfo> params;
  params.emplace_back("l", ftg::to_string(left), type_to_string<T>());
  params.emplace_back("r", ftg::to_string(right), type_to_string<U>());
  return params;
}

template<ToStringable T, ToStringable U, ToStringable V>
std::vector<ParamInfo> Test::formatBinaryTolerantCheck(T const& left, U const& right, V const& tolerance)
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
  return check_true(b, "check_true", std::vector<ParamInfo>());
}

inline Check Test::check_true(bool b, std::string const& description, std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, b);
}

inline Check Test::check_false(bool b)
{
  return check_false(b, "check_false", std::vector<ParamInfo>());
}

inline Check Test::check_false(bool b, std::string const& description, std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, !b);
}

template<ToStringable T, ToStringable U>
requires EQComparable<T, U> Check Test::check_equal(T const& left, U const& right)
{
  return check_equal(left, right, "check_equal", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires EQComparable<T, U> Check
Test::check_equal(T const& left, U const& right, std::string const& description, std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left == right);
}

template<ToStringable T, ToStringable U, ToStringable V>
requires LEComparable<T, U>&& GEComparable<T, U>&& Addable<U, V>&& Subbable<U, V>
    Check Test::check_near_equal(T const& left, U const& right, V const& tolerance)
{
  return check_near_equal(left,
			  right,
			  tolerance,
			  "check_near_equal",
			  formatBinaryTolerantCheck(left, right, tolerance));
}

template<typename T, typename U, typename V>
requires LEComparable<T, U>&& GEComparable<T, U>&& Addable<U, V>&& Subbable<U, V>
    Check Test::check_near_equal(T const& left,
				 U const& right,
				 V const& tolerance,
				 std::string const& description,
				 std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left <= right + tolerance && left >= right - tolerance);
}

template<ToStringable T, ToStringable U>
requires NEComparable<T, U> Check Test::check_not_equal(T const& left, U const& right)
{
  return check_not_equal(left, right, "check_not_equal", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires NEComparable<T, U> Check Test::check_not_equal(T const& left,
							U const& right,
							std::string const& description,
							std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left != right);
}

template<ToStringable T, ToStringable U>
requires LTComparable<T, U> Check Test::check_less_than(T const& left, U const& right)
{
  return check_less_than(left, right, "check_less_than", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires LTComparable<T, U> Check Test::check_less_than(T const& left,
							U const& right,
							std::string const& description,
							std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left < right);
}

template<ToStringable T, ToStringable U>
requires LEComparable<T, U> Check Test::check_less_equal(T const& left, U const& right)
{
  return check_less_equal(left, right, "check_less_equal", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires LEComparable<T, U> Check Test::check_less_equal(T const& left,
							 U const& right,
							 std::string const& description,
							 std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left <= right);
}

template<ToStringable T, ToStringable U>
requires GTComparable<T, U> Check Test::check_greater_than(T const& left, U const& right)
{
  return check_greater_than(left, right, "check_greater_than", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires GTComparable<T, U> Check Test::check_greater_than(T const& left,
							   U const& right,
							   std::string const& description,
							   std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left > right);
}

template<ToStringable T, ToStringable U>
requires GEComparable<T, U> Check Test::check_greater_equal(T const& left, U const& right)
{
  return check_greater_equal(left, right, "check_greater_equal", formatBinaryCheck(left, right));
}

template<typename T, typename U>
requires GEComparable<T, U> Check Test::check_greater_equal(T const& left,
							    U const& right,
							    std::string const& description,
							    std::vector<ParamInfo> const& params)
{
  return raw_check(description, params, left >= right);
}


template<typename Except, LambdaProxy T>
Check Test::check_throw(T const& expr)
{
  std::cout << "passing down throw" << std::endl;
  return check_throw<Except>(expr,
			     std::string("check_throw<") + type_to_string<Except>() + std::string(">"),
			     std::vector<ParamInfo>());
}


template<typename Except, LambdaProxy T>
Check Test::check_throw(T const& expr, std::string const& description, std::vector<ParamInfo> const& params)
{
  bool thrown = false;
  try {
    expr();
  } catch (Except& e) {
    thrown = true;
  }
  return raw_check(description, params, thrown);
}

template<LambdaProxy T>
Check Test::check_nothrow(T const& expr)
{
  return check_nothrow(expr, "check_nothrow", std::vector<ParamInfo>());
}


template<LambdaProxy T>
Check Test::check_nothrow(T const& expr, std::string const& description, std::vector<ParamInfo> const& params)
{
  bool nothrown = true;
  try {
    expr();
  } catch (...) {
    nothrown = false;
  }
  return raw_check(description, params, nothrown);
}

} // namespace ftg

#endif
