/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_avx.c
 * \brief LibSWIFFT public C implementation for AVX
 *
 * See "src/swifft.inl" for code expanded here with SWIFFT_ISET set to AVX.
 */
#include "libswifft/common.h"

#if defined(__AVX__)
	#include "libswifft/swifft_avx.h"
	#define SWIFFT_LOG2_O 0
	#include "swifft.inl"
#else
        #pragma message "Disabling generation of LibSWIFFT API for AVX"
#endif
