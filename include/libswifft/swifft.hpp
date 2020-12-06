/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft.hpp
 * \brief LibSWIFFT public C++ API
 */

#ifndef __LIBSWIFFT_SWIFFT_HPP__
#define __LIBSWIFFT_SWIFFT_HPP__

#include "libswifft/swifft.h"
#include <string.h>

namespace LibSwifft {

struct SWIFFT_ALIGN SwifftInput;
SwifftInput& Copy(SwifftInput &dst, const SwifftInput &src);
SwifftInput & Set(SwifftInput &lhs, const BitSequence value);

struct SWIFFT_ALIGN SwifftOutput;
SwifftOutput& Copy(SwifftOutput &dst, const SwifftOutput &src);
SwifftOutput & Set(SwifftOutput &lhs, const int16_t value);

struct SWIFFT_ALIGN SwifftCompact;
SwifftCompact& Copy(SwifftCompact &dst, const SwifftCompact &src);
SwifftCompact & Set(SwifftCompact &lhs, const BitSequence value);

//! \brief An auto-memory-aligned SWIFFT input data structure.
struct SWIFFT_ALIGN SwifftInput {
	//! \brief The input data structure.
	BitSequence data[SWIFFT_INPUT_BLOCK_SIZE];

	//! \brief Copy a source SWIFFT input.
	//!
	//! \param[in] rhs the source to copy from.
	//! \returns this SWIFFT input.
	LIBSWIFFT_INLINE SwifftInput & operator=(const SwifftInput &rhs) { Copy(*this, rhs); return *this; }
	//! \brief Set all 8-bit elements to a value.
	//!
	//! \param[in] value the value to set to.
	//! \returns this SWIFFT input.
	LIBSWIFFT_INLINE SwifftInput & operator=(const BitSequence value) { Set(*this, value); return *this; }

};

//! \brief An auto-memory-aligned SWIFFT output data structure.
struct SWIFFT_ALIGN SwifftOutput {
	//! \brief The output data structure.
	BitSequence data[SWIFFT_OUTPUT_BLOCK_SIZE];

	//! \brief Copy a source SWIFFT output.
	//!
	//! \param[in] rhs the source to copy from.
	//! \returns this SWIFFT output.
	LIBSWIFFT_INLINE SwifftOutput & operator=(const SwifftOutput &rhs) { Copy(*this, rhs); return *this; }
	//! \brief Set all 16-bit elements to a value.
	//!
	//! \param[in] value the value to set to.
	//! \returns this SWIFFT output.
	LIBSWIFFT_INLINE SwifftOutput & operator=(const int16_t value) { Set(*this, value); return *this; }

};

//! \brief An auto-memory-aligned SWIFFT compact-form data structure.
struct SWIFFT_ALIGN SwifftCompact {
	//! \brief The compact-form data structure.
	BitSequence data[SWIFFT_COMPACT_BLOCK_SIZE];

