/*
 * Copyright (C) 2021 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_fft_api.inl
 * \brief LibSWIFFT internal C FFT API
 */

#ifndef LIBSWIFFT_API
	#define LIBSWIFFT_API(name) name
#endif

//
//  This is the basic SwiFFT routine. Notice that it gets 'm' as parameter.
//  It means that input is of size 8*m and key is of size N*m.
//
//! \brief Computes the FFT phase of SWIFFT.
//!
//! \param[in] input the blocks of input, each of 256 bytes (2048 bits).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bits).
//! \param[in] m number of 8-elements in the input.
//! \param[out] fftout the blocks of FFT-output elements, totaling N*m.
void LIBSWIFFT_API(SWIFFT_fft)(const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t * LIBSWIFFT_RESTRICT fftout);

//! \brief Computes the FFT-sum phase of SWIFFT.
//!
//! \param[in] ikey the SWIFFT key.
//! \param[in] ifftout the blocks of FFT-output elements, totaling N*m
//! \param[in] m number of 8-elements in the input.
//! \param[out] iout the blocks of output elements, each of 64 double-bytes (1024 bits).
void LIBSWIFFT_API(SWIFFT_fftsum)(const int16_t * LIBSWIFFT_RESTRICT ikey,
        const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout);

//! \brief Computes the FFT phase of SWIFFT for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bits).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bits).
//! \param[in] m number of 8-elements in the input.
//! \param[out] fftout the blocks of FFT-output elements, totaling N*m.
void LIBSWIFFT_API(SWIFFT_fftMultiple)(int nblocks, const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t * LIBSWIFFT_RESTRICT fftout);

//! \brief Computes the FFT-sum phase of SWIFFT for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] ikey the SWIFFT key.
//! \param[in] ifftout the blocks of FFT-output elements, totaling N*m
//! \param[in] m number of 8-elements in the input.
//! \param[out] iout the blocks of output elements, each of 64 double-bytes (1024 bits).
void LIBSWIFFT_API(SWIFFT_fftsumMultiple)(int nblocks, const int16_t * LIBSWIFFT_RESTRICT ikey,
        const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout);
