/******************************************************************************
 * RtActivation.cpp defines a class for an activation image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#include"RtActivation.h"
#include<cmath>


//*** constructors/destructors  ***//

// default constructor
RtActivation::RtActivation() : RtDataImage<double>() {

}

// destructor
RtActivation::~RtActivation() {
}

// write an activation image to a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtActivation::write(const string &filename) {
  
  return true;
}

// read an activation image from a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtActivation::read(const string &filename) {
  
  return true;
}

// print info about this image
void RtActivation::printInfo(ostream &os) {
  info.print(os);
}

//********  methods for getting data from the image *******//

// get the threshold
double RtActivation::getThreshold() const {
  return info.threshold;
}

// get the image info
RtActivationInfo &RtActivation::getInfo() {
  return info;
}

// get a smart contrast level
float RtActivation::getAutoContrast() {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));
  
  if(!info.minMaxSet) {
    setMinMax();
  }

  float c = (float) (( fabs(info.minVal) > fabs(info.maxVal) 
		       ? info.minVal : info.maxVal) - threshold);
  return c > 0 ? c : 0;
}

// get a smart brightness level
float RtActivation::getAutoBrightness() {
  ACE_TRACE(("RtDataImage<T>::getAutoContrast"));
  
  if(!info.minMaxSet) {
    setMinMax();
  }

  return (float) threshold;
}

//*** sets  ***//

// sets the threshold
void RtActivation::setThreshold(double thresh) {
  info.threshold = thresh;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


