/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file test/catch_main.cpp
 * \brief Catch2 main program
 */
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main( int argc, char* const argv[] )
{
	return Catch::Session().run( argc, argv );
}
