/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft_object.inl
 * \brief LibSWIFFT object internal C implementation
 *
 * Implementation using the best instruction set available at build time.
 */

#include "libswifft/swifft_common.h"

#undef SWIFFT_SUFFIX
#undef SWIFFT_ISET_NAME
#ifndef SWIFFT_ISET
	#define SWIFFT_SUFFIX()
#else
	#define SWIFFT_SUFFIX() LIBSWIFFT_CONCAT(_,SWIFFT_ISET())
#endif
#define SWIFFT_ISET_NAME(name) LIBSWIFFT_CONCAT(name,SWIFFT_SUFFIX()) ///< Adds a suffix based on SWIFFT_ISET, a macro which must be defined prior to including


LIBSWIFFT_BEGIN_EXTERN_C

void SWIFFT_ISET_NAME(SWIFFT_InitFftObject)(swifft_fft_object_t *swifft_fft)
{
	swifft_fft->SWIFFT_fft = SWIFFT_ISET_NAME(SWIFFT_fft);
	swifft_fft->SWIFFT_fftsum = SWIFFT_ISET_NAME(SWIFFT_fftsum);
	swifft_fft->SWIFFT_fftMultiple = SWIFFT_ISET_NAME(SWIFFT_fftMultiple);
	swifft_fft->SWIFFT_fftsumMultiple = SWIFFT_ISET_NAME(SWIFFT_fftsumMultiple);
}

void SWIFFT_ISET_NAME(SWIFFT_InitArithObject)(swifft_arith_object_t *swifft_arith)
{
	swifft_arith->SWIFFT_ConstSet = SWIFFT_ISET_NAME(SWIFFT_ConstSet);
	swifft_arith->SWIFFT_ConstAdd = SWIFFT_ISET_NAME(SWIFFT_ConstAdd);
	swifft_arith->SWIFFT_ConstSub = SWIFFT_ISET_NAME(SWIFFT_ConstSub);
	swifft_arith->SWIFFT_ConstMul = SWIFFT_ISET_NAME(SWIFFT_ConstMul);
	swifft_arith->SWIFFT_Set = SWIFFT_ISET_NAME(SWIFFT_Set);
	swifft_arith->SWIFFT_Add = SWIFFT_ISET_NAME(SWIFFT_Add);
	swifft_arith->SWIFFT_Sub = SWIFFT_ISET_NAME(SWIFFT_Sub);
	swifft_arith->SWIFFT_Mul = SWIFFT_ISET_NAME(SWIFFT_Mul);
	swifft_arith->SWIFFT_ConstSetMultiple = SWIFFT_ISET_NAME(SWIFFT_ConstSetMultiple);
	swifft_arith->SWIFFT_ConstAddMultiple = SWIFFT_ISET_NAME(SWIFFT_ConstAddMultiple);
	swifft_arith->SWIFFT_ConstSubMultiple = SWIFFT_ISET_NAME(SWIFFT_ConstSubMultiple);
	swifft_arith->SWIFFT_ConstMulMultiple = SWIFFT_ISET_NAME(SWIFFT_ConstMulMultiple);
	swifft_arith->SWIFFT_SetMultiple = SWIFFT_ISET_NAME(SWIFFT_SetMultiple);
	swifft_arith->SWIFFT_AddMultiple = SWIFFT_ISET_NAME(SWIFFT_AddMultiple);
	swifft_arith->SWIFFT_SubMultiple = SWIFFT_ISET_NAME(SWIFFT_SubMultiple);
	swifft_arith->SWIFFT_MulMultiple = SWIFFT_ISET_NAME(SWIFFT_MulMultiple);
}

void SWIFFT_ISET_NAME(SWIFFT_InitHashObject)(swifft_hash_object_t *swifft_hash)
{
	swifft_hash->SWIFFT_Compact = SWIFFT_Compact;
	swifft_hash->SWIFFT_Compute = SWIFFT_ISET_NAME(SWIFFT_Compute);
	swifft_hash->SWIFFT_ComputeSigned = SWIFFT_ISET_NAME(SWIFFT_ComputeSigned);
	swifft_hash->SWIFFT_CompactMultiple = SWIFFT_ISET_NAME(SWIFFT_CompactMultiple);
	swifft_hash->SWIFFT_ComputeMultiple = SWIFFT_ISET_NAME(SWIFFT_ComputeMultiple);
	swifft_hash->SWIFFT_ComputeMultipleSigned = SWIFFT_ISET_NAME(SWIFFT_ComputeMultipleSigned);
}

void SWIFFT_ISET_NAME(SWIFFT_InitObject)(swifft_object_t *swifft)
{
	SWIFFT_ISET_NAME(SWIFFT_InitFftObject)(&swifft->fft);
	SWIFFT_ISET_NAME(SWIFFT_InitArithObject)(&swifft->arith);
	SWIFFT_ISET_NAME(SWIFFT_InitHashObject)(&swifft->hash);
}

LIBSWIFFT_END_EXTERN_C
