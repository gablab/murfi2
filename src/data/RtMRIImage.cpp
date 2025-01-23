/*=========================================================================
 *  RtMRIImage.h declares a class for an MR image
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include"RtMRIImage.h"

#include"RtExperiment.h"
#include"RtDataIDs.h"

#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;

// default constructor
RtMRIImage::RtMRIImage() : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()"));

  dataID.setModuleID("mri");

  elType = RT_SHORT_TYPE;

  magicNumber = MAGIC_NUMBER;
}

// construct from an image info struct and some byte data
RtMRIImage::RtMRIImage(RtExternalImageInfo &extinfo, short *bytes)
    : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtExternalImageInfo,short*)"));

  magicNumber = MAGIC_NUMBER;

  elType = RT_SHORT_TYPE;

  setInfo(extinfo);

  // fill fields of data id
  dataID.setModuleID(ID_SCANNERIMG);
  dataID.setDataName(NAME_SCANNERIMG_EPI);
  dataID.setSeriesNum(getSeriesNumFromUID(extinfo.seriesUID));
  dataID.setTimePoint(extinfo.currentTR);

  // allocate and copy the img data
  if(DEBUG_LEVEL & ALLOC) {
    cerr << "mr1 allocating data for " << this << endl; cerr.flush();
  }

  data = new short[numPix];

  if(bytes != NULL) {
    memcpy(data,bytes,imgDataLen);
  }
}

// construct from another image (deep copy)
// only use this with like datatypes
RtMRIImage::RtMRIImage(RtMRIImage &img) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImage)"));

  elType = RT_SHORT_TYPE;

  (*this) = img;

  magicNumber = MAGIC_NUMBER;

  // copy the data
  if(DEBUG_LEVEL & ALLOC) {
    cerr << "mr2 allocating data for " << this << endl; cerr.flush();
  }

  data = new short[numPix];
  memcpy(data, img.data, imgDataLen);
}

// construct by reading a nifti file
RtMRIImage::RtMRIImage(const string& filename) :
  RtDataImage<short>(filename) {}

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
  os.write((char*) &readFOV,    sizeof(double));
  os.write((char*) &phaseFOV,   sizeof(double));
  os.write((char*) &sliceThick, sizeof(double));

  char boolcon;

  unsigned int uint_tmp;

  uint_tmp = dataID.getSeriesNum();
  os.write((char*) uint_tmp, sizeof(unsigned int));

  uint_tmp = dataID.getTimePoint();
  os.write((char*) uint_tmp, sizeof(unsigned int));

  os.write((char*) &tr,             sizeof(double));

  char t[] = "hhmmss.xxxxxx";
  string tm = ACE_Date_Time2TimeStr(time);
  strcpy(t,tm.c_str());
  os.write(t, TIMESTR_LEN*sizeof(char));

  tm = ACE_Date_Time2TimeStr(refFrameTime);
  strcpy(t,tm.c_str());
  os.write(t, TIMESTR_LEN*sizeof(char));

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
  is.read((char*) &readFOV,    sizeof(double));
  is.read((char*) &phaseFOV,   sizeof(double));
  is.read((char*) &sliceThick, sizeof(double));

  unsigned int uint_tmp = 0;

  is.read((char*) uint_tmp, sizeof(unsigned int));
  dataID.setSeriesNum(uint_tmp);

  is.read((char*) uint_tmp, sizeof(unsigned int));
  dataID.setTimePoint(uint_tmp);

  is.read((char*) &tr,             sizeof(double));

  char t[] = "hhmmss.xxxxxx";

  is.read(t, TIMESTR_LEN*sizeof(char));
  time = timeStr2ACE_Date_Time(t);

  refFrameTime = timeStr2ACE_Date_Time(t);

  bool boolcon;
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

  os << setw(wid) << "readFOV phaseFOV" << readFOV << " " << phaseFOV << endl
     << setw(wid) << "sliceThick" << sliceThick << endl
     << setw(wid) << "acqNum" << dataID.getTimePoint() << endl
     << setw(wid) << "tr" << tr << endl
     << setw(wid) << "refFrameTime" << ACE_Date_Time2TimeStr(refFrameTime)
     << setw(wid) << "moco" << moco << endl
     << setw(wid) << "fromScanner" << fromScanner << endl
     << setw(wid) << "MatrixSize" << getMatrixSize() << endl
     << setw(wid) << "NumSlices" << getNumSlices() << endl
     << "---------------------------" << endl;
}

// destructor
RtMRIImage::~RtMRIImage() {
  ACE_TRACE(("RtMRIImage::~RtMRIImage"));
}

// set info struct
//  in
//   _info: struct to copy
void RtMRIImage::setInfo(const RtExternalImageInfo &info) {

  if (!info.isMosaic) {
    // volume is not mosaiced
    dims.resize(3);
    dims[0] = info.numPixelsRead;
    dims[1] = info.numPixelsPhase;
    dims[2] = info.numSlices;
  } else {
    // determine the dimensions and voxel size
    dims.resize(2);
    dims[0] = info.numPixelsRead*info.getMosaicSize();
    dims[1] = info.numPixelsPhase*info.getMosaicSize();
  }

  pixdims.resize(3);
  pixdims[0] = info.pixelSpacingReadMM;
  pixdims[1] = info.pixelSpacingPhaseMM;
  pixdims[2] = info.pixelSpacingSliceMM + info.sliceGapMM;

  // calculate image size
  imgDataLen = bytesPerPix;
  numPix = 1;
  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    imgDataLen *= (*i);
    numPix *= (*i);
  }

  // scaling matrix
  vnl_matrix_fixed<double,4,4> scaleMat;
  scaleMat.set_identity();
  scaleMat.put(0,0, pixdims[0]);
  scaleMat.put(1,1, pixdims[1]);
  scaleMat.put(2,2, pixdims[2]);

  vxl2ras.set_identity();
  vxl2ras.put(0,0, info.voxelToWorldMatrix[0][0]);
  vxl2ras.put(1,0, info.voxelToWorldMatrix[1][0]);
  vxl2ras.put(2,0, info.voxelToWorldMatrix[2][0]);

  vxl2ras.put(0,1, info.voxelToWorldMatrix[0][1]);
  vxl2ras.put(1,1, info.voxelToWorldMatrix[1][1]);
  vxl2ras.put(2,1, info.voxelToWorldMatrix[2][1]);

  vxl2ras.put(0,2, info.voxelToWorldMatrix[0][2]);
  vxl2ras.put(1,2, info.voxelToWorldMatrix[1][2]);
  vxl2ras.put(2,2, info.voxelToWorldMatrix[2][2]);

  vxl2ras.put(0,3, info.voxelToWorldMatrix[0][3]);
  vxl2ras.put(1,3, info.voxelToWorldMatrix[1][3]);
  vxl2ras.put(2,3, info.voxelToWorldMatrix[2][3]);

  // build RAS 2 REF transformation matrix
  // TODO set this properly
  ras2ref.set_identity();

  // image info
  readFOV =  info.numPixelsRead*info.pixelSpacingReadMM;
  phaseFOV = info.numPixelsPhase*info.pixelSpacingPhaseMM;
  matrixSize = info.numPixelsPhase;
  numSlices = info.numSlices;
  sliceThick = info.pixelSpacingSliceMM;
  sliceGap = info.sliceGapMM;
  seriesInstanceUID = info.seriesUID;

  dataID.setTimePoint(info.currentTR);
  tr = info.repetitionTimeMS;
  moco = info.isMotionCorrected;
  mcTranslationXMM = info.mcTranslationXMM;
  mcTranslationYMM = info.mcTranslationYMM;
  mcTranslationZMM = info.mcTranslationZMM;
  mcRotationXDeg = info.mcRotationXDeg;
  mcRotationYDeg = info.mcRotationYDeg;
  mcRotationZDeg = info.mcRotationZDeg;

  fromScanner = !strcmp(info.magic, EXTERNALSENDER_MAGIC);
}

int RtMRIImage::getTotalRepetitions() const {
  return totalRepetitions;
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

// get whether this is a moco volume
bool RtMRIImage::getMoco() {
  return moco;
}

// get motion parameters
float64_t RtMRIImage::getTranslationX() {
  return mcTranslationXMM;
}

float64_t RtMRIImage::getTranslationY() {
  return mcTranslationYMM;
}

float64_t RtMRIImage::getTranslationZ() {
  return mcTranslationZMM;
}

float64_t RtMRIImage::getRotationX() {
  return mcRotationXDeg;
}

float64_t RtMRIImage::getRotationY() {
  return mcRotationYDeg;
}

float64_t RtMRIImage::getRotationZ() {
  return mcRotationZDeg;
}
