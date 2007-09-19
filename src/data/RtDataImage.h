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

  // magic number for image files
  const static long MAGIC_NUMBER = 0x5081;

  //*** constructors/destructors  ***//
  
  // default constructor
  RtDataImage(); 

  // constructor with a filename to read the image from
  RtDataImage(const string &filename); 

  // construct from raw bytes -- BE CAREFUL WITH THIS
  RtDataImage(char *bytes, unsigned int len);

  // construct from  byte data and dim sizes
  //  RtDataImage(unsigned short *bytes, vector<int> &_dims); 

  // construct from an external image info struct (siemens) and some image data
  RtDataImage(RtExternalImageInfo &info, unsigned short *data); 

  // construct from an image info struct and (possibly blank) data
  RtDataImage(RtDataImageInfo &info, unsigned short *data = NULL); 

  // construct from another image
  RtDataImage(RtDataImage &img);

  // write an image to a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool write(const string &filename);

  // read an image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &filename);

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
  string getCreationTime() const;

  // get the acquisition number
  unsigned int getAcquisitionNum() const;

  // get dimensions
  int getDim(int i);

  // get number of pix
  int getNumPix();

  // get pixel value
  unsigned short getPixel(unsigned int i);

  // get data size
  unsigned int getImgDataLen();

  // get the image info
  RtDataImageInfo &getInfo();

  // get a pointer to the image data
  unsigned short *getData();


  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  //*** sets  ***//

  // sets the min and max pixel value for this data image
  void setMinMax();

  // set pixel value
  void setPixel(unsigned int i, unsigned short v);

  // set pixel value when locked
  void setPixelLocked(RtLocker *locker, unsigned int i, unsigned short v); 
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


