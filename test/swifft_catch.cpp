/*
 * Copyright (C) 2020 Yaron Gvili and Gvili Tech Ltd.
 *
 * See the accompanying LICENSE.txt file for licensing information.
 */
/*! \file test/swifft_catch.cpp
 * \brief LibSWIFFT Catch2 test cases
 */
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <catch2/catch.hpp>
#include "libswifft/swifft.hpp"
#include "swifft_io.hpp"
#include "testcommon.h"

#include "libswifft/swifft.h"
#include "libswifft/swifft_avx.h"
#include "libswifft/swifft_avx2.h"
#include "libswifft/swifft_avx512.h"

#undef SWIFFT_ISET
#define SWIFFT_ISET() SWIFFT_INSTRUCTION_SET
#include "swifft_ops.inl"

namespace LibSwifft {

static uint64_t rdtsc_cycles() {
	uint64_t n = 1000, cycles = 0;
	for (uint64_t i=0; i<n; i++) {
		uint64_t t0 = rdtsc_start();
		uint64_t t1 = rdtsc_stop();
		uint64_t dt = t1 - t0;
		cycles += dt;
	}
	return cycles / n;
}

TEST_CASE( "rdtsc", "[swifft]" ) {
	rdtsc_cycles();
}

TEST_CASE( "swifft block sizes are sane", "[swifft]" ) {
	REQUIRE( SWIFFT_INPUT_BLOCK_SIZE == 4 * SWIFFT_COMPACT_BLOCK_SIZE );
	REQUIRE( sizeof(SwifftInput) == 4 * sizeof(SwifftCompact) );
}

static void randomize(SwifftInput * input, size_t size) {
	for (size_t i=0; i<size; i++) {
		int * data = (int *)(input[i].data);
		for (size_t j=0; j<SWIFFT_INPUT_BLOCK_SIZE/sizeof(int); j++) {
			data[j] = rand();
		}
	}
}

template<class Callable>
static void test_swifft_iter_cycles(int nrepeats, int niters, double cycles_per_iter_limit, const char * iterobj, const Callable & callable) {
	uint64_t cycles_per_rdtsc = rdtsc_cycles();
	timespec ts0;
	clock_gettime(CLOCK_REALTIME, &ts0);
	uint64_t t0 = rdtsc_start();
	callable();
	uint64_t t1 = rdtsc_stop();
	uint64_t dt = t1 - t0;
	dt = (dt < cycles_per_rdtsc) ? 0 : dt - cycles_per_rdtsc;
	timespec ts1;
	clock_gettime(CLOCK_REALTIME, &ts1);
	double dts_sec = ts1.tv_sec - ts0.tv_sec + (ts1.tv_nsec - ts0.tv_nsec) / 1.0e9;
	double cycles_per_iter = 1.0 * dt / nrepeats / niters,
	       cycles_per_byte = cycles_per_iter / SWIFFT_INPUT_BLOCK_SIZE,
	       MBps = 1.0 * SWIFFT_INPUT_BLOCK_SIZE * nrepeats * niters / 1024.0 / 1024.0 / dts_sec,
	       gcps = 1.0 * dt / dts_sec / 1.0e9;
	std::cerr << "running " << nrepeats << "*" << niters << " " << iterobj << ":"
	       << " cycles/" << iterobj << "=" << cycles_per_iter
	       << " cycles/byte=" << cycles_per_byte
	       << " Giga-cycles/sec=" << gcps
	       << " MB/sec=" << MBps
	       << " cycles/rdtsc=" << cycles_per_rdtsc
	       << std::endl;
	REQUIRE( cycles_per_iter < cycles_per_iter_limit );
}

TEST_CASE( "swifft takes at most 2000 cycles per call", "[.][swifftperf]" ) {
	srand(1);
	SwifftInput input = {0};
	SwifftOutput output = {0};
	randomize(&input, 1);
	int nrepeats = 1, nrounds = 10000000;
	test_swifft_iter_cycles(nrepeats, nrounds, 2000, "rounds", [&input, &output, nrepeats, nrounds]() {
		for (int r=0; r<nrepeats; r++) {
			for (int64_t i=0; i<nrounds; i++) {
				SWIFFT_Compute(input.data, output.data);
			}
		}
	});
}

template <class T>
struct Array
{
	T* array;
	Array(int length) :
		array(static_cast<T*>(aligned_alloc(SWIFFT_ALIGNMENT, length * sizeof(T))))
	{
	}
	~Array() {
		free(array);
		array = NULL;
	}
};

#ifdef _OPENMP
	#define LABEL_OPENMP "(OpenMP)"
#else
	#define LABEL_OPENMP ""
#endif
void test_swifft_block_cycles(int nblocks, int nrepeats, double cycles_per_block_limit) {
	srand(1);
	Array<SwifftInput> input(nblocks);
	Array<SwifftOutput> output(nblocks);
	randomize(input.array, nblocks);
	test_swifft_iter_cycles(nrepeats, nblocks, cycles_per_block_limit, "blocks" LABEL_OPENMP, [&input, &output, nblocks, nrepeats]() {
		for (int r=0; r<nrepeats; r++) {
			SWIFFT_ComputeMultiple(nblocks, input.array[0].data, output.array[0].data);
		}
	});
}

TEST_CASE( "swifft takes at most 2000 cycles per block in-small-memory", "[.][swifftperf]" ) {
	int nblocks = 1000, nrepeats = 10, cycles_per_block_limit = 2000;
	test_swifft_block_cycles(nblocks, nrepeats, cycles_per_block_limit);
}

TEST_CASE( "swifft takes at most 4000 cycles per block in-medium-memory", "[.][swifftperf]" ) {
	int nblocks = 10000, nrepeats = 10, cycles_per_block_limit = 4000;
	test_swifft_block_cycles(nblocks, nrepeats, cycles_per_block_limit);
}

TEST_CASE( "swifft takes at most 4000 cycles per block in-large-memory", "[.][swifftperf]" ) {
	test_swifft_block_cycles(1000000, 1, 4000);
}

TEST_CASE( "swifft compact takes at most 150 cycles per call", "[.][swifftperf]" ) {
	srand(1);
	SwifftInput input = {0};
	SwifftOutput output;
	SwifftCompact compact;
	randomize(&input, 1);
	SWIFFT_Compute(input.data, output.data);
	int nrepeats = 1, nrounds=100000;
	test_swifft_iter_cycles(nrepeats, nrounds, 150, "compact-rounds", [&output, &compact, nrepeats, nrounds]() {
		for (int r=0; r<nrepeats; r++) {
			for (int64_t i=0; i<nrounds; i++) {
				SWIFFT_Compact(output.data, compact.data);
			}
		}
	});
}

TEST_CASE( "swifft FFT-only takes at most 1500 cycles per call", "[.][swifftperf]" ) {
	srand(1);
	SwifftInput input = {0};
	SwifftInput sign = {0};
	SWIFFT_ALIGN int16_t fftout[SWIFFT_N*SWIFFT_M];
	randomize(&input, 1);
	int nrepeats = 1, nrounds=100000;
	test_swifft_iter_cycles(nrepeats, nrounds, 1500, "FFT-rounds", [&input, &sign, &fftout, nrepeats, nrounds]() {
		for (int r=0; r<nrepeats; r++) {
			for (int64_t i=0; i<nrounds; i++) {
				SWIFFT_fft(input.data, sign.data, SWIFFT_M, fftout);
			}
		}
	});
}

TEST_CASE( "swifft FFT-sum-only takes at most 500 cycles per call", "[.][swifftperf]" ) {
	srand(1);
	SwifftInput input = {0};
	SwifftInput sign = {0};
	SWIFFT_ALIGN int16_t fftout[SWIFFT_N*SWIFFT_M];
	SwifftOutput output = {0};
	randomize(&input, 1);
	SWIFFT_fft(input.data, sign.data, SWIFFT_M, fftout);
	int nrepeats = 1, nrounds=100000;
	test_swifft_iter_cycles(nrepeats, nrounds, 500, "FFT-sum-rounds", [&fftout, &output, nrepeats, nrounds]() {
		for (int r=0; r<nrepeats; r++) {
			for (int64_t i=0; i<nrounds; i++) {
				SWIFFT_fftsum(SWIFFT_PI_key, fftout, SWIFFT_M, (int16_t *)output.data);
			}
		}
	});
}

TEST_CASE( "swifft speed is dependent on input size only", "[.][swifftperf]" ) {
	srand(1);
	SwifftInput input = {0};
	SwifftOutput output = {0};
	uint64_t tmin1 = 0;
	int n_inputs = 10, n_rounds = 1000, n_calls = 10;
	for (int i=0; i<n_inputs; i++) {
		for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
			input.data[j] = rand() & 0xFF;
		}
		uint64_t tmin = 999999999;
		for (int j=0; j<n_rounds; j++) {
			uint64_t sum_t = 0;
			for (int k=0; k<n_calls; k++) {
				uint64_t t0 = rdtsc_start();
				SWIFFT_Compute(input.data, output.data);
				uint64_t t1 = rdtsc_stop();
				sum_t += t1 - t0;
			}
			uint64_t t = (sum_t + n_calls - 1) / n_calls;
			if (t < tmin) tmin = t;
		}
		if (i == 0) {
			tmin1 = tmin;
		} else {
			REQUIRE( tmin1 * 1.1 > tmin );
			REQUIRE( tmin1 / 1.1 < tmin );
		}
	}
	std::cerr << "running " << n_inputs << " inputs, " << n_rounds << " rounds, " << n_calls << " calls: "
	       << "cycles/call=" << tmin1 << " cycles/byte=" << (1.0 * tmin1 / SWIFFT_INPUT_BLOCK_SIZE) << std::endl;
}

