/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_avx512.c
 * \brief LibSWIFFT public C implementation for AVX512
 *
 * See "src/swifft.inl" for code expanded here with SWIFFT_ISET set to AVX512.
 */
#include "libswifft/common.h"

#if defined(__AVX512F__)
	#include "libswifft/swifft_avx512.h"
	#define SWIFFT_LOG2_O 2
	#include "swifft.inl"
#else
	#pragma message "Disabling generation of LibSWIFFT API for AVX512"
#endif
