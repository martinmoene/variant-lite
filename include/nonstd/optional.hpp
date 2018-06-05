// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_OPTIONAL_LITE_HPP
#define NONSTD_OPTIONAL_LITE_HPP

#include "variant.hpp"

namespace nonstd {

template< typename T >
class optional
{
private:
    struct none_type{};    
    variant<none_type, T> variant_;
    
public:
};

}

#endif // NONSTD_OPTIONAL_LITE_HPP
