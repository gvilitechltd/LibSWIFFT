---
title: 'LibSWIFFT - A fast C/C++ Library for the SWIFFT Secure Homomorphic Hash Function'
tags:
  - cryptography
  - post-quantum-cryptography 
  - lattice-based-crypto
  - secure-hashing
  - hash-functions
  - homomorphic
authors:
  - name: Yaron Gvili
    affiliation: 1
affiliations:
 - name: Gvili Tech Ltd
   index: 1
date: 1 January 2021
bibliography: paper.bib
---

# Summary

LibSWIFFT is an open-source production-ready C/C++ library providing SWIFFT, one
of the fastest available secure hash functions that is also collision-resistant
as well as facilitates post-quantum digital signature schemes and zero-knowledge
proofs of knowledge of a preimage (ZKPoKP). LibSWIFFT is optimized for short
blocks of input and runs at a rate of less than 5 cycles/byte single-threaded on
a modern commodity computer with AVX2. Other software providing SWIFFT, which
are not claiming production-readiness as LibSWIFFT is, are the original
implementation by the authors of SWIFFT [@swifftgit] and the SWIFFT 8-bit
[@k2snswifft8git] and 16-bit [@k2snswifft16git] AVX2 implementations for the
multi-signature scheme K2SN-MSS [@cryptoeprint:2019:442].

LibSWIFFT is currently intended to be used by cryptography researchers and
developers. It provides clean easy-to-use C/C++ APIs with high-performance
implementations and is well-tested and well-documented. Other available
implementations of the SWIFFT function do not provide all these benefits. For
further details, the reader is referred to the official LibSWIFFT repository
[@libswifftgit].

# Statement of Need

LibSWIFFT implements the SWIFFT [@swifft2008] secure homomorphic hash function
useful in constructing post-quantum protocols -- ones that are resistant to
attacks utilizing quantum computers. Such protocols are relevant today due to
recent advances in quantum computing technology. In late 2017, NIST started a
process for standardizing post-quantum cryptography [@nistpqc], suggesting that
it believes it may not be too long before a practical quantum-computer that
threatens critical security standards (including Internet ones) based on
classical cryptography will become a reality. Consequently, post-quantum
cryptography is becoming more relevant today and perhaps even urgent to develop.

# Acknowledgements

LibSWIFFT was developed with reference to the SWIFFTX [@swifftx2008] submission
to the NIST SHA-3 competition in 2008.

# References
