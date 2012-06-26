#!/bin/bash
#
# Oliver Hinds <ohinds@mit.edu> 2008-09-23

usage() {
    echo $0
    echo ""
    echo " register same subject epis "
    echo "  -t target_image   target epi for registration (required)"
    echo "  -m movable_image  movable epi for registration (required)"
    echo "  -i images         quoted list of images to transform"
    echo "  -o images         quoted list of output names (same length as -i)"
    echo "  -s xfmfile        save the transformation to this file"
    echo "  -r xfmfile        use this transformation file"
    echo "  -I interp         interpolation method (nearestneighbour*, trilinear, sinc)"
    echo "  -F fsl_path       path to fsl setup script (fsl.sh)"
}


# parse command line arguments
#
# in: $@
parse_args() {
   # get the options
    while getopts ":t:m:i:o:s:r:h?" Option
    do
    case $Option in
      t ) targ="$OPTARG"
        ;;
      m ) move="$OPTARG"
        ;;
      i ) input="$OPTARG"
        ;;
      o ) output="$OPTARG"
        ;;
      s ) xfmfile="$OPTARG"
        ;;
      r ) xfmfile="$OPTARG"
        ;;
      F ) fslpath="$OPTARG"
        ;;
      * ) usage;
        exit;;
    esac
    done
    shift $(($OPTIND - 1))
}

# $1: transformation file
# $2: volume to be transformed
# $3: target volume for registration
# $4: output volume
apply_reg() {
    parms="-interp $interp"

    init=$1
    in=$2
    ref=$3
    out=$4

    # set the desired datatype (short or double or float)
    dtype=`nifti_tool -quiet -disp_hdr -fields datatype -infiles "$in" | sed "s/.* //"`;
    if [ "$dtype" == 16 ]; then
    otype=float
    elif [ "$dtype" == 64 ]; then
    otype=double
    else
    otype=short
    fi

  echo flirt $parms -applyxfm -init "$1" -in "$2" -ref "$3" -out "$4" -datatype $otype
    flirt $parms -applyxfm -init "$1" -in "$2" -ref "$3" -out "$4" -datatype $otype
}


if [ ! "$1" ]; then
    usage
    exit
fi

## defaults
export FSLOUTPUTTYPE=NIFTI
interp=nearestneighbour

parse_args "$@"

## check required args
if [ ! "$targ" ]; then
    echo targ image required
    usage
    exit
fi

if [ ! "$move" ]; then
    echo move image required
    usage
    exit
fi

if [ "$fslpath" ]; then
    source "$fslpath"/fsl.sh
fi

## build vars
movepath=`echo $move | sed "s|/[.A-Za-z0-9]*$|/|"`
targpath=`echo $move | sed "s|/[.A-Za-z0-9]*$|/|"`

if [ ! "$savexfmfile" ]; then
    savexfmfile=`tempfile`.xfm
fi

# find a good name for the transformed files
movedir=`echo $move | sed -n "/\//p" | sed "s|\(.*/\).*|\1|"`
movestem=`echo $move | sed "s|.*/||; s|\(.*.*\)\..*$|\1|"`
targstem=`echo $targ | sed "s|.*/||; s|\(.*.*\)\..*$|\1|"`
newstem="$movestem"2"$targstem"

default_options=""

## run mcflirt to get the transformation
if [ ! -f "$xfmfile" ]; then
    of="$movedir""$newstem"

    echo mcflirt $default_options -in "$move" -out "$of" -reffile "$targ" -mats
    mcflirt $default_options -in "$move" -out "$of" -reffile "$targ" -mats

    echo mv "$of".mat/MAT_0000 "$xfmfile"
    mv "$of".mat/MAT_0000 "$xfmfile"

    echo rmdir "$of".mat
    rmdir "$of".mat
fi
savexfmfile=$xfmfile

## apply the transform to the required files
for file in $input; do
    transfiles="$transfiles $file"

    if [ "$output" ]; then
    of=`echo $output | sed "s/\([^*]\) .*/\1/"`
    output=`echo $output | sed "s/[^*] \(.*\)/\1/"`
    else
    ostem=`echo $file | sed "s|.*/||; s|\(.*.*\)\..*$|\1|"`
    of="$movedir""$ostem"2"$targstem"
    fi

    apply_reg "$savexfmfile" "$file" "$targ" "$of"
done

