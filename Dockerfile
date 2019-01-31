ARG DEBIAN_FRONTEND="noninteractive"

FROM debian:jessie-slim as builder
RUN apt-get update -qq \
    && apt-get install --yes --quiet \
        build-essential \
        libace-dev \
        libboost-filesystem-dev \
        libgsl0-dev \
        libnifti-dev \
        libvxl1-dev \
        qt5-default \
        zlib1g-dev \
    && rm -rf /var/apt/lib/lists/* \
    # Murfi expects qt5 header files here.
    && ln -s /usr/include/x86_64-linux-gnu/qt5 /usr/include/qt5
WORKDIR /opt/murfi
COPY [".", "."]
RUN make clean \
    && make

FROM debian:jessie-slim
COPY --from=builder ["/opt/murfi/bin", "/opt/murfi/bin"]
RUN apt-get update -qq \
    && apt-get install --yes --quiet --no-install-recommends \
        libace-6.2.8 \
        libboost-filesystem1.55.0 \
        libgsl0ldbl \
        libnifti2 \
        libqt5core5a \
        libqt5gui5 \
        libqt5opengl5 \
        libqt5printsupport5 \
        libqt5widgets5 \
        libvxl1.17 \
    && rm -rf /var/apt/lib/lists/* \
    && ln -sv /opt/murfi/bin/murfi /usr/bin/ \
    && useradd --no-user-group --create-home --shell /bin/bash murfi
USER murfi
WORKDIR /work
ENTRYPOINT ["/usr/bin/murfi"]
