# Release Checklist

To prepare a pull request for release:

- All code changes should already be in their own commits.
- Decide the version of this release.
- Run the tests-executable to ensure all tests pass.
- Run performance tests (see section below) on appropriate machines to ensure performance is no worse than that of the previous version.
- Run coverage tests (see section below) to ensure code is sufficiently covered.
- Run doxygen (see section below) to test it correctly processes the source code.
- Change the version in `CMakeLists.txt`.
- Repeat the above tests.
- If any test or doxygen issues are found, abort the release until they are fixed.
- Commit with a comment like `version 1.0.0` using the decided version.
- Tag with the decided version preceded by `v`, e.g., `v1.0.0`.
- Create a pull-request.

To prepare a pull request for development, use tag with suffix `+dev`, e.g., `v1.0.0+dev`.

## Running Performance Tests

Performance tests are executed using a release build:

    mkdir -p build/release
    cd build/release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    ./test/swifft_catch "[swifftperf]"

## Running Coverage Tests

Coverage tests are executed using a debug build:

    mkdir -p build/debug
    cd build/debug
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    ./test/swifft_catch
    for file in $(find . -name "*.gcda"); do echo "=== $file ==="; gcov $file; done | less
    for file in $(find . -name "*swifft*.gcov"); do echo "=== $file ==="; cat $file; done | less

This procedure is specific to GCC builds and does not cover performance testing code.

## Running doxygen

To generate documentation:

- Run `doxygen -g doxygen.conf`
- Edit `doxygen.conf`:
  - Set `PROJECT_NAME` to `LibSWIFFT`
  - Set `PROJECT_NUMBER` to the decided version
  - Set `RECURSIVE` to `YES`.
  - Set `EXTRACT_ALL` to `YES`
  - Set `EXCLUDE` to `build`
- Run `doxygen doxygen.conf`
