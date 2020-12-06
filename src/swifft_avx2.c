/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_avx2.c
 * \brief LibSWIFFT public C implementation for AVX2
 *
 * See "src/swifft.inl" for code expanded here with SWIFFT_ISET set to AVX2.
 */
#include "libswifft/common.h"

#if defined(__AVX2__)
	#include "libswifft/swifft_avx2.h"
	#define SWIFFT_LOG2_O 1
	#include "swifft.inl"
#else
        #pragma message "Disabling generation of LibSWIFFT API for AVX2"
#endif
