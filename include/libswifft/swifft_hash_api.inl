/*
 * Copyright (C) 2021 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_hash_api.inl
 * \brief LibSWIFFT internal C hash API
 */

#ifndef LIBSWIFFT_API
	#define LIBSWIFFT_API(name) name
#endif

//! \brief Compacts a hash value of SWIFFT.
//! The result is not composable with other compacted hash values.
//!
//! \param[in] output the hash value of SWIFFT, of size 128 bytes (1024 bit).
//! \param[out] compact the compacted hash value of SWIFFT, of size 64 bytes (512 bit).
void LIBSWIFFT_API(SWIFFT_Compact)(const BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	BitSequence compact[SWIFFT_COMPACT_BLOCK_SIZE]);

//! \brief Compacts a hash value of SWIFFT for multiple blocks.
//! The result is not composable with other compacted hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] output the hash value of SWIFFT, of size 128 bytes (1024 bit).
//! \param[out] compact the compacted hash value of SWIFFT, of size 64 bytes (512 bit).
void LIBSWIFFT_API(SWIFFT_CompactMultiple)(int nblocks, const BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	BitSequence compact[SWIFFT_COMPACT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void LIBSWIFFT_API(SWIFFT_Compute)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[in] sign the sign bits corresponding to the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void LIBSWIFFT_API(SWIFFT_ComputeSigned)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	const BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void LIBSWIFFT_API(SWIFFT_ComputeMultiple)(int nblocks, const BitSequence * input, BitSequence * output);

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void LIBSWIFFT_API(SWIFFT_ComputeMultipleSigned)(int nblocks, const BitSequence * input,
	const BitSequence * sign, BitSequence * output);
