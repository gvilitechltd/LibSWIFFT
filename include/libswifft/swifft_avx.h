/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_avx.h
 * \brief LibSWIFFT public C API for AVX
 *
 * See "include/libswifft/swifft_iset.inl" for code expanded here with SWIFFT_ISET set to AVX.
 */
#ifndef __LIBSWIFFT_SWIFFT_AVX_H_
#define __LIBSWIFFT_SWIFFT_AVX_H_

#if defined(__AVX__)
	#undef SWIFFT_ISET
        #define SWIFFT_ISET() AVX
        #include "libswifft/swifft_iset.inl"
#else
        #pragma message "LibSWIFFT API for AVX is disabled"
#endif

#endif /* __LIBSWIFFT_SWIFFT_AVX_H_ */
