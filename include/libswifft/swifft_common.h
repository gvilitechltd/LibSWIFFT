/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_common.h
 * \brief LibSWIFFT public C definitions
 */
#ifndef __LIBSWIFFT_SWIFFT_COMMON_H__
#define __LIBSWIFFT_SWIFFT_COMMON_H__

#include <stdint.h> // for int16_t
#include "libswifft/common.h"

typedef unsigned char BitSequence; ///< Basic buffer type 

//! \brief Maximum log base-2 size of SWIFFT vector, used for defining alignment
#define MAX_SWIFFT_VECTOR_LOG2_SIZE 5
//! \brief Maximum size of SWIFFT vector, used for defining alignment
#define MAX_SWIFFT_VECTOR_SIZE (1 << MAX_SWIFFT_VECTOR_LOG2_SIZE)

//! Alignment of SWIFFT vector, good for the maximum SWIFFT vector size
#define SWIFFT_ALIGNMENT (sizeof(int16_t)*MAX_SWIFFT_VECTOR_SIZE)
//! Align attribute for SWIFFT vector
#define SWIFFT_ALIGN __attribute__ ((aligned (SWIFFT_ALIGNMENT)))

//! The size in bytes of SWIFFT input.
#define SWIFFT_INPUT_BLOCK_SIZE 256

//! The size in bytes of SWIFFT output block.
#define SWIFFT_OUTPUT_BLOCK_SIZE 128

//! The size in bytes of SWIFFT compact-form.
#define SWIFFT_COMPACT_BLOCK_SIZE 64

#endif /* __LIBSWIFFT_SWIFFT_COMMON_H__ */
