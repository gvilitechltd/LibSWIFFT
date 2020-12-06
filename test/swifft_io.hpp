/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file test/swifft_io.hpp
 * \brief LibSWIFFT internal C++ IO API
 */

#ifndef __LIBSWIFFT_SWIFFT_IO_HPP__
#define __LIBSWIFFT_SWIFFT_IO_HPP__

#include <ostream>
#include "libswifft/swifft.hpp"

namespace LibSwifft {

//! \brief Writes a SWIFFT input data structure to an output stream.
//!
//! \param[in,out] os the output stream.
//! \param[in] input the SWIFFT input.
//! \returns the output stream.
std::ostream & operator<<(std::ostream &os, const SwifftInput &input);

//! \brief Writes a SWIFFT output data structure to an output stream
//!
//! \param[in,out] os the output stream.
//! \param[in] output the SWIFFT output.
//! \returns the output stream.
std::ostream & operator<<(std::ostream &os, const SwifftOutput &output);

//! \brief Writes a SWIFFT compact-form data structure to an output stream
//!
//! \param[in,out] os the output stream.
//! \param[in] compact the SWIFFT compact-form.
//! \returns the output stream.
std::ostream & operator<<(std::ostream &os, const SwifftCompact &compact);

} // end namespace LibSwifft

#endif // __LIBSWIFFT_SWIFFT_IO_HPP__