	//! \brief Copy a source SWIFFT compact-form.
	//!
	//! \param[in] rhs the source to copy from.
	//! \returns this SWIFFT compact-form.
	LIBSWIFFT_INLINE SwifftCompact & operator=(const SwifftCompact &rhs) { Copy(*this, rhs); return *this; }
	//! \brief Set all 8-bit elements to a value.
	//!
	//! \param[in] value the value to set to.
	//! \returns this SWIFFT compact-form.
	LIBSWIFFT_INLINE SwifftCompact & operator=(const BitSequence value) { Set(*this, value); return *this; }

};

//! \brief Tests for equality of two SWIFFT input data structures.
//!
//! \param[in] si1 the first SWIFFT input.
//! \param[in] si2 the second SWIFFT input.
LIBSWIFFT_INLINE bool operator==(const SwifftInput &si1, const SwifftInput &si2) {
	return memcmp(si1.data, si2.data, sizeof(SwifftInput)) == 0;
}

//! \brief Tests for inequality of two SWIFFT input data structures.
//!
//! \param[in] si1 the first SWIFFT input.
//! \param[in] si2 the second SWIFFT input.
LIBSWIFFT_INLINE bool operator!=(const SwifftInput &si1, const SwifftInput &si2) {
	return !(si1 == si2);
}

//! \brief Tests for equality of two SWIFFT output data structures.
//!
//! \param[in] so1 the first SWIFFT output.
//! \param[in] so2 the second SWIFFT output.
LIBSWIFFT_INLINE bool operator==(const SwifftOutput &so1, const SwifftOutput &so2) {
	return memcmp(so1.data, so2.data, sizeof(SwifftOutput)) == 0;
}

//! \brief Tests for inequality of two SWIFFT output data structures.
//!
//! \param[in] so1 the first SWIFFT output.
//! \param[in] so2 the second SWIFFT output.
LIBSWIFFT_INLINE bool operator!=(const SwifftOutput &so1, const SwifftOutput &so2) {
	return !(so1 == so2);
}

//! \brief Tests for equality of two SWIFFT compact-form data structures.
//!
//! \param[in] sc1 the first SWIFFT compact-form.
//! \param[in] sc2 the second SWIFFT compact-form.
LIBSWIFFT_INLINE bool operator==(const SwifftCompact &sc1, const SwifftCompact &sc2) {
	return memcmp(sc1.data, sc2.data, sizeof(SwifftCompact)) == 0;
}

//! \brief Tests for inequality of two SWIFFT compact-form data structures.
//!
//! \param[in] sc1 the first SWIFFT compact-form.
//! \param[in] sc2 the second SWIFFT compact-form.
LIBSWIFFT_INLINE bool operator!=(const SwifftCompact &sc1, const SwifftCompact &sc2) {
	return !(sc1 == sc2);
}

//! \brief Copy a SWIFFT input data structure.
//!
//! \param[out] dst the destination to copy to.
//! \param[in] src the source to copy from.
//! \returns the destination.
LIBSWIFFT_INLINE SwifftInput& Copy(SwifftInput &dst, const SwifftInput &src) {
	memcpy(dst.data, src.data, sizeof(dst.data));
	return dst;
}

//! \brief Copy a SWIFFT output data structure.
//!
//! \param[out] dst the destination to copy to.
//! \param[in] src the source to copy from.
//! \returns the destination.
LIBSWIFFT_INLINE SwifftOutput& Copy(SwifftOutput &dst, const SwifftOutput &src) {
	memcpy(dst.data, src.data, sizeof(dst.data));
	return dst;
}

//! \brief Copy a SWIFFT compact-form data structure.
//!
//! \param[out] dst the destination to copy to.
//! \param[in] src the source to copy from.
//! \returns the destination.
LIBSWIFFT_INLINE SwifftCompact& Copy(SwifftCompact &dst, const SwifftCompact &src) {
	memcpy(dst.data, src.data, sizeof(dst.data));
	return dst;
}

//! \brief Set all 8-bit elements of a SWIFFT input data structure to a value.
//!
//! \param[out] lhs the SWIFFT input to set.
//! \param[in] value the value to set to.
//! \returns the SWIFFT input.
LIBSWIFFT_INLINE SwifftInput & Set(SwifftInput &lhs, BitSequence value) {
	memset(lhs.data, value, sizeof(lhs.data));
	return lhs;
}

//! \brief Set all 16-bit elements of a SWIFFT output data structure to a value.
//!
//! \param[out] lhs the SWIFFT output to set.
//! \param[in] value the value to set to.
//! \returns the SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & Set(SwifftOutput &lhs, const int16_t value) {
	SWIFFT_ConstSet(lhs.data, value);
	return lhs;
}

//! \brief Set all 8-bit elements of a SWIFFT compact-form data structure to a value.
//!
//! \param[out] lhs the SWIFFT compact-form to set.
//! \param[in] value the value to set to.
//! \returns the SWIFFT compact-form.
LIBSWIFFT_INLINE SwifftCompact & Set(SwifftCompact &lhs, BitSequence value) {
	memset(lhs.data, value, sizeof(lhs.data));
	return lhs;
}

//! \brief Adds a value to each 16-bit element of a SWIFFT output data structure.
//!
//! \param[in,out] lhs the SWIFFT output.
//! \param[in] value the value.
//! \returns the SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator+=(SwifftOutput &lhs, const int16_t value) {
	SWIFFT_ConstAdd(lhs.data, value);
	return lhs;
}

//! \brief Subtracts a value from each 16-bit element of a SWIFFT output data structure.
//!
//! \param[in,out] lhs the SWIFFT output.
//! \param[in] value: the value.
//! \returns the SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator-=(SwifftOutput &lhs, const int16_t value) {
	SWIFFT_ConstSub(lhs.data, value);
	return lhs;
}

//! \brief Multiplies each 16-bit element of a SWIFFT output data structure by a value.
//! \param[in,out] lhs the SWIFFT output.
//! \param[in] value: the value.
//! \returns the SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator*=(SwifftOutput &lhs, const int16_t value) {
	SWIFFT_ConstMul(lhs.data, value);
	return lhs;
}

//! \brief Adds a second SWIFFT output data structure to a first one, element-wise.
//!
//! \param[in,out] lhs the first SWIFFT output.
//! \param[in] rhs the second SWIFFT output.
//! \returns the first SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator+=(SwifftOutput &lhs, const SwifftOutput &rhs) {
	SWIFFT_Add(lhs.data, rhs.data);
	return lhs;
}

//! \brief Subtracts a first SWIFFT output data structure from a second one, element-wise.
//!
//! \param[in,out] lhs the first SWIFFT output.
//! \param[in] rhs the second SWIFFT output.
//! \returns the first SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator-=(SwifftOutput &lhs, const SwifftOutput &rhs) {
	SWIFFT_Sub(lhs.data, rhs.data);
	return lhs;
}

//! \brief Multiplies a first SWIFFT output data structure by a second one, element-wise.
//!
//! \param[in,out] lhs the first SWIFFT output.
//! \param[in] rhs the second SWIFFT output.
//! \returns the first SWIFFT output.
LIBSWIFFT_INLINE SwifftOutput & operator*=(SwifftOutput &lhs, const SwifftOutput &rhs) {
	SWIFFT_Mul(lhs.data, rhs.data);
	return lhs;
}

} // end namespace LibSwifft

#endif // __LIBSWIFFT_SWIFFT_HPP__
