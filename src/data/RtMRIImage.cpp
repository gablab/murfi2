/******************************************************************************
 * RtMRIImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 * 
 *****************************************************************************/

#include"RtMRIImage.h"
#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;
  
// default constructor
RtMRIImage::RtMRIImage() : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()")); 
  addToID("mri");
  magicNumber = MAGIC_NUMBER;
}

// constructor that accepts a filename to read an image from
//RtMRIImage::RtMRIImage(const string &filename) {
//  ACE_TRACE(("RtMRIImage::RtMRIImage(string)"));
//  addToID("mri");
//  
//  data = NULL;
//
//  info.setBytesPerPix(sizeof(short));
//
//  read(filename);
//}
//
//// construct from raw bytes sent by RtInputScannerImages
//// BE CAREFUL WITH THIS
//RtMRIImage::RtMRIImage(char *bytes, unsigned int len) 
//    : RtDataImage<short>(bytes,len) {
//  ACE_TRACE(("RtMRIImage::RtMRIImage(char*,unsigned int)"));
//
//  addToID("mri");
//}

// construct from an image info struct and some byte data
RtMRIImage::RtMRIImage(RtExternalImageInfo &extinfo, short *bytes) 
    : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtExternalImageInfo,short*)"));

  addToID("mri");
  magicNumber = MAGIC_NUMBER;

  setInfo(extinfo);

  // allocate and copy the img data
  data = new short[numPix];

  if(bytes != NULL) {
    memcpy(data,bytes,imgDataLen);
  }
}


//// construct from an image info struct and (possibly blank) data
//RtMRIImage::RtMRIImage(RtMRIImageInfo &_info, short  *_data) 
//    : RtDataImage<short>() {
//  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImageInfo,T*)"));
//  addToID("mri");
//  magicNumber = MAGIC_NUMBER;
//
//  setImage(_info, _data);
//}

// construct from another image (deep copy)
// only use this with like datatypes
RtMRIImage::RtMRIImage(RtMRIImage &img) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImage)"));
  
  (*this) = img;

  magicNumber = MAGIC_NUMBER;

  // copy the data 
  data = new short[numPix];
  memcpy(data, img.data, imgDataLen);
}

// set this image based on a passed image info and data
//  in
//   info: struct
//   data: array (optional, image data will be  allocated and set
//         to all zeros if null) 
//void RtMRIImage::setImage(RtMRIImageInfo &_info, short *_data) {
//  info = _info;
//  data = new short[info.numPix];
//
//  if(_data != NULL) {
//    memcpy(data, _data, info.imgDataLen);
//  }
//  else {
//    for(unsigned int i = 0; i < info.numPix; i++) {
//      data[i] = 0;
//    }
//  }
//}

// write the info (all but data) to a stream
//  in
//   stream to write to
//  out
//   success or failure
bool RtMRIImage::writeInfo(ostream &os) {
  if(os.fail()) return false;

  if(!RtDataImage<short>::writeInfo(os)) {
    return false;
  }

  // write mri fields
  os.write((char*) &slice,      sizeof(unsigned long));
  os.write((char*) &readFOV,    sizeof(double));
  os.write((char*) &phaseFOV,   sizeof(double));
  os.write((char*) &sliceThick, sizeof(double));

  char boolcon;
  
  boolcon = (char) swapReadPhase;
  os.write((char*) &boolcon, sizeof(char));
  
  os.write((char*) &seriesNum, sizeof(unsigned int));
  os.write((char*) &acqNum, sizeof(unsigned int));

  os.write((char*) &timeAfterStart, sizeof(double));
  os.write((char*) &te,             sizeof(double));
  os.write((char*) &tr,             sizeof(double));
  os.write((char*) &ti,             sizeof(double));
  os.write((char*) &triggerTime,    sizeof(double));

  char t[] = "hhmmss.xxxxxx";
  string tm = ACE_Date_Time2SiemensTime(time);
  strcpy(t,tm.c_str());
  os.write(t, SIEMENS_TIMESTR_LEN*sizeof(char));

  tm = ACE_Date_Time2SiemensTime(refFrameTime);
  strcpy(t,tm.c_str());
  os.write(t, SIEMENS_TIMESTR_LEN*sizeof(char));

  os.write((char*) &reconDelay, sizeof(double));

  boolcon = (char) distCorrect2D;
  os.write((char*) &boolcon, sizeof(char));

  boolcon = (char) moco;
  os.write((char*) &boolcon, sizeof(char));

  boolcon = (char) fromScanner;
  os.write((char*) &boolcon, sizeof(char));

  return os.good();
}

// read the info (all but data) from a stream
//  in
//   stream to read from
//  out
//   success or failure
bool RtMRIImage::readInfo(istream &is) {
  if(is.fail()) return false;

  if(!RtDataImage<short>::readInfo(is)) {
    return false;
  }

  // read mri fields
  is.read((char*) &slice,      sizeof(unsigned long));
  is.read((char*) &readFOV,    sizeof(double));
  is.read((char*) &phaseFOV,   sizeof(double));
  is.read((char*) &sliceThick, sizeof(double));

  char boolcon;
  
  is.read((char*) &boolcon, sizeof(char));
  swapReadPhase = (bool) boolcon;
  
  is.read((char*) &seriesNum, sizeof(unsigned int));
  is.read((char*) &acqNum, sizeof(unsigned int));

  is.read((char*) &timeAfterStart, sizeof(double));
  is.read((char*) &te,             sizeof(double));
  is.read((char*) &tr,             sizeof(double));
  is.read((char*) &ti,             sizeof(double));
  is.read((char*) &triggerTime,    sizeof(double));

  char t[] = "hhmmss.xxxxxx";

  is.read(t, SIEMENS_TIMESTR_LEN*sizeof(char));
  time = siemensTime2ACE_Date_Time(t);

  is.read(t, SIEMENS_TIMESTR_LEN*sizeof(char));
  refFrameTime = siemensTime2ACE_Date_Time(t);

  is.read((char*) &reconDelay, sizeof(double));

  is.read((char*) &boolcon, sizeof(char));
  distCorrect2D = (bool) boolcon;

  is.read((char*) &boolcon, sizeof(char));
  moco = (bool) boolcon;

  is.read((char*) &boolcon, sizeof(char));
  fromScanner = (bool) boolcon;

  return is.good();
}


