/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_avx2.h
 * \brief LibSWIFFT public C API for AVX2
 *
 * See "include/libswifft/swifft_iset.inl" for code expanded here with SWIFFT_ISET set to AVX2.
 */
#ifndef __LIBSWIFFT_SWIFFT_AVX2_H_
#define __LIBSWIFFT_SWIFFT_AVX2_H_

#if defined(__AVX2__)
	#undef SWIFFT_ISET
        #define SWIFFT_ISET() AVX2
        #include "libswifft/swifft_iset.inl"
#else
        #pragma message "LibSWIFFT API for AVX2 is disabled"
#endif

#endif /* __LIBSWIFFT_SWIFFT_AVX2_H_ */
