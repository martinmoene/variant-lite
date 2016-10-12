// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef TEST_VARIANT_LITE_H_INCLUDED
#define TEST_VARIANT_LITE_H_INCLUDED

#include "variant.hpp"
#include "lest_cpp03.hpp"

using namespace nonstd;

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

#endif // TEST_VARIANT_LITE_H_INCLUDED

// end of file
