/******************************************************************************
 * RtDataImageInfo.h declares a class for generic image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#ifndef RTDATAIMAGEINFO_H
#define RTDATAIMAGEINFO_H

#include<iostream>
#include<vector>
#include<iomanip>
#include"gsl_matrix.h"
#include"RtData.h"

// info class
class RtDataImageInfo {

public:

  static const unsigned int MAGIC_NUMBER = 0;

  RtDataImageInfo()  :
       magicNumber(MAGIC_NUMBER),
       filename(""),
       imgDataLen(0),
       numPix(0),
       bytesPerPix(sizeof(unsigned short)),
       vxl2ras(gsl_matrix_calloc(4,4)),
       ras2ref(gsl_matrix_calloc(4,4))
  {
    
  }

  // destruct
  virtual ~RtDataImageInfo() {

    // free matrices
    gsl_matrix_free(vxl2ras);
    gsl_matrix_free(ras2ref);
  }

  // deep copy constructor
  RtDataImageInfo(const RtDataImageInfo &info);

  // check whether a magic number is supported by this class
  virtual bool validateMagicNumber(unsigned int mn);

  // set the bytes per pixel and guess the magic number
  void setBytesPerPix(unsigned short _bpp);

  // set the magic number
  void setMagicNumber(const unsigned int mn);

  // print info members to cout 
  void print();

  // print some of the info members
  void print(ostream &os);

  // convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
  static ACE_Date_Time siemensTime2ACE_Date_Time(const char *tc);

  // convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
  static string ACE_Date_Time2SiemensTime(const ACE_Date_Time &t);

 public: // should be protected, find a way to make the template a friend

  unsigned int magicNumber;
  string filename;

  int numDims;
  vector<int> dims;

  // info about the pixels
  unsigned long  imgDataLen;
  unsigned long  numPix;
  unsigned short bytesPerPix;

  gsl_matrix *vxl2ras;         // transformation matrix: voxels to RAS space
  gsl_matrix *ras2ref;         // transformation matrix: RAS to reference space
  
};

#endif
