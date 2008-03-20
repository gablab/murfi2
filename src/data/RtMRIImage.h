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

#include"RtDataImage.h"
#include"RtExternalSenderImageInfo.h"

using namespace std;


// class declaration
class RtMRIImage : public RtDataImage<unsigned short> {

public:

  const static unsigned int MAGIC_NUMBER = 0x5081;

  //*** constructors/destructors  ***//
  
  // default constructor
  RtMRIImage(); 

  // destructor
  virtual ~RtMRIImage();

//  // constructor with a filename to read the image from
//  RtMRIImage(const string &filename); 
//
//  // construct from raw bytes -- BE CAREFUL WITH THIS
//  RtMRIImage(char *bytes, unsigned int len);
//
//  // construct from an image info struct and (possibly blank) data
//  RtMRIImage(RtMRIImageInfo &_info, unsigned short  *_data = NULL);

  // construct from an external image info struct (siemens) and some image data
  RtMRIImage(RtExternalImageInfo &info, unsigned short *data = NULL); 

  // construct from another image
  RtMRIImage(RtMRIImage &img);

  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool writeInfo(ostream &os);

  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool readInfo(istream &is);

  // set info struct
  //  in
  //   _info: struct to copy
  void setInfo(const RtExternalImageInfo &_info);

  // set this image based on a passed image info and data
  //  in
  //   info: struct
  //   data: array (optional, image data will be  allocated and set
  //         to all zeros if null) 
  //void setImage(RtMRIImageInfo &_info, unsigned short *_data);

  // print info about this image
  void printInfo(ostream &os);

  //********  methods for getting data from the image *******//

  // get the acquisition number
  string getCreationTime() const;

  // get the acquisition number
  unsigned int getAcquisitionNum() const;

  // set the matrix size
  void setMatrixSize(unsigned int ms);

  // get the matrix size
  unsigned int getMatrixSize();

  // set the matrix size
  void setNumSlices(unsigned int ns);

  // get the matrix size
  unsigned int getNumSlices();

  // get the series number
  unsigned int getSeriesNum() const;

  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  //*** sets  ***//

  // set the series number
  void setSeriesNum(unsigned int);

protected:

  unsigned long slice;                 // slice index (zero based)

  double readFOV;             // mm
  double phaseFOV;            // mm
  double sliceThick;          // mm


  // imaging parms
  bool         pace;                // pace-enabled sequence
  bool         swapReadPhase;       
  unsigned int  seriesNum;           // series number
  unsigned int  acqNum;              // acquisition number
  double timeAfterStart;      // time after start
  double te;                  // echo time (ms)
  double tr;                  // repetition time (ms)
  double ti;                  // inversion time (ms)
  double triggerTime;         // trigger time (ms)

  // actual acquision info parms
  ACE_Date_Time time;                // acquisition time
  ACE_Date_Time refFrameTime;        // acquisition time of frame of reference
  double        reconDelay;          // time delay of the reconstruction (ms)


  // scanner online post-processing parms
  bool distCorrect2D;         // 2d distortion correction
  bool moco;                  // motion correction

  // received data parms
  bool fromScanner;
  
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


