/******************************************************************************
 * RtDataImage.cpp defines a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtDataImage.h"

  
// default constructor
RtDataImage::RtDataImage() {
  data = NULL;
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
RtDataImage::RtDataImage(char *bytes, unsigned int len) {
  // try to do some checks
  if(len < sizeof(RtDataImageInfo)) {
    return;
  }

  // extract the info
  memcpy(&info,bytes,sizeof(RtDataImageInfo));

  // check if we have enough for the image data
  if(len-sizeof(RtDataImageInfo) != info.imgDataLen) {
    return;
  }

  data = new unsigned short[info.numPix];
  memcpy(data,bytes+sizeof(RtDataImageInfo),info.imgDataLen);
}

// constructor with data
//RtDataImage::RtDataImage(unsigned short *_data, vector<int> &_dims) {
//  int numPix = 0;
//  dims = _dims;
//  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
//    numPix *= *i;
//  }
//
//  memcpy(data,_data,numPix*sizeof(unsigned short));
//}

// construct from an image info struct and some byte data
RtDataImage::RtDataImage(RtExternalImageInfo &extinfo, unsigned short *bytes) 
    : info(extinfo) {

  // allocate and copy the img data
  data = (unsigned short*) malloc(info.imgDataLen);
  memcpy(data,bytes,info.imgDataLen);
}

// print info about this image
void RtDataImage::printInfo(ostream &os) {
  info.print(os);
}


// destructor
RtDataImage::~RtDataImage() {
  delete [] data;
}

// write the image to a file
//  in
//   filename: string filename
//  out
//   success or failure
bool RtDataImage::write(const string &filename) {

  unsigned int one = 1;
  unsigned int rows = info.dims[0];
  unsigned int cols = info.dims[1];
  

  FILE *imgFile = fopen(filename.c_str(),"wb");
  
  fwrite(&rows, sizeof(unsigned int), 1, imgFile);
  fwrite(&cols, sizeof(unsigned int), 1, imgFile);
  fwrite(&one,  sizeof(unsigned int), 1, imgFile);
  fwrite(data,  info.bytesPerPix, info.numPix, imgFile);
  fclose(imgFile);

  return true;
}

// set info struct
//  in
//   _info: struct to copy
void RtDataImage::setInfo(const RtExternalImageInfo &_info) {
  info = _info;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


