![LibSWIFFT](assets/LibSWIFFT-logo.png)

# LibSWIFFT - A fast C/C++ library for the SWIFFT secure homomorphic hash function

- [Official Repository](https://github.com/gvilitechltd/LibSWIFFT)
- [![Documentation Status](https://readthedocs.org/projects/libswifft/badge/?version=latest)](http://libswifft.readthedocs.io/en/latest/)
- [![Packaging Status](https://github.com/gvilitechltd/libswifft/actions/workflows/new-code.yml/badge.svg?branch=main)](https://github.com/gvilitechltd/libswifft/actions/workflows/new-code.yml/)
- **Quick start on Linux**: Ensure `docker` is installed and runnable, clone this repository and go to its root directory, and run the command `docker build . && docker run --rm -it $(docker build -q .)` to build and test LibSWIFFT.

## Introducing LibSWIFFT

LibSWIFFT is a production-ready C/C++ library providing SWIFFT, one of the fastest available secure hash functions, which is also collision-resistant as well as facilitates zero-knowledge proofs of knowledge of a preimage (ZKPoKP) and post-quantum digital signatures. It is based on academic work from 2007 described further below.

**Why now, in early 2021?** In late 2017, NIST has started a [process for standardizing post-quantum cryptography](https://csrc.nist.gov/Projects/post-quantum-cryptography/post-quantum-cryptography-standardization), suggesting that it believes it may not be too long before a practical quantum-computer that threatens critical security standards (including Internet ones) based on classical cryptography will become a reality. About two years later, Google announced it had achieved [quantum supremacy](https://en.wikipedia.org/wiki/Quantum_supremacy), by completing in 200 seconds a task they claimed would have taken a classical supercomputer about 10,000 years to complete. Though IBM, maker of the most powerful supercomputer at the time, disputed this claim and asserted the supercomputer would take only about 2.5 days for the task, it is clear quantum computing technology is advancing quickly. Consequently, post-quantum cryptography is becoming more relevant today and perhaps even urgent to develop.

**Why another implementation of SWIFFT?** LibSWIFFT is a reliable building block for fast and scalable cryptographic protocols. It is simple to use and maintain, has clean APIs, is well documented and tested, and is at least as fast as other implementations of SWIFFT and often faster. Other implementations of SWIFFT are:

- The [8-bit](https://github.com/anon1985/Swifft-avx2-8) and [16-bit](https://github.com/anon1985/K2SN-MSS/swifft16) AVX2 implementations for [K2SN-MSS](https://eprint.iacr.org/2019/442.pdf). Both are arguably not as easy to use nor as well documented and tested as LibSWIFFT. The former is slower and uses less memory while the latter is about as fast as LibSWIFFT for AVX2 yet does not support AVX512.
- [The original implementation](https://github.com/micciancio/SWIFFT) written in 2007. It is minimal non-production code. [The AVX2 implementation for K2SN-MSS](https://eprint.iacr.org/2019/442.pdf) is reported to be 25% faster.

An invocation of the tests-executable of LibSWIFFT running single-threaded using AVX2 on an Intel Skylake microarchitecture (Intel(R) Core(TM) i7-10875H CPU @ 2.30GHz):

    $ ./test/swifft_catch "swifft takes at most 2000 cycles per call"
    Filters: swifft takes at most 2000 cycles per call
    running 1*10000000 rounds: cycles/rounds=1097.94 cycles/byte=4.28882 Giga-cycles/sec=2.30399 MB/sec=512.322 cycles/rdtsc=16

demonstrates that LibSWIFFT is quite fast on short inputs (here, 256 bytes), often used in practical zero-knowledge proofs and post-quantum digital signatures. This is more than an order of magnitude faster than the [originally reported](https://www.alonrosen.net/PAPERS/lattices/swifft.pdf) 40MB/sec on a 3.2 GHz Intel Pentium 4. It also compares well with modern hash functions:

- [Blake3](https://github.com/BLAKE3-team/BLAKE3) - cryptographic hash function achieving about [3-to-4 cycles/byte using AVX512 on short inputs](https://github.com/BLAKE3-team/BLAKE3-specs/blob/master/blake3.pdf) and are non-homomorphic nor facilitating proofs of knowledge of a preimage.
- [Seahash](https://docs.rs/seahash/4.0.1/seahash/index.html) - a hash function achieving ~0.24 cycles/byte but is non-cryptographic.

## On SWIFFT

[SWIFFT](https://en.wikipedia.org/wiki/SWIFFT) is a family of homomorphic hash functions provided as part of a candidate submission to the [NIST hash function competition](https://en.wikipedia.org/wiki/NIST_hash_function_competition) in 2008 by Vadim Lyubashevsky, Daniele Micciancio, Chris Peikert, and Alon Rosen. The family has an interesting set of characteristics:

1. **Provably collision-resistant**: it is provably computationally hard to find a binary-valued `x` given its image under `f` that is chosen randomly.
2. **Universal**: for any `x_1,x_2` in the domain the probability that `f(x_1) = f(x_2)` over the choice of `f` in the family is the inverse of the size of the range.
3. **Regular**: if `x` is distributed uniformly then `f(x)` is distributed uniformly for any `f` in the family.
4. **Randomness extractor**: even if `x` is not distributed uniformly, `f(x)` is distributed uniformly over the choice of `f` in the family.
5. **Homomorphic**: each `f` in the family is homomorphic over the domain.
6. **Facilitates ZKPoKPs**: the resulting hash value can be readily used in ZKPoKPs.
7. **Facilitates post-quantum digital signatures**: its homomorphism property enables short post-quantum hash-based digital signature schemes, such as [K2SN-MSS](https://eprint.iacr.org/2019/442.pdf).
8. **Constant-time**: any `f` in the family is free of data-dependent branching and therefore facilitates avoidance of timing side-channel attacks.

Nevertheless, the family is not pseudorandom, due to the homomorphism property, nor claimed to behave like a random oracle.

## On LibSWIFFT

LibSWIFFT was implemented with reference to the [SWIFFTX submission to NIST](https://csrc.nist.gov/projects/hash-functions/sha-3-project) and provides the same SWIFFT hash function that is part of the submission. High speed is achieved using various code optimization techniques, including SIMD instructions that are very natural for the implementation of the SWIFFT function. Compared to the SWIFFT code in the submission, LibSWIFFT adds the following:

1. Automatic library initialization using build-time generation of internal tables.
2. Convenient APIs, including for homomorphic operations and parallel variations based on OpemMP, for computing SWIFFT on short inputs.
3. Support for input vectors of either binary-valued (in {0,1}) or trinary-valued (in {-1,0,1}) elements.
4. Bug fixes with respect to the reference submission, in particular related to the homomorphism property.
5. Performance improvements compared to the reference submission.
6. Support for newer CPU instruction sets: AVX, AVX2, and AVX512.
7. Over 30 test-cases providing excellent coverage of the APIs and the mathematical properties of SWIFFT.

Formally, LibSWIFFT provides a single hash function that maps from an input domain `Z_2^{2048}` (taking 256B) to an output domain `Z_{257}^{64}` (taking 128B, at 2B per element) and then to a compact domain `Z_{256}^{65}` (taking 65B). The computation of the first map is done over `Z_{257}`. The homomorphism property applies to the input and output domains, but not to the compact domain, and is revealed when the binary-valued input domain is naturally embedded in `Z_{257}^{2048}`. Generally, it is computationally hard to find a binary-valued pre-image given an output computed as the sum of `N` outputs corresponding to known binary-valued pre-images. On the other hand, it is easy to find a small-valued pre-image (over `Z_{257}^{2048}`) when `N` is small, since it is simply the sum of the known pre-images due to the homomorphism property.

## Using LibSWIFFT

LibSWIFFT is intended to be used by cryptography researchers and by software developers knowledgeable in cryptography programming. LibSWIFFT is most useful in use cases that require provable-security and speed on short inputs. It may also be interesting in use cases that take advantage of its uncommon homomorphism property. Future versions of LibSWIFFT may target a larger audience.

The main LibSwifft C API is documented in `include/libswifft/swifft.h`. The following API variations are available:

- `include/libswifft/swifft_avx.h`: Same functions as in `include/libswifft/swifft.h` but with an added suffix `_AVX` and implemented using AVX instruction set.
- `include/libswifft/swifft_avx2.h`: Same functions as in `include/libswifft/swifft.h` but with an added suffix `_AVX2` and implemented using AVX2 instruction set.
- `include/libswifft/swifft_avx512.h`: Same functions as in `include/libswifft/swifft.h` but with an added suffix `_AVX512` and implemented using AVX512 instruction set.
- `include/libswifft/swifft.h`: Selects the implementations using the most advanced instruction set that was built into the library.

The version of LibSWIFFT is provided by the API in `include/libswifft/swifft_ver.h`.

The main LibSWIFFT C++ API is documented in `include/libswifft/swifft.hpp`.

Refer to the [release checklist document](RELEASE-CHECKLIST.md) for how to generate the documentation for the APIs using doxygen.

Typical code using the C API:

```C
#include "libswifft/swifft.h"
/* later, inside a function: */
SWIFFT_ALIGN BitSequence input[SWIFFT_INPUT_BLOCK_SIZE]; /* memory-aligned */
SWIFFT_ALIGN BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE]; /* memory-aligned */
SWIFFT_ALIGN BitSequence compact[SWIFFT_COMPACT_BLOCK_SIZE]; /* optional, memory-aligned */
SWIFFT_ALIGN BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE]; /* optional, memory-aligned */
/* after input (and optionally sign) is populated (not shown here), it is time to compute the hash: */
SWIFFT_Compute(input, output); /* compute the hash of the input into the output */
SWIFFT_Compact(output, compact); /* optionally, compact the hash */
/* or the signed hash can be computed instead */
SWIFFT_Compute(input, sign, output); /* compute the hash of the signed input into the output */
SWIFFT_Compact(output, compact); /* optionally, compact the hash */
```

Buffers must be memory-aligned in order to avoid a segmentation fault when passed to `LibSWIFFT` functions: statically allocated buffers should be aligned using `SWIFFT_ALIGN`, and dynamically allocated buffers should use an alignment of `SWIFFT_ALIGNMENT`, e.g., via `aligned_alloc` function in `stdlib.h`. The transformation functions `SWIFFT_{Compute,Compact}Multiple{,Signed}*` apply operations to multiple blocks. The arithmetic functions `SWIFFT_{Const,}{Set,Add,Sub,Mul}*` provide vectorized and homomorphic operations on an output block, while `SWIFFT_{Const,}{Set,Add,sub,Mul}Multiple*` provide corresponding operations to multiple blocks.

Typical code using the C++ API:

```C
#include "libswifft/swifft.hpp"
using namespace LibSwifft;
/* later, inside a function: */
SwifftInput input; /* auto-memory-aligned */
SwifftOutput output; /* auto-memory-aligned */
SwifftCompact compact; /* optional, auto-memory-aligned */
SwifftInput sign; /* optional, auto-memory-aligned */
/* after input (and optionally sign) is populated (not shown here), it is time to compute the hash: */
SWIFFT_Compute(input.data, output.data); /* compute the hash of the input into the output */
SWIFFT_Compact(output.data, compact.data); /* optionally, compact the hash */
/* or the signed hash can be computed instead */
SWIFFT_Compute(input.data, sign.data, output.data); /* compute the hash of the signed input into the output */
SWIFFT_Compact(output.data, compact.data); /* optionally, compact the hash */
```

Assignment and equality operators are available for `Swifft{Input,Output,Compact}` instances. Arithemtic and arithmetic-assignment operators, corresponding to the arithmetic functions in the C API, are available for `SwifftOutput` instances.

## Building LibSWIFFT

Currently, LibSWIFFT is implemented to be built using GCC. It has been tested on Linux Ubuntu 20.04 LTS using

- `GCC 9.3.0`: GCC is normally installed using `sudo apt-get install gcc g++`.
- `cmake 3.16.3`: CMake is normally installed using `sudo apt-get install cmake`.
- `Catch 2.13.2`: Catch2 is normally installed (as [documented here](https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#installing-catch2-from-git-repository)) using

```sh
git clone https://github.com/catchorg/Catch2.git
cd Catch2
git checkout v2.13.2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
```

The build is also expected to work on older versions of

- `GCC` supporting C++11 as well as avx, avx2, or avx512f
- `cmake` supporting `target_include_directories`
- `Catch2`

Running the following commands:

```sh
mkdir -p build/release
cd build/release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```

will build:

- The static library `src/libswifft.a`.
- The shared library `src/libswifft.so`.
- The tests-executable `test/swifft_catch`.

By default, the build will be for the native machine. To build with different machine settings, set `SWIFFT_MACHINE_COMPILE_FLAGS` on the `cmake` command line, for example:

```sh
cmake -DCMAKE_BUILD_TYPE=Release ../.. -DSWIFFT_MACHINE_COMPILE_FLAGS=-march=skylake
```

To build with OpemMP, in particular for parallelizing multiple-block operations, add `-DSWIFFT_ENABLE_OPENMP=on` on the `cmake` command line, for example:

```sh
cmake -DCMAKE_BUILD_TYPE=Release ../.. -DSWIFFT_ENABLE_OPENMP=On
```

After building, run the tests-executable from the `build/release` directory:

```sh
./test/swifft_catch
```

If all tests pass, LibSWIFFT is good to go!

For development with LibSWIFFT, use the headers in the `include` directory and either the static or dynamic library.

## Roadmap

Please see [ROADMAP.md](ROADMAP.md).

## Contributing

Please see [CONTRIBUTING.md](CONTRIBUTING.md).
