/******************************************************************************
 * RtActivation.h declares a class for an activation image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#ifndef RTACTIVATION_H
#define RTACTIVATION_H

#include<iostream>
#include"tinyxml/tinyxml.h"

#include"RtDataImage.h"
#include"RtMRIImage.h"
#include"RtMaskImage.h"
#include"RtTypes.h"


class RtMaskImage;

using namespace std;

// class declaration
class RtActivation : public RtDataImage<double> {

public:

  // magic numbers for image types
  const static unsigned int MAGIC_NUMBER = 0x5083;

  //*** constructors/destructors  ***//

  // default constructor
  RtActivation();

  // construct with info from an MRI image
  RtActivation(RtMRIImage &img);

  // construct with a certain number of elements
  RtActivation(unsigned int numElements);

  // destructor
  virtual ~RtActivation();

  // serialize the data as xml for transmission or saving to a file
  virtual TiXmlDocument *serializeAsXML();

  // print info about this image
  void printInfo(ostream &os);

  // create a mask by thresholding the activation
  // in:
  //  sign of activation to take (POS,NEG,POSNEG)
  // out:
  //  binary mask image 
  RtMaskImage *toMask(Sign direction);

  //********  methods for getting data from the image *******//

  // get the threshold
  double getThreshold() const;

  // get the ceiling
  double getCeiling() const;

  // get the scale inverted flag
  bool getScaleIsInverted() const;

  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  //*** sets  ***//

  // sets the threshold
  void setThreshold(double thresh);

  // sets the ceiling
  void setCeiling(double thresh);

  // set the scale inverted flag
  void setScaleIsInverted(bool sii);

  // set the info based on a generic data image info
  void setInfo(RtMRIImage &_info);

  // reallocate data array based on current number of pixels
  void reallocateData();

protected:
  
  // for display
  double threshold;
  double ceiling;

  bool scaleIsInverted; // true for data where near zero is active (two-tailed)

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


