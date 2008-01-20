/******************************************************************************
 * RtDataIDs.h contains defines for data id strings
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-12
 * 
 *****************************************************************************/

#ifndef RTDATAIDS_H
#define RTDATAIDS_H

// remember to modify RtStream.cpp to add the new module type

#define ID_SCANNERIMG   "data.image.mri"
#define ID_DIFFIMG      "data.image.mri.voxel-difference"
#define ID_MEANIMG      "data.image.mri.voxel-mean"
#define ID_VARIMG       "data.image.mri.voxel-variance"
#define ID_ZSCOREIMG    "data.image.activation.voxel-zscore"
#define ID_SLIDEWINCOR  "data.image.activation.voxel-slidewincor"
#define ID_ACCUMCOR     "data.image.activation.voxel-accumcor"
#define ID_SINGLEIMCOR  "data.image.activation.voxel-singleimcor"

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


