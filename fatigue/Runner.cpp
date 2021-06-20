// Copyright 2021 Felix Bertoni
//
// SPDX-License-Identifier: MIT

#include "Runner.hpp"

namespace ftg {

Logger::Message::Message(Mode mode,
			 std::string const& description,
			 std::vector<ParamInfo> const& params,
			 bool expected,
			 bool result,
			 bool important) :
    mode(mode),
    description(description),
    params(params),
    expected(expected),
    result(result),
    important(important)
{
}

} // namespace ftg