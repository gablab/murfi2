/******************************************************************************
 * RtDataImage.h declares a base class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATAIMAGE_H
#define RTDATAIMAGE_H

#include<vector>
#include"RtData.h"
#include"ExternalSenderImageInfo.h"

using namespace std;

// class declaration
class RtDataImage : public RtData {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtDataImage(); 

  // construct from  byte data and dim sizes
  RtDataImage(unsigned short *bytes, vector<int> &_dims); 

  // construct from an image info struct and some byte data
  RtDataImage(ExternalImageInfo &info, char *bytes); 

  // destructor
  virtual ~RtDataImage();

private:

  //*** private data members  ***//
  
  unsigned int creationTime;

  unsigned short *data;

  vector<int> dims;

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


