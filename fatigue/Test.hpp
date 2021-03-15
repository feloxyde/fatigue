#ifndef FATIGUE_TEST_HPP
#define FATIQUE_TEST_HPP

#include "TestDriver.hpp"
#include "utils.hpp"
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
// clang-format on

class Test : public TestDriver {
public:
  Test();
  virtual ~Test();
  

public:
  virtual void run() = 0;
  virtual TestId name() = 0;
  virtual void load();
  virtual void unload();

protected:
  // clang-format off

  CheckReporter check_true(bool b);

  CheckReporter check_true(bool b, std::string const& description);

  CheckReporter check_false(bool b);

  CheckReporter check_false(bool b, std::string const& description);

  template <Displayable T, Displayable U>
  requires EQComparable<T, U>
  CheckReporter check_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires EQComparable<T, U>
  CheckReporter check_equal(T const& left, U const& right, std::string const& description);
  
  template <Displayable T, Displayable U, Displayable V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  CheckReporter check_near_equal(T const& left, U const& right, V const& tolerance);

  template <typename T, typename U, typename V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  CheckReporter check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& description);

  template <Displayable T, Displayable U>
  requires NEComparable<T, U>
  CheckReporter check_not_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires NEComparable<T, U>
  CheckReporter check_not_equal(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires LTComparable<T, U>
  CheckReporter check_less_than(T const& left, U const& right);

  template <typename T, typename U>
  requires LTComparable<T, U>
  CheckReporter check_less_than(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires LEComparable<T, U>
  CheckReporter check_less_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires LEComparable<T, U>
  CheckReporter check_less_equal(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires GTComparable<T, U>
  CheckReporter check_greater_than(T const& left, U const& right);

  template <typename T, typename U>
  requires GTComparable<T, U>
  CheckReporter check_greater_than(T const& left, U const& right, std::string const& description);

  template <Displayable T, Displayable U>
  requires GEComparable<T, U>
  CheckReporter check_greater_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires GEComparable<T, U>
  CheckReporter check_greater_equal(T const& left, U const& right, std::string const& description);
//clang-format on

private: 
  
  template<Displayable T, Displayable U>
  std::string formatBinaryCheck(std::string check, T const& left, U const& right);

  template<Displayable T, Displayable U, Displayable V>
  std::string formatBinaryTolerantCheck(std::string check, T const& left, U const& right, V const& tolerance);
};

/** OTHER METHODS IMPLEMENTATION */

template<Displayable T, Displayable U>
std::string Test::formatBinaryCheck(std::string check, T const& left, U const& right){
  std::stringstream ss;
  ss << "" << check << " with ";
  if constexpr (ToStringable<T>){
    ss << ftg::to_string(left);
  } else {
    ss << left;
  }
  if(showTypes()){
    ss << " (" + type_to_string<T>() + ")";
  }
  ss << " and ";
  if constexpr (ToStringable<U>){
    ss << ftg::to_string(right);
  } else {
    ss << right;
  }
  if(showTypes()){
    ss << " (" + type_to_string<U>() + ")";
  }
  return ss.str();
}

template<Displayable T, Displayable U, Displayable V>
std::string Test::formatBinaryTolerantCheck(std::string check, T const& left, U const& right, V const& tolerance)
{
  std::stringstream ss;
  ss << formatBinaryCheck(check, left, right);
  ss << " tolerance ";
  if constexpr (ToStringable<V>){
    ss << ftg::to_string(tolerance);
  } else {
    ss << tolerance;
  }
  if(showTypes()){
    ss << "(" + type_to_string<V>() + ")";
  }
}


/* CHECK IMPLEMENTATIONS */
inline CheckReporter Test::check_true(bool b)
{
  return check_true(b, "check_true");
}

inline CheckReporter Test::check_true(bool b, std::string const& description)
{
  return CheckReporter(*this, description, b);
}

inline CheckReporter Test::check_false(bool b)
{
  return check_false(b, "check_false");
}
  
inline CheckReporter Test::check_false(bool b, std::string const& description)
{
  return CheckReporter(*this, description, !b);
}

template <Displayable T, Displayable U>
requires EQComparable<T, U>
CheckReporter Test::check_equal(T const& left, U const& right)
{
  return check_equal(left, right, formatBinaryCheck("check_equal", left, right));
}

template <typename T, typename U>
requires EQComparable<T, U>
CheckReporter Test::check_equal(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left == right);
}
  
template <Displayable T, Displayable U, Displayable V>
requires LEComparable<T, U> && GEComparable<T, U>  && Addable<U, V> && Subbable<U, V>
CheckReporter Test::check_near_equal(T const& left, U const& right, V const& tolerance)
{
  return check_near_equal(left, right, tolerance, formatBinaryTolerantCheck("check_near_equal", left, right, tolerance));
}

template <typename T, typename U, typename V>
requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
CheckReporter Test::check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& description)
{
  return CheckReporter(*this, description, left <= right + tolerance && left >= right - tolerance);
}

template <Displayable T, Displayable U>
requires NEComparable<T, U>
CheckReporter Test::check_not_equal(T const& left, U const& right)
{
  return check_not_equal(left, right, formatBinaryCheck("check_not_equal", left, right));
}

template <typename T, typename U>
requires NEComparable<T, U>
CheckReporter Test::check_not_equal(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left != right);
}

template <Displayable T, Displayable U>
requires LTComparable<T, U>
CheckReporter Test::check_less_than(T const& left, U const& right)
{
  return check_less_equal(left, right, formatBinaryCheck("check_less_than", left, right));
}

template <typename T, typename U>
requires LTComparable<T, U>
CheckReporter Test::check_less_than(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left < right);
}

template <Displayable T, Displayable U>
requires LEComparable<T, U>
CheckReporter Test::check_less_equal(T const& left, U const& right)
{
  return check_less_equal(left, right, formatBinaryCheck("check_less_equal", left, right));
}

template <typename T, typename U>
requires LEComparable<T, U>
CheckReporter Test::check_less_equal(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left <= right);
}

template <Displayable T, Displayable U>
requires GTComparable<T, U>
CheckReporter Test::check_greater_than(T const& left, U const& right)
{
  return check_greater_than(left, right, formatBinaryCheck("check_greater_than", left, right));
}

template <typename T, typename U>
requires GTComparable<T, U>
CheckReporter Test::check_greater_than(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left > right);
}

template <Displayable T, Displayable U>
requires GEComparable<T, U>
CheckReporter Test::check_greater_equal(T const& left, U const& right)
{
  return check_greater_equal(left, right, formatBinaryCheck("check_greater_equal", left, right));
}

template <typename T, typename U>
requires GEComparable<T, U>
CheckReporter Test::check_greater_equal(T const& left, U const& right, std::string const& description)
{
  return CheckReporter(*this, description, left >= right);
}

} // namespace ftg

#endif
