# Dockerfile for murfi2. Dependencies are all installed as pre-compiled pacakges.
#
# Please note that because murfi has a GUI, the version of OpenGL has to be the
# same -- similar is probably fine -- on the host and within the container.
# The root operating system of this container is debian:jessie, which was
# initially released in April, 2015. If you get output like
#
#     X Error of failed request:  GLXBadContext
#       Major opcode of failed request:  151 (GLX)
#       Minor opcode of failed request:  6 (X_GLXIsDirect)
#       Serial number of failed request:  45
#
# then please try using the Dockerfile that builds things from source. You can
# choose a base image that suits your host.

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
