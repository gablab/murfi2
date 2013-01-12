/*=========================================================================
 *  RtMaskImage.h declares a class for an MR image
 * 
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RTMASKIMAGE_H
#define RTMASKIMAGE_H

#include"RtDataImage.h"
#include"RtMRIImage.h"
#include"RtActivation.h"

#include<vector>
using namespace std;

class RtActivation;

// class declaration
class RtMaskImage : public RtDataImage<short> {
 public:

  const static unsigned int MAGIC_NUMBER = 0x5084;

  //*** constructors/destructors  ***//

  // default constructor
  RtMaskImage();

  // destructor
  virtual ~RtMaskImage();

  // construct from another mask
  RtMaskImage(RtMaskImage &img);

  // construct by thresholding mean intensity of an MRI image
  // in:
  //  img:       image to build mask from
  //  threshold: mean intensity multiplier to threshold at (set to
  //             zero to leave mask allocated but uninitialized)
  RtMaskImage(RtMRIImage &img, double threshold = 0.0);

  // construct by thresholding mean intensity of an activation image
  // in:
  //  img:       image to build mask from
  //  threshold: mean intensity multiplier to threshold at (set to
  //             zero to leave mask allocated but uninitialized)
  RtMaskImage(RtActivation &img, double threshold = 0.0);

  // initialize mask by mean intensity threshold of another image
  // NOTE: will change mask geometry must match the img
  // in:
  //  img:    image to build mask from
  //  thresh: mean intensity multiplier to threshold at
  unsigned int initByMeanIntensityThreshold(RtMRIImage &img, double threshold);

  // initialize mask by mean intensity threshold of another image
  // NOTE: will change mask geometry must match the img
  // in:
  //  img:    image to build mask from
  //  thresh: mean intensity multiplier to threshold at
  unsigned int initByMeanIntensityThreshold(RtActivation &img,
                                            double threshold);

  // set the info based on an activation's image info
  // NOTE: will change mask geometry to match the img
  // in:
  //  img:    image to build mask from
  void setInfo(RtActivation &img);

  // set the info based on a mri image's info
  // NOTE: will change mask geometry must match the img
  // in:
  //  img:    image to build mask from
  void setInfo(RtMRIImage &img);

  // sets all voxels
  // in:
  //  val: value to set all voxels to
  void setAll(short val);

  // compute the number of "on" voxels
  void updateOnVoxelIndices();

  // get the indices of "on" voxels
  vector<unsigned int> getOnVoxelIndices() const;

  // get the number of "on" voxels
  unsigned int getNumberOfOnVoxels() const;

  // read the image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &_filename);

 protected:

  // "on" voxel indices
  vector<unsigned int> onIndices;
};



#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
