/******************************************************************************
 * RtActivationInfo.h declares a class for MR image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#ifndef RTACTIVATIONINFO_H
#define RTACTIVATIONINFO_H

#include"RtDataImageInfo.h"

// info class
class RtActivationInfo : public RtDataImageInfo {

public:
  
  // magic numbers for image types
  const static unsigned int MAGIC_NUMBER  = 0x5083;

  RtActivationInfo()  
    : RtDataImageInfo() {
    magicNumber = MAGIC_NUMBER;
    bytesPerPix = sizeof(double);
    minMaxSet = false;
    minVal = 1e30; 
    maxVal = -1e30; 
    threshold = 2.0;    
  }

  // destruct
  ~RtActivationInfo() {

  }

  // deep copy constructor
  RtActivationInfo(const RtActivationInfo &info);

  // print some of the info members
  void print(ostream &os);

 public: // should be protected, find a way to make the template a friend

  bool minMaxSet;
  double minVal, maxVal;

  double threshold;
};

#endif
