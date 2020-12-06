# Contributing to LibSWIFFT

We welcome contributions to LibSWIFFT of any type, including the following:

- Issue reports
- Patch proposals
- Porting to other operating systems and C/C++ tool chains
- Wrappers of LibSWIFFT in other programming languages
- In- and out-of-code documentation
- Improvements to source code, testing code, build, and performance

Please see [ROADMAP.md](ROADMAP.md) for development directions of interest.

## Issue Reports

Before reporting issues, please check whether the issue is already reported at the [issues page](https://github.com/gvilitechltd/LibSWIFFT/issues). If not, please open a new issue there.

If the issue is a bug report, please provide any information that could help diagnose the issue, such as version of operating system, version of tool chain used to build the library, version of LibSWIFFT used, and the steps for reproducing the problem.

## Patch Proposals

You are encouraged to propose patches via [a pull request](https://github.com/gvilitechltd/LibSWIFFT/pulls), preferrably with reference to an existing issue or one you created for it.

To help us handle your pull request as efficiently as possible, please ensure that:

- each file include a reference to the license as in existing code.
- the style of new code is similar to that of the existing code.
- the commits in the pull request are to the `main` branch only.
- the purpose and method of a code change is documented in the code.
- there are test cases covering code changes that are invoked via the tests-executable.
- all test cases are using Catch2 with appropriate names and categories.
- the changed code builds and tests using the normal procedures.

## Wrappers of LibSWIFFT

Please note that a wrapper of LibSWIFFT in a different programming language should go in a separate project. If you developed such a wrapper, please let us know about it by creating an issue, so we could add it to a published list.

## Acknowledgements

Contributions accepted to LibSWIFFT will be acknowledged in the contributors list.
