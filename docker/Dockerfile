# Dockerfile for murfi2. Dependencies are mostly installed as pre-compiled pacakges.
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
ARG MAKE_TARGET=

FROM ubuntu:22.04
RUN apt-get update -qq && \
    apt-get install -y -q \
      curl \
      build-essential \
      cmake \
      python3


RUN curl -fsSL https://github.com/vxl/vxl/archive/v3.5.0.tar.gz | tar xz --strip-components 1 \
    && mkdir build \
    && cd build \
    && cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr .. \
    && make -j$(nproc) \
    && make install \
    && rm -rf build

RUN ln -s /usr/include/vxlinclude/vxlcoreinclude/vxlvnl/ /usr/include/vxlinclude/vxlcoreinclude/vnl


WORKDIR /opt/murfi
COPY ["docker/fsl_clean.sh", "."]

RUN curl https://fsl.fmrib.ox.ac.uk/fsldownloads/fslconda/releases/fslinstaller.py > fslinstaller.py && \
    python3 fslinstaller.py -d /opt/fsl && \
    sh fsl_clean.sh

RUN apt install -y -q \
    libace-dev \
    libgsl-dev \
    libnifti-dev \
    nifti-bin \
    libboost-filesystem-dev \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5opengl5-dev \
    libqt5gui5 \
    language-pack-en-base \
    yaru-theme-icon \
    shared-mime-info

RUN update-locale "LANG=en_US.UTF-8" && \
    locale-gen en_US.UTF-8 && \
    dpkg-reconfigure --frontend noninteractive locales

RUN update-mime-database /usr/share/mime

RUN rm -rf /var/apt/lib/lists/* /var/tmp/* /tmp/* \
    && apt-get clean


WORKDIR /opt/murfi
COPY [".", "."]


RUN make clean && make -j$(nproc) $MAKE_TARGET


ENV XDG_RUNTIME_DIR="/tmp"
ENV PATH=${PATH}:/opt/murfi/bin:/opt/murfi/util/scanner_sim
ENTRYPOINT ["/opt/murfi/bin/murfi"]
