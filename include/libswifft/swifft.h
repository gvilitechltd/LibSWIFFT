/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft.h
 * \brief LibSWIFFT public C API
 *
 * This API operates on the following data structures:
 * - BitSequence[SWIFFT_INPUT_BLOCK_SIZE] : Input bits to the SWIFFT function.
 * - BitSequence[SWIFFT_OUTPUT_BLOCK_SIZE]: Output bits of the SWIFFT function,
 *   each 16-bit representing an element of Z_{257}.
 * - BitSequence[SWIFFT_COMPACT_BLOCK_SIZE] : Compact-form bits of the output
 *   of the SWIFFT function.
 *
 * Use SWIFFT_ALIGN, an attribute macro, on each declaration of these data
 * structures. This will avoid segmentation faults due to incorrect memory
 * alignment for SIMD instructions.
 */

#ifndef __LIBSWIFFT_SWIFFT_H__
#define __LIBSWIFFT_SWIFFT_H__

#include "libswifft/swifft_common.h"
#include "libswifft/swifft_ver.h"

LIBSWIFFT_BEGIN_EXTERN_C

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
void SWIFFT_fft(const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t * LIBSWIFFT_RESTRICT fftout);

//! \brief Computes the FFT-sum phase of SWIFFT.
//!
//! \param[in] ikey the SWIFFT key.
//! \param[in] ifftout the blocks of FFT-output elements, totaling N*m
//! \param[in] m number of 8-elements in the input.
//! \param[out] iout the blocks of output elements, each of 64 double-bytes (1024 bits).
void SWIFFT_fftsum(const int16_t * LIBSWIFFT_RESTRICT ikey,
        const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout);

//! \brief Compacts a hash value of SWIFFT.
//! The result is not composable with other compacted hash values.
//!
//! \param[in] output the hash value of SWIFFT, of size 128 bytes (1024 bit).
//! \param[out] compact the compacted hash value of SWIFFT, of size 64 bytes (512 bit).
void SWIFFT_Compact(const BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	BitSequence compact[SWIFFT_COMPACT_BLOCK_SIZE]);

//! \brief Sets a constant value at each SWIFFT hash value element.
//!
//! \param[out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to set.
void SWIFFT_ConstSet(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Adds a constant value to each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to add.
void SWIFFT_ConstAdd(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Subtracts a constant value from each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to subtract.
void SWIFFT_ConstSub(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Multiply a constant value into each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to multiply by.
void SWIFFT_ConstMul(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Adds a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to add.
void SWIFFT_Add(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Subtracts a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to subtract.
void SWIFFT_Sub(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Multiplies a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to multiply by.
void SWIFFT_Mul(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_Compute(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[in] sign the sign bits corresponding to the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_ComputeSigned(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	const BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ComputeMultiple(int nblocks, const BitSequence * input, BitSequence * output);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ComputeMultipleSigned(int nblocks, const BitSequence * input,
	const BitSequence * sign, BitSequence * output);

LIBSWIFFT_END_EXTERN_C

#endif /* __LIBSWIFFT_SWIFFT_H__ */
