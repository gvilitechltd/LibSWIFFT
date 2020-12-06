/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_impl.inl
 * \brief LibSWIFFT internal C definitions
 */

#include "libswifft/swifft_common.h"

#if defined(__AVX512F__)
        #define SWIFFT_INSTRUCTION_SET AVX512
        #define SWIFFT_VECTOR_LOG2_SIZE 5
#elif defined(__AVX2__)
        #define SWIFFT_INSTRUCTION_SET AVX2
        #define SWIFFT_VECTOR_LOG2_SIZE 4
#elif defined(__AVX__)
        #define SWIFFT_INSTRUCTION_SET AVX
        #define SWIFFT_VECTOR_LOG2_SIZE 3
#else
        #error "AVX, AVX2, or AVX512F must be enabled"
#endif
#define SWIFFT_VECTOR_SIZE (1 << SWIFFT_VECTOR_LOG2_SIZE)


#define SWIFFT_LOG2_N 6                        ///< Log base-2 of SWIFFT_N
#define SWIFFT_N (1<<SWIFFT_LOG2_N)            ///< Size of fft
#define SWIFFT_P 257                           ///< Modulus for performing arithmetic operations
#define SWIFFT_M (SWIFFT_INPUT_BLOCK_SIZE/8)   ///< fft blocks
#define OMEGA 42                               ///< Generator of multiplicative subgroup
#define SWIFFT_LOG2_W 3                        ///< Log base-2 of SWIFFT_W
#define SWIFFT_W (1<<SWIFFT_LOG2_W)            ///< Number of coordinates in SSE2 register
#define SWIFFT_LOG2_V 8                        ///< Log base-2 of SWIFFT_V
#define SWIFFT_V (1<<SWIFFT_LOG2_V)            ///< Number of values in a byte

#define SWIFFT_INT16(high,low) (((high) << SWIFFT_LOG2_V) | (low))   ///< Compose a 16-bit value from two 8-bit ones
#define SWIFFT_AddSub(a, b) { b = a - b; a += a - b; }               ///< Replace a pair of numbers with their addition and subtraction


LIBSWIFFT_BEGIN_EXTERN_C

extern const BitSequence SWIFFT_sign0[SWIFFT_INPUT_BLOCK_SIZE];

extern const int16_t SWIFFT_multipliers[SWIFFT_N];
extern const int16_t SWIFFT_fftTable[SWIFFT_V*SWIFFT_V*SWIFFT_W];
extern const int16_t SWIFFT_PI_key[SWIFFT_M*SWIFFT_N];

LIBSWIFFT_END_EXTERN_C
