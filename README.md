MURFI (multivariate and univariate real-time functional imaging) is a software package for real-time processing of functional brain images for neuroscience applications. At the moment murfi is fMRI-centric, but we plan to add support for other imaging modalities.

This is an open-source release of the MURFI platform (see LICENSE for details).

For using MURFI with Siemens scanners, a proprietary data transfer sequence can be obtained via a C2P agreement. Alternatively, for non-Siemens scanners or for your own Siemens functors, Murfi will accept data over TCPIP conformant with the following structured information:

https://github.com/gablab/murfi2/blob/master/src/io/RtExternalSenderImageInfo.h

If you use MURFI in your own research, please cite:

Oliver Hinds, Satrajit Ghosh, Todd W Thompson, Julie J Yoo, Susan Whitfield-Gabrieli, Christina Triantafyllou, John D E Gabrieli (2011)  Computing moment-to-moment BOLD activation for real-time neurofeedback.   Neuroimage 54: 1. 361-368 Jan

http://dx.doi.org/10.1016/j.neuroimage.2010.07.060

Quickstart
----------

1. Install apptainer >= 1.3.0 or SingularityCE >= 4.0.0

1. Download murfi as a pre-built singularity image:

        singularity pull oras://ghcr.io/gablab/murfi2-sif:latest
    
Running the example
-------------------

    mkdir murfi_example_data && \
        cd murfi_example_data && \
        curl -L -o murfi_example_data.tgz 'https://www.dropbox.com/scl/fi/61krbyoj293qpeaxcs9zl/murfi_example_data.tgz?rlkey=paljzpw3f9pbpirbdu8fcrhdm&st=jm686tif&dl=1' && \
        tar xzvf murfi_example_data.tgz
    MURFI_SUBJECTS_DIR=.. MURFI_SUBJECT_NAME=murfi_example_data singularity run ../murfi2-sif_latest.sif murfi -f scripts/neurofeedback.xml

In another terminal:

    cd murfi_example_data/scripts
    singularity exec ../../murfi2-sif_latest.sif bash servedata.sh 3000 15000 $(hostname)

Development
-----------

#### Dependencies:

1. libace
1. freeglut
1. libgsl
1. vxl
1. qt5
1. libniftiio

On recent Ubuntu distros, the following command will install all dependencies except vxl:

    sudo apt-get install \
        build-essential \
        curl \
        cmake \
        libace-dev \
        libgsl-dev \
        libnifti-dev \
        libboost-filesystem-dev \
        qtbase5-dev \
        qtchooser \
        qt5-qmake \
        qtbase5-dev-tools \
        libqt5opengl5-dev

To install vxl, use the following commands:

    curl -fsSL https://github.com/vxl/vxl/archive/v3.5.0.tar.gz | tar xz --strip-components 1 \
        && mkdir build \
        && cd build \
        && cmake -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr .. \
        && make -j $(nproc) \
        && sudo make install \
        && rm -rf build \
        && ln -s /usr/include/vxlinclude/vxlcoreinclude/vxlvnl/ /usr/include/vxlinclude/vxlcoreinclude/vnl


#### Compilation:

    cd <path to toplevel murfi directory>
    make -j$(nproc)

#### Installation (optional):

    sudo ln -sf bin/murfi /usr/bin

Funding
-------

This project was partially funded by several sponsors:

- DARPA
- NIH
- McGovern Institute Neurotechnology Development Grant
