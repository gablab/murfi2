/******************************************************************************
 * RtDataImage.cpp defines a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtDataImage.h"

  
// default constructor
RtDataImage::RtDataImage() {

}

// constructor with data
RtDataImage::RtDataImage(unsigned short *_data, vector<int> &_dims) {
  int numPix = 0;
  dims = _dims;
  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    numPix *= *i;
  }

  memcpy(data,_data,numPix*sizeof(unsigned short));
}

// construct from an image info struct and some byte data
RtDataImage::RtDataImage(ExternalImageInfo &info, char *bytes) {
  memcpy(data,bytes,info.lImageDataLength);
  dims[0] = info.nLin;
  dims[1] = info.nCol;
}

// destructor
RtDataImage::~RtDataImage() {
  delete [] data;
}



//private:
//
//  //*** private data members  ***//
//  
//  unsigned int creationTime;
//
//  unsigned short *data;
//
//  vector<int> dims;


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


