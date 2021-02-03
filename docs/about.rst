About SWIFFT and LibSWIFFT
==========================

On SWIFFT
---------

`SWIFFT <https://en.wikipedia.org/wiki/SWIFFT>`_ is a family of homomorphic hash functions provided as part of a candidate submission to the `NIST hash function competition <https://en.wikipedia.org/wiki/NIST_hash_function_competition>`_ in 2008 by Vadim Lyubashevsky, Daniele Micciancio, Chris Peikert, and Alon Rosen. The family has an interesting set of characteristics:

1. **Provably collision-resistant**: it is provably computationally hard to find a binary-valued `x` given its image under `f` that is chosen randomly.
2. **Universal**: for any `x_1,x_2` in the domain the probability that `f(x_1) = f(x_2)` over the choice of `f` in the family is the inverse of the size of the range.
3. **Regular**: if `x` is distributed uniformly then `f(x)` is distributed uniformly for any `f` in the family.
4. **Randomness extractor**: even if `x` is not distributed uniformly, `f(x)` is distributed uniformly over the choice of `f` in the family.
5. **Homomorphic**: each `f` in the family is homomorphic over the domain.
6. **Facilitates ZKPoKPs**: the resulting hash value can be readily used in ZKPoKPs.
7. **Facilitates post-quantum digital signatures**: its homomorphism property enables short post-quantum hash-based digital signature schemes, such as `K2SN-MSS <https://eprint.iacr.org/2019/442.pdf>`_.
8. **Constant-time**: any `f` in the family is free of data-dependent branching and therefore facilitates avoidance of timing side-channel attacks.

Nevertheless, the family is not pseudorandom, due to the homomorphism property, nor claimed to behave like a random oracle.

On LibSWIFFT
------------

LibSWIFFT was implemented with reference to the `SWIFFTX submission to NIST <https://csrc.nist.gov/projects/hash-functions/sha-3-project>`_ and provides the same SWIFFT hash function that is part of the submission. High speed is achieved using various code optimization techniques, including SIMD instructions that are very natural for the implementation of the SWIFFT function. Compared to the SWIFFT code in the submission, LibSWIFFT adds the following:

1. Automatic library initialization using build-time generation of internal tables.
2. Convenient APIs, including for homomorphic operations, for computing SWIFFT on short inputs.
3. Support for input vectors of either binary-valued (in {0,1}) or trinary-valued (in {-1,0,1}) elements.
4. Bug fixes with respect to the reference submission, in particular related to the homomorphism property.
5. Performance improvements compared to the reference submission.
6. Support for newer CPU instruction sets: AVX, AVX2, and AVX512.
7. Over 20 test-cases providing excellent coverage of the APIs and the mathematical properties of SWIFFT.

Formally, LibSWIFFT provides a single hash function that maps from an input domain `Z_2^{2048}` (taking 256B) to an output domain `Z_{257}^{64}` (taking 128B, at 2B per element) and then to a compact domain `Z_{256}^{65}` (taking 65B). The computation of the first map is done over `Z_{257}`. The homomorphism property applies to the input and output domains, but not to the compact domain, and is revealed when the binary-valued input domain is naturally embedded in `Z_{257}^{2048}`. Generally, it is computationally hard to find a binary-valued pre-image given an output computed as the sum of `N` outputs corresponding to known binary-valued pre-images. On the other hand, it is easy to find a small-valued pre-image (over `Z_{257}^{2048}`) when `N` is small, since it is simply the sum of the known pre-images due to the homomorphism property.
