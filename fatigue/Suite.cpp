// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "Suite.hpp"

namespace ftg {

Suite::Suite(std::string const& name) : m_name(name)
{
}

Suite::~Suite(){};

//is this mandatory ?
//std::vector<std::unique_ptr<Suite>> Suite::uses() const
//{
//return std::move(std::vector<std::unique_ptr<Suite>>());
//}

std::string const& Suite::name() const
{
  return m_name;
}

} // namespace ftg
