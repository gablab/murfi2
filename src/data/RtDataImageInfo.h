/******************************************************************************
 * RtDataImageInfo.h declares a class for MR image info
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
#include"RtExternalSenderImageInfo.h"

// info class
class RtDataImageInfo : public RtData {

public:
  
  RtDataImageInfo()  :
       imgDataLen(0),
       numPix(0),
       bytesPerPix(sizeof(unsigned short)),
       minMaxSet(false),
       minVal(USHRT_MAX),
       maxVal(0),
       vxl2ras(gsl_matrix_calloc(4,4)),
       ras2ref(gsl_matrix_calloc(4,4)),
       slice(0),
       readFOV(0),
       phaseFOV(0),
       sliceThick(0),
       pace(false),
       swapReadPhase(false),
       acqNum(0),
       timeAfterStart(0),
       te(0), tr(0), ti(0),
       triggerTime(0),
       reconDelay(0),
       distCorrect2D(false),
       moco(false),
       fromScanner(false)
  {
    
  }

  // destruct
  ~RtDataImageInfo() {

    // free matrices
    gsl_matrix_free(vxl2ras);
    gsl_matrix_free(ras2ref);
  }

  // deep copy constructor
  RtDataImageInfo(const RtDataImageInfo &info);

  // copy constructor accepting a siemens ExternalImageInfo struct
  RtDataImageInfo(const RtExternalImageInfo &info);

  // print info members to cout 
  void print();

  // print some of the info members
  void print(ostream &os);

  // convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
  static ACE_Date_Time siemensTime2ACE_Date_Time(const char *tc);

  // convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
  static string ACE_Date_Time2SiemensTime(const ACE_Date_Time &t);

protected:

  int numDims;
  vector<int> dims;

  // info about the pixels
  unsigned long  imgDataLen;
  unsigned long  numPix;
  unsigned short bytesPerPix;

  bool minMaxSet;
  unsigned short minVal, maxVal;


  gsl_matrix *vxl2ras;         // transformation matrix: voxels to RAS space
  gsl_matrix *ras2ref;         // transformation matrix: RAS to reference space
  
  long slice;                 // slice index (zero based)

  double readFOV;             // mm
  double phaseFOV;            // mm
  double sliceThick;          // mm


  // imaging parms
  bool   pace;                // pace-enabled sequence
  bool   swapReadPhase;       
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
  friend class RtDataImage;

};

#endif
