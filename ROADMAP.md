# Roadmap of LibSWIFFT

The list below details, in no particular order, a number of directions for future development of LibSWIFFT. We welcome [contributions](CONTRIBUTING.md) in each of these as well as suggestions for additional directions.

- Support for higher-modulus variations of SWIFFT.
- Improved build modularity: multi-versioned build installation, packaging for availability via `find_package` in cmake.
- Build-support for additional platforms, operating systems and toolchains.
- Improved test coverage: numerical edge cases.
- Automatic runtime selection of the best SWIFFT implementation for the native platform.
- Support for parallel processing using OpenMP.
- SWIFFT based hashing for long inputs.
- GPU kernels for SWIFFT functions.

## Out of Scope for LibSWIFFT

The list below gives a few examples of relevant directions that are out of scope for LibSWIFFT and should go into separate projects.

- Wrappers of LibSWIFFT in other programming languages.
- Integrations of LibSWIFFT with established cryptography libraries.
- Implementations of proofs of knowledge or digital signatures based on LibSWIFFT.
