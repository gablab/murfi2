/******************************************************************************
 * RtDataImage.h declares a base class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATAIMAGE_H
#define RTDATAIMAGE_H

#include<vector>
#include<iostream>
#include"ace/Date_Time.h"

#include"RtDataImageInfo.h"
#include"RtData.h"
#include"RtExternalSenderImageInfo.h"

using namespace std;


// class declaration
class RtDataImage : public RtData {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtDataImage(); 

  // construct from raw bytes -- BE CAREFUL WITH THIS
  RtDataImage(char *bytes, unsigned int len);

  // construct from  byte data and dim sizes
  //  RtDataImage(unsigned short *bytes, vector<int> &_dims); 

  // construct from an image info struct and some image data
  RtDataImage(RtExternalImageInfo &info, unsigned short *data); 

  // write an image to a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool write(const string &filename);

  // set info struct
  //  in
  //   _info: struct to copy
  void setInfo(const RtExternalImageInfo &_info);

  // print info about this image
  void printInfo(ostream &os);

  // destructor
  virtual ~RtDataImage();

  //********  methods for getting data from the image *******//

  // get the acquisition number
  string getCreationTime() const {
    return RtDataImageInfo::ACE_Date_Time2SiemensTime(creationTime);
  }

  // get the acquisition number
  unsigned int getAcquisitionNum() const {
    return info.acqNum;
  }

  // get dimensions
  int getDim(int i) {
    return info.numDims > i && i >= 0 ? info.dims[i] : -1;
  }

  // get data size
  unsigned int getImgDataLen() {
    return info.imgDataLen;
  }

  // get the image info
  RtDataImageInfo &getInfo() {
    return info;
  }

  // get a pointer to the image data
  unsigned short *getData() {
    return data;
  }

  // sets the min and max pixel value for this data image
  void setMinMax();


  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

private:

  //*** private data members  ***//
  
  ACE_Date_Time creationTime;

  unsigned short *data;

  RtDataImageInfo info;
  
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


