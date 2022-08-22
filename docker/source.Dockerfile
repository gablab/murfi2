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
RUN curl -fsSL https://boostorg.jfrog.io/artifactory/main/release/1.69.0/source/boost_1_69_0.tar.gz \
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

RUN apt-get -y update \
   && DEBIAN_FRONTEND="noninteractive" apt-get install -y dbus-x11 \
       firefox \
       xfce4 \
       xfce4-panel \
       xfce4-session \
       xfce4-terminal \
       xfce4-settings \
       xorg \
       xubuntu-icon-theme \
       wget \
       curl \
       python3-distutils \
       gcc \
       make \
       locales \
    && rm -rf /tmp/* \
    && rm -rf /var/lib/apt/lists/* \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG en_US.utf8

ARG WEBSOCKIFY_VERSION=0.10.0
ARG NOVNC_VERSION=1.2.0

RUN curl -fsSL https://github.com/novnc/noVNC/archive/v${NOVNC_VERSION}.tar.gz | tar -xzf - -C /opt && \
    curl -fsSL https://github.com/novnc/websockify/archive/v${WEBSOCKIFY_VERSION}.tar.gz | tar -xzf - -C /opt && \
    mv /opt/noVNC-${NOVNC_VERSION} /opt/noVNC && \
    mv /opt/websockify-${WEBSOCKIFY_VERSION} /opt/websockify && \
    ln -s /opt/noVNC/vnc_lite.html /opt/noVNC/index.html && \
    cd /opt && curl -sSL https://bootstrap.pypa.io/pip/3.6/get-pip.py -o get-pip.py && \
    python3 get-pip.py && \
    cd /opt/websockify && pip install . && make

# Remove light-locker to prevent screen lock
ARG TURBOVNC_VERSION=2.2.6
RUN wget -q "https://sourceforge.net/projects/turbovnc/files/${TURBOVNC_VERSION}/turbovnc_${TURBOVNC_VERSION}_amd64.deb/download" -O turbovnc_${TURBOVNC_VERSION}_amd64.deb && \
    apt-get install -y -q ./turbovnc_${TURBOVNC_VERSION}_amd64.deb && \
    apt-get remove -y -q light-locker && \
    rm ./turbovnc_${TURBOVNC_VERSION}_amd64.deb && \
    ln -s /opt/TurboVNC/bin/* /usr/local/bin/ && \
    mkdir -p ~/.vnc && \
    bash -c "echo 'murfi123' | vncpasswd -f > ~/.vnc/passwd" && \
    chmod 0600 ~/.vnc/passwd \
    && rm -rf /tmp/*

RUN wget -q https://raw.githubusercontent.com/jupyterhub/jupyter-remote-desktop-proxy/main/jupyter_desktop/share/xstartup -O /opt/xstartup \
    && chmod +x /opt/xstartup

RUN useradd -m -u 1000 -s /bin/bash murfi && \
    usermod -aG sudo murfi

USER 1000

RUN mkdir -p ~/.vnc && \
    bash -c "echo 'murfi123' | vncpasswd -f > ~/.vnc/passwd" && \
    chmod 0600 ~/.vnc/passwd

WORKDIR /home/murfi

# CMD ["/src/murfi/bin/murfi"]

# vncserver -SecurityTypes None -xstartup /opt/xstartup :1
# /opt/websockify/websockify.py -v --web /opt/noVNC/ --heartbeat 30 8080 localhost:5901
