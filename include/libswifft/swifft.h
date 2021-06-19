/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft.h
 * \brief LibSWIFFT public C API
 *
 * This API operates on the following data structures:
 * - BitSequence[SWIFFT_INPUT_BLOCK_SIZE] : Input bits to the SWIFFT function.
 * - BitSequence[SWIFFT_OUTPUT_BLOCK_SIZE]: Output bits of the SWIFFT function,
 *   each 16-bit representing an element of Z_{257}.
 * - BitSequence[SWIFFT_COMPACT_BLOCK_SIZE] : Compact-form bits of the output
 *   of the SWIFFT function.
 *
 * Use SWIFFT_ALIGN, an attribute macro, on each declaration of these data
 * structures. This will avoid segmentation faults due to incorrect memory
 * alignment for SIMD instructions.
 */

#ifndef __LIBSWIFFT_SWIFFT_H__
#define __LIBSWIFFT_SWIFFT_H__

#include "libswifft/swifft_common.h"
#include "libswifft/swifft_ver.h"

LIBSWIFFT_BEGIN_EXTERN_C

#undef LIBSWIFFT_API
#define LIBSWIFFT_API(name) name
#include "libswifft/swifft_api.inl"
#undef LIBSWIFFT_API

LIBSWIFFT_END_EXTERN_C

#endif /* __LIBSWIFFT_SWIFFT_H__ */
