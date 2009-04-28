/******************************************************************************
 * RtDataIDs.h contains defines for data id strings
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-12
 *
 *****************************************************************************/

#ifndef RTDATAIDS_H
#define RTDATAIDS_H

// remember to modify RtStream.cpp to add the new module type

// setup is that processing module ids have the format:
//  #define ID_<MODULENAME>
// and below that define names of all types of data that the module produces:
//  #define NAME_<MODULENAME>_<DATANAME>

const string ID_STREAM = "generic-stream";

const string ID_SCANNERIMG = "mri";
const string NAME_SCANNERIMG_EPI = "epi";
const string NAME_SCANNERIMG_T1 = "t1";
const string NAME_SCANNERIMG_T2 = "t2";
const string NAME_SCANNERIMG_T1EPI = "t1wepi";

const string ID_MASK = "mask";
const string ID_MASKLOAD = "mask-load";
const string ID_MASKGEN = "mask-gen";
const string NAME_MASK = "mask-img";

const string ID_MOSAIC = "mosaic";
// same names as scanner image

const string ID_UNMOSAIC = "unmosaic";
// same names as scanner image

const string ID_TEMPDIFF = "temporal-difference";
const string NAME_TEMPDIFF_IMG = "diff-img";

const string ID_TEMPMEAN = "temporal-mean";
const string NAME_TEMPMEAN_IMG = "mean-img";

const string ID_TEMPVAR = "temporal-variance";
const string NAME_TEMPVAR_IMG = "var-img";

const string ID_SPATIALINTENSITYNORM = "spatial-intensity-norm";
const string NAME_SPATIALINTENSITYNORM_IMG = "inorm-img";

// model fit ids
const string ID_MODEL_FIT = "model-fit";
const string ID_ACCUMCOR = "accumcor";
const string ID_INCREMENTALGLM = "incremental-glm";
const string ID_SINGLEIMCOR = "singleimcor";
const string ID_SLIDEWINCOR = "slidewincor";
const string ID_FLUCTUATIONMONITOR = "fluctuation-monitor";
const string ID_CONTRAST = "contrast";
const string ID_CURRENTACTIVATION = "current-activation";

// activation estimator names
const string NAME_DESIGN = "design-matrix";
const string NAME_BETA = "beta-img";
const string NAME_RESIDUAL_MSE = "residual_mse-img";
const string NAME_CORRELATION = "correlation-img";
const string NAME_STAT = "stat-img";
const string NAME_TSTAT = "tstat-img";
const string NAME_ACTIVATION = "activation-img";
const string NAME_BETA_MEAN = "beta-mean-img";
const string NAME_BETA_ACT = "beta-activation-img";
const string NAME_CONTRAST = "contrast-img";

// ROI computations
const string ID_ROICOMBINE = "roi-combine";
const string NAME_ROIVAL = "roi-val";
const string NAME_ROIMEAN = "roi-mean";
const string NAME_ROIVAR = "roi-variance";
const string NAME_ROIMEDIAN = "roi-median";
const string NAME_ROIWEIGHTEDAVE = "roi-weightedave";
const string NAME_ROISUM = "roi-sum";
const string NAME_ROIMAX = "roi-max";
const string NAME_ROIMIN = "roi-min";
const string ID_ROIDIFFERENCE = "roi-difference";
const string NAME_ROIDIFF = "roi-difference";
const string ID_ROIFEEDBACKSYNTH = "roi-feedbacksynth";


const string ID_EVENTTRIGGER = "event-trigger";
const string ID_EVENTTRIGGER_SYNTH = "event-trigger-synth";
const string ID_EVENTTRIGGER_ACTIVATIONSUMS = "event-trigger-activation-sums";
const string ID_EVENTTRIGGER_ACTIVATIONDIFF = "event-trigger-activation-diff";
const string NAME_EVENTTRIGGER_SIGNAL = "signal";
const string NAME_EVENTTRIGGER_BAD = "bad";
const string NAME_EVENTTRIGGER_GOOD = "good";

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


