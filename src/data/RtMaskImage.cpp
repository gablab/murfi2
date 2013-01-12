/*=========================================================================
 *  RtMaskImage.cpp defines a class for an MR image
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

#include"RtMaskImage.h"

#include"RtDataIDs.h"
#include"ace/Log_Msg.h"
#include"constants.h"
#include<fstream>

using namespace std;

// default constructor
RtMaskImage::RtMaskImage() : RtDataImage<short>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage()"));

  dataID.setModuleID(ID_MASK);
  dataID.setDataName(NAME_MASK);

  magicNumber = MAGIC_NUMBER;
  bytesPerPix = sizeof(short);
}

// destructor
RtMaskImage::~RtMaskImage() {
  // nothing to do
}

// construct from another image (deep copy)
// only use this with like datatypes
RtMaskImage::RtMaskImage(RtMaskImage &img) {
  ACE_TRACE(("RtMaskImage::RtMaskImage(RtMaskImage)"));

  (*this) = img;

  magicNumber = MAGIC_NUMBER;

  // copy the data
  data = new short[numPix];
  imgDataLen = numPix*sizeof(short);
  memcpy(data, img.data, imgDataLen);
  bytesPerPix = sizeof(short);
}


// construct from an mri image
RtMaskImage::RtMaskImage(RtMRIImage &img, double threshold)
    : RtDataImage<short>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage(RtMaskImage)"));

  dataID.setHistory
      (img.getDataID().getHistory() + "." + img.getDataID().getModuleID());
  dataID.setModuleID(ID_MASK);
  dataID.setDataName(NAME_MASK);

  magicNumber = MAGIC_NUMBER;
  bytesPerPix = sizeof(short);

  setInfo(img);

  // build mask if threshold is specified
  if(fabs(threshold) > EPS) {
    initByMeanIntensityThreshold(img,threshold);
  }
}


// construct from an activation image
RtMaskImage::RtMaskImage(RtActivation &img, double threshold)
    : RtDataImage<short>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage(RtMaskImage)"));

  dataID.setHistory
      (img.getDataID().getHistory() + "." + img.getDataID().getModuleID());
  dataID.setModuleID(ID_MASK);
  dataID.setDataName(NAME_MASK);

  magicNumber = MAGIC_NUMBER;
  bytesPerPix = sizeof(short);

  setInfo(img);

  // build mask if threshold is specified
  if(fabs(threshold) > EPS) {
    initByMeanIntensityThreshold(img,threshold);
  }
}


// initialize mask by mean intensity threshold of another image
// in:
//  img:    image to build mask from
//  thresh: mean intensity multiplier to threshold at
// out:
//  the number of voxels in the new mask
unsigned int RtMaskImage::initByMeanIntensityThreshold(RtMRIImage &image,
                                                       double threshold) {
  // validate size
  if(getNumPix() != image.getNumPix()) {
    setInfo(image);
  }

  onIndices.clear();

  // first compute the mean voxel intensity
  double mean = 0;
  for(unsigned int i = 0; i < image.getNumEl(); i++) {
    mean += image.getElement(i);
  }
  mean /= image.getNumEl();

  // find voxels above threshold
  double maskThresh = threshold*mean;

  // assign ones to mask positive voxels and count the number of comparisons
  for(unsigned int i = 0; i < image.getNumEl(); i++) {
    if(image.getElement(i) > maskThresh) {
      setPixel(i,1);
      onIndices.push_back(i);
    }
    else {
      setPixel(i,0);
    }
  }

  return onIndices.size();
}

// initialize mask by mean intensity threshold of another image
// in:
//  img:    image to build mask from
//  thresh: mean intensity multiplier to threshold at
// out:
//  the number of voxels in the new mask
unsigned int RtMaskImage::initByMeanIntensityThreshold(RtActivation &image,
                                                       double threshold) {
  // validate size
  if(getNumPix() != image.getNumPix()) {
    setInfo(image);
  }

  onIndices.clear();

  // first compute the mean voxel intensity
  double mean = 0;
  for(unsigned int i = 0; i < image.getNumEl(); i++) {
    mean += image.getElement(i);
  }
  mean /= image.getNumEl();

  // find voxels above threshold
  double maskThresh = threshold*mean;

  // assign ones to mask positive voxels and count the number of comparisons
  for(unsigned int i = 0; i < image.getNumEl(); i++) {
    if(image.getElement(i) > maskThresh) {
      setPixel(i,1);
      onIndices.push_back(i);
    }
    else {
      setPixel(i,0);
    }
  }

  return onIndices.size();
}

// get the number of "on" voxels
unsigned int RtMaskImage::getNumberOfOnVoxels() const {
  return onIndices.size();
}

// get the indices of "on" voxels
vector<unsigned int> RtMaskImage::getOnVoxelIndices() const {
  return onIndices;
}

// set the number of "on" voxels
void RtMaskImage::updateOnVoxelIndices() {
  onIndices.clear();
  for(unsigned int i = 0; i < getNumEl(); i++) {
    if(getElement(i)) {
      onIndices.push_back(i);
    }
  }
}

// read the image from a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtMaskImage::read(const string &_filename) {
  bool ret = RtDataImage<short>::read(_filename);
  updateOnVoxelIndices();

  return ret;
}


// sets all voxels
// in:
//  val: value to set all voxels to
void RtMaskImage::setAll(short val) {
  for(unsigned int i = 0; i < getNumEl(); i++) {
    setPixel(i,val);
  }
}

// set the info based on a generic data image info
void RtMaskImage::setInfo(RtMRIImage &img) {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));

  dims = img.getDims();
  pixdims = img.getPixDims();

  imgDataLen = img.getImgDataLen();
  numPix = img.getNumPix();
  bytesPerPix = sizeof(short);

  vxl2ras = img.getVxl2Ras();
  ras2ref = img.getRas2Ref();

  if(data != NULL) {
    delete [] data;
  }

  data = new short[numPix];
}

// set the info based on a generic data image info
void RtMaskImage::setInfo(RtActivation &img) {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));

  dims = img.getDims();
  pixdims = img.getPixDims();

  imgDataLen = img.getImgDataLen();
  numPix = img.getNumPix();
  bytesPerPix = sizeof(short);

  vxl2ras = img.getVxl2Ras();
  ras2ref = img.getRas2Ref();

  if(data != NULL) {
    delete [] data;
  }

  data = new short[numPix];
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