TEST_CASE( "SWIFFT_modP after 0|1|2 SWIFFT_qReduce invocations is consistent", "[swifft]" ) {
	int16_t i = 0;
	for (int j=0; j<1<<16; j++,i++) {
		ZOvec v = ZOCONST(i);
		ZOvec q1 = SWIFFT_qReduce(v);
		ZOvec q2 = SWIFFT_qReduce(q1);
		ZOvec m = SWIFFT_modP(v);
		ZOvec m1 = SWIFFT_modP(q1);
		ZOvec m2 = SWIFFT_modP(q2);
		REQUIRE( *(int16_t *)&m == *(int16_t *)&m1 );
		REQUIRE( *(int16_t *)&m == *(int16_t *)&m2 );
	}
}

TEST_CASE( "SWIFFT_fft is consistent across instruction-sets", "[swifft]" ) {
	srand(1);
	SwifftInput input = {0};
	for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
		input.data[j] = rand() & 0xFF;
	}
	SwifftOutput output0 = {0};
	SWIFFT_ALIGN int16_t fftout0[SWIFFT_N*SWIFFT_M] = {0};
	SWIFFT_fft(input.data, SWIFFT_sign0, SWIFFT_M, fftout0);
	SWIFFT_fftsum(SWIFFT_PI_key, fftout0, SWIFFT_M, (int16_t *)output0.data);
#if defined(__AVX__)
	SwifftOutput output1 = {0};
	SWIFFT_ALIGN int16_t fftout1[SWIFFT_N*SWIFFT_M] = {0};
	SWIFFT_fft_AVX(input.data, SWIFFT_sign0, SWIFFT_M, fftout1);
	SWIFFT_fftsum_AVX(SWIFFT_PI_key, fftout1, SWIFFT_M, (int16_t *)output1.data);
	CHECK( 0 == memcmp(fftout0, fftout1, sizeof(fftout1)) );
#endif
#if defined(__AVX2__)
	SwifftOutput output2 = {0};
	SWIFFT_ALIGN int16_t fftout2[SWIFFT_N*SWIFFT_M] = {0};
	SWIFFT_fft_AVX2(input.data, SWIFFT_sign0, SWIFFT_M, fftout2);
	SWIFFT_fftsum_AVX2(SWIFFT_PI_key, fftout2, SWIFFT_M, (int16_t *)output2.data);
	CHECK( 0 == memcmp(fftout0, fftout2, sizeof(fftout1)) );
#endif
#if defined(__AVX512F__)
	SwifftOutput output3 = {0};
	SWIFFT_ALIGN int16_t fftout3[SWIFFT_N*SWIFFT_M] = {0};
	SWIFFT_fft_AVX512(input.data, SWIFFT_sign0, SWIFFT_M, fftout3);
	SWIFFT_fftsum_AVX512(SWIFFT_PI_key, fftout3, SWIFFT_M, (int16_t *)output3.data);
	CHECK( 0 == memcmp(fftout0, fftout3, sizeof(fftout1)) );
#endif
}

TEST_CASE( "SWIFFT_safeMult is correct on the range [-128+1,128-1]*[-128,128]", "[swifft]" ) {
	for (int16_t i=-128+1; i<=128-1; i++) {
		CAPTURE( i );
		for (int16_t j=-128; j<=128; j++) {
			CAPTURE( j );
			ZOvec v = ZOCONST(i);
			ZOvec u = ZOCONST(j);
			ZOvec m0 = u * v;
			ZOvec m1 = SWIFFT_safeMult(u, v);
			int16_t m0x = *(int16_t *)&m0;
			CAPTURE( m0x );
			int32_t m32 = (int32_t)i * (int32_t)j;
			CAPTURE( m32 );
			if (m32 >= 1 << 15) m0x += 1;
			if (-m32 > 1 << 15) m0x -= 1;
			CAPTURE( m0x );
			int16_t m0xp = m0x % SWIFFT_P;
			if (m0xp < 0) m0xp += SWIFFT_P;
			int32_t m32p = m32 % SWIFFT_P;
			if (m32p < 0) m32p += SWIFFT_P;
			REQUIRE( m32p == m0xp );
			int16_t m1x = *(int16_t *)&m1;
			int16_t m1xp = m1x % SWIFFT_P;
			if (m1xp < 0) m1xp += SWIFFT_P;
			REQUIRE( m0xp == m1xp );
		}
	}
}

TEST_CASE( "swifft with sign is consistent", "[swifft]" ) {
	srand(1);
	SwifftInput input[3] = {0}, sign[3] = {0}, zeroinput = {0};
	SwifftOutput output[3] = {0}, zerooutput = {0};
	// set up input
	for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
		input[0].data[j] = rand() & 0xFF;
	}
	// check input and its output are not zero
	REQUIRE( input[0] != zeroinput );
	SWIFFT_Compute(input[0].data, output[0].data);
	REQUIRE( output[0] != zerooutput );
	// check positive sign gets the same output
	SWIFFT_ComputeSigned(input[0].data, sign[1].data, output[1].data);
	REQUIRE( output[0] == output[1] );
	// check negative sign gets the mod-SWIFFT_P negative output
	for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
		sign[2].data[j] = 0xFF;
	}
	SWIFFT_ComputeSigned(input[0].data, sign[2].data, output[2].data);
	REQUIRE( output[2] != zerooutput );
	for (int j=0; j<SWIFFT_OUTPUT_BLOCK_SIZE/2; j++) {
		int v0 = ((int16_t *)output[0].data)[j];
		int v2 = ((int16_t *)output[2].data)[j];
		CAPTURE( j );
		CAPTURE( v0 );
		CAPTURE( v2 );
		int summod = (v0 + v2) % SWIFFT_P;
		REQUIRE( summod == 0 );
	}
	// check that adding positive and negative gives zero
	SWIFFT_Add(output[2].data, output[1].data);
	REQUIRE( output[2] == zerooutput );
	// check that negating a sign vector gives mod-SWIFFT_P negative output
	for (int i=0; i<8; i++) {
		CAPTURE( i );
		for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
			sign[1].data[j] = j & ~(1 << i);
			sign[2].data[j] = ~sign[1].data[j];
		}
		SWIFFT_ConstSet(output[0].data, 0);
		for (int k=1; k<3; k++) {
			CAPTURE( k );
			SWIFFT_ComputeSigned(input[0].data, sign[k].data, output[k].data);
			REQUIRE( output[k] != zerooutput );
			SWIFFT_Add(output[0].data, output[k].data);
		}
		REQUIRE( output[0] == zerooutput );
	}
	// check splitting a signed input and composing back outputs
	for (int i=0; i<8; i++) {
		CAPTURE( i );
		for (int j=0; j<SWIFFT_INPUT_BLOCK_SIZE; j++) {
			CAPTURE( j );
			// input is random with random sign
			input[0].data[j] = rand() & 0xFF;
			sign[0].data[j] = rand() & 0xFF;
			// select where one split would be non-zero (half of bits on average)
			BitSequence select = rand() & 0xFF;
			// one split has the same sign except where zero is selected
			input[1].data[j] = select;
			sign[1].data[j] = sign[0].data[j];
			// second split has inverted sign when data is zero and inverted zero selection otherwise
			input[2].data[j] = input[0].data[j] ^ select;
			sign[2].data[j] = sign[0].data[j] ^ ~input[0].data[j];
			// canonicalize: input sign is zero if data is zero
			CAPTURE( (int)sign[0].data[j] );
			CAPTURE( (int)sign[1].data[j] );
			CAPTURE( (int)sign[2].data[j] );
			sign[0].data[j] &= input[0].data[j];
			sign[1].data[j] &= input[1].data[j];
			sign[2].data[j] &= input[2].data[j];
			CAPTURE( (int)input[0].data[j] );
			CAPTURE( (int)input[1].data[j] );
			CAPTURE( (int)input[2].data[j] );
			CAPTURE( (int)sign[0].data[j] );
			CAPTURE( (int)sign[1].data[j] );
			CAPTURE( (int)sign[2].data[j] );
			// sanity check that, per signed bit the sum of the splits recovers the input
			for (int k=0; k<8; k++) {
				CAPTURE( k );
				BitSequence mask = 1 << k;
				int x = (select & mask) == 0 ? 0 : 1;
				int v0 = (input[0].data[j] & mask) == 0 ? 0 : 1;
				int v1 = (input[1].data[j] & mask) == 0 ? 0 : 1;
				int v2 = (input[2].data[j] & mask) == 0 ? 0 : 1;
				int s0 = (sign[0].data[j] & mask) == 0 ? 0 : 1;
				int s1 = (sign[1].data[j] & mask) == 0 ? 0 : 1;
				int s2 = (sign[2].data[j] & mask) == 0 ? 0 : 1;
				CAPTURE( x );
				CAPTURE( v0 );
				CAPTURE( v1 );
				CAPTURE( v2 );
				CAPTURE( s0 );
				CAPTURE( s1 );
				CAPTURE( s2 );
				int val0 = (s0 == 0 ? 1 : -1) * v0;
				int val1 = (s1 == 0 ? 1 : -1) * v1;
				int val2 = (s2 == 0 ? 1 : -1) * v2;
				CAPTURE( val0 );
				CAPTURE( val1 );
				CAPTURE( val2 );
				REQUIRE( val0 == val1 + val2 );
			}
		}
		// check that sum of swifft of signed splits recovers swifft of signed input
		for (int k=0; k<3; k++) {
			REQUIRE( input[k] != zeroinput );
			REQUIRE( sign[k] != zeroinput );
			SWIFFT_ComputeSigned(input[k].data, sign[k].data, output[k].data);
			REQUIRE( output[k] != zerooutput );
		}
		SWIFFT_Sub(output[0].data, output[1].data);
		SWIFFT_Sub(output[0].data, output[2].data);
		REQUIRE( output[0] == zerooutput );
	}
}

// n is the number of bit selections
// select is the list of bit selections, each a bit pattern for which bits are selected per byte
// an input is generated
// then the first bit selection is made from it to yield a first vector (unselected bits are set to 0)
// then the second bit selection is made from the bits not previously selected
// and so on until n bit selections are made
// finally, the remaining bits yield a last vector
// the test checks that the swifft of each vector together sum to the swifft of the input
template <int n>
void test_swifft_composes_bit_by_bit(const BitSequence (&select)[n])
{
	REQUIRE( n > 0 );
	const int m = n + 2;
	SwifftInput input[m] = {0};
	for (int i=0; i<SWIFFT_INPUT_BLOCK_SIZE; i++) {
		CAPTURE( i );
		input[0].data[i] = i;
		for (int j=1; j<m; j++) {
			CAPTURE( j );
			BitSequence mask = 0xFF;
			for (int k=0; k<j-1; k++) {
				mask &= ~select[k];
			}
			if (j - 1 < n) {
				mask &= select[j-1];
			}
			input[j].data[i] = input[0].data[i] & mask;
		}
		BitSequence b = 0;
		bool good = input[0].data[i] == 0;
		for (int j=1; j<m; j++) {
			b ^= input[j].data[i];
			good = good || input[0].data[i] != input[j].data[i];
		}
		REQUIRE( b == input[0].data[i] );
		REQUIRE( good );
	}
	SwifftOutput output[m] = {0};
	for (int j=0; j<m; j++) {
		SWIFFT_Compute(input[j].data, output[j].data);
	}
	SwifftOutput outputx = {0};
	for (int j=1; j<m; j++) {
		SWIFFT_Add(outputx.data, output[j].data);
	}
	if (output[0] != outputx) {
		for (int i=0; i<m; i++) {
			std::cerr << "input" << i << "   " << input[i] << std::endl;
		}
		for (int i=0; i<m; i++) {
			std::cerr << "output" << i << "  " << output[i] << std::endl;
		}
		std::cerr << "outputx  " << outputx << std::endl;
	}
	for (int j=0; j<SWIFFT_OUTPUT_BLOCK_SIZE; j++) {
		CAPTURE( j );
		REQUIRE( (int)output[0].data[j] == (int)outputx.data[j] );
	}
}

TEST_CASE( "swifft composes bit by bit", "[swifft]" ) {
	BitSequence select[] = { 0x6B };
	test_swifft_composes_bit_by_bit(select);
}

TEST_CASE( "swifft composes twice bit by bit", "[swifft]" ) {
	BitSequence select[] = { 0x6B, 0x11 };
	test_swifft_composes_bit_by_bit(select);
}

static const int given = 0;
static const int shares = 3;
static const int carry = given + shares + 1;
static const int ninputs = carry + 1;
static const int accum = ninputs;
static const int noutputs = accum + 1;
static const bool fail = true;
static const bool print = true;

static SwifftInput specific_input1[ninputs] = {
	{
		0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8,
		0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2, 0xF1, 0xF0,
		0xEF, 0xEE, 0xED, 0xEC, 0xEB, 0xEA, 0xE9, 0xE8,
		0xE7, 0xE6, 0xE5, 0xE4, 0xE3, 0xE2, 0xE1, 0xE0,
		0xDF, 0xDE, 0xDD, 0xDC, 0xDB, 0xDA, 0xD9, 0xD8,
		0xD7, 0xD6, 0xD5, 0xD4, 0xD3, 0xD2, 0xD1, 0xD0,
		0xCF, 0xCE, 0xCD, 0xCC, 0xCB, 0xCA, 0xC9, 0xC8,
		0xC7, 0xC6, 0xC5, 0xC4, 0xC3, 0xC2, 0xC1, 0xC0,
		0xBF, 0xBE, 0xBD, 0xBC, 0xBB, 0xBA, 0xB9, 0xB8,
		0xB7, 0xB6, 0xB5, 0xB4, 0xB3, 0xB2, 0xB1, 0xB0,
		0xAF, 0xAE, 0xAD, 0xAC, 0xAB, 0xAA, 0xA9, 0xA8,
		0xA7, 0xA6, 0xA5, 0xA4, 0xA3, 0xA2, 0xA1, 0xA0,
		0x9F, 0x9E, 0x9D, 0x9C, 0x9B, 0x9A, 0x99, 0x98,
		0x97, 0x96, 0x95, 0x94, 0x93, 0x92, 0x91, 0x90,
		0x8F, 0x8E, 0x8D, 0x8C, 0x8B, 0x8A, 0x89, 0x88,
		0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x80,
		0x7F, 0x7E, 0x7D, 0x7C, 0x7B, 0x7A, 0x79, 0x78,
		0x77, 0x76, 0x75, 0x74, 0x73, 0x72, 0x71, 0x70,
		0x6F, 0x6E, 0x6D, 0x6C, 0x6B, 0x6A, 0x69, 0x68,
		0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60,
		0x5F, 0x5E, 0x5D, 0x5C, 0x5B, 0x5A, 0x59, 0x58,
		0x57, 0x56, 0x55, 0x54, 0x53, 0x52, 0x51, 0x50,
		0x4F, 0x4E, 0x4D, 0x4C, 0x4B, 0x4A, 0x49, 0x48,
		0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 0x40,
		0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
		0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
		0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
		0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
		0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18,
		0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10,
		0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
		0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
	},
	{
		0x5A, 0xE8, 0x1D, 0xDE, 0x4E, 0x20, 0xC3, 0x33,
		0xAE, 0x5C, 0x0A, 0x64, 0x90, 0x94, 0x6E, 0x61,
		0x20, 0xF9, 0xA1, 0x9D, 0x10, 0x4A, 0xA2, 0x08,
		0x8D, 0x01, 0x03, 0x99, 0xC3, 0x14, 0x3A, 0x96,
		0x8F, 0x9C, 0x60, 0x02, 0x91, 0x3B, 0xF0, 0x64,
		0x4B, 0x85, 0x47, 0xCE, 0x5D, 0xCF, 0x83, 0x5D,
		0x7E, 0xFF, 0x47, 0x13, 0x16, 0x46, 0x84, 0x5B,
		0x1A, 0xB3, 0x54, 0xDA, 0xC4, 0x66, 0xF6, 0xD3,
		0xE2, 0xA1, 0x93, 0x76, 0x20, 0xD1, 0xA1, 0x37,
		0x6D, 0x7B, 0x73, 0x8F, 0xF9, 0x7A, 0x9A, 0x1B,
		0x4F, 0x99, 0x69, 0x51, 0xF9, 0x18, 0x77, 0xDD,
		0xEE, 0x8E, 0xA8, 0x67, 0x82, 0x2F, 0xB0, 0x8E,
		0x76, 0xC3, 0x56, 0x64, 0x49, 0xF6, 0x0C, 0x4A,
		0x84, 0xF9, 0x7C, 0xFF, 0x24, 0xA1, 0x76, 0x17,
		0x17, 0xAA, 0x95, 0xF1, 0xEF, 0x7B, 0x1F, 0x26,
		0xC9, 0x64, 0x36, 0xB3, 0xD3, 0x2E, 0x1C, 0x82,
		0x7C, 0x52, 0xA4, 0xF8, 0x87, 0x3B, 0x82, 0x25,
		0xFA, 0x00, 0x90, 0x21, 0x2F, 0xAF, 0xF8, 0x45,
		0x87, 0x8B, 0xC9, 0xC1, 0xA1, 0x53, 0x8B, 0xE8,
		0x7A, 0xE0, 0x00, 0x6C, 0xE1, 0xCF, 0xE4, 0x38,
		0x90, 0xF9, 0x50, 0xCC, 0x60, 0x75, 0xE4, 0x63,
		0xAF, 0x4B, 0xFC, 0xA5, 0x65, 0x48, 0x91, 0xB3,
		0x53, 0xBC, 0x5E, 0x8A, 0x02, 0x1D, 0x3D, 0xB1,
		0x8C, 0x40, 0x6C, 0xB7, 0xC2, 0x1A, 0x87, 0xC3,
		0x3A, 0xBB, 0xBF, 0x6F, 0x33, 0x0C, 0xC0, 0x95,
		0xF3, 0xC2, 0x74, 0x8C, 0x5C, 0x33, 0x50, 0x17,
		0xEA, 0xF8, 0xB5, 0x57, 0x8B, 0xCE, 0x9E, 0x99,
		0xCD, 0x6F, 0x0E, 0xA8, 0x00, 0x5A, 0x65, 0xD1,
		0x9E, 0xEF, 0xCF, 0xF2, 0x4F, 0xA4, 0x13, 0x39,
		0x32, 0x1B, 0x69, 0xD4, 0x09, 0x81, 0x1C, 0xC4,
		0xB1, 0xCF, 0x8C, 0xDB, 0x50, 0x23, 0xE4, 0x48,
		0x8C, 0xE8, 0x43, 0x75, 0x2A, 0x1B, 0x8F, 0xAB,
	},
	{
		0xE9, 0x67, 0x4F, 0xBC, 0x33, 0xB9, 0x92, 0x4F,
		0x83, 0x37, 0x23, 0xF8, 0x41, 0x32, 0x2F, 0xD4,
		0x3F, 0x1E, 0xC3, 0x67, 0xE7, 0x20, 0xBA, 0xC3,
		0x89, 0xE8, 0x49, 0x4D, 0xEE, 0x1B, 0x5D, 0x53,
		0x10, 0xDB, 0xA8, 0x26, 0xFA, 0x10, 0x88, 0x89,
		0xC7, 0x4F, 0xEB, 0x23, 0xB0, 0xA7, 0x50, 0x50,
		0x90, 0x4E, 0xD8, 0x7A, 0x16, 0x45, 0xDF, 0x89,
		0x32, 0x00, 0x9C, 0x14, 0x4B, 0x36, 0x0F, 0x42,
		0x3B, 0x04, 0xCB, 0x7E, 0xCB, 0xBA, 0x08, 0x44,
		0x6C, 0x20, 0x27, 0x87, 0x37, 0x42, 0x81, 0x3D,
		0x6B, 0x6F, 0x89, 0x08, 0xEC, 0xBE, 0x67, 0x5C,
		0x5D, 0x9B, 0xCF, 0xEB, 0x90, 0xA7, 0xF7, 0xA6,
		0x84, 0x46, 0x0E, 0x5D, 0x13, 0x5B, 0x64, 0xB8,
		0x7C, 0xCB, 0x12, 0x18, 0xC4, 0x6E, 0x54, 0x6B,
		0xF4, 0x26, 0x89, 0x65, 0x90, 0x77, 0xB9, 0x0D,
		0x3B, 0x29, 0xEE, 0x5E, 0x5E, 0xB8, 0xBE, 0x38,
		0x84, 0x05, 0x46, 0x57, 0xB1, 0x05, 0xB5, 0x93,
		0x6C, 0x07, 0x7F, 0x52, 0x76, 0xF4, 0x05, 0x4D,
		0x6F, 0x64, 0xD1, 0x0A, 0x04, 0x30, 0x30, 0xAC,
		0x93, 0x43, 0x83, 0x9D, 0x07, 0xE3, 0x64, 0x8B,
		0x6C, 0x86, 0xB2, 0xD3, 0x9A, 0xCE, 0x4A, 0x4F,
		0xD1, 0x80, 0x9F, 0x90, 0x80, 0x9F, 0x49, 0x94,
		0x2E, 0x9A, 0x88, 0xEF, 0x7C, 0xA2, 0xD5, 0x99,
		0x49, 0x97, 0x4E, 0xB7, 0x6D, 0x65, 0xD0, 0x5D,
		0xEF, 0x6D, 0x4A, 0x38, 0x1E, 0x70, 0xBE, 0x48,
		0xB0, 0x0A, 0x80, 0x9F, 0x7E, 0x58, 0xE5, 0xB3,
		0xF6, 0x09, 0x70, 0x38, 0xB2, 0x5A, 0xCD, 0x27,
		0xE1, 0xDE, 0x86, 0xE1, 0x62, 0x5E, 0x06, 0x9E,
		0x2F, 0x4C, 0xAB, 0x25, 0xDB, 0x35, 0x34, 0xCD,
		0x67, 0x9B, 0xA1, 0xC8, 0x42, 0xCA, 0xBA, 0x54,
		0x90, 0x5E, 0x64, 0xE4, 0x18, 0xEA, 0xAB, 0x01,
		0x85, 0x2D, 0x8C, 0xDD, 0xE4, 0x41, 0x32, 0xCD,
	},
	{
		0x4C, 0x71, 0xAF, 0x9E, 0x86, 0x63, 0xA8, 0x84,
		0xDA, 0x9D, 0xDC, 0x68, 0x22, 0x54, 0xB0, 0x45,
		0xF0, 0x09, 0x8F, 0x16, 0x1C, 0x80, 0xF1, 0x23,
		0xE3, 0x0F, 0xAF, 0x30, 0xCE, 0xED, 0x86, 0x25,
		0x40, 0x99, 0x15, 0xF8, 0xB0, 0xF1, 0xA1, 0x35,
		0x5B, 0x1C, 0x79, 0x39, 0x3E, 0xBA, 0x02, 0xDD,
		0x21, 0x7F, 0x52, 0xA5, 0xCB, 0xC9, 0x92, 0x1A,
		0xEF, 0x75, 0x0D, 0x0A, 0x4C, 0x92, 0x38, 0x51,
		0x66, 0x1B, 0xE5, 0xB4, 0x50, 0xD1, 0x10, 0xCB,
		0xB6, 0xED, 0xE1, 0xBC, 0x7D, 0x8A, 0xAA, 0x96,
		0x8B, 0x58, 0x4D, 0xF5, 0xBE, 0x0C, 0xB9, 0x29,
		0x14, 0xB3, 0xC2, 0x28, 0xB1, 0x2A, 0xE6, 0x88,
		0x6D, 0x1B, 0xC5, 0xA5, 0xC1, 0x37, 0xF1, 0x6A,
		0x6F, 0xA4, 0xFB, 0x73, 0x73, 0x5D, 0xB3, 0xEC,
		0x6C, 0x02, 0x91, 0x18, 0xF4, 0x86, 0x2F, 0xA3,
		0x75, 0xCB, 0x5D, 0x69, 0x0E, 0x14, 0x23, 0x3A,
		0x87, 0x29, 0x9F, 0xD3, 0x4D, 0x44, 0x4E, 0xCE,
		0xE1, 0x71, 0x9A, 0x07, 0x2A, 0x29, 0x8C, 0x78,
		0x87, 0x81, 0x75, 0xA7, 0xCE, 0x09, 0xD2, 0x2C,
		0x8E, 0xC5, 0xE6, 0x95, 0x85, 0x4E, 0xE1, 0xD3,
		0xA3, 0x21, 0xBF, 0x43, 0xA1, 0xE1, 0xF7, 0x74,
		0x29, 0x9D, 0x36, 0x61, 0xB6, 0x85, 0x89, 0x77,
		0x32, 0x68, 0x9B, 0x29, 0x35, 0xF5, 0xA1, 0x60,
		0x82, 0x91, 0x67, 0x44, 0xEC, 0x3D, 0x16, 0xDE,
		0xEA, 0xE8, 0xC8, 0x6B, 0x16, 0x46, 0x47, 0xE5,
		0x74, 0xFE, 0xC1, 0x27, 0x11, 0x59, 0x84, 0x94,
		0x33, 0xDF, 0xE8, 0x43, 0x12, 0xBE, 0x7A, 0x96,
		0x0B, 0x97, 0xAD, 0x6D, 0x41, 0x26, 0x42, 0x6F,
		0xAE, 0xBD, 0x79, 0xCB, 0x8F, 0x8B, 0x3E, 0xEC,
		0x42, 0x96, 0xDD, 0x08, 0x58, 0x59, 0xB7, 0x80,
		0x2E, 0x9F, 0xE5, 0x33, 0x43, 0xC3, 0x46, 0x41,
		0x0E, 0xC3, 0xCA, 0xAC, 0xCD, 0x58, 0xBC, 0x66,
	},
	{0}, // carry
};

static SwifftOutput specific_output1[ninputs] = {
	{
		0x2A, 0x00, 0xF8, 0x00, 0x73, 0x00, 0xC5, 0x00,
		0x88, 0x00, 0xBA, 0x00, 0x20, 0x00, 0xDB, 0x00,
		0x62, 0x00, 0x67, 0x00, 0x5B, 0x00, 0xEA, 0x00,
		0x6B, 0x00, 0x0A, 0x00, 0x08, 0x00, 0x0E, 0x00,
		0x2A, 0x00, 0xE8, 0x00, 0xA7, 0x00, 0xEE, 0x00,
		0x12, 0x00, 0xA9, 0x00, 0x99, 0x00, 0xCC, 0x00,
		0x1F, 0x00, 0x09, 0x00, 0x7F, 0x00, 0x86, 0x00,
		0xA7, 0x00, 0x62, 0x00, 0x87, 0x00, 0xBD, 0x00,
		0x65, 0x00, 0x87, 0x00, 0xC3, 0x00, 0xED, 0x00,
		0x99, 0x00, 0x71, 0x00, 0xE9, 0x00, 0x52, 0x00,
		0x6E, 0x00, 0x19, 0x00, 0x57, 0x00, 0x37, 0x00,
		0xC1, 0x00, 0x65, 0x00, 0x45, 0x00, 0x5F, 0x00,
		0xA2, 0x00, 0xC1, 0x00, 0x74, 0x00, 0x52, 0x00,
		0xEA, 0x00, 0x45, 0x00, 0x3F, 0x00, 0xAD, 0x00,
		0x36, 0x00, 0x8E, 0x00, 0x39, 0x00, 0x5B, 0x00,
		0x2B, 0x00, 0xCC, 0x00, 0xCF, 0x00, 0x4F, 0x00,
	},
	{
		0xA3, 0x00, 0x62, 0x00, 0x2A, 0x00, 0xB8, 0x00,
		0xB4, 0x00, 0x33, 0x00, 0xB9, 0x00, 0x14, 0x00,
		0xB3, 0x00, 0x26, 0x00, 0x76, 0x00, 0x91, 0x00,
		0x3C, 0x00, 0x0F, 0x00, 0x4F, 0x00, 0x4D, 0x00,
		0x7F, 0x00, 0xE3, 0x00, 0x86, 0x00, 0x8C, 0x00,
		0x3C, 0x00, 0xCF, 0x00, 0x0A, 0x00, 0x17, 0x00,
		0x35, 0x00, 0xD3, 0x00, 0xA5, 0x00, 0xB1, 0x00,
		0x3D, 0x00, 0x02, 0x00, 0x5D, 0x00, 0xB6, 0x00,
		0xF3, 0x00, 0x84, 0x00, 0x07, 0x00, 0x6A, 0x00,
		0xA4, 0x00, 0x42, 0x00, 0xF2, 0x00, 0x4C, 0x00,
		0xC0, 0x00, 0xB6, 0x00, 0x3D, 0x00, 0x71, 0x00,
		0xF6, 0x00, 0x31, 0x00, 0x43, 0x00, 0x04, 0x00,
		0x61, 0x00, 0xC2, 0x00, 0xD5, 0x00, 0x07, 0x00,
		0x9B, 0x00, 0xFA, 0x00, 0xA2, 0x00, 0x47, 0x00,
		0xAA, 0x00, 0xAD, 0x00, 0x0B, 0x00, 0x82, 0x00,
		0x94, 0x00, 0x85, 0x00, 0x61, 0x00, 0x60, 0x00,
	},
	{
		0x7E, 0x00, 0x74, 0x00, 0x06, 0x00, 0x57, 0x00,
		0x6B, 0x00, 0x9C, 0x00, 0xB6, 0x00, 0x3B, 0x00,
		0xDE, 0x00, 0x12, 0x00, 0x3C, 0x00, 0x49, 0x00,
		0x84, 0x00, 0x0D, 0x00, 0xA8, 0x00, 0x73, 0x00,
		0x05, 0x00, 0xF8, 0x00, 0xBC, 0x00, 0x4F, 0x00,
		0xD2, 0x00, 0x3D, 0x00, 0x7C, 0x00, 0x8C, 0x00,
		0xD8, 0x00, 0x58, 0x00, 0xF1, 0x00, 0xC8, 0x00,
		0x9A, 0x00, 0xBB, 0x00, 0x76, 0x00, 0x78, 0x00,
		0xE4, 0x00, 0x69, 0x00, 0x3E, 0x00, 0xD1, 0x00,
		0x9C, 0x00, 0x4E, 0x00, 0xED, 0x00, 0xDF, 0x00,
		0x61, 0x00, 0xF6, 0x00, 0x80, 0x00, 0x30, 0x00,
		0x9B, 0x00, 0x3B, 0x00, 0x00, 0x01, 0xE0, 0x00,
		0xD1, 0x00, 0x76, 0x00, 0x18, 0x00, 0x71, 0x00,
		0x6D, 0x00, 0xE0, 0x00, 0xF6, 0x00, 0x17, 0x00,
		0x26, 0x00, 0x75, 0x00, 0x89, 0x00, 0x51, 0x00,
		0x39, 0x00, 0xCC, 0x00, 0xC7, 0x00, 0x64, 0x00,
	},
	{
		0x09, 0x00, 0xFB, 0x00, 0xED, 0x00, 0xD6, 0x00,
		0x4B, 0x00, 0xE8, 0x00, 0x9C, 0x00, 0xFC, 0x00,
		0x94, 0x00, 0xE8, 0x00, 0xFE, 0x00, 0x4D, 0x00,
		0x65, 0x00, 0x12, 0x00, 0xE7, 0x00, 0x89, 0x00,
		0xB0, 0x00, 0xD5, 0x00, 0x48, 0x00, 0x89, 0x00,
		0x80, 0x00, 0xCB, 0x00, 0xA9, 0x00, 0x81, 0x00,
		0x48, 0x00, 0xD3, 0x00, 0x1B, 0x00, 0xD5, 0x00,
		0x70, 0x00, 0x14, 0x00, 0xFF, 0x00, 0x85, 0x00,
		0x8A, 0x00, 0xF6, 0x00, 0x78, 0x00, 0x2F, 0x00,
		0xB7, 0x00, 0xCC, 0x00, 0xD8, 0x00, 0x65, 0x00,
		0x62, 0x00, 0x0E, 0x00, 0x21, 0x00, 0x54, 0x00,
		0x89, 0x00, 0xB3, 0x00, 0x7E, 0x00, 0x71, 0x00,
		0x28, 0x00, 0x64, 0x00, 0x02, 0x00, 0x33, 0x00,
		0x07, 0x00, 0x20, 0x00, 0x25, 0x00, 0xF4, 0x00,
		0x94, 0x00, 0x01, 0x00, 0x68, 0x00, 0x7F, 0x00,
		0x20, 0x00, 0x2B, 0x00, 0xF0, 0x00, 0x66, 0x00,
	},
	{0},
};

static SwifftCompact specific_compact1[ninputs] = {
	{
		0x97, 0xB0, 0x1D, 0xBB, 0x25, 0x95, 0x30, 0xE1,
		0x99, 0x4D, 0xA3, 0x8B, 0x04, 0x63, 0x6B, 0x0E,
		0xC7, 0xC3, 0x3B, 0xCC, 0x69, 0x23, 0x42, 0xD2,
		0x7A, 0x77, 0x48, 0x7A, 0x7C, 0x31, 0xC5, 0xC2,
		0xE1, 0x25, 0xFC, 0x44, 0xD0, 0xBC, 0x33, 0x55,
		0xDF, 0xA2, 0x54, 0xA3, 0xDB, 0xE1, 0xE7, 0x61,
		0x44, 0xD9, 0x87, 0x5A, 0xBE, 0x0D, 0x0A, 0xB2,
		0x6D, 0xBF, 0xED, 0x14, 0x39, 0x3C, 0x04, 0x52,
	},
	{
		0x9B, 0x92, 0x0E, 0xCB, 0x5C, 0x3C, 0x4B, 0x15,
		0xC7, 0xF7, 0x20, 0xD9, 0xC1, 0x49, 0x72, 0x4F,
		0xA0, 0x6E, 0x2A, 0x8D, 0x0F, 0xF6, 0xAD, 0x17,
		0xB0, 0x59, 0xA7, 0xF7, 0xBD, 0x3C, 0x68, 0xBB,
		0x0C, 0x6E, 0x28, 0xF6, 0xA2, 0x44, 0x12, 0x4F,
		0x92, 0x01, 0x88, 0x08, 0x70, 0x1D, 0x61, 0x04,
		0x7D, 0x90, 0xB0, 0xB1, 0xD2, 0xB1, 0x9D, 0x49,
		0xBE, 0x1B, 0xD6, 0xCF, 0x18, 0xC1, 0x0B, 0x63,
	},
	{
		0x47, 0x21, 0x32, 0x7D, 0x50, 0xCD, 0x5C, 0x3D,
		0x21, 0xCE, 0x02, 0xCC, 0x75, 0x86, 0xDA, 0x76,
		0x1F, 0x96, 0xC5, 0xE8, 0x8D, 0xBD, 0x5E, 0x90,
		0x2C, 0xB2, 0x05, 0x3D, 0xB7, 0x72, 0xCB, 0x7B,
		0x12, 0xFA, 0xA5, 0x77, 0xB9, 0x55, 0x1E, 0xE6,
		0xBD, 0x3D, 0x72, 0xB2, 0x99, 0xCA, 0x38, 0xE7,
		0x2A, 0x76, 0x14, 0x5F, 0x7D, 0x9D, 0x9F, 0x18,
		0xA5, 0xC3, 0xB5, 0x83, 0xB0, 0xC7, 0x90, 0x67,
	},
	{
		0x92, 0x9C, 0x26, 0xE2, 0xA4, 0x6C, 0x99, 0x03,
		0xAE, 0xE6, 0xCB, 0x79, 0x2A, 0xDA, 0xB6, 0x8D,
		0xCB, 0x78, 0x5D, 0x6F, 0x2A, 0x76, 0x3F, 0x85,
		0x13, 0x4D, 0xEB, 0x97, 0x1D, 0x19, 0xB3, 0x89,
		0xE7, 0x22, 0x49, 0xD4, 0x53, 0x44, 0xA9, 0x68,
		0x10, 0xFD, 0x04, 0xDD, 0xFF, 0x06, 0xA2, 0x74,
		0x01, 0x49, 0x3D, 0xE7, 0x53, 0x26, 0xE6, 0xFA,
		0x1D, 0x12, 0xCD, 0x79, 0x1C, 0x46, 0xC8, 0x69,
	},
	{0},
};

static const char * specific_input1_string[] = {
	"{0001020304050607 08090A0B0C0D0E0F 1011121314151617 18191A1B1C1D1E1F 2021222324252627 28292A2B2C2D2E2F 3031323334353637 38393A3B3C3D3E3F 4041424344454647 48494A4B4C4D4E4F 5051525354555657 58595A5B5C5D5E5F 6061626364656667 68696A6B6C6D6E6F 7071727374757677 78797A7B7C7D7E7F 8081828384858687 88898A8B8C8D8E8F 9091929394959697 98999A9B9C9D9E9F A0A1A2A3A4A5A6A7 A8A9AAABACADAEAF B0B1B2B3B4B5B6B7 B8B9BABBBCBDBEBF C0C1C2C3C4C5C6C7 C8C9CACBCCCDCECF D0D1D2D3D4D5D6D7 D8D9DADBDCDDDEDF E0E1E2E3E4E5E6E7 E8E9EAEBECEDEEEF F0F1F2F3F4F5F6F7 F8F9FAFBFCFDFEFF}",
	"{AB8F1B2A7543E88C 48E42350DB8CCFB1 C41C8109D4691B32 3913A44FF2CFEF9E D1655A00A80E6FCD 999ECE8B57B5F8EA 1750335C8C74C2F3 95C00C336FBFBB3A C3871AC2B76C408C B13D1D028A5EBC53 B3914865A5FC4BAF 63E47560CC50F990 38E4CFE16C00E07A E88B53A1C1C98B87 45F8AF2F219000FA 25823B87F8A4527C 821C2ED3B33664C9 261F7BEFF195AA17 1776A124FF7CF984 4A0CF6496456C376 8EB02F8267A88EEE DD7718F95169994F 1B9A7AF98F737B6D 37A1D1207693A1E2 D3F666C4DA54B31A 5B8446161347FF7E 5D83CF5DCE47854B 64F03B9102609C8F 963A14C39903018D 08A24A109DA1F920 616E9490640A5CAE 33C3204EDE1DE85A}",
	"{CD3241E4DD8C2D85 01ABEA18E4645E90 54BACA42C8A19B67 CD3435DB25AB4C2F 9E065E62E186DEE1 27CD5AB2387009F6 B3E5587E9F800AB0 48BE701E384A6DEF 5DD0656DB74E9749 99D5A27CEF889A2E 94499F80909F80D1 4F4ACE9AD3B2866C 8B64E3079D834393 AC3030040AD1646F 4D05F476527F076C 93B505B157460584 38BEB85E5EEE293B 0DB97790658926F4 6B546EC41812CB7C B8645B135D0E4684 A6F7A790EBCF9B5D 5C67BEEC08896F6B 3D8142378727206C 4408BACB7ECB043B 420F364B149C0032 89DF45167AD84E90 5050A7B023EB4FC7 898810FA26A8DB10 535D1BEE4D49E889 C3BA20E767C31E3F D42F3241F8233783 4F92B933BC4F67E9}",
	"{66BC58CDACCAC30E 4146C34333E59F2E 80B7595808DD9642 EC3E8B8FCB79BDAE 6F4226416DAD970B 967ABE1243E8DF33 9484591127C1FE74 E54746166BC8E8EA DE163DEC44679182 60A1F535299B6832 778985B661369D29 74F7E1A143BF21A3 D3E14E8595E6C58E 2CD209CEA7758187 788C292A079A71E1 CE4E444DD39F2987 3A23140E695DCB75 A32F86F41891026C ECB35D7373FBA46F 6AF137C1A5C51B6D 88E62AB128C2B314 29B90CBEF54D588B 96AA8A7DBCE1EDB6 CB10D150B4E51B66 5138924C0A0D75EF 1A92C9CBA5527F21 DD02BA3E39791C5B 35A1F1B0F8159940 2586EDCE30AF0FE3 23F1801C168F09F0 45B0542268DC9DDA 84A863869EAF714C}",
	"{0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000}",
};
static const char * specific_output1_string[] = {
	"{004F00CF00CC002B005B0039008E0036 00AD003F004500EA0052007400C100A2 005F0045006500C1003700570019006E 005200E90071009900ED00C300870065 00BD0087006200A70086007F0009001F 00CC009900A9001200EE00A700E8002A 000E0008000A006B00EA005B00670062 00DB002000BA008800C5007300F8002A}",
	"{00600061008500940082000B00AD00AA 004700A200FA009B000700D500C20061 00040043003100F60071003D00B600C0 004C00F2004200A4006A0007008400F3 00B6005D0002003D00B100A500D30035 0017000A00CF003C008C008600E3007F 004D004F000F003C00910076002600B3 001400B9003300B400B8002A006200A3}",
	"{006400C700CC00390051008900750026 001700F600E0006D00710018007600D1 00E00100003B009B0030008000F60061 00DF00ED004E009C00D1003E006900E4 0078007600BB009A00C800F1005800D8 008C007C003D00D2004F00BC00F80005 007300A8000D00840049003C001200DE 003B00B6009C006B005700060074007E}",
	"{006600F0002B0020007F006800010094 00F40025002000070033000200640028 0071007E00B3008900540021000E0062 006500D800CC00B7002F007800F6008A 008500FF0014007000D5001B00D30048 008100A900CB00800089004800D500B0 008900E700120065004D00FE00E80094 00FC009C00E8004B00D600ED00FB0009}",
	"{00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000}",
};
static const char * specific_compact1_string[] = {
	"{52043C3914EDBF6DB20A0DBE5A87D944 61E7E1DBA354A2DF5533BCD044FC25E1 C2C5317C7A48777AD2422369CC3BC3C7 0E6B63048BA34D99E1309525BB1DB097}",
	"{630BC118CFD61BBE499DB1D2B1B0907D 04611D70088801924F1244A2F6286E0C BB683CBDF7A759B017ADF60F8D2A6EA0 4F7249C1D920F7C7154B3C5CCB0E929B}",
	"{6790C7B083B5C3A5189F9D7D5F14762A E738CA99B2723DBDE61E55B977A5FA12 7BCB72B73D05B22C905EBD8DE8C5961F 76DA8675CC02CE213D5CCD507D322147}",
	"{69C8461C79CD121DFAE62653E73D4901 74A206FFDD04FD1068A94453D44922E7 89B3191D97EB4D13853F762A6F5D78CB 8DB6DA2A79CBE6AE03996CA4E2269C92}",
	"{00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000 00000000000000000000000000000000}",
};

template<int n>
bool test_swifft_composes_with_carry_bit_by_bit(SwifftInput (&input)[n])
{
	REQUIRE( n == 5 );
	SwifftOutput output[noutputs] = {0};
	for (int j=0; j<ninputs; j++) {
		SWIFFT_Compute(input[j].data, output[j].data);
	}
	// add shares
	for (int j=0; j<shares; j++) {
		SWIFFT_Add(output[accum].data, output[given+1+j].data);
	}
	// subtract carry twice
	SWIFFT_Sub(output[accum].data, output[carry].data);
	SWIFFT_Sub(output[accum].data, output[carry].data);
	// validate
	bool valid = output[given] == output[accum];
	if (!valid && print) {
		std::cerr << "fail : " << fail << std::endl;
		for (int j=0; j<ninputs; j++) {
			std::cerr << "input" << j << "   " << input[j] << std::endl;
		}
		std::cerr << std::endl;
		for (int j=0; j<noutputs; j++) {
			std::cerr << "output" << j << "  " << output[j] << std::endl;
		}
		std::cerr << std::endl;
	}
	if (fail) {
		REQUIRE( valid );
	}
	return valid;
}

void test_swifft_composes_with_carry_bit_by_bit(unsigned int seed)
{
	if (seed != 0) {
		srand(seed);
	}
	SwifftInput input[ninputs] = {0};
	for (int i=0; i<SWIFFT_INPUT_BLOCK_SIZE; i++) {
		CAPTURE( i );
		input[given].data[i] = seed == 0 ? i : static_cast<BitSequence>(rand() & 0xFF);
		// compute shares
		BitSequence x1 = input[given+1].data[i] = (BitSequence)(i * 31); // in place of a random value
		BitSequence x2 = input[given+2].data[i] = (BitSequence)(i * 97); // in place of a random value
		BitSequence x3 = input[given+3].data[i] = input[given].data[i] ^ input[given+1].data[i] ^ input[given+2].data[i];
		// compute carry
		input[carry].data[i] = (x1 & x2) | (x2 & x3) | (x1 & x3); // carry
	}
	test_swifft_composes_with_carry_bit_by_bit(input);
}

TEST_CASE( "swifft composes with carry bit by bit (no rand)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(0);
}

TEST_CASE( "swifft composes with carry bit by bit (seed 1)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(1);
}

TEST_CASE( "swifft composes with carry bit by bit (seed 2)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(2);
}

TEST_CASE( "swifft composes with carry bit by bit (seed 3)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(3);
}

TEST_CASE( "swifft composes with carry bit by bit (seed 4)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(4);
}

template<int n>
bool test_swifft_composes_with_carry_bit_by_bit_specific_input(SwifftInput (&input)[n],
	const int imod = -1, const int jmod = -1, const int smod = -1)
{
	REQUIRE( n == ninputs );
	// reverse byte order
	for (int j=0; j<ninputs; j++) {
		for (int i=0; i<SWIFFT_INPUT_BLOCK_SIZE/2; i++) {
			BitSequence tmp = input[j].data[i];
			input[j].data[i] = input[j].data[SWIFFT_INPUT_BLOCK_SIZE-1-i];
			input[j].data[SWIFFT_INPUT_BLOCK_SIZE-1-i] = tmp;
		}
	}
	// optionally modify slightly
	if (imod >= 0 && imod < SWIFFT_INPUT_BLOCK_SIZE && jmod >= 0 && jmod < 8 && smod >= 1 && smod <= shares) {
		BitSequence b = (BitSequence)1 << jmod;
		input[given].data[imod] ^= b;
		input[given+smod].data[imod] ^= b;
	}
	// verify input and compute carry
	for (int i=0; i<SWIFFT_INPUT_BLOCK_SIZE; i++) {
		BitSequence x0 = input[given].data[i];
		BitSequence x1 = input[given+1].data[i];
		BitSequence x2 = input[given+2].data[i];
		BitSequence x3 = input[given+3].data[i];
		BitSequence x = x1 ^ x2 ^ x3;
		REQUIRE( x0 == x );
		// compute carry
		input[carry].data[i] = (x1 & x2) | (x2 & x3) | (x1 & x3); // carry
	}
	return test_swifft_composes_with_carry_bit_by_bit(input);
}

bool test_swifft_composes_with_carry_bit_by_bit_specific_input0(const int imod = -1, const int jmod = -1, const int smod = -1)
{
	SwifftInput input[ninputs] = {0};
	return test_swifft_composes_with_carry_bit_by_bit_specific_input(input, imod, jmod, smod);
}

bool test_swifft_composes_with_carry_bit_by_bit_specific_input1(const int imod = -1, const int jmod = -1, const int smod = -1)
{
	SwifftInput input[ninputs];
	for (int i=0; i<ninputs; i++) {
		memcpy(input[i].data, specific_input1[i].data, sizeof(input[i].data));
	}
	return test_swifft_composes_with_carry_bit_by_bit_specific_input(input, imod, jmod, smod);
}

bool test_swifft_composes_with_carry_bit_by_bit_specific_input2(const int imod = -1, const int jmod = -1, const int smod = -1)
{
	int i = SWIFFT_INPUT_BLOCK_SIZE - 1 - 28;
	SwifftInput input[ninputs] = {0};
	input[0].data[i] = 0xE2;
	input[1].data[i] = 0x00;
	input[2].data[i] = 0x20;
	input[3].data[i] = 0xC2;
	input[4].data[i] = 0x00;
	return test_swifft_composes_with_carry_bit_by_bit_specific_input(input, imod, jmod, smod);
}

void test_swifft_composes_with_carry_bit_by_bit(bool (*test)(int, int, int)) {
	int passcount[SWIFFT_INPUT_BLOCK_SIZE] = {0};
	for (int imod=0; imod<SWIFFT_INPUT_BLOCK_SIZE; imod++) {
		CAPTURE( imod );
		for (int jmod=0; jmod<8; jmod++) {
			CAPTURE( jmod );
			for (int smod=0; smod<=3; smod++) {
				CAPTURE( smod );
				if (test(imod, jmod, smod)) {
					++passcount[imod];
				}
			}
		}
	}
	for (int imod=0; imod<SWIFFT_INPUT_BLOCK_SIZE; imod++) {
		REQUIRE( passcount[imod] == 32 );
	}
}

TEST_CASE( "swifft composes with carry bit by bit (specific input 0)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(test_swifft_composes_with_carry_bit_by_bit_specific_input0);
}

TEST_CASE( "swifft composes with carry bit by bit (specific input 1)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(test_swifft_composes_with_carry_bit_by_bit_specific_input1);
}

TEST_CASE( "swifft composes with carry bit by bit (specific input 2)", "[swifft]" ) {
	test_swifft_composes_with_carry_bit_by_bit(test_swifft_composes_with_carry_bit_by_bit_specific_input2);
}

TEST_CASE( "swifft compacts correctly (specific input)", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftOutput output; \
		SwifftCompact compact; \
		for (int i=0; i<ninputs; i++) { \
			CAPTURE( i ); \
			SWIFFT_Compute##suffix(specific_input1[i].data, output.data); \
			REQUIRE( output == specific_output1[i] ); \
			SWIFFT_Compact(output.data, compact.data); \
			REQUIRE( compact == specific_compact1[i] ); \
		} \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft compacts signed correctly (specific input)", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftOutput output; \
		SwifftCompact compact; \
		for (int i=0; i<ninputs; i++) { \
			CAPTURE( i ); \
			SWIFFT_ComputeSigned##suffix(specific_input1[i].data, SWIFFT_sign0, output.data); \
			REQUIRE( output == specific_output1[i] ); \
			SWIFFT_Compact(output.data, compact.data); \
			REQUIRE( compact == specific_compact1[i] ); \
		} \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft computes multiple correctly (specific input)", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftOutput output[ninputs]; \
		SWIFFT_ComputeMultiple##suffix(ninputs, specific_input1[0].data, output[0].data); \
		for (int i=0; i<ninputs; i++) { \
			CAPTURE( i ); \
			REQUIRE( output[i] == specific_output1[i] ); \
		} \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft computes multiple signed correctly (specific input)", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftInput sign0[ninputs] = {0}; \
		SwifftOutput output[ninputs]; \
		SWIFFT_ComputeMultipleSigned##suffix(ninputs, specific_input1[0].data, sign0[0].data, output[0].data); \
		for (int i=0; i<ninputs; i++) { \
			CAPTURE( i ); \
			REQUIRE( output[i] == specific_output1[i] ); \
		} \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft vector-and-const operations compute correctly", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftOutput output1 = {0}, output2 = {0}; \
		SWIFFT_ConstSet##suffix(output1.data, 1); \
		SWIFFT_ConstSet##suffix(output2.data, 1); \
		REQUIRE( output1 == output2 ); \
		SWIFFT_ConstAdd##suffix(output1.data, 0); \
		SWIFFT_ConstAdd##suffix(output2.data, 1); \
		REQUIRE( output1 != output2 ); \
		SWIFFT_ConstAdd##suffix(output1.data, 3); \
		SWIFFT_ConstAdd##suffix(output2.data, 2); \
		REQUIRE( output1 == output2 ); \
		SWIFFT_ConstSub##suffix(output1.data, 1); \
		SWIFFT_ConstSub##suffix(output2.data, 2); \
		REQUIRE( output1 != output2 ); \
		SWIFFT_ConstMul##suffix(output1.data, 2); \
		SWIFFT_ConstMul##suffix(output2.data, 3); \
		REQUIRE( output1 == output2 ); \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft two-vectors operations compute correctly", "[swifft]" ) {
#define TESTCODE(suffix) \
	{ \
		SwifftOutput output1 = {0}, output2 = {0}; \
		for (int i=0; i<SWIFFT_OUTPUT_BLOCK_SIZE; i+=sizeof(int16_t)) { \
			output1.data[i] = output2.data[i] = 1; \
		} \
		SWIFFT_Add##suffix(output1.data, output2.data); \
		REQUIRE( output1 != output2 ); \
		SWIFFT_Mul##suffix(output2.data, output1.data); \
		REQUIRE( output1 == output2 ); \
		SWIFFT_Sub##suffix(output1.data, output2.data); \
		REQUIRE( output1 != output2 ); \
		SWIFFT_Sub##suffix(output2.data, output2.data); \
		REQUIRE( output1 == output2 ); \
	}
	TESTCODE()
#if defined(__AVX__)
	TESTCODE(_AVX)
#endif
#if defined(__AVX2__)
	TESTCODE(_AVX2)
#endif
#if defined(__AVX512F__)
	TESTCODE(_AVX512)
#endif
#undef TESTCODE
}

TEST_CASE( "swifft prints correctly (specific input)", "[swifft]" ) {
	for (int i=0; i<ninputs; i++) {
		std::stringstream s;
		s << specific_input1[i];
		REQUIRE( s.str() == specific_input1_string[i] );
	}
	for (int i=0; i<ninputs; i++) {
		std::stringstream s;
		s << specific_output1[i];
		REQUIRE( s.str() == specific_output1_string[i] );
	}
	for (int i=0; i<ninputs; i++) {
		std::stringstream s;
		s << specific_compact1[i];
		REQUIRE( s.str() == specific_compact1_string[i] );
	}
	std::cerr << std::endl;
}

#define TEST_CODE(op) \
TEST_CASE( "swifft multiple " LIBSWIFFT_QUOTE(op) " computes correctly", "[swifft]" ) { \
	const int n = 128; \
	SwifftInput input1[n]; \
	SwifftOutput output1[n]; \
	SwifftOutput output2[n]; \
	int16_t operand[n]; \
	srand(1); \
	for (int i=0; i<n; i++) { \
		operand[i] = (int16_t)rand(); \
	} \
	randomize(input1, n); \
	SWIFFT_ComputeMultiple(n, input1[0].data, output1[0].data); \
	for (int i=0; i<n; i++) { \
		output2[i] = output1[i]; \
	} \
	for (int i=0; i<n; i++) { \
		SWIFFT_##op(output1[i].data, operand[i]); \
	} \
	for (int i=0; i<n; i++) { \
		REQUIRE(output1[i] != output2[i]); \
	} \
	SWIFFT_##op##Multiple(n, output2[0].data, operand); \
	for (int i=0; i<n; i++) { \
		REQUIRE(output1[i] == output2[i]); \
	} \
}

TEST_CODE(ConstSet)
TEST_CODE(ConstAdd)
TEST_CODE(ConstSub)
TEST_CODE(ConstMul)
#undef TEST_CODE

#define TEST_CODE(op) \
TEST_CASE( "swifft multiple " LIBSWIFFT_QUOTE(op) " computes correctly", "[swifft]" ) { \
	const int n = 128; \
	SwifftInput input1[n]; \
	SwifftInput input2[n]; \
	SwifftOutput output0[n]; \
	SwifftOutput output1[n]; \
	SwifftOutput output2[n]; \
	srand(1); \
	randomize(input1, n); \
	randomize(input2, n); \
	SWIFFT_ComputeMultiple(n, input1[0].data, output1[0].data); \
	SWIFFT_ComputeMultiple(n, input2[0].data, output2[0].data); \
	for (int i=0; i<n; i++) { \
		output0[i] = output1[i]; \
	} \
	for (int i=0; i<n; i++) { \
		SWIFFT_##op(output1[i].data, output2[i].data); \
	} \
	for (int i=0; i<n; i++) { \
		REQUIRE(output0[i] != output1[i]); \
	} \
	SWIFFT_##op##Multiple(n, output0[0].data, output2[0].data); \
	for (int i=0; i<n; i++) { \
		REQUIRE(output0[i] == output1[i]); \
	} \
}

TEST_CODE(Set)
TEST_CODE(Add)
TEST_CODE(Sub)
TEST_CODE(Mul)
#undef TEST_CODE

TEST_CASE( "swifft multiple compact computes correctly", "[swifft]" ) {
	const int n = 128;
	SwifftInput input1[n];
	SwifftOutput output1[n];
	SwifftOutput output2[n];
	SwifftCompact compact1[n];
	SwifftCompact compact2[n];
	srand(1);
	randomize(input1, n);
	SWIFFT_ComputeMultiple(n, input1[0].data, output1[0].data);
	for (int i=0; i<n; i++) {
		output2[i] = output1[i];
	}
	for (int i=0; i<n; i++) {
		SWIFFT_Compact(output1[i].data, compact1[i].data);
	}
	for (int i=0; i<n; i++) {
		REQUIRE(compact1[i] != compact2[i]);
	}
	SWIFFT_CompactMultiple(n, output2[0].data, compact2[0].data);
	for (int i=0; i<n; i++) {
		REQUIRE(compact1[i] == compact2[i]);
	}
}

} // end namespace LibSwifft
