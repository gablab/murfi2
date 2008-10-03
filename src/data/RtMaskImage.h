/******************************************************************************
 * RtMaskImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 * 
 *****************************************************************************/

#ifndef RTMASKIMAGE_H
#define RTMASKIMAGE_H

#include"RtDataImage.h"
#include"RtMRIImage.h"
#include"RtActivation.h"

class RtActivation;

// class declaration
class RtMaskImage : public RtDataImage<unsigned short> {

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
  unsigned int initByMeanIntensityThreshold(RtActivation &img, double threshold);

  // set the info based on an activation's image info
  // NOTE: will change mask geometry must match the img
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
  void setAll(unsigned short val);

  // compute the number of "on" voxels
  void computeNumberOfOnVoxels();

  // get the number of "on" voxels
  unsigned int getNumberOfOnVoxels() const;


  // read the image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &_filename);

protected:

  // number of "on" voxels
  unsigned int numOnVoxels;
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


