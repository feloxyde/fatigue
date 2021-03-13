#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP
#include <concepts>
#include <ostream>
#include <string>
#include <type_traits>

namespace ftg {
struct LogMessage {};
//#FIXME count checks when they pass/fail, and report number in case of uncaught
// exception.
typedef std::string TestId;

// clang-format off
template <typename T> concept OutStreamable = requires(std::ostream &a, T const& b) {
  { a << b } ->std::same_as<std::ostream &>;
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
// clang-format on

class Test {
public:
  virtual void run() = 0;
  virtual TestId name() = 0;
  virtual void load();
  virtual void unload();

public:
  class CheckReporter {
  public:
    CheckReporter(Test const &test, std::string const &message, bool res);
    ~CheckReporter();

    // used to tell what to do with result
    // void check();  // reports and marks test as failed (default)
    void warn();   // reports and does not marks test as failed
    void fatal();  // reports and exit tests
    void assert(); // same as fatal

    //#FIXME should add an EMPHASE option or something like that to mark answer
    // as important ?
    CheckReporter &important(); // marks assertion fail as important
    // CheckReporter & notImportant(); // marks assertion as not especially
    // important (default)

    // used to tell that result should be inverted
    CheckReporter &
    fails(); // means that check should be considered as OK if res = false
    // CheckReporter &succeeds(); // means that check shoube be considered as OK
    // if res = true (default)

  private:
    enum Mode { MODE_CHECK, MODE_WARN, MODE_FATAL };
    std::string m_message;
    Mode m_mode;
    bool m_res;
    bool m_shouldfail;
    bool m_important;
  };

protected:
  // clang-format off

  
  CheckReporter check_true(bool b);

  CheckReporter check_true(bool b, std::string const& message);

  CheckReporter check_false(bool b);

  CheckReporter check_false(bool b, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires EQComparable<T, U>
  CheckReporter check_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires EQComparable<T, U>
  CheckReporter check_equal(T const& left, U const& right, std::string const& message);
  
  template <OutStreamable T, OutStreamable U, OutStreamable V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  CheckReporter check_near_equal(T const& left, U const& right, V const& tolerance);

  template <typename T, typename U, typename V>
  requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
  CheckReporter check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires NEComparable<T, U>
  CheckReporter check_not_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires NEComparable<T, U>
  CheckReporter check_not_equal(T const& left, U const& right, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires LTComparable<T, U>
  CheckReporter check_less_than(T const& left, U const& right);

  template <typename T, typename U>
  requires LTComparable<T, U>
  CheckReporter check_less_than(T const& left, U const& right, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires LEComparable<T, U>
  CheckReporter check_less_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires LEComparable<T, U>
  CheckReporter check_less_equal(T const& left, U const& right, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires GTComparable<T, U>
  CheckReporter check_greater_than(T const& left, U const& right);

  template <typename T, typename U>
  requires GTComparable<T, U>
  CheckReporter check_greater_than(T const& left, U const& right, std::string const& message);

  template <OutStreamable T, OutStreamable U>
  requires GEComparable<T, U>
  CheckReporter check_greater_equal(T const& left, U const& right);

  template <typename T, typename U>
  requires GEComparable<T, U>
  CheckReporter check_greater_equal(T const& left, U const& right, std::string const& message);
//clang-format on

};


/* CHECK IMPLEMENTATIONS */
Test::CheckReporter Test::check_true(bool b)
{
  return check_true(b, "check_true failed : found false");
}

Test::CheckReporter Test::check_true(bool b, std::string const& message)
{
  return Test::CheckReporter(*this, message, b);
}

Test::CheckReporter Test::check_false(bool b)
{
  return check_true(b, "check_false failed : found true");
}
  
Test::CheckReporter Test::check_false(bool b, std::string const& message)
{
  return Test::CheckReporter(*this, message, !b);
}

template <OutStreamable T, OutStreamable U>
requires EQComparable<T, U>
Test::CheckReporter Test::check_equal(T const& left, U const& right)
{
  return check_equal(left, right, "implement check_equal auto message");
}

template <typename T, typename U>
requires EQComparable<T, U>
Test::CheckReporter Test::check_equal(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left == right);
}
  
template <OutStreamable T, OutStreamable U, OutStreamable V>
requires LEComparable<T, U> && GEComparable<T, U>  && Addable<U, V> && Subbable<U, V>
Test::CheckReporter Test::check_near_equal(T const& left, U const& right, V const& tolerance)
{
  return check_near_equal(left, right, tolerance, "implement check_near_equal auto message");
}

template <typename T, typename U, typename V>
requires LEComparable<T, U> && GEComparable<T, U> && Addable<U, V> && Subbable<U, V>
Test::CheckReporter Test::check_near_equal(T const& left, U const& right, V const& tolerance, std::string const& message)
{
  return Test::CheckReporter(*this, message, left <= right + tolerance && left >= right - tolerance);
}

template <OutStreamable T, OutStreamable U>
requires NEComparable<T, U>
Test::CheckReporter Test::check_not_equal(T const& left, U const& right)
{
  return check_not_equal(left, right, "implement check_not_equal auto message");
}

template <typename T, typename U>
requires NEComparable<T, U>
Test::CheckReporter Test::check_not_equal(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left != right);
}

template <OutStreamable T, OutStreamable U>
requires LTComparable<T, U>
Test::CheckReporter Test::check_less_than(T const& left, U const& right)
{
  return check_less_equal(left, right, "implement check_less_than auto message");
}

template <typename T, typename U>
requires LTComparable<T, U>
Test::CheckReporter Test::check_less_than(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left < right);
}

template <OutStreamable T, OutStreamable U>
requires LEComparable<T, U>
Test::CheckReporter Test::check_less_equal(T const& left, U const& right)
{
  return check_less_equal(left, right, "implement check_less_equal");
}

template <typename T, typename U>
requires LEComparable<T, U>
Test::CheckReporter Test::check_less_equal(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left <= right);
}

template <OutStreamable T, OutStreamable U>
requires GTComparable<T, U>
Test::CheckReporter Test::check_greater_than(T const& left, U const& right)
{
  return check_greater_than(left, right, "implement check_greater_than auto message");
}

template <typename T, typename U>
requires GTComparable<T, U>
Test::CheckReporter Test::check_greater_than(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left > right);
}

template <OutStreamable T, OutStreamable U>
requires GEComparable<T, U>
Test::CheckReporter Test::check_greater_equal(T const& left, U const& right)
{
  return check_greater_equal(left, right, "implement check_greater_equal auto message");
}

template <typename T, typename U>
requires GEComparable<T, U>
Test::CheckReporter Test::check_greater_equal(T const& left, U const& right, std::string const& message)
{
  return Test::CheckReporter(*this, message, left >= right);
}

} // namespace ftg

#endif
