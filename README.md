MURFI (multivariate and univariate real-time functional imaging) is a software package for real-time processing of functional brain images for neuroscience applications. At the moment murfi is fMRI-centric, but we plan to add support for other imaging modalities.

This is an open-source release of the MURFI platform (see LICENSE for details).

For using MURFI with Siemens scanners, a proprietary data transfer sequence can be obtained via a C2P agreement. Alternatively, for non-Siemens scanners or for your own Siemens functors, Murfi will accept data over TCPIP conformant with the following structured information:

https://github.com/gablab/murfi2/blob/master/src/io/RtExternalSenderImageInfo.h

If you use MURFI in your own research, please cite:

Oliver Hinds, Satrajit Ghosh, Todd W Thompson, Julie J Yoo, Susan Whitfield-Gabrieli, Christina Triantafyllou, John D E Gabrieli (2011)  Computing moment-to-moment BOLD activation for real-time neurofeedback.   Neuroimage 54: 1. 361-368 Jan

http://dx.doi.org/10.1016/j.neuroimage.2010.07.060

Installation
------------

#### Dependencies:

1. libace-dev
1. freeglut3-dev
1. libgsl0-dev
1. libxi-dev
1. libxmu-dev
1. vxl
1. qt5
1. libniftiio

On recent Ubuntu distros, the following command will install all dependencies:

    $ sudo apt-get install build-essential libace-dev freeglut3-dev libgsl0-dev cmake libxi-dev libxmu-dev libboost-filesystem-dev libnifti-dev nifti-bin libqt4-dev libqwt-dev libvtk5-qt4-dev libvxl1-dev qt5-default libqt5*-dev

#### Compilation:

    $ cd <path to toplevel murfi directory>
    $ make -j 8

#### Installation (optional):

    $ sudo ln -sf bin/murfi /usr/bin

Running the example
-------------------

    $ wget https://www.dropbox.com/s/1vvrz2g4tbzoh5c/murfi_example_data.tgz
    $ tar -xzvf murfi_example_data.tgz
    $ export MURFI_SUBJECTS_DIR=$PWD
    $ export MURFI_SUBJECT_NAME=murfi_example_data
    $ murfi -f murfi_example_data/scripts/neurofeedback.xml

In another terminal:

    $ cd murfi_example_data/scripts
    $ ./servedata.sh

Using Vagrant
-------------

Download and install [Vagrant](https://www.vagrantup.com/) and [VirtualBox](https://www.virtualbox.org/). Then download this [archive](https://github.com/gablab/murfi2/archive/master.zip). Requires an X-server installed locally.

Unzip the archive and using a terminal change to the unzipped directory.

    $ vagrant up
    $ vagrant -X ssh
    $ cd /vagrant
    $ source murfi_vagrant.sh

Then follow the [Running the example] section above.

On another terminal from the same unzipped directory do:

    $ cd murfi_example_data/scripts
    $ ./servedata.sh

Funding
-------

This project was partially funded by several sponsors:

- DARPA
- NIH
- McGovern Institute Neurotechnology Development Grant
