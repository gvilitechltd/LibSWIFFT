#
# Copyright (C) 2021 Yaron Gvili and Gvili Tech Ltd.
#
# See the accompanying LICENSE.txt file for licensing information.
#
FROM ubuntu:latest

# set up tool chain
RUN apt-get update && apt-get install -yq cmake gcc g++
RUN cmake --version && gcc --version && g++ --version
# set up tools for getting source code
RUN apt-get update && apt-get install -yq wget unzip

# get and build Catch2 source code
ARG CATCH2_VERSION=v2.13.2
RUN wget https://github.com/catchorg/Catch2/archive/${CATCH2_VERSION}.zip -O Catch2.zip && unzip Catch2.zip -d .
RUN (CATCH2_VER=$(echo ${CATCH2_VERSION} | sed 's~^v~~'); ln -s Catch2-${CATCH2_VER} Catch2 && cd Catch2 && cmake -Bbuild -H. -DBUILD_TESTING=OFF && MAKEFLAGS=$(( $(nproc) + 1 )) cmake --build build/ --target install)

# get and build LibSWIFFT source code
ARG LIBSWIFFT_VERSION=v1.2.0
ARG LIBSWIFFT_MCFLAGS=-march=native
RUN wget https://github.com/gvilitechltd/LibSWIFFT/archive/${LIBSWIFFT_VERSION}.zip -O LibSWIFFT.zip && unzip LibSWIFFT.zip -d .
RUN (LIBSWIFFT_VER=$(echo ${LIBSWIFFT_VERSION} | sed 's~^v~~'); ln -s LibSWIFFT-${LIBSWIFFT_VER} LibSWIFFT)
RUN (cd LibSWIFFT && cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release -DSWIFFT_MACHINE_COMPILE_FLAGS=${LIBSWIFFT_MCFLAGS} && MAKEFLAGS=-j$(( $(nproc) - 1 )) cmake --build build/ --target all)
RUN (cd LibSWIFFT && cmake -Bbuild-omp -H. -DCMAKE_BUILD_TYPE=Release -DSWIFFT_MACHINE_COMPILE_FLAGS=${LIBSWIFFT_MCFLAGS} -DSWIFFT_ENABLE_OPENMP=On && MAKEFLAGS=-j$(( $(nproc) - 1 )) cmake --build build-omp/ --target all)

# provide tester as entry point
ENTRYPOINT ["LibSWIFFT/build/test/swifft_catch"]
