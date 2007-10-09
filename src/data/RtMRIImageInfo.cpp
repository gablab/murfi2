/******************************************************************************
 * RtMRIImageInfo.h declares a class for an MR image info
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-08
 * 
 *****************************************************************************/

#include"RtMRIImageInfo.h"


// deep copy constructor
RtMRIImageInfo::RtMRIImageInfo(const RtMRIImageInfo &info) {
  (*this) = info;

  // deep copy matrices
  gsl_matrix_memcpy(vxl2ras, info.vxl2ras);
  gsl_matrix_memcpy(ras2ref, info.ras2ref);
}

// copy constructor accepting a siemens ExternalImageInfo struct
RtMRIImageInfo::RtMRIImageInfo(const RtExternalImageInfo &info) :
  minMaxSet(false), minVal(0), maxVal(0) {

  ACE_TRACE(("RtMRIImageInfo::RtMRIImageInfo"));

  bytesPerPix = sizeof(unsigned short);

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

// print some of the info members
void RtMRIImageInfo::print(ostream &os) {
  ACE_TRACE(("RtMRIImageInfo::print"));

  int wid = 30;

  os << setiosflags(ios::left);

  RtDataImageInfo::print(os);

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
