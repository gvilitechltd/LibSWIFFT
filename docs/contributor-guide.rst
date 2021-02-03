Contributor Guide
=================

Contributing to LibSWIFFT
-------------------------

We welcome contributions to LibSWIFFT of any type, including the following:

- :ref:`Issue reports <issue_reports>`
- :ref:`Discussions <discussions>`
- :ref:`Patch proposals <patch_proposals>`
- :ref:`Porting <porting>` to other operating systems and C/C++ tool chains
- :ref:`Wrappers <wrappers>` of LibSWIFFT in other programming languages
- :ref:`documentation <documentation>` within and outside the code
- :ref:`Improvements <improvements>` to source code, testing code, build system, and performance

Contributors are encouraged to watch the
`LibSWIFFT repository <https://gihub.com/gvilitechltd/LibSWIFFT>`_ for updates.

.. _roadmap:

Roadmap of LibSWIFFT
--------------------

The list below details, in no particular order, a number of directions for future development of LibSWIFFT. We welcome contributions in each of these as well as suggestions for additional directions.

- Support for higher-modulus variations of SWIFFT.
- Improved build modularity: multi-versioned build installation, packaging for availability via `find_package` in cmake.
- Build-support for additional platforms, operating systems and toolchains.
- Improved test coverage: numerical edge cases.
- Automatic runtime selection of the best SWIFFT implementation for the native platform.
- Support for parallel processing using OpenMP.
- SWIFFT based hashing for long inputs.
- GPU kernels for SWIFFT functions.

.. _out_of_scope:

Out of Scope for LibSWIFFT
--------------------------

The list below gives a few examples of relevant directions that are out of scope for LibSWIFFT and should go into separate projects.

- Wrappers of LibSWIFFT in other programming languages.
- Integrations of LibSWIFFT with established cryptography libraries.
- Implementations of proofs of knowledge or digital signatures based on LibSWIFFT.

.. _issue_reports:

Issue Reports
-------------

Before reporting issues, please check whether the issue is already reported at the `issues page <https://github.com/gvilitechltd/LibSWIFFT/issues>`_. If not, please open a new issue there.

If the issue is a bug report, please provide any information that could help diagnose the issue, such as version of operating system, version of tool chain used to build the library, version of LibSWIFFT used, and the steps for reproducing the problem.

.. _discussions:

Discussions
-----------

Before starting new discussions, please consider whether an issue report would be more appropriate. If a discussion is appropriate, check whether you can join an existing discussion that has already been created at the `discussion page <https://github.com/gvilitechltd/LibSWIFFT/discussions>`_. If you cannot find such a discussion, feel free to open a new one there.

Please provide a clear topic for your new discussion and elaborate in the body of the first message. The goal is to keep each discussion focused on one topic.

.. _patch_proposals:

Patch Proposals
---------------

You are encouraged to propose patches via `a pull request <https://github.com/gvilitechltd/LibSWIFFT/pulls>`_, preferrably with reference to an existing issue or one you created for it.

To help us handle your pull request as efficiently as possible, please ensure that:

- each file include a reference to the license as in existing code.
- the style of new code is similar to that of the existing code.
- the commits in the pull request are to the `main` branch only.
- the purpose and method of a code change is documented in the code.
- there are test cases covering code changes that are invoked via the tests-executable.
- all test cases are using Catch2 with appropriate names and categories.
- the changed code builds and tests using the normal procedures.

.. _porting:

Porting
-------

Please note that porting to other operating systems and C/C++ tool chains is expected to enhance the existing `cmake <http://cmake.org>`_-based build system. In case of running into an issue with meeting this expectation for some porting effort, please open a discussion about it.

.. _wrappers:

Wrappers of LibSWIFFT
---------------------

Please note that a wrapper of LibSWIFFT in a different programming language should go in a separate project. If you developed such a wrapper, please let us know about it by creating an issue, so we could add it to a published list.

.. _documentation:

Documentation
-------------

.. _improvements:

Contributions to documentation of various kinds, within and outside of the code, are welcomed, for example:

* Documentation of the build system.
* Explanation of mathematics, algorithms, techniques, and logic.
* Documentation of correctness and performance tests.
* Description of architecture, design, and conventions.
* Discussion or examples of secure and insecure uses of the APIs.
* Guidance for specific types of code contributions, such as multi-threading features.

Improvements
------------

Contributions that improve the source code, testing code, build system, and performance of LibSWIFFT are welcomed. Please refer to the :ref:`roadmap <roadmap>` for possible directions and know what is considered :ref:`out of scope <out_of_scope>` for LibSWIFFT.

Acknowledgements
----------------

Contributions accepted to LibSWIFFT will be acknowledged in the contributors list.

