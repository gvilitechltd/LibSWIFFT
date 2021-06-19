/*
 * Copyright (C) 2021 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file include/libswifft/swifft_arith_api.inl
 * \brief LibSWIFFT internal C arithmetic API
 */

#ifndef LIBSWIFFT_API
	#define LIBSWIFFT_API(name) name
#endif

//! \brief Sets a constant value at each SWIFFT hash value element.
//!
//! \param[out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to set.
void LIBSWIFFT_API(SWIFFT_ConstSet)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Adds a constant value to each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to add.
void LIBSWIFFT_API(SWIFFT_ConstAdd)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Subtracts a constant value from each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to subtract.
void LIBSWIFFT_API(SWIFFT_ConstSub)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Multiply a constant value into each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to multiply by.
void LIBSWIFFT_API(SWIFFT_ConstMul)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand);

//! \brief Sets a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to set to.
void LIBSWIFFT_API(SWIFFT_Set)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Adds a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to add.
void LIBSWIFFT_API(SWIFFT_Add)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Subtracts a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to subtract.
void LIBSWIFFT_API(SWIFFT_Sub)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Multiplies a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to multiply by.
void LIBSWIFFT_API(SWIFFT_Mul)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE]);

//! \brief Sets a constant value at each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to set, per block.
void LIBSWIFFT_API(SWIFFT_ConstSetMultiple)(int nblocks, BitSequence * output,
	const int16_t * operand);

//! \brief Adds a constant value to each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to add, per block.
void LIBSWIFFT_API(SWIFFT_ConstAddMultiple)(int nblocks, BitSequence * output,
	const int16_t * operand);

//! \brief Subtracts a constant value from each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to subtract, per block.
void LIBSWIFFT_API(SWIFFT_ConstSubMultiple)(int nblocks, BitSequence * output,
	const int16_t * operand);

//! \brief Multiply a constant value into each SWIFFT hash value element for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify, per block.
//! \param[in] operand the constant value to multiply by, per block.
void LIBSWIFFT_API(SWIFFT_ConstMulMultiple)(int nblocks, BitSequence * output,
	const int16_t * operand);

//! \brief Sets a SWIFFT hash value to another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to set to.
void LIBSWIFFT_API(SWIFFT_SetMultiple)(int nblocks, BitSequence * output,
	const BitSequence * operand);

//! \brief Adds a SWIFFT hash value to another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to add.
void LIBSWIFFT_API(SWIFFT_AddMultiple)(int nblocks, BitSequence * output,
	const BitSequence * operand);

//! \brief Subtracts a SWIFFT hash value from another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to subtract.
void LIBSWIFFT_API(SWIFFT_SubMultiple)(int nblocks, BitSequence * output,
	const BitSequence * operand);

//! \brief Multiplies a SWIFFT hash value from another, element-wise, for multiple blocks.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to multiply by.
void LIBSWIFFT_API(SWIFFT_MulMultiple)(int nblocks, BitSequence * output,
	const BitSequence * operand);
