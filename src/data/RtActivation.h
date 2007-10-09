/******************************************************************************
 * RtActivation.h declares a class for an activation image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#ifndef RTACTIVATION_H
#define RTACTIVATION_H

#include<iostream>

#include"RtDataImage.h"
#include"RtActivationInfo.h"
#include"RtMRIImageInfo.h"

using namespace std;

// class declaration
class RtActivation : public RtDataImage<double> {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtActivation();

  // construct with info from an MRI image
  RtActivation(const RtMRIImageInfo &_info);

  // destructor
  virtual ~RtActivation();

  // write an activation image to a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool write(const string &filename);

  // read an activation image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &filename);

  // print info about this image
  void printInfo(ostream &os);

  //********  methods for getting data from the image *******//

  // get the threshold
  double getThreshold() const;

  // get the image info
  RtActivationInfo &getInfo();

  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  //*** sets  ***//

  // sets the threshold
  void setThreshold(double thresh);

  // sets the threshold
  void setMinMax();

  // set info from an MRI image
  void setInfo(const RtMRIImageInfo &_info);

private:

  double threshold;

  RtActivationInfo info;

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