// print info about this image
void RtMRIImage::printInfo(ostream &os) {
  if(os.fail()) return;

  int wid = 30;

  os << setiosflags(ios::left);

  os << setw(wid) << "slice" << slice << endl 
     << setw(wid) << "readFOV phaseFOV" 
     << readFOV << " " << phaseFOV << endl
     << setw(wid) << "sliceThick" << sliceThick << endl
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


// destructor
RtMRIImage::~RtMRIImage() {
  ACE_TRACE(("RtMRIImage::~RtMRIImage"));

  // notify our locker that we are being deleted
  if(lock != NULL) {
    lock->beingDeleted();
  }

  // don't need this cause our parent will delete the data
//  if(data != NULL) {
//    delete [] data;
//    data = NULL;
//  }
}



// get the acquisition number
string RtMRIImage::getCreationTime() const {
  return ACE_Date_Time2SiemensTime(creationTime);
}

// get the acquisition number
unsigned int RtMRIImage::getAcquisitionNum() const {
  return acqNum;
}

// get the series number
unsigned int RtMRIImage::getSeriesNum() const {
  return seriesNum;
}


// DEBUGGGING
#include"printVnl44Mat.cpp"


// set info struct
//  in
//   _info: struct to copy
void RtMRIImage::setInfo(const RtExternalImageInfo &info) {

  // determine the dimensions and voxel size
  dims.resize(2);
  dims[0] = info.nLin*info.iMosaicGridSize;
  dims[1] = info.nCol*info.iMosaicGridSize;

  pixdims.resize(3);
  pixdims[0] = info.dFOVread / info.nLin;
  pixdims[1] = info.dFOVphase / info.nCol;

  // calculate image size
  imgDataLen = bytesPerPix;
  numPix = 1;
  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    imgDataLen *= (*i);
    numPix *= (*i);
  }

  // set geometry info
  setPixDim(0,info.dFOVread/info.nLin);
  setPixDim(1,info.dFOVphase/info.nCol);
  setPixDim(2,info.dThick * sliceGap);

  // build xform (no translations included yet, just scales and rotations)

  // scaling matrix
  vnl_matrix_fixed<double,4,4> scaleMat;
  scaleMat.set_identity();
  scaleMat.put(0,0, -info.dFOVread/info.nLin);
  scaleMat.put(1,1, -info.dFOVphase/info.nCol);
  scaleMat.put(2,2,  info.dThick * (1+sliceGap));

  // rotation matrix
  vnl_matrix_fixed<double,4,4> rotMat;
  rotMat.set_identity();

  rotMat.put(0,0, info.dRowSag);
  rotMat.put(0,1, info.dRowCor);
  rotMat.put(0,2, info.dRowTra);

  rotMat.put(1,0, info.dColSag);
  rotMat.put(1,1, info.dColCor);
  rotMat.put(1,2, info.dColTra);

  rotMat.put(2,0, info.dNorSag);
  rotMat.put(2,1, info.dNorCor);
  rotMat.put(2,2, info.dNorTra);

  vxl2ras = scaleMat*rotMat;

  // debugging
//  cout << "scale" << endl;
//  printVnl44Mat(scaleMat);
//
//  cout << "rot" << endl;
//  printVnl44Mat(rotMat);
//
//  cout << "xform" << endl;
//  printVnl44Mat(vxl2ras);

  // build RAS 2 REF transformation matrix
  ras2ref.set_identity();

  // image info
  slice = info.lSliceIndex;
  readFOV = info.dFOVread;
  phaseFOV = info.dFOVphase;
  matrixSize = info.nCol;
  numSlices = info.iNoOfImagesInMosaic;
  sliceThick = info.dThick;
  seriesInstanceUID = info.cSeriesInstanceUID;

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
  moco = info.bIsMoCo;  

  // received data parms
  fromScanner = info.iDataSource == 0;
}

// set the matrix size
void RtMRIImage::setMatrixSize(unsigned int ms) {
  matrixSize = ms;
}

// get the matrix size
unsigned int RtMRIImage::getMatrixSize() {
  return matrixSize;
}

// set the matrix size
void RtMRIImage::setNumSlices(unsigned int ns) {
  numSlices = ns;
}

// get the matrix size
unsigned int RtMRIImage::getNumSlices() {
  return numSlices;
}

// get a smart contrast level
float RtMRIImage::getAutoContrast() {
  ACE_TRACE(("RtMRIImage::getAutoContrast"));
  
  if(!minMaxSet) {
    setMinMax();
  }

  return SHRT_MAX/(float) maxVal;
}


// get a smart brightness level
float RtMRIImage::getAutoBrightness() {
  ACE_TRACE(("RtMRIImage::getAutoBrightness"));

  if(!minMaxSet) {
    setMinMax();
  }

  return (float) minVal;

}


// set the series number
void RtMRIImage::setSeriesNum(unsigned int sn) {
  seriesNum = sn;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


