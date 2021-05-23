// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_CONCAT_HPP
#define FATIGUE_TUTORIAL_CONCAT_HPP

#include <string>


/* This interface defines an object aggregating string, 
   from which we can recover a concatenation of all the added strings, in order.
   In case of no string added, returns an empty string.
*/
struct Concat {
  virtual ~Concat() {}
  virtual void add(std::string const& str) = 0;
  virtual std::string concatenated() const = 0;
};


#endif