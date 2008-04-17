/******************************************************************************
 * RtDataIDs.h contains defines for data id strings
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-12
 * 
 *****************************************************************************/

#ifndef RTDATAIDS_H
#define RTDATAIDS_H

// remember to modify RtStream.cpp to add the new module type

#define ID_SCANNERIMG                 "data.image.mri"
#define ID_DIFFIMG                    "data.image.mri.voxel-difference"
#define ID_MEANIMG                    "data.image.mri.voxel-mean"
#define ID_VARIMG                     "data.image.mri.voxel-variance"
#define ID_INTENSITYNORMIMG           "data.image.mri.intensity-norm"
#define ID_ZSCOREIMG                  "data.image.activation.voxel-zscore"
#define ID_SLIDEWINCOR                "data.image.activation.voxel-slidewincor"
#define ID_ACCUMCOR                   "data.image.activation.voxel-accumcor"
#define ID_INCREMENTALGLM             "data.image.activation.voxel-incrglm"
#define ID_SINGLEIMCOR                "data.image.activation.voxel-singleimcor"
#define ID_FLUCTUATIONMONITOR         "data.image.activation.voxel-fluctuationmonitor"
#define ID_ACTIVATIONSUM              "data.image.activation.activation-sum"
#define ID_ACTIVATIONSUMDIFFERENCE    "data.image.activation.activation-sum-difference"
#define ID_EVENTTRIGGER               "data.event.trigger"
#define ID_EVENTTRIGGERACTIVATIONSUMS "data.event.trigger.activation-sums"
#define ID_EVENTTRIGGERACTIVATIONDIFF "data.event.trigger.activation-diff"

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


