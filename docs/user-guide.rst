User Guide
==========

Getting LibSWIFFT
-----------------

LibSWIFFT is available on `GitHub <https://github.com/gvilitechltd/LibSWIFFT>`.
The code repository can be cloned using `git <https://git-scm.com>`_:

.. code-block:: sh

    git clone https://github.com/gvilitechltd/LibSWIFFT
    cd LibSWIFFT

To switch to a specific version of LibSWIFFT, such as `v1.1.0`, use:

.. code-block:: sh

    git checkout v1.1.0

As an alternative to getting the repository, a specific version of LibSWIFFT can
be obtained from `GitHub <https://github.com/gvilitechltd/LibSWIFFT>`_ by
switching to the tag of this version and downloading the code as an archive.
Then, unpack the archive and change into the unpacked directory, e.g.:

.. code-block:: sh

    wget https://github.com/gvilitechltd/LibSWIFFT/archive/v1.1.0.zip
    unzip v1.1.0.zip
    cd LibSWIFFT-1.1.0

Building LibSWIFFT
------------------

Currently, LibSWIFFT is implemented to be built using GCC. It has been tested on Linux Ubuntu 20.04 LTS using

- `GCC 9.3.0`: GCC is normally installed using `sudo apt-get install gcc g++`.
- `cmake 3.16.3`: CMake is normally installed using `sudo apt-get install cmake`.
- `Catch 2.13.2`: Catch2 is normally installed (as `documented here <https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#installing-catch2-from-git-repository>`_) using

.. code-block:: sh

    git clone https://github.com/catchorg/Catch2.git
    cd Catch2
    git checkout v2.13.2
    cmake -Bbuild -H. -DBUILD_TESTING=OFF
    sudo cmake --build build/ --target install

The build is also expected to work on older versions of

- `GCC` supporting C++11 as well as avx, avx2, or avx512f
- `cmake` supporting `target_include_directories`
- `Catch2`

Running the following commands:

.. code-block:: sh

    mkdir -p build/release
    cd build/release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make

will build:

- The static library `src/libswifft.a`.
- The shared library `src/libswifft.so`.
- The tests-executable `test/swifft_catch`.

By default, the build will be for the native machine. To build with different machine settings, set `SWIFFT_MACHINE_COMPILE_FLAGS` on the `cmake` command line, for example:

.. code-block:: sh

    cmake -DCMAKE_BUILD_TYPE=Release ../.. -DSWIFFT_MACHINE_COMPILE_FLAGS=-march=skylake

To build with OpenMP, in particular for parallelizing multiple-block operations, add `-DSWIFFT_ENABLE_OPENMP=on` on the `cmake` command line, for example:

.. code-block:: sh

    cmake -DCMAKE_BUILD_TYPE=Release ../.. -DSWIFFT_ENABLE_OPENMP=On

After building, run the tests-executable from the `build/release` directory:

.. code-block:: sh

    ./test/swifft_catch

If all tests pass, LibSWIFFT is good to go!

For development with LibSWIFFT, use the headers in the `include` directory and either the static or dynamic library.

Using LibSWIFFT
---------------

LibSWIFFT is intended to be used by cryptography researchers and by software developers knowledgeable in cryptography programming. LibSWIFFT is most useful in use cases that require provable-security and
speed on short inputs. It may also be interesting in use cases that take advantage of its uncommon homomorphism property. Future versions of LibSWIFFT may target a larger audience.

The main LibSwifft C API is documented in :libswifft:`swifft.h`. The following API variations are available:

- :libswifft:`swifft_avx.h`: Same functions as in :libswifft:`swifft.h` but with an added suffix `_AVX` and implemented using AVX instruction set.
- :libswifft:`swifft_avx2.h`: Same functions as in :libswifft:`swifft.h` but with an added suffix `_AVX2` and implemented using AVX2 instruction set.
- :libswifft:`swifft_avx512.h`: Same functions as in :libswifft:`swifft.h` but with an added suffix `_AVX512` and implemented using AVX512 instruction set.
- :libswifft:`swifft.h`: Selects the implementations using the most advanced instruction set that was built into the library.

The version of LibSWIFFT is provided by the API in :libswifft:`swifft_ver.h`.

The main LibSWIFFT C++ API is documented in :libswifft:`swifft.hpp`.

Typical code using the C API:

.. code-block:: c

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

Buffers must be memory-aligned in order to avoid a segmentation fault when passed to `LibSWIFFT` functions: statically allocated buffers should be aligned using `SWIFFT_ALIGN`, and dynamically allocated buffers should use an alignment of `SWIFFT_ALIGNMENT`, e.g., via `aligned_alloc` function in `stdlib.h`. The transformation functions `SWIFFT_{Compute,Compact}Multiple{,Signed}*` apply operations to multiple blocks. The arithmetic functions `SWIFFT_{Const,}{Set,Add,Sub,Mul}*` provide vectorized and homomorphic operations on an output block, while `SWIFFT_{Const,}{Set,Add,sub,Mul}Multiple*` provide corresponding operations to multiple blocks.

Typical code using the C++ API:

.. code-block:: cpp

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

Assignment and equality operators are available for `Swifft{Input,Output,Compact}` instances. Arithemtic and arithmetic-assignment operators, corresponding to the arithmetic functions in the C API, are available for `SwifftOutput` instances.

