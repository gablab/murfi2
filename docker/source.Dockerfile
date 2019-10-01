# Dockerfile for murfi2. Most dependencies are compiled from source, except Qt5
# and libace.
#
# Please note that because murfi has a GUI, the version of OpenGL has to be the
# same -- similar is probably fine -- on the host and within the container.
# The root operating system of this container is ubuntu:18.04, which was
# released in April, 2018. If you get output like
#
#     X Error of failed request:  GLXBadContext
#       Major opcode of failed request:  151 (GLX)
#       Minor opcode of failed request:  6 (X_GLXIsDirect)
#       Serial number of failed request:  45
#
# you should choose a different base image that better suits your host.

FROM buildpack-deps:18.04 as builder

ARG DEBIAN_FRONTEND="noninteractive"
ARG NPROC="8"

# Build and install CMake.
WORKDIR /src/cmake
RUN curl -fsSL https://github.com/Kitware/CMake/releases/download/v3.13.3/cmake-3.13.3.tar.gz \
    | tar xz --strip-components 1 \
    && ./bootstrap --parallel=$NPROC \
    && make -j$NPROC \
    && make install

# Build and install vxl.
WORKDIR /src/vxl
RUN apt-get update -qq \
    && apt-get install --yes --quiet --no-install-recommends \
          libgeotiff-dev \
    && rm -r /var/lib/apt/lists/* \
    && curl -fsSL https://github.com/vxl/vxl/archive/v2.0.2.tar.gz \
    | tar xz --strip-components 1 \
    && mkdir build \
    && cd build \
    && cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr .. \
    && make -j$NPROC \
    && make install

# Build and install Boost (filesystem and system).
WORKDIR /src/boost
RUN curl -fsSL https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz \
    | tar xz --strip-components 1 \
    && ./bootstrap.sh \
    && ./b2 -j$NPROC --stagedir=/usr --with-filesystem --with-system \
    && ln -s /src/boost/boost /usr/include/boost

# Build and install nifticlib.
WORKDIR /src/nifti
RUN curl -fsSL https://versaweb.dl.sourceforge.net/project/niftilib/nifticlib/nifticlib_2_0_0/nifticlib-2.0.0.tar.gz \
    | tar xz --strip-components 1 \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr \
             -DBUILD_SHARED_LIBS=ON \
             -DBUILD_TESTING=OFF \
             .. \
    && make -j$NPROC install

# Build and install gsl.
WORKDIR /src/gsl
RUN curl -fsSL  https://ftpmirror.gnu.org/gsl/gsl-2.5.tar.gz \
    | tar xz --strip-components 1 \
    && ./configure --prefix=/usr \
    && make -j$NPROC \
    && make install

# Install the more difficult things...
RUN apt-get update -qq \
    && apt-get install --yes --quiet --no-install-recommends \
        libace-dev \
        libqt5opengl5-dev \
        qt5-default \
    && rm -rf /var/lib/apt/lists/* \
    && ln -sv /usr/include/x86_64-linux-gnu/qt5 /usr/include/qt5

# Build murfi.
WORKDIR /src/murfi
COPY [".", "."]
RUN make clean \
    && make -j8


FROM ubuntu:18.04
COPY --from=builder [ \
    "/usr/lib/libACE-6.4.5.so", \
    "/usr/lib/libniftiio.so.2", \
    "/usr/lib/libboost_filesystem.so.1.69.0", \
    "/usr/lib/libznz.so.2", \
    "/usr/lib/"]

COPY --from=builder ["/src/murfi", "/src/murfi"]
RUN ln -sv /src/murfi/bin/murfi \
           /src/murfi/util/scanner_sim/servenii \
           /src/murfi/util/scanner_sim/servenii4d \
           /usr/bin/ \
    && apt-get update -qq \
    && apt-get install --yes --no-install-recommends --quiet \
        iputils-ping \
        libace-6.4.5 \
        libqt5core5a \
        libqt5gui5 \
        libqt5opengl5 \
        libqt5printsupport5 \
        libqt5widgets5 \
        net-tools \
    && rm -rf /var/lib/apt/lists/*

CMD ["/src/murfi/bin/murfi"]
