/*
 * Copyright (C) 2021 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_object_iset.inl
 * \brief LibSWIFFT object public C API expansion for instruction-sets
 *
 * Same as swifft.h API but as function pointers.
 */

#undef SWIFFT_SUFFIX
#undef SWIFFT_ISET_NAME
#ifndef SWIFFT_ISET
        #define SWIFFT_SUFFIX()
#else
        #define SWIFFT_SUFFIX() LIBSWIFFT_CONCAT(_,SWIFFT_ISET())
#endif
#define SWIFFT_ISET_NAME(name) LIBSWIFFT_CONCAT(name,SWIFFT_SUFFIX()) ///< Adds a suffix based on SWIFFT_ISET, a macro which must be defined prior to including


LIBSWIFFT_BEGIN_EXTERN_C

void SWIFFT_ISET_NAME(SWIFFT_InitObject)(swifft_object_t *swifft);

LIBSWIFFT_END_EXTERN_C
