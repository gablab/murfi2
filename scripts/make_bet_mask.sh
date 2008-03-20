#!/bin/bash
# make a brain mask using bet
# Oliver Hinds <ohinds@mit.edu> 2008-03-20

usage() {
    echo "$0 <nifti_file> <mask_file> [bet_flags]"
}

# validate args
if [ ! "$2" ]; then
    usage
    exit 0;
fi

infile="$1"
shift;

maskfile="$1"
shift;

if [ ! "$1" ]; then
    betflags="-f 0.5 -g 0 -n -m";
else
    betflags="$@";
fi

# setup the environment
if [ -z $FSLDIR ]; then
    export FSLDIR=/usr/share/fsl;
fi
source $FSLDIR/etc/fslconf/fsl.sh

# run bet
bet $infile $maskfile $betflags &

