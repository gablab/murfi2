/******************************************************************************
 * RtMRIImageInfo.h declares a class for MR image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#ifndef RTMRIIMAGEINFO_H
#define RTMRIIMAGEINFO_H

#include<iostream>
#include<vector>
#include<iomanip>
#include"gsl_matrix.h"
#include"RtDataImageInfo.h"
//#include"RtDataImage.h"
#include"RtExternalSenderImageInfo.h"

// info class
class RtMRIImageInfo : public RtDataImageInfo {

public:
  
  // magic numbers for image types
  const static unsigned int MAGIC_NUMBER  = 0x5081;

  RtMRIImageInfo()  : RtDataImageInfo(),
       slice(0),
       readFOV(0),
       phaseFOV(0),
       sliceThick(0),
       pace(false),
       swapReadPhase(false),
       seriesNum(1),
       acqNum(1),
       timeAfterStart(0),
       te(0), tr(0), ti(0),
       triggerTime(0),
       reconDelay(0),
       distCorrect2D(false),
       moco(false),
       fromScanner(false) {
    magicNumber = MAGIC_NUMBER;
    bytesPerPix = sizeof(unsigned short);
    minMaxSet = false;
    minVal = USHRT_MAX;
    maxVal = 0;     
  }

  // destruct
  ~RtMRIImageInfo() {

    // free matrices
    gsl_matrix_free(vxl2ras);
    gsl_matrix_free(ras2ref);
  }

  // deep copy constructor
  RtMRIImageInfo(const RtMRIImageInfo &info);

  // copy constructor accepting a siemens ExternalImageInfo struct
  RtMRIImageInfo(const RtExternalImageInfo &info);

  // print some of the info members
  void print(ostream &os);

 public: // should be protected, find a way to make the template a friend

  bool minMaxSet;
  unsigned short minVal, maxVal;

  long slice;                 // slice index (zero based)

  double readFOV;             // mm
  double phaseFOV;            // mm
  double sliceThick;          // mm


  // imaging parms
  bool   pace;                // pace-enabled sequence
  bool   swapReadPhase;       
  int    seriesNum;           // series number
  int    acqNum;              // acquisition number
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

  // let images access our members
//  template<class T>
//  friend class RtDataImage<T>;

};

#endif
