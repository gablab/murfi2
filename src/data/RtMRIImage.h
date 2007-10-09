/******************************************************************************
 * RtMRIImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 * 
 *****************************************************************************/

#ifndef RTMRIIMAGE_H
#define RTMRIIMAGE_H

#include<vector>
#include<iostream>
#include"ace/Date_Time.h"

#include"RtMRIImageInfo.h"
#include"RtDataImage.h"
#include"RtExternalSenderImageInfo.h"

using namespace std;


// class declaration
class RtMRIImage : public RtDataImage<unsigned short> {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtMRIImage(); 

  // constructor with a filename to read the image from
  RtMRIImage(const string &filename); 

  // construct from raw bytes -- BE CAREFUL WITH THIS
  RtMRIImage(char *bytes, unsigned int len);

  // construct from an image info struct and (possibly blank) data
  RtMRIImage(RtMRIImageInfo &_info, unsigned short  *_data = NULL);

  // construct from an external image info struct (siemens) and some image data
  RtMRIImage(RtExternalImageInfo &info, unsigned short *data); 

  // construct from another image
  RtMRIImage(RtMRIImage &img);

  // write the image to a file
  // TODO: templatize this or something so dont have to copy (info is the issue)
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool write(const string &filename);

  // read the image from a file
  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool read(const string &filename);  

  // set info struct
  //  in
  //   _info: struct to copy
  void setInfo(const RtExternalImageInfo &_info);

  // set this image based on a passed image info and data
  //  in
  //   info: struct
  //   data: array (optional, image data will be  allocated and set
  //         to all zeros if null) 
  void setImage(RtMRIImageInfo &_info, unsigned short *_data);

  // print info about this image
  void printInfo(ostream &os);

  // destructor
  virtual ~RtMRIImage();

  //********  methods for getting data from the image *******//

  // get the acquisition number
  string getCreationTime() const;

  // get the acquisition number
  unsigned int getAcquisitionNum() const;

  // get the series number
  unsigned int getSeriesNum() const;

  // get the image info
  RtMRIImageInfo &getInfo();


  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  //*** sets  ***//

  // sets the min and max pixel value for this data image
  void setMinMax();

  // set the series number
  void setSeriesNum(unsigned int);


private:

  RtMRIImageInfo info;
  
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


