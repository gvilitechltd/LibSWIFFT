.. LibSWIFFT documentation master file, created by
   sphinx-quickstart on Sat Jan 30 16:08:46 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :maxdepth: 2

.. image:: ../assets/LibSWIFFT-logo.png

LibSWIFFT: Fast Secure Homomorphic Hashing
==========================================

LibSWIFFT is a production-ready C/C++ library providing SWIFFT, one of the
fastest available secure hash functions, which is also collision-resistant as
well as facilitates zero-knowledge proofs of knowledge of a preimage (ZKPoKP)
and post-quantum digital signatures.

Secure Homomorphic Hashing
--------------------------

LibSWIFFT provides a function to hash an input vector of 2048 values in [0..1],
given in a buffer of 256 bytes (packed 8-bits), to an output vector of 64 values
in [0..256], given in a buffer of 64 words (padded 16-bits). It also provides a
function to hash a vector of 2048 values in [-1..1], given in an input and a
sign vector, each of 2048 bits. These are implementations of the SWIFFT secure
hash function homomorphic (i.e., the hash of the sum of vectors is the sum of
their hashes) in modulu-257 arithmetic.

Fast
----

LibSWIFFT is fast. Here is the output from a test-case of LibSwifft v1.0.0 that
demonstrates hashing (for an input of 256 bytes) running at a rate of less than
5 cycles-per-byte single-threaded using AVX2 on an Intel Skylake
microarchitecture (Intel(R) Core(TM) i7-10875H CPU @ 2.30GHz):

.. code-block:: sh

    $ ./test/swifft_catch "swifft takes at most 2000 cycles per call"
    Filters: swifft takes at most 2000 cycles per call
    running 1*10000000 rounds: cycles/rounds=1097.94 cycles/byte=4.28882 Giga-cycles/sec=2.30399 MB/sec=512.322 cycles/rdtsc=16

This is faster than the rate of the recent
`K2SN-MSS's binary 16-bit SWIFFT function implementation <https://github.com/gvilitechltd/K2SN-MSS/tree/swifftperf>`_
(for an input of 128 bytes) on the same execution settings, i.e. single-threaded
using AVX2 on an Intel Skylake microarchitecture (Intel(R) Core(TM) i7-10875H
CPU @ 2.30GHz):

.. code-block:: sh

   $ ./tester
    1000000 SWIFFT16 rounds: cycles/round=737.363098 cycles/byte=5.760649

LibSWIFFT supports microarchitectures with AVX, AVX2, and AVX512 instruction
sets to take advantage of available hardware acceleration.

Production-Ready
----------------

LibSWIFFT v1.1.0 ships with over 30 test-cases including millions of checks that
provide excellent coverage of the library's code and the mathematical properties
of the SWIFFT function it implements:

.. code-block:: sh

    $ ./test/swifft_catch

    ===============================================================================
    All tests passed (6648383 assertions in 32 test cases)

LibSWIFFT also ships with detailed documentation, including references for its
:libswifft:`C API <swifft.h>` and :libswifft:`C++ API <LibSwifft>`. It has a
robust and portable build system based on `cmake <http://cmake.org>`_. All
include files are conveniently organized under a dedicated
`libswifft <api/files.html>`_ directory. Finally,
it requires few dependencies at build-time and run-time.

Easy to use C and C++ APIs
--------------------------

LibSWIFFT is also easy to use. Minimal code using the
:libswifft:`C API <swifft.h>` looks like this:

.. code-block:: c

    #include "libswifft/swifft.h"
    /* later, inside a function: */
    SWIFFT_ALIGN BitSequence input[SWIFFT_INPUT_BLOCK_SIZE];
    SWIFFT_ALIGN BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE];
    /* after input is populated (not shown here), it is time to compute the hash: */
    SWIFFT_Compute(input, output); /* compute the hash of the input into the output */

The :libswifft:`SWIFFT_ALIGN` macro is needed for the required memory-alignment
of buffers passed to LibSWIFFT functions. The equivalent code using the
:libswifft:`C++ API <LibSwifft>` looks like this:

.. code-block:: cpp

    #include "libswifft/swifft.hpp"
    using namespace LibSwifft;
    /* later, inside a function: */
    SwifftInput input;
    SwifftOutput output;
    /* after input is populated (not shown here), it is time to compute the hash: */
    SWIFFT_Compute(input.data, output.data); /* compute the hash of the input into the output */

:libswifft:`SwifftInput` and :libswifft:`SwifftOutput` are auto-memory-aligned.

The code using the :libswifft:`C API <swifft.h>` for signed hashing in
compact-form looks like this:

.. code-block:: c

    #include "libswifft/swifft.h"
    /* later, inside a function: */
    SWIFFT_ALIGN BitSequence input[SWIFFT_INPUT_BLOCK_SIZE];
    SWIFFT_ALIGN BitSequence output[SWIFFT_OUTPUT_BLOCK_SIZE];
    SWIFFT_ALIGN BitSequence compact[SWIFFT_COMPACT_BLOCK_SIZE];
    SWIFFT_ALIGN BitSequence sign[SWIFFT_INPUT_BLOCK_SIZE];
    /* after input and sign are populated (not shown here), it is time to compute the hash: */
    SWIFFT_Compute(input, sign, output); /* compute the hash of the signed input into the output */
    SWIFFT_Compact(output, compact); /* compact the hash */

And the equivalent code using the :libswifft:`C++ API <LibSwifft>` looks like
this:

.. code-block:: cpp

    #include "libswifft/swifft.hpp"
    using namespace LibSwifft;
    /* later, inside a function: */
    SwifftInput input;
    SwifftOutput output;
    SwifftCompact compact;
    SwifftInput sign;
    /* after input and sign are populated (not shown here), it is time to compute the hash: */
    SWIFFT_Compute(input.data, sign.data, output.data); /* compute the hash of the signed input into the output */
    SWIFFT_Compact(output.data, compact.data);

Rationale
=========

Here the rationale for making LibSWIFFT is explained.

.. toctree::
   :maxdepth: 2

   rationale

About SWIFFT and LibSWIFFT
==========================

This part describes SWIFFT and LibSWIFFT.

.. toctree::
   :maxdepth: 2

   about

User Guide
==========

The User Guide explains how to get started with LibSWIFFT and how to make the
most of it.

.. toctree::
   :maxdepth: 2

   user-guide

Community Guide
===============

The Community Guide explains how to participate in LibSWIFFT's community.

.. toctree::
   :maxdepth: 2

   community-guide
   code-design

API Guide
=========

The API Guide leads to the LibSWIFFT API reference.

.. toctree::
   :maxdepth: 2

   api-guide

Contributor Guide
=================

The Contributor Guide explains how to contribute to LibSWIFFT.

.. toctree::
   :maxdepth: 2

   contributor-guide
   code-of-conduct
