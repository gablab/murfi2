# preprocess single run fmri data using fsl
#
# Oliver Hinds <ohinds@mit.edu> 2009-08-12

usage() {
    echo $0 [options] input_file output_file
    echo "  options:"
    echo "   -m [1|0]: whether to motion correct (default 1)"
    echo "   -M [1|0]: whether to regress out motion (default 1)"
    echo "   -f num  : smoothing fwhm in mm (default 6)"
    echo "   -t type : method for trend removal. one of: "
    echo "             none, linear (default), or filter (before model)"
    echo "   -h num  : high pass filter cutoff (default 32s)"
    echo "   -b num  : initial baseline length (default 8 TRs)"
    echo "   -o type : output units. one of: "
    echo "             z (default), psc, or raw"
}


# parse command line arguments
#
# in: $@
parse_args() {
   # get the options
    while getopts ":m:M:f:s:t:h:b:z:F?" Option
    do
	case $Option in
	    f ) fwhm="$OPTARG"
		;;
	    s ) num_skip="$OPTARG"
		;;
	    m ) motion_correct="$OPTARG"
		;;
	    M ) regress_motion="$OPTARG"
		;;
	    t ) trend_method="$OPTARG"
		;;
	    h ) hpf="$OPTARG"
		;;
	    b ) baseline="$OPTARG"
		;;
	    z ) out_units="$OPTARG"
		;;
	    F ) skip_feat=1
		;;
	    * ) usage;
		exit;;
	esac
    done
    shift $(($OPTIND - 1))

    if [ ! "$1" ]; then
	echo "input_file is a required argument"
	usage;
	exit 1;
    fi
    export input_file=$1;


    if [ ! "$2" ]; then
	echo "output_file is a required argument"
	usage;
	exit 1;
    fi
    export output_file=$2;
}

# set a variable value in an fsf
set_feat_var() {
    var=$1
    val=$2
    sed "s|set\s*$var\s*.*|set $var $val|" $feat > $scratch
    mv $scratch $feat
}

## set default parm values

num_skip=0
motion_correct=1
regress_motion=1
fwhm=6
trend_method=linear
hpf=32
baseline=8
out_units=z
skip_feat=0

# get any command line arguments
parse_args "$@"

to_delete=""

if [ "$skip_feat" == "0" ]; then
    # setup the feat file
    export feat=`tempfile`.fsf
    export scratch=`tempfile`.fsf
    cp $FESTR_HOME/util/svm/preproc/feat_template.fsf $feat
    
    to_delete="$to_delete $feat $scratch"
    
    # get tr from input file
    tr=`fslinfo $input_file | sed -n /pixdim4/p | sed "s/.*\s\(.*\)$/\1/"`
    set_feat_var "fmri(tr)" $tr
    
    num_trs=`fslinfo $input_file | sed -n /^dim4/p | sed "s/.*\s\(.*\)$/\1/"`
    set_feat_var "fmri(ntp)" $num_trs
    
    set_feat_var "fmri(ndelete)" $num_skip
    
    set_feat_var "fmri(mc)" $motion_correct
    
    set_feat_var "fmri(motionevs)" $regress_motion
    
    set_feat_var "fmri(smooth)" $fwhm
    
    if [ "$trend_method" == "filter" ]; then
	set_feat_var "fmri(temphp_yn)" 1
	set_feat_var "fmri(paradigm_hp)" $hpf
    else
	set_feat_var "fmri(temphp_yn)" 0
    fi
    
    # setup design
    if [ "$trend_method" == "linear" ]; then
	trend_ev=`tempfile`
	step=`echo "$num_trs $num_skip" | awk '{ x = 1/($1-$2) } END {print x}'`
	seq $step $step 1 > $trend_ev
	set_feat_var "fmri(confoundevs)" 1
	set_feat_var "confoundev_files(1)" "$trend_ev"
	to_delete="$todelete $trend_ev"
    else
	set_feat_var "fmri(confoundevs)" 0
    fi
    
    set_feat_var "feat_files(1)" "$PWD/$input_file"
    
    outdir=`echo "$PWD/$input_file" | sed "s/\..*/.preproc/"`
    set_feat_var "fmri(outputdir)"  "$outdir"
    outdir=$outdir.feat

    # remove old output dirs
    if [ -d "$outdir" ]; then
	rm -rf "$outdir"
    fi
    
    # run feat
    echo feat $feat
    feat $feat
else
    outdir=`echo "$PWD/$input_file" | sed "s/\..*/.preproc.feat/"`
fi

# convert to output units
if [ "$out_units" == "z" ]; then # z scores

    # get the baseline
    echo fslroi "$outdir"/stats/res4d "$outdir"/stats/baseline 0 $baseline
    fslroi "$outdir"/stats/res4d "$outdir"/stats/baseline 0 $baseline

    echo fslmaths "$outdir"/stats/baseline -Tstd "$outdir"/stats/baseline -odt float
    fslmaths "$outdir"/stats/baseline -Tstd "$outdir"/stats/baseline -odt float

    # transform to zscores
    echo fslmaths "$outdir"/stats/res4d -div "$outdir"/stats/baseline "$output_file"
    fslmaths "$outdir"/stats/res4d -div "$outdir"/stats/baseline "$output_file"

elif [ "$out_units" == "psc" ]; then # percent signal change

    tmean=`tempfile`
    echo fslmaths "$outdir"/stats/res4d -Tmean $tmean
    fslmaths "$outdir"/stats/res4d -Tmean $tmean
    echo fslmaths "$outdir"/stats/res4d -sub $tmean -div $tmean $output_file -mull 100 -odt float
    fslmaths "$outdir"/stats/res4d -sub $tmean -div $tmean $output_file -mul 100 -odt float

    to_delete="$to_delete $tmean"

elif [ "$out_units" == "raw" ]; then
    fslmaths "$outdir"/stats/res4d "$output_file"
else
    echo "error: unknown output type"
    exit
fi

#rm $to_delete