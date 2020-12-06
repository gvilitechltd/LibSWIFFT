/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file src/swifft.inl
 * \brief LibSWIFFT internal C code expansion
 */
#include <stddef.h> // for size_t
#include "libswifft/swifft_iset.inl"
#include "swifft_ops.inl"

LIBSWIFFT_BEGIN_EXTERN_C

//! \brief Computes the FFT phase of SWIFFT.
//!
//! \param[in] input the blocks of input, each of 256 bytes (2048 bits).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bits).
//! \param[in] m number of 8-elements in the input.
//! \param[out] fftout the blocks of FFT-output elements, totaling SWIFFT_N*m.
void SWIFFT_ISET_NAME(SWIFFT_fft_)(const BitSequence * LIBSWIFFT_RESTRICT input, const BitSequence * LIBSWIFFT_RESTRICT sign, int m, int16_t * LIBSWIFFT_RESTRICT fftout)
{
	int i,j,k;
	Z1vec *out = (Z1vec *) fftout;
	const Z1vec *Mult = (const Z1vec *) SWIFFT_multipliers;
	const Z1vec *Tabl = (const Z1vec *) SWIFFT_fftTable;

	const BitSequence *t = input;
	const BitSequence *u = sign;
	ZOvec v[8];

	for (i=0; i<(m>>SWIFFT_LOG2_O); i++) {
		for (j=0; j<SWIFFT_O; j++,t+=8,u+=8) {
			// no need for SWIFFT_safeMult because multipliers do not hit an edge case
			((Z1vec *)&v[0])[j] = Tabl[SWIFFT_INT16(u[0],t[0])];
			for (k=1; k<8; k++) {
				((Z1vec *)&v[k])[j] = Tabl[SWIFFT_INT16(u[k],t[k])] * Mult[k];
			}
		}

		SWIFFT_AddSub(v[0],v[1]);
		SWIFFT_AddSub(v[2],v[3]);
		SWIFFT_AddSub(v[4],v[5]);
		SWIFFT_AddSub(v[6],v[7]);

		v[2] = SWIFFT_qReduce(v[2]);
		v[3] = SWIFFT_shift(v[3],4);
		v[6] = SWIFFT_qReduce(v[6]);
		v[7] = SWIFFT_shift(v[7],4);

		SWIFFT_AddSub(v[0],v[2]);
		SWIFFT_AddSub(v[1],v[3]);
		SWIFFT_AddSub(v[4],v[6]);
		SWIFFT_AddSub(v[5],v[7]);

		v[4] = SWIFFT_qReduce(v[4]);
		v[5] = SWIFFT_shift(v[5],2);
		v[6] = SWIFFT_shift(v[6],4);
		v[7] = SWIFFT_shift(v[7],6);

		SWIFFT_AddSub(v[0],v[4]);
		SWIFFT_AddSub(v[1],v[5]);
		SWIFFT_AddSub(v[2],v[6]);
		SWIFFT_AddSub(v[3],v[7]);

		for (k=0; k<8; k++) {
			v[k] = SWIFFT_qReduce(v[k]);
		}

		for (j=0; j<SWIFFT_O; j++,out+=8) {
			for (k=0; k<8; k++) {
				out[k] = ((Z1vec *)&v[k])[j];
			}
		}
	}
}

void SWIFFT_ISET_NAME(SWIFFT_fftsum_)(const int16_t * LIBSWIFFT_RESTRICT ikey,
	const int16_t * LIBSWIFFT_RESTRICT ifftout, int m, int16_t * LIBSWIFFT_RESTRICT iout)
{
	int i,j;
	const ZOvec *key = (const ZOvec *)ikey;
	const ZOvec *fftout = (const ZOvec *)ifftout;
	ZOvec *out = (ZOvec *)iout;

	ZOvec v[8 >> SWIFFT_LOG2_O] = {0};
	for (i=0; i<m; i++,fftout+=(8>>SWIFFT_LOG2_O),key+=(8>>SWIFFT_LOG2_O)) {
		for (j=0; j<(8>>SWIFFT_LOG2_O); j++) {
			// reducing fftout to avoid overflow
			v[j] += SWIFFT_qReduce(SWIFFT_safeMult((fftout[j]), key[j]));
		}
	}
	for (j=0; j<(8>>SWIFFT_LOG2_O); j++) {
		out[j] = SWIFFT_modP(v[j]);
	}
}

//! \brief Sets a constant value at each SWIFFT hash value element.
//!
//! \param[out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to set.
void SWIFFT_ISET_NAME(SWIFFT_ConstSet_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand)
{
	size_t i;
	ZOvec zoperand = ZOCONST(operand);
	zoperand = SWIFFT_modP(zoperand);
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoutput++) {
		*zoutput = zoperand;
	}
}

//! \brief Adds a constant value to each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to add.
void SWIFFT_ISET_NAME(SWIFFT_ConstAdd_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand)
{
	size_t i;
	const ZOvec zoperand = ZOCONST(operand);
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput + zoperand);
	}
}

//! \brief Subtracts a constant value from each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to subtract.
void SWIFFT_ISET_NAME(SWIFFT_ConstSub_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand)
{
	size_t i;
	const ZOvec zoperand = ZOCONST(operand);
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput - zoperand);
	}
}

//! \brief Multiply a constant value into each SWIFFT hash value element.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the constant value to multiply by.
void SWIFFT_ISET_NAME(SWIFFT_ConstMul_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const int16_t operand)
{
	size_t i;
	const ZOvec zoperand = ZOCONST(operand);
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput * zoperand);
	}
}

//! \brief Adds a SWIFFT hash value to another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to add.
void SWIFFT_ISET_NAME(SWIFFT_Add_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	size_t i;
	const ZOvec *zoperand = (const ZOvec *)operand;
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoperand++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput + *zoperand);
	}
}

//! \brief Subtracts a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to subtract.
void SWIFFT_ISET_NAME(SWIFFT_Sub_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	size_t i;
	const ZOvec *zoperand = (const ZOvec *)operand;
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoperand++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput - *zoperand);
	}
}

//! \brief Multiplies a SWIFFT hash value from another, element-wise.
//!
//! \param[in,out] output the hash value of SWIFFT to modify.
//! \param[in] operand the hash value to multiply by.
void SWIFFT_ISET_NAME(SWIFFT_Mul_)(BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE],
	const BitSequence operand[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	size_t i;
	const ZOvec *zoperand = (const ZOvec *)operand;
	ZOvec *zoutput = (ZOvec *)output;
	size_t size = SWIFFT_OUTPUT_BLOCK_SIZE/sizeof(ZOvec);
	for (i=0; i<size; i++,zoperand++,zoutput++) {
		*zoutput = SWIFFT_modP(*zoutput * *zoperand);
	}
}

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[in] sign the sign bits corresponding to the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
static inline void SWIFFT_compute(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	const BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	// do FFT and linear combination of FFT coefficients
	SWIFFT_ALIGN int16_t fftout[SWIFFT_N*SWIFFT_M];
	SWIFFT_ISET_NAME(SWIFFT_fft_)(input, sign, SWIFFT_M, fftout);
	SWIFFT_ISET_NAME(SWIFFT_fftsum_)(SWIFFT_PI_key, fftout, SWIFFT_M, (int16_t *)output);
}

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_Compute_)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	SWIFFT_compute(input, SWIFFT_sign0, output);
}

//! \brief Computes the result of a SWIFFT operation.
//! The result is composable with other hash values.
//!
//! \param[in] input the input of 256 bytes (2048 bit).
//! \param[in] sign the sign bits corresponding to the input of 256 bytes (2048 bit).
//! \param[out] output the resulting hash value of SWIFFT, of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeSigned_)(const BitSequence input[SWIFFT_INPUT_BLOCK_SIZE],
	const BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE],
	BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE])
{
	SWIFFT_compute(input, sign, output);
}

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeMultiple_)(int nblocks, const BitSequence * input, BitSequence * output)
{
	while (nblocks-- > 0) {
		SWIFFT_compute(input, SWIFFT_sign0, output);
		input += SWIFFT_INPUT_BLOCK_SIZE;
		output += SWIFFT_OUTPUT_BLOCK_SIZE;
	}
}

//! \brief Computes the result of multiple SWIFFT operations.
//! The result is composable with other hash values.
//!
//! \param[in] nblocks the number of blocks to operate on.
//! \param[in] input the blocks of input, each of 256 bytes (2048 bit).
//! \param[in] sign the blocks of sign bits corresponding to blocks of input of 256 bytes (2048 bit).
//! \param[out] output the resulting blocks of hash values of SWIFFT, each of size 128 bytes (1024 bit).
void SWIFFT_ISET_NAME(SWIFFT_ComputeMultipleSigned_)(int nblocks, const BitSequence * input,
	const BitSequence * sign, BitSequence * output)
{
	while (nblocks-- > 0) {
		SWIFFT_compute(input, sign, output);
		input += SWIFFT_INPUT_BLOCK_SIZE;
		sign += SWIFFT_INPUT_BLOCK_SIZE;
		output += SWIFFT_OUTPUT_BLOCK_SIZE;
	}
}

LIBSWIFFT_END_EXTERN_C
