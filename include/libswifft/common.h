/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
#ifndef __LIBSWIFFT_COMMON_H__
#define __LIBSWIFFT_COMMON_H__

/*! \file include/libswifft/common.h
 * \brief LibSWIFFT public C common definitions
 */

#define LIBSWIFFT_QUOTE0(x) #x
#define LIBSWIFFT_QUOTE(x) LIBSWIFFT_QUOTE0(x)

#define LIBSWIFFT_CONCAT0(x1,x2) x1##x2
#define LIBSWIFFT_CONCAT(x1,x2) LIBSWIFFT_CONCAT0(x1,x2)

#define LIBSWIFFT_INLINE inline __attribute__((always_inline))

#ifdef __cplusplus
	#define LIBSWIFFT_BEGIN_EXTERN_C extern "C" {
	#define LIBSWIFFT_END_EXTERN_C }
#else
	#define LIBSWIFFT_BEGIN_EXTERN_C
	#define LIBSWIFFT_END_EXTERN_C
#endif

#ifdef __cplusplus
	#define LIBSWIFFT_STATIC_ASSERT(expr, iden) static_assert(expr, LIBSWIFFT_QUOTE(iden))
#else
	#define LIBSWIFFT_STATIC_ASSERT(expr, iden) typedef char LIBSWIFFT_CONCAT(libswifft_static_assert_, iden)[(expr) ? 1 : -1]
#endif

#define LIBSWIFFT_RESTRICT __restrict__

#endif /* __LIBSWIFFT_COMMON_H__ */
