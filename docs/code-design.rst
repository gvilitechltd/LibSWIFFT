Design of LibSWIFFT Code
========================

At a high level, LibSWIFFT includes a layer of C code that provides the C API
and a layer of C++ code on top of it that provides the C++ API. New features are
normally added to the C layer and then wrapped by the C++ layer.

The C layer is composed of the main API and the microarchitecture-specific APIs,
discussed in more detail below. In regular use cases, one would normally use the
main API only, which selects the best microarchitecture-specific API for the
platform the library was built for. In more sophisticated use cases, one could
use a microarchitecture-specific APIs directly, e.g., to provide an optimized
feature. The main C and C++ APIs are respectively provided by the `swifft.h` and
`swifft.hpp` headers.

File and Directory Structure
----------------------------

LibSWIFFT has the following file and directory structure:

.. list-table::
   :widths: 50 50
   :header-rows: 1

   * - File or Directory
     - Description                                         
   * - include
     - root directory of headers                           
   * - . libswifft
     - directory of LibSWIFFT headers                      
   * - . . :libswifft:`common.h`
     - LibSWIFFT public C common definitions               
   * - . . :libswifft:`swifft.h`
     - LibSWIFFT public C API                              
   * - . . :libswifft:`swifft.hpp`
     - LibSWIFFT public C++ API                            
   * - . . :libswifft:`swifft_avx.h`
     - LibSWIFFT public C API for AVX                      
   * - . . :libswifft:`swifft_avx2.h`
     - LibSWIFFT public C API for AVX2                     
   * - . . :libswifft:`swifft_avx512.h`
     - LibSWIFFT public C API for AVX512                   
   * - . . :libswifft:`swifft_common.h`
     - LibSWIFFT public C definitions                      
   * - . . :libswifft:`swifft_iset.inl`
     - LibSWIFFT public C API expansion for instruction-sets
   * - . . :libswifft:`swifft_ver.h`
     - LibSWIFFT public C API                              
   * -  src
     - directory of LibSWIFFT sources                      
   * - . :libswifft:`swifft.c`
     - LibSWIFFT public C implementation                   
   * - . :libswifft:`swifft.inl`
     - LibSWIFFT internal C code expansion                 
   * - . :libswifft:`swifft_avx.c`
     - LibSWIFFT public C implementation for AVX           
   * - . :libswifft:`swifft_avx2.c`
     - LibSWIFFT public C implementation for AVX2          
   * - . :libswifft:`swifft_avx512.c`
     - LibSWIFFT public C implementation for AVX512        
   * - . :libswifft:`swifft_impl.inl`
     - LibSWIFFT internal C definitions                    
   * - . :libswifft:`swifft_keygen.cpp`
     - LibSWIFFT internal C code generation                
   * - . :libswifft:`swifft_ops.inl`
     - LibSWIFFT internal C code expansion                 

Main API
--------

The main C API has the following organization:

- **FFT functions**: :libswifft:`SWIFFT_fft`, :libswifft:`SWIFFT_fftsum`.
  These are the two low-level stages in a SWIFFT hash computation and are
  normally not used directly.
- **Transformation functions**: :libswifft:`SWIFFT_Compute`,
  :libswifft:`SWIFFT_Compact`. These transform from input to output and from
  output to compact forms.
- **Arithmetic functions**: :libswifft:`SWIFFT_Set`, :libswifft:`SWIFFT_Add`,
  :libswifft:`SWIFFT_Sub`, :libswifft:`SWIFFT_Mul`,
  :libswifft:`SWIFFT_ConstSet`, :libswifft:`SWIFFT_ConstAdd`,
  :libswifft:`SWIFFT_ConstSub`, :libswifft:`SWIFFT_ConstMul`. These set, add,
  subtract, or multiply given two output forms or one output form and a constant
  value.
- **Functions for multiple blocks**: These are functions with `Multiple` as part
  of their name. They operate on a number of blocks given as a parameter, rather 
  than one block like the corresponding (i.e., without `Multiple`) single-block
  functions.

The main C++ API has the following organization:

- **Wrapper classes**: :libswifft:`SwifftInput`, :libswifft:`SwifftOutput`,
  :libswifft:`SwifftCompact`. These wrap input, output, and compact buffers with
  automatic memory-alignment.
- **Wrapper logical operators**: The wrapper classes provide equality and
  inequality operators.
- **Wrapper arithmetic operators**: The wrapper classes provides operators for
  setting, adding, subtracting, and multiplying the current `SwifftOutput`
  instance with another or with a constant value.

Microarchitecture-Specific APIs
-------------------------------

The microarchitecture-specific APIs have the following organization:

- **Similarity to the main C API**: Each microarchitecure-specific function has
  the same name as a corresponding main C API but with an added suffix, the
  same parameter signature, and the same semantics.
- **Name-suffix depending on microarchitecture feature**: There are 3 sets of
  microarchitecture-specific functions corresponding to the 3 suffixes `_AVX`,
  `_AVX2`, and `_AVX512` that respectively provide implementations optimized
  for a microarchitecture supporting AVX, AVX2, and AVX512F instruction-sets.

Code Conventions
----------------

Major code conventions used in LibSWIFFT are:

- SWIFFT-related symbols of the C API start with `SWIFFT\_`.
- Non-SWIFFT-related symbols of the C API start with `LIBSWIFFT\_`
- Symbols in the C++ API are in namespace `LibSwifft`.
- SWIFFT-related functions are thread-safe and constant-time.
- Arguments are assumed non-overlapping in memory.

Conditional Compilation
-----------------------

Major goals of LibSWIFFT code in using conditional compilation are:

- Prevent a header for being compiled multiple times.
- Compile microarchitecture-specific code only on a supporting platform.
- Compile C++ or OpenMP code only on a supporting compiler.
