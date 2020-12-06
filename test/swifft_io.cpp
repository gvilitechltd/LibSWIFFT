/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file test/swifft_io.cpp
 * \brief LibSWIFFT internal C++ IO implementation
 */
#include "swifft_io.hpp"

namespace LibSwifft {

//! \brief Hex characters
static char hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

//! \brief Prints data in hex-format
//!
//! \param[in,out] os the output stream to print to.
//! \param[in] data the data to print.
//! \param[in] size the size of data in bytes.
//! \param[in] block_bits log base-2 of the number of bytes to insert a space after
//! \returns the output stream.
static inline std::ostream & print(std::ostream &os, const unsigned char * data, size_t size, size_t block_bits) {
	size_t mask = (1 << block_bits) - 1;
	os << '{';
	size_t i = size;
	do {
		--i;
		unsigned char x = data[i];
		os << hex[x >> 4] << hex[x & 15];
		if ((i & mask) == 0) {
			os << (i == 0 ? '}' : ' ');
		}
	} while (i > 0);
	return os;
}

std::ostream & operator<<(std::ostream &os, const SwifftInput &input) {
	return print(os, input.data, sizeof(input.data), 3);
}

std::ostream & operator<<(std::ostream &os, const SwifftOutput &output) {
	return print(os, output.data, sizeof(output.data), 4);
}

std::ostream & operator<<(std::ostream &os, const SwifftCompact &compact) {
	return print(os, compact.data, sizeof(compact.data), 4);
}

} // end namespace LibSwifft
