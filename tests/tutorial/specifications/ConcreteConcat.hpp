// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#ifndef FATIGUE_TUTORIAL_CONCRETECONCAT_HPP
#define FATIGUE_TUTORIAL_CONCRETECONCAT_HPP

#include "Concat.hpp"
#include <list>
#include <vector>

//those classes are made different on purpose,
//in a real development context, they should
//probably be factorized as a template


class ListConcat : public Concat {
public:
  virtual ~ListConcat() {}
  virtual void add(std::string const& str) { m_strs.push_back(str); }
  virtual std::string concatenated() const
  {
    if (m_strs.empty()) {
      return "";
    }

    std::string concat;
    for (auto const& s : m_strs) {
      concat += s;
    }
    return concat;
  }

private:
  std::list<std::string> m_strs;
};


class VectorConcat : public Concat {
public:
  virtual ~VectorConcat() {}
  virtual void add(std::string const& str) { m_strings.push_back(str); }
  virtual std::string concatenated() const
  {
    std::string concat;
    for (auto const& s : m_strings) {
      concat += s;
    }
    return concat;
  }

private:
  std::vector<std::string> m_strings;
};


#endif