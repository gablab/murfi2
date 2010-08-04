#!/bin/bash
# script to facilitate making ROIs from a functional localizer task
# run from the subject directory with the name of the localizer (e.g., mid)
# Oliver Hinds <ohinds@mit.edu> 2010-07-01

function usage() {
    echo "$0 <funcloc_name> [options]"
    echo "  the feat setup file file scripts/<funcloc_name>.fsf must exist"
    echo " options:"
    echo "  -m:            don't make the 4d fmri volume from the 3d murfi images"
    echo "  -f:            don't run fsl analysis"
    echo "  -r:            don't make roi masks"
    echo "  -c <int>:      use contrast with this index [default is 1]"
    echo "  -n <roi_name>: name of the roi to save [default is <funcloc_name>]"
    echo "  -b:            don't make a background roi"
    echo "  -N:            background roi name [default is <funcloc_name>_background]"
    echo "  -h:            print help"
    echo "  -?:            print help"
    exit
}

# validate
if [ "x$1" == "x" ]; then
    usage
fi
anal=$1
shift

if [ ! -f "scripts/$anal.fsf" ]; then
    echo "fsf not found"
    usage
fi

# config vars
do_make_vol=1
do_run_fsl=1
do_choose_mask=1
do_make_background=1
con_num=1
mask_name=$anal
background_mask_name="$anal"_background

# parse options
while getopts  "mfrc:n:bN:h?" flag
do
    case $flag in
	m) do_make_vol=0;;
	f) do_run_fsl=0;;
	r) do_choose_mask=0;;
	c) con_num=$OPTARG;
	   if [ `echo $con_num | sed s/[0-9]//` != "" ]; then
	       echo "ERROR: non-integer contrast number specified"
	       usage;
	   fi;;
	n) mask_name=$OPTARG;;
	b) do_make_background=0;;
	N) background_mask_name=$OPTARG;;
    [h,?]) usage;;
	*) echo "unknown option $flag, ignored";;
    esac
done

subj=`pwd | sed "s|.*/||"`


function make_vol {
    # find the last run of right length, then create a 4d vol from it
    nvols=`cat scripts/$anal.fsf | sed -n "/fmri(npts)/p" | sed "s/.*npts) //"`
    let nvolsplusone=nvols+1
    nruns=`ls img/img-*.nii | tail -1 | sed "s/.*img-0*\([0-9]*\)-[0-9]*.nii/\1/"`
    run=$nruns
    while [ $run -gt 0 ]; do
	lastvol=`printf "img/img-%05d-%05d.nii" $run $nvols`
	afterlastvol=`printf "img/img-%05d-%05d.nii" $run $nvolsplusone`

        if [ -f  $lastvol -a ! -f $afterlastvol ]; then	
    	    break
        fi
        let run=run-1
    done
    
    if [ $run -lt 1 ]; then
        echo "ERROR: didn't find a run with $nvols volumes, which is a requirement to run the analysis. "
        echo "if your scan is valid but has the wrong number of volumes, please add/remove images to correct this."
        exit
    fi
    
    echo "found run $run"
    
    echo -n "merging into a 4d file..."
    fslmerge -t "img/$anal.nii" `printf "img/img-%05d-*.nii" $run`
    echo "done"
}

function run_fsl {
    # check for existing analysis
    if [ -e "funcloc/$anal.feat" ]; then
        echo "it seems that an $anal analysis has already been run for this subject."
        echo -n "proceed, skip, or exit [p,s,e]: "
        read action
    
        case $action in 
    	    p*) rm -rf "funcloc/$anal.feat";;
    	    s*) do_run_fsl=0;;
    	    *)  echo exit; exit;;
        esac
    fi

    # run
    if [ $do_run_fsl ]; then
	echo -n "running feat..."
	feat "scripts/$anal.feat"
	echo "done"
    fi
}

function choose_mask() {
    echo "creating masks from $anal analysis"
    
    # allow creation of masks using simple math. 
    # after a mask has been created the process can be repeated and
    # the result combined with the existing mask. 
    want_done=0
    while [ "$want_done" == "0" ]; do

	# allow threshold picking
	echo "starting fslview with contrast $con_num from the $anal analysis"
	fslview funcloc/$anal.feat/example_func funcloc/$anal.feat/stats/tstat$con_num &

	echo "enter the desired threshold for creating a mask region: "
	read thresh
	while [ ! `echo $thresh | sed "s/[0-9.]//g"` == "" ]; do
	    echo -n "entered threshold is non-numeric, reenter: "
	    read thresh
	done
	
	echo -n "thresholding..."
	fn=`tempfile`.nii
	cluster -i funcloc/$anal.feat/stats/tstat$con_num -t $thresh -o "$fn"
	echo "done"

	# allow cluster picking
	echo "starting fslview with cluster vol"
	fslview funcloc/$anal.feat/example_func $fn &

	echo -n "enter the index of the cluster of interest: "
	read clust
	while [ ! `echo $clust | sed "s/[0-9]*//"` == "" ]; do
	    echo -n "entered index is non-integer, reenter:"
	    read clust
	done

	fslmaths "$fn" -thr $clust -uthr $clust -bin "$fn" 
	
	# save/combine
	save_response=""
	while [ \
	       "$save_response" != "r" \
	    -a "$save_response" != "a" \
	    -a "$save_response" != "s" \
	    -a "$save_response" != "d" \
	    -a "$save_response" != "m" \
	    ]; do
	    echo -n "replace, add, subtract, discard, manual [r,a,s,d,m]: "
	    read save_response
	done

	case $save_response in
	    r)  mv "$fn" "mask/$mask_name"
		;;
	    a)  fslmaths "mask/$mask_name" -max "$fn" "mask/$mask_name"
	        rm "$fn"
		;;
	    s)  fslmaths "mask/$mask_name" -sub "$fn" "mask/$mask_name"
		fslmaths "mask/$mask_name" -thr 0 "mask/$mask_name"
		rm "$fn"
		;;
	    d) 	rm "$fn"
		;;
	    m) 	rm "$fn"
		;;
	    
	esac

	fslmaths "mask/$mask_name" -odt short 

	# query for finished
	done_response=""
	while [ "$done_response" != "y" -a "$done_response" != "n" ]; do
	    echo -n "done? [y/n]: "
	    read done_response
	done
	if [ "$done_response" == "y" ]; then
	    want_done=1
	fi
    done

}

function make_background() {
    echo "creating background mask from analysis $anal contrast $con_num"

    # 
    want_done=0
    first_time=1
    while [ "$want_done" == "0" ]; do

	# allow threshold picking after first time
	if [ ! $first_time == 1 ]; then
	    echo "enter the desired upper absolute threshold for creating a background mask region: "
	    read thresh
	    while [ ! `echo $thresh | sed "s/[0-9.]//g"` == "" ]; do
		echo -n "entered threshold is non-numeric, reenter: "
		read thresh
	    done
	else
	    first_time=0
	    thresh=0.1;
	fi
	
	# build background mask as the tstats between $thresh and
	# -$thresh, binarized and brain masked.
	echo "fslmaths 
            funcloc/$anal.feat/stats/tstat$con_num 
	    -uthr $thresh 
	    -thr -$thresh 
	    -bin 
	    -mas funcloc/$anal.feat/mask 
	    $background_mask_name
	    -odt float"
	fslmaths \
	    funcloc/$anal.feat/stats/tstat$con_num \
	    -uthr $thresh \
	    -thr -$thresh \
	    -bin \
	    -mas funcloc/$anal.feat/mask \
	    mask/$background_mask_name \
	    -odt float

	# display
	echo "starting fslview with background mask from contrast $con_num from the $anal analysis"
	fslview funcloc/$anal.feat/example_func mask/$background_mask_name &

	# query for finished
	done_response=""
	while [ "$done_response" != "y" -a "$done_response" != "n" ]; do
	    echo -n "done? [y/n]: "
	    read done_response
	done
	if [ "$done_response" == "y" ]; then
	    want_done=1
	fi
    done

}

## main

if [ $do_make_vol == "1" ]; then
    make_vol
fi

if [ $do_run_fsl == "1" ]; then
    run_fsl
fi

if [ $do_choose_mask == "1" ]; then
    choose_mask
fi

if [ $do_make_background == "1" ]; then
    make_background
fi