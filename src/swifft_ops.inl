/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_ops.inl
 * \brief LibSWIFFT internal C code expansion
 */
#include "libswifft/swifft.h"
#include "swifft_impl.inl"

#ifndef SWIFFT_LOG2_O
	#define SWIFFT_LOG2_O (SWIFFT_VECTOR_LOG2_SIZE - 3) ///< Log base-2 of SWIFFT_O
#endif
#if (SWIFFT_LOG2_O < 0) || (SWIFFT_LOG2_O > 2)
	#pragma message "internal error: invalid SWIFFT_LOG2_O " LIBSWIFFT_QUOTE(SWIFFT_LOG2_O)
	#pragma message "check SWIFFT_VECTOR_LOG2_SIZE: " LIBSWIFFT_QUOTE(SWIFFT_VECTOR_LOG2_SIZE)
	#error "aborting compilation"
#endif
//! Number of doublings of the wide SWIFFT vector  - used with the best available instruction set
#define SWIFFT_O (1 << SWIFFT_LOG2_O)

//! Constant SWIFFT vector with all entries set to n
#define Z1CONST(n) {n,n,n,n,n,n,n,n}
#if SWIFFT_O == 4
	//! Constant wide SWIFFT vector with all entries set to n
	#define ZOCONST(n) {n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n}
#elif SWIFFT_O == 2
	//! Constant wide SWIFFT vector with all entries set to n
	#define ZOCONST(n) {n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n}
#elif SWIFFT_O == 1
	//! Constant wide SWIFFT vector with all entries set to n
	#define ZOCONST(n) {n,n,n,n,n,n,n,n}
#endif

LIBSWIFFT_BEGIN_EXTERN_C

//! SWIFFT vector type
typedef int16_t Z1vec   __attribute__ ((vector_size (SWIFFT_W*sizeof(int16_t))));
//! Wide SWIFFT vector type
typedef int16_t ZOvec   __attribute__ ((vector_size (SWIFFT_O*SWIFFT_W*sizeof(int16_t))));


//! \brief Shift a SWIFFT vector element-wise
//! (x*2^s mod 256) - floor(x/2^{8-s})
//! This effectively multiplies input by  2^s mod SWIFFT_P=257 while
//! reducing the range by approximately (8-s) bits
//!
//! \param[in] x the SWIFFT vector
//! \param[in] s the shift.
//! \returns the shifted SWIFFT vector.
static inline ZOvec SWIFFT_shift(ZOvec x, int16_t s)
{
	ZOvec ZO_s = ZOCONST(s), ZO_255 = ZOCONST(255), ZO_8Ms = ZOCONST( (int16_t)(8-s) );
	return ((x << ZO_s) & ZO_255) - (x >> ZO_8Ms);
}

//! \brief Reduces a SWIFFT vector element-wise mod-257 to the range {-128+1,3*128-1}
//! \param[in] x the SWIFFT vector
//! \returns the reduced SWIFFT vector.
static inline ZOvec SWIFFT_qReduce(ZOvec x) // (x mod 256) - floor(x/256)
{
	ZOvec ZO_255 = ZOCONST(255), ZO_8 = ZOCONST(8);
	return (x & ZO_255) - (x >> ZO_8);
}

//! \brief Reduces a SWIFFT vector element-wise mod-257 to the range {0,..,SWIFFT_P-1}
//! \param[in] x the SWIFFT vector.
//! \returns the reduced SWIFFT vector.
static inline ZOvec SWIFFT_modP(ZOvec x)
{
	ZOvec ZO_M1 = ZOCONST(-1), ZO_M257 = ZOCONST(-257);
	ZOvec tmp = SWIFFT_qReduce(SWIFFT_qReduce(x));
	return tmp ^ ((tmp == ZO_M1) & ZO_M257);
}

//! \brief Safely multiply two SWIFFT vectors element-wise in the range [-128..128] * [0..256]
//! Overflow (which happens only for 128 * 256) is adjusted mod SWIFFT_P.
//! This method corrects for one specific case of overflow only, relying on SWIFFT_qReduce to limit the parameter range
static inline ZOvec SWIFFT_safeMult(ZOvec v, ZOvec u)
{
	ZOvec ZO_256 = ZOCONST(256), ZO_128 = ZOCONST(128);
	v = SWIFFT_qReduce(v);
	ZOvec m = v * u;
	ZOvec adjust = ((v == ZO_256) & (u == ZO_128)); // inequality returns ~0
	return m - adjust; // adds 1 on overflow (the case of 256*128), 0 otherwise
}


LIBSWIFFT_END_EXTERN_C
