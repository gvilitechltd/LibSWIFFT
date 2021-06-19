/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_object.c
 * \brief LibSWIFFT object public C implementation
 *
 * Implementation using the best instruction set available at build time.
 */

#include "libswifft/swifft_object.h"
#include "libswifft/swifft.h"

#undef SWIFFT_ISET
#include "swifft_object.inl"

#if defined(__AVX__)
	#include "libswifft/swifft_avx.h"
        #define SWIFFT_ISET() AVX
        #include "swifft_object.inl"
        #undef SWIFFT_ISET
#else
        #pragma message "LibSWIFFT API for AVX is disabled"
#endif

#if defined(__AVX2__)
	#include "libswifft/swifft_avx2.h"
        #define SWIFFT_ISET() AVX2
        #include "swifft_object.inl"
        #undef SWIFFT_ISET
#else
        #pragma message "LibSWIFFT API for AVX2 is disabled"
#endif

#if defined(__AVX512F__)
	#include "libswifft/swifft_avx512.h"
        #define SWIFFT_ISET() AVX512
        #include "swifft_object.inl"
        #undef SWIFFT_ISET
#else
        #pragma message "LibSWIFFT API for AVX512 is disabled"
#endif
