#!/bin/bash
## run a single fun of fsl analysis
##
## $Id: fsl_contrast.sh,v 1.3 2008-07-22 21:13:49 ohinds Exp $

usage() {
    echo "usage: $0 [options] subj run model"
    echo " options:"
    echo "  -h hpf:          high pass filter cutoff (default 32 seconds)"
    echo "  -d drop:         drop scans from the begining"
    echo "  -t target:       target nii image for motion correction. if "
    echo "                   left blank this will search for xfm/study_ref.nii" 
    echo "                   as the target"
    echo "  -a analysis_dir: analysis directory (default ../../analysis)"
    echo "  -o :             run in online mode (use 3d files)"
}

# parse command line arguments
#
# in: $@
parse_args() {
   # get the options
    while getopts ":oh:d:t:a:?" Option
    do
	case $Option in
	    h ) hpf="$OPTARG"
		;;
	    d ) drop="$OPTARG"
		;;
	    t ) mc_target="$OPTARG"
		;;
	    a ) adir="$OPTARG"
		;;
	    o ) online=1
		;;
	    * ) echo unknown option $Option
		usage;
		exit;;
	esac
    done
    shift $(($OPTIND - 1))

    if [ ! "$3" ]; then
	usage
	exit
    fi

    export subj=$1
    export run=$2
    export model=$3
}

export FSLOUTPUTTYPE=NIFTI

# defaults
sdir=../
adir=../../analysis/

hpf=32
drop=0
online=0

parse_args "$@"

aname=$model

odir=$sdir/$subj/"$aname"/
echo mkdir -p "$odir"
mkdir -p "$odir"

# make the design if you need to
if [ ! -f "$adir/"$aname"_design.mat" ]; then
    echo "save a design in the file: $adir/"$aname"_design.mat"
    Glm
fi

# copy the images for analysis
input_file="$odir/"input.nii
if [ "$online" == 1 ]; then
    vol_str=`printf $sdir/$subj/img/img-%05d-*.nii $run`
    echo fslmerge -t "$odir/"input.nii "$vol_str"
    fslmerge -t "$input_file" $vol_str
else
    cp "$sdir/$subj"/nii/[0-9]*-$run.nii "$input_file"
fi

# motion correct
if [ ! "$mc_target" ]; then
    mc_target="$sdir/$subj/xfm/study_ref.nii"
fi

if [ ! -f "$mc_target" ]; then
    echo "error: motion correction target $mc_target doesn't exist"
    exit
fi

echo mcflirt -in "$input_file" -out "$odir/"mc_input.nii -r "$mc_target" -plots
mcflirt -in "$input_file" -out "$odir/"mc_input.nii -r "$mc_target" -plots
input_file="$odir/"mc_input.nii


# run model fit
echo fsl_model_fit.sh -S $input_file -M $adir/"$aname"_design.mat -s $drop -o $odir -O
fsl_model_fit.sh -S $input_file -M $adir/"$aname"_design.mat -s $drop -o $odir -O

# make the contrast vector if you need to
if [ ! -f "$adir/"$aname"_contrast_vector.txt" ]; then
    echo "enter the contrast vector: "
    read con_vec
    echo $con_vec > "$adir/"$aname"_contrast_vector.txt"
fi

# initial contrast for zstat
echo fsl_contrast.sh -n "$aname" -c `cat $adir/"$aname"_contrast_vector.txt` -i $odir
fsl_contrast.sh -n "$aname" -c "`cat $adir/"$aname"_contrast_vector.txt`" -i $odir

# visualize
fslview $odir/example_func.nii $odir/stats/zstat_"$aname"1.nii -b 3,8

# get the threshold and rerun
happy=0
while [ "$happy" -ne 1 ]; do
    echo "enter the z threshold for clustering: "
    read thresh
    echo "enter the P threshold for clusters: "
    read pthresh
    echo fsl_contrast.sh -n "$aname" -c `cat $adir/"$aname"_contrast_vector.txt`  -i $odir -z $thresh -P $pthresh
    fsl_contrast.sh -n "$aname" -c "`cat $adir/"$aname"_contrast_vector.txt`"  -i $odir -z $thresh -P $pthresh

    # visualize and get cluster numbers
    num_clust=`ls $odir/roi/"$aname"_cluster[0-9]*_mask.nii | wc -l`
    fslview $odir/example_func.nii $odir/cluster_mask_zstat_"$aname"1.nii -b 0.99,$num_clust
    echo "enter the cluster numbers to make the "$aname" mask or r to rerun: "
    read clust

    if [ "$clust" != "r" ]; then
	happy=1
    fi
done

maskdir=$sdir/$subj/mask
mkdir -p $maskdir

names=`echo $clust | sed "s/\([0-9]*\)/"$aname"_cluster\1_mask.nii/g"`
first_name=
other_names=
for n in $names; do
    cp $odir/roi/$n $maskdir

    if [ ! "$first_name" ]; then
	first_name=$maskdir/$n
    else
	other_names="$other_names -add $maskdir/$n"
    fi
done
echo fslmaths $first_name $other_names $maskdir/"$aname"_mask.nii -odt short
fslmaths $first_name $other_names $maskdir/"$aname"_mask.nii -odt short

fslview $odir/example_func.nii $maskdir/"$aname"_mask.nii
