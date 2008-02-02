/******************************************************************************
 * RtMaskImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 * 
 *****************************************************************************/

#include"RtMaskImage.h"
#include"ace/Log_Msg.h"
#include"constants.h"
#include<fstream>

using namespace std;
  
// default constructor
RtMaskImage::RtMaskImage() : RtDataImage<bool>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage()")); 
  addToID("mask");
  magicNumber = MAGIC_NUMBER;
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
  data = new bool[numPix];
  imgDataLen = numPix*sizeof(bool);
  memcpy(data, img.data, imgDataLen);
}


// construct from an mri image
RtMaskImage::RtMaskImage(RtMRIImage &img, double threshold) 
    : RtDataImage<bool>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage(RtMaskImage)"));
  addToID("mask");
  magicNumber = MAGIC_NUMBER;

  setInfo(img);

  // build mask if threshold is specified
  if(fabs(threshold) > EPS) {
    initByMeanIntensityThreshold(img,threshold);
  }
}
  

// construct from an activation image
RtMaskImage::RtMaskImage(RtActivation &img, double threshold) 
    : RtDataImage<bool>() {
  ACE_TRACE(("RtMaskImage::RtMaskImage(RtMaskImage)"));
  addToID("mask");
  magicNumber = MAGIC_NUMBER;

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

  numOnVoxels = 0;

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
      setPixel(i,true);
      numOnVoxels++;
    }
    else {
      setPixel(i,false);
    }
  }
  
  return numOnVoxels;
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

  numOnVoxels = 0;

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
      setPixel(i,true);
      numOnVoxels++;
    }
    else {
      setPixel(i,false);
    }
  }
  
  return numOnVoxels;
}

// get the number of "on" voxels
unsigned int RtMaskImage::getNumberOfOnVoxels() {
  return numOnVoxels;
}


// set the info based on a generic data image info
void RtMaskImage::setInfo(RtMRIImage &img) {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));

  dims = img.getDims();

  imgDataLen = img.getImgDataLen();
  numPix = img.getNumPix();
  bytesPerPix = img.getBytesPerPix();

  gsl_matrix_memcpy(vxl2ras, img.getVxl2Ras());
  gsl_matrix_memcpy(ras2ref, img.getRas2Ref());

  if(data != NULL) {
    delete [] data;
  }

  data = new bool[numPix];
}

// set the info based on a generic data image info
void RtMaskImage::setInfo(RtActivation &img) {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));

  dims = img.getDims();

  imgDataLen = img.getImgDataLen();
  numPix = img.getNumPix();
  bytesPerPix = img.getBytesPerPix();

  gsl_matrix_memcpy(vxl2ras, img.getVxl2Ras());
  gsl_matrix_memcpy(ras2ref, img.getRas2Ref());

  if(data != NULL) {
    delete [] data;
  }

  data = new bool[numPix];
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


