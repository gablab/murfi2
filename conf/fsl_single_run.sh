#!/bin/bash
# script to perform analysis on a single fmri run using fsl and make masks 5D 
#
# Oliver Hinds <ohinds@mit.edu> 2009-05-07

export FSLOUTPUTTYPE=NIFTI

sdir=../
adir=../../analysis/

if [ "$3" ]; then
    subj=$1
    run=$2
    model=$3
else
    echo usage: $0 subject run model
    exit
fi

aname=$3

odir=$sdir/$subj/"$aname"/

#run model fit
echo fsl_model_fit.sh -S $sdir/$subj/nii/[0-9]*-$run.nii -M $adir/"$aname"_design.mat -o $odir -O
fsl_model_fit.sh -S $sdir/$subj/nii/[0-9]*-$run.nii -M $adir/"$aname"_design.mat -o $odir -O

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
