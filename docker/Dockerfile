# Dockerfile for murfi2.
FROM ghcr.io/gablab/murfi2-base:latest

ARG DEBIAN_FRONTEND="noninteractive"
ARG MAKE_TARGET

WORKDIR /opt/murfi

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
    shared-mime-info \
    python3-pip \
    python3-setuptools \
    dcm2niix

RUN update-locale "LANG=en_US.UTF-8" && \
    locale-gen en_US.UTF-8 && \
    dpkg-reconfigure --frontend noninteractive locales

RUN update-mime-database /usr/share/mime

RUN rm -rf /var/apt/lib/lists/* /var/tmp/* /tmp/* \
    && apt-get clean

RUN pip install click

WORKDIR /opt/murfi
COPY [".", "."]

RUN echo ${MAKE_TARGET}
RUN make clean && make -j$(nproc) ${MAKE_TARGET}


ENV XDG_RUNTIME_DIR="/tmp"
ENV PATH=${PATH}:/opt/murfi/bin:/opt/murfi/util/scanner_sim
ENTRYPOINT ["/opt/murfi/bin/murfi"]
