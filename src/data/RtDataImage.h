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
#include<iomanip>
#include"ace/Date_Time.h"
#include"gsl_matrix.h"

#include"RtData.h"
#include"RtExternalSenderImageInfo.h"

using namespace std;

class RtDataImage;

// info class
class RtDataImageInfo : public RtData {

public:
  
  RtDataImageInfo()  :
       bytesPerPix(sizeof(unsigned short)),
       vxl2ras(gsl_matrix_calloc(4,4)),
       ras2ref(gsl_matrix_calloc(4,4))
  {

  }
  
  // copy constructor accepting a siemens ExternalImageInfo struct
  RtDataImageInfo(const RtExternalImageInfo &info) :
    bytesPerPix(sizeof(unsigned short))  {

    // determine the dimensions
//    if(info.nCol != rint(info.dFOVread*info.dVoxSizRead)) {// probably mosaiced
//      
//    }
//    else {
      numDims = 2;
      dims.resize(numDims);
      dims[0] = info.nLin;
      dims[1] = info.nCol;
//    }

    // calculate image size
    imgDataLen = bytesPerPix;
    numPix = 1;
    for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
      imgDataLen *= (*i);
      numPix *= (*i);
    }

    // build voxel 2 RAS transformation matrix
    vxl2ras = gsl_matrix_calloc(4,4);
    gsl_matrix_set_identity(vxl2ras);

    gsl_matrix_set(vxl2ras,0,0,info.dRowSag);
    gsl_matrix_set(vxl2ras,0,1,info.dRowCor);
    gsl_matrix_set(vxl2ras,0,2,info.dRowTra);

    gsl_matrix_set(vxl2ras,1,0,info.dColSag);
    gsl_matrix_set(vxl2ras,1,1,info.dColCor);
    gsl_matrix_set(vxl2ras,1,2,info.dColTra);

    gsl_matrix_set(vxl2ras,2,0,info.dNorSag);
    gsl_matrix_set(vxl2ras,2,1,info.dNorCor);
    gsl_matrix_set(vxl2ras,2,2,info.dNorTra);

    gsl_matrix_set(vxl2ras,0,3,info.dPosSag);
    gsl_matrix_set(vxl2ras,1,3,info.dPosCor);
    gsl_matrix_set(vxl2ras,2,3,info.dPosTra);

    // build RAS 2 REF transformation matrix
    ras2ref = gsl_matrix_calloc(4,4);
    gsl_matrix_set_identity(ras2ref);


    // image info
    slice = info.lSliceIndex;
    readFOV = info.dFOVread;
    phaseFOV = info.dFOVphase;
    sliceThick = info.dThick;
    

    pace = false;
    swapReadPhase = info.bSwapReadPhase;       
    acqNum = info.iAcquisitionNumber;
    timeAfterStart = info.dTimeAfterStart;
    te = info.dTE;
    tr = info.dTR;
    ti = info.dTI;
    triggerTime = info.dTriggerTime;

    // actual acquision info parms
    time = siemensTime2ACE_Date_Time(info.chAcquisitionTime);
    refFrameTime = siemensTime2ACE_Date_Time(info.chframeOfReference);
    reconDelay = info.dTimeDelay;


    // scanner online post-processing parms
    distCorrect2D = false;
    moco = false;  

    // received data parms
    fromScanner = info.iDataSource == 0;

  }

  // destruct
  ~RtDataImageInfo() {

    // free matrices
    gsl_matrix_free(vxl2ras);
    gsl_matrix_free(ras2ref);
  }

  // print info members to cout 
  void print() {
    print(cout);
  }

  // print some of the info members
  void print(ostream &os) {
    int wid = 30;

    os << setiosflags(ios::left);

    os << "---------------------------" << endl
       << "dumping RtDataImageInfo" << endl
       << "---------------------------" << endl
       << setw(wid) << "created at: " 
       << ACE_Date_Time2SiemensTime(creationTime) << endl
       << setw(wid) << "ndims: / dim1 / dim2... /" << numDims << ": / ";

    for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
      os << *i << " / ";
    }

    os << endl
       << setw(wid) << "imgDataLen in bytes " << imgDataLen << endl
       << setw(wid) << "numPix" << numPix << endl
       << setw(wid) << "bytesPerPix" << bytesPerPix << endl;
    
    os << setw(wid) << "vxl2ras transform";
    for(int i = 0; i < 4; i++) {
      if(i > 0) {
	os << setw(wid) << "";
      }
      for(int j = 0; j < 4; j++) {
	os << gsl_matrix_get(vxl2ras,i,j) << " ";
      }
      os << endl;
    }

    os << setw(wid) << "ras2ref transform";
    for(int i = 0; i < 4; i++) {
      if(i > 0) {
	os << setw(wid) << "";
      }
      for(int j = 0; j < 4; j++) {
	os << gsl_matrix_get(ras2ref,i,j) << " ";
      }
      os << endl;
    }

    os << setw(wid) << "slice" << slice << endl 
       << setw(wid) << "readFOV phaseFOV" 
       << readFOV << " " << phaseFOV << endl
       << setw(wid) << "sliceThick" << sliceThick << endl
       << setw(wid) << "pace" << pace << endl
       << setw(wid) << "swapReadPhase" << swapReadPhase << endl
       << setw(wid) << "acqNum" << acqNum << endl
       << setw(wid) << "timeAfterStart" << timeAfterStart << endl
       << setw(wid) << "te / tr / ti" << te << " / " 
       << tr << " / " << ti << endl
       << setw(wid) << "triggerTime" << triggerTime << endl
       << setw(wid) << "time" << ACE_Date_Time2SiemensTime(time) << endl
       << setw(wid) << "refFrameTime" 
       << ACE_Date_Time2SiemensTime(refFrameTime) << endl
       << setw(wid) << "reconDelay" << reconDelay << endl
       << setw(wid) << "distCorrect2D" << distCorrect2D << endl
       << setw(wid) << "moco" << moco << endl
       << setw(wid) << "fromScanner" << fromScanner << endl
       << "---------------------------" << endl;
  }

  // convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
  static ACE_Date_Time siemensTime2ACE_Date_Time(const char *tc) {
    string timeStr = tc;
    ACE_Date_Time t;
    t.hour(atol(timeStr.substr(0,2).c_str()));
    t.minute(atol(timeStr.substr(2,2).c_str()));
    t.second(atol(timeStr.substr(4,2).c_str()));
    t.microsec(atol(timeStr.substr(7,6).c_str()));
    return t;
  }

  // convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
  static string ACE_Date_Time2SiemensTime(const ACE_Date_Time &t) {
    char str[] = "hhmmss.xxxxxx";
    sprintf(str,"%02ld%02ld%02ld.%06ld", 
	    t.hour(), t.minute(), t.second(), t.microsec());

    string s(str);
    return s;
  }

protected:

  int numDims;
  vector<int> dims;

  unsigned long  imgDataLen;
  unsigned long  numPix;
  unsigned short bytesPerPix;

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

  friend class RtDataImage;
};


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


