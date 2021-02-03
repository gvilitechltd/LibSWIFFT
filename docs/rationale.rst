Rationale for Making LibSWIFFT
==============================

LibSWIFFT is based on academic work from 2007 described further below.

Why now, in early 2021?
-----------------------

In late 2017, NIST has started a `process for standardizing post-quantum cryptography <https://csrc.nist.gov/Projects/post-quantum-cryptography/post-quantum-cryptography-standardization>`_, suggesting that it believes it may not be too long before a practical quantum-computer that threatens critical security standards (including Internet ones) based on classical cryptography will become a reality. About two years later, Google announced it had achieved `quantum supremacy <https://en.wikipedia.org/wiki/Quantum_supremacy>`_, by completing in 200 seconds a task they claimed would have taken a classical supercomputer about 10,000 years to complete. Though IBM, maker of the most powerful supercomputer at the time, disputed this claim and asserted the supercomputer would take only about 2.5 days for the task, it is clear quantum computing technology is advancing quickly. Consequently, post-quantum cryptography is becoming more relevant today and perhaps even urgent to develop.

Why another implementation of SWIFFT?
-------------------------------------

LibSWIFFT is a reliable building block for fast and scalable cryptographic protocols. It is simple to use and maintain, has clean APIs, is well documented and tested, and is at least as fast as other implementations of SWIFFT and often faster. Other implementations of SWIFFT are:

- The `8-bit <https://github.com/anon1985/Swifft-avx2-8>`_ and `16-bit <https://github.com/anon1985/K2SN-MSS/swifft16>`_ AVX2 implementations for `K2SN-MSS <https://eprint.iacr.org/2019/442.pdf>`_. Both are arguably not as easy to use nor as well documented and tested as LibSWIFFT. The former is slower and uses less memory while the latter is about as fast as LibSWIFFT for AVX2 yet does not support AVX512.
- `The original implementation <https://github.com/micciancio/SWIFFT>`_ written in 2007. It is minimal non-production code. `The AVX2 implementation for K2SN-MSS <https://eprint.iacr.org/2019/442.pdf>`_ is reported to be 25% faster.

An invocation of the tests-executable of LibSWIFFT running single-threaded using AVX2 on an Intel Skylake microarchitecture (Intel(R) Core(TM) i7-10875H CPU @ 2.30GHz):

.. code-block:: sh

    $ ./test/swifft_catch "swifft takes at most 2000 cycles per call"
    Filters: swifft takes at most 2000 cycles per call
    running 1*10000000 rounds: cycles/rounds=1097.94 cycles/byte=4.28882 Giga-cycles/sec=2.30399 MB/sec=512.322 cycles/rdtsc=16

demonstrates that LibSWIFFT is quite fast on short inputs (here, 256 bytes), often used in practical zero-knowledge proofs and post-quantum digital signatures. This is more than an order of magnitude faster than the `originally reported <https://www.alonrosen.net/PAPERS/lattices/swifft.pdf>`_ 40MB/sec on a 3.2 GHz Intel Pentium 4. It also compares well with modern hash functions:

- `Blake3 <https://github.com/BLAKE3-team/BLAKE3>`_ - cryptographic hash function achieving about `3-to-4 cycles/byte using AVX512 on short inputs <https://github.com/BLAKE3-team/BLAKE3-specs/blob/master/blake3.pdf>`_ and are non-homomorphic nor facilitating proofs of knowledge of a preimage.
- `Seahash <https://docs.rs/seahash/4.0.1/seahash/index.html>`_ - a hash function achieving ~0.24 cycles/byte but is non-cryptographic.
