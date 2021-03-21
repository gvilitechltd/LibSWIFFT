/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_iset.inl
 * \brief LibSWIFFT public C API expansion for instruction-sets
 */
#include "libswifft/swifft_common.h"

#ifndef SWIFFT_ISET
        #error "SWIFFT_ISET() must be defined as AVX, AVX2, or AVX512"
#endif
#define SWIFFT_ISET_NAME(name) LIBSWIFFT_CONCAT(name,SWIFFT_ISET()) ///< Adds a suffix SWIFFT_ISET, a macro which must be defined prior to including

LIBSWIFFT_BEGIN_EXTERN_C

//! \brief Computes the FFT phase of SWIFFT.
//!
//! \param[in] input the blocks of input, each of 256 bytes (2048 bits).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bits).
//! \param[in] m number of 8-elements in the input.
//! \param[out] fftout the blocks of FFT-output elements, totaling N*m.
void SWIFFT_ISET_NAME(SWIFFT_fft_)(const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t *fftout);

//! \brief Computes the FFT-sum phase of SWIFFT.
//!
//! \param[in] ikey the SWIFFT key.
//! \param[in] ifftout the blocks of FFT-output elements, totaling N*m
//! \param[in] m number of 8-elements in the input.
//! \param[out] iout the blocks of output elements, each of 64 double-bytes (1024 bits).
void SWIFFT_ISET_NAME(SWIFFT_fftsum_)(const int16_t * LIBSWIFFT_RESTRICT ikey,
        const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout);

//! \brief Sets a constant value at each SWIFFT hash value element.
//!
//! \param[out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to set.
void SWIFFT_ISET_NAME(SWIFFT_ConstSet_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const int16_t operand);

//! \brief Adds a constant value to each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to add.
void SWIFFT_ISET_NAME(SWIFFT_ConstAdd_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const int16_t operand);

//! \brief Subtracts a constant value from each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to subtract.
void SWIFFT_ISET_NAME(SWIFFT_ConstSub_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const int16_t operand);

//! \brief Multiply a constant value into each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to multiply by.
void SWIFFT_ISET_NAME(SWIFFT_ConstMul_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const int16_t operand);

//! \brief Sets a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to set to.
void SWIFFT_ISET_NAME(SWIFFT_Set_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Adds a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to add.
void SWIFFT_ISET_NAME(SWIFFT_Add_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Subtracts a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to subtract.
void SWIFFT_ISET_NAME(SWIFFT_Sub_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Multiplies a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to multiply by.
void SWIFFT_ISET_NAME(SWIFFT_Mul_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
        const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_Compute_)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
        BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[in] sign the sign bits corresponding to the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeSigned_)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
        const BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE],
        BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the FFT phase of SWIFFT for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bits).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bits).
//! \param[in] m number of 8-elements in the input.
//! \param[out] fftout the blocks of FFT-output elements, totaling nblocks*N*m.
void SWIFFT_ISET_NAME(SWIFFT_fftMultiple_)(int nblocks, const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t * LIBSWIFFT_RESTRICT fftout);

//! \brief Computes the FFT-sum phase of SWIFFT for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] ikey the SWIFFT key.
//! \param[in] ifftout the blocks of FFT-output elements, totaling N*m
//! \param[in] m number of 8-elements in the input.
//! \param[out] iout the blocks of output elements, each of 64 double-bytes (1024 bits).
void SWIFFT_ISET_NAME(SWIFFT_fftsumMultiple_)(int nblocks, const int16_t * LIBSWIFFT_RESTRICT ikey,
        const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout);

//! \brief Compacts a hash value of SWIFFT for multiple blocks.
//! The result is not composable with other compacted hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] output the hash value of SWIFFT, of size 128 bytes (1024 bit) per block.
//! \param[out] compact the compacted hash value of SWIFFT, of size 64 bytes (512 bit) per block.
void SWIFFT_ISET_NAME(SWIFFT_CompactMultiple_)(int nblocks, const BitSequence * output,
        BitSequence * compact);

//! \brief Sets a constant value at each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to set, per block.
void SWIFFT_ISET_NAME(SWIFFT_ConstSetMultiple_)(int nblocks, BitSequence * output,
        const int16_t * operand);

//! \brief Adds a constant value to each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to add, per block.
void SWIFFT_ISET_NAME(SWIFFT_ConstAddMultiple_)(int nblocks, BitSequence * output,
        const int16_t * operand);

//! \brief Subtracts a constant value from each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the constant value to subtract, per block.
void SWIFFT_ISET_NAME(SWIFFT_ConstSubMultiple_)(int nblocks, BitSequence * output,
        const int16_t * operand);

//! \brief Multiply a constant value into each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the constant value to multiply by, per block.
void SWIFFT_ISET_NAME(SWIFFT_ConstMulMultiple_)(int nblocks, BitSequence * output,
        const int16_t * operand);

//! \brief Sets a SWIFFT hash value to another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the hash value to set to, per block.
void SWIFFT_ISET_NAME(SWIFFT_SetMultiple_)(int nblocks, BitSequence * output,
        const BitSequence * operand);

//! \brief Adds a SWIFFT hash value to another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the hash value to add, per block.
void SWIFFT_ISET_NAME(SWIFFT_AddMultiple_)(int nblocks, BitSequence * output,
        const BitSequence * operand);

//! \brief Subtracts a SWIFFT hash value from another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the hash value to subtract, per block.
void SWIFFT_ISET_NAME(SWIFFT_SubMultiple_)(int nblocks, BitSequence * output,
        const BitSequence * operand);

//! \brief Multiplies a SWIFFT hash value from another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block..
//! \param[in] operand the hash value to multiply by, per block.
void SWIFFT_ISET_NAME(SWIFFT_MulMultiple_)(int nblocks, BitSequence * output,
        const BitSequence * operand);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeMultiple_)(int nblocks, const BitSequence * input, BitSequence * output);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeMultipleSigned_)(int nblocks, const BitSequence * input,
        const BitSequence * sign, BitSequence * output);

LIBSWIFFT_END_EXTERN_C
