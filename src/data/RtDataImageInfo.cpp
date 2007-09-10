/******************************************************************************
 * RtDataImageInfo.h declares a class for an MR image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#include"RtDataImageInfo.h"


// deep copy constructor
RtDataImageInfo::RtDataImageInfo(const RtDataImageInfo &info) {
  (*this) = info;

  // deep copy matrices
  gsl_matrix_memcpy(vxl2ras, info.vxl2ras);
  gsl_matrix_memcpy(ras2ref, info.ras2ref);
}

// copy constructor accepting a siemens ExternalImageInfo struct
RtDataImageInfo::RtDataImageInfo(const RtExternalImageInfo &info) :
  bytesPerPix(sizeof(unsigned short)), minMaxSet(false), minVal(0), maxVal(0) {

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


// print info members to cout 
void RtDataImageInfo::print() {
  print(cout);
}

// print some of the info members
void RtDataImageInfo::print(ostream &os) {
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
ACE_Date_Time RtDataImageInfo::siemensTime2ACE_Date_Time(const char *tc) {
  string timeStr = tc;
  ACE_Date_Time t;
  t.hour(atol(timeStr.substr(0,2).c_str()));
  t.minute(atol(timeStr.substr(2,2).c_str()));
  t.second(atol(timeStr.substr(4,2).c_str()));
  t.microsec(atol(timeStr.substr(7,6).c_str()));
  return t;
}

// convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
string RtDataImageInfo::ACE_Date_Time2SiemensTime(const ACE_Date_Time &t) {
  char str[] = "hhmmss.xxxxxx";
  sprintf(str,"%02ld%02ld%02ld.%06ld", 
	  t.hour(), t.minute(), t.second(), t.microsec());

  string s(str);
  return s;
}
