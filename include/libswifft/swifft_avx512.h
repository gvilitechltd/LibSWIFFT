/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_avx512.h
 * \brief LibSWIFFT public C API for AVX512
 *
 * See "include/libswifft/swifft_iset.inl" for code expanded here with SWIFFT_ISET set to AVX512.
 */
#ifndef __LIBSWIFFT_SWIFFT_AVX512_H_
#define __LIBSWIFFT_SWIFFT_AVX512_H_

#if defined(__AVX512F__)
	#undef SWIFFT_ISET
        #define SWIFFT_ISET() AVX512
        #include "libswifft/swifft_iset.inl"
#else
        #pragma message "LibSWIFFT API for AVX512 is disabled"
#endif

#endif /* __LIBSWIFFT_SWIFFT_AVX512_H_ */
