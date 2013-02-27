/*=========================================================================
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

#include <RtExternalSenderImageInfo.h>
#include <math.h>

RtExternalImageInfo::RtExternalImageInfo()
    :
    headerVersion(EXTERNALSENDER_VERSION),
    isLittleEndian(false),
    isMosaic(false),
    pixelSpacingReadMM(0.0),
    pixelSpacingPhaseMM(0.0),
    pixelSpacingSliceMM(0.0),
    sliceGapMM(0.0),      
    numPixelsRead(0),
    numPixelsPhase(0),
    numSlices(0),
    repetitionTimeMS(0),
    repetitionDelayMS(0),
    currentTR(0),
    totalTR(0),
    isMotionCorrected(false),
    mcTranslationXMM(0.0),
    mcTranslationYMM(0.0),
    mcTranslationZMM(0.0),
    mcRotationXRAD(0.0),
    mcRotationYRAD(0.0),
    mcRotationZRAD(0.0) {
  strcpy((char*) magic, EXTERNALSENDER_MAGIC);
  scanType[0] = '\0';
  strcpy((char*) imageType,EXTERNALSENDER_IMGTYPE);
  note[0] = '\0';
  datatype[0] = '\0';
  voxelToWorldMatrix[0][0] = '\0';
  mcOrder[0] = '\0';
}

// constructor for data from physical scanner
//  in
//   data: byte data to read the structure from
//   len:  number of bytes we have
RtExternalImageInfo::RtExternalImageInfo(char *data, unsigned int len) {
  char *readptr = data;

  // test that we have enough data
  if(len < getHeaderSize()) {
    return;
  }
  
  memcpy(magic, readptr, 5*sizeof(char));
  readptr += 5*sizeof(char);
  
  memcpy(&headerVersion, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(scanType, readptr, 64*sizeof(char));
  readptr += 64*sizeof(char);
  
  memcpy(imageType, readptr, 16*sizeof(char));
  readptr += 16*sizeof(char);
  
  memcpy(note, readptr, 256*sizeof(char));
  readptr += 256*sizeof(char);
  
  memcpy(datatype, readptr, 16*sizeof(char));
  readptr += 16*sizeof(char);
  
  memcpy(&isLittleEndian, readptr, sizeof(bool));
  readptr += sizeof(bool);
  
  memcpy(&isMosaic, readptr, sizeof(bool));
  readptr += sizeof(bool);
  
  memcpy(&pixelSpacingReadMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&pixelSpacingPhaseMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
	
  memcpy(&pixelSpacingSliceMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&sliceGapMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
	
  memcpy(&numPixelsRead, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&numPixelsPhase, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&numSlices, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);

  for (int ii=0; ii<4; ii++) {
    for (int jj=0; jj<4; jj++) {
      memcpy(&voxelToWorldMatrix[ii][jj], readptr, sizeof(float32_t));
      readptr += sizeof(float32_t);
    }
  }
  
  memcpy(&repetitionTimeMS, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&repetitionDelayMS, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&currentTR, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&totalTR, readptr, sizeof(int32_t));
  readptr += sizeof(int32_t);
  
  memcpy(&isMotionCorrected, readptr, sizeof(bool));
  readptr += sizeof(bool);
  
  memcpy(mcOrder, readptr, 5*sizeof(char));
  readptr += 5*sizeof(char);
  
  memcpy(&mcTranslationXMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&mcTranslationYMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&mcTranslationZMM, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&mcRotationXRAD, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&mcRotationYRAD, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
  
  memcpy(&mcRotationZRAD, readptr, sizeof(float64_t));
  readptr += sizeof(float64_t);
}

//  Dumps all fields to STDOUT
void RtExternalImageInfo::displayImageInfo() const {
  cout << "-----------------------------" << endl;
  cout << "Dumping RtExternalImageInfo         " << endl;
  cout << "Data:"  << sizeof (RtExternalImageInfo) << " Bytes used" << endl;
  cout << "-----------------------------" << endl;
  cout << "Functor Code / Header Version:          " << magic << " / " << headerVersion << endl
       << "scanType:                               " << scanType << endl
       << "imageType / isMosaic:                   " << imageType << " / " << isMosaic << endl
       << "datatype / isLittleEndian:              " << datatype << " / " << isLittleEndian << endl
       << "note:                                   " << note << endl
       << "numPixels*:  Read / Phase / Slice       " << numPixelsRead << " / " << numPixelsPhase << " / " << numSlices << endl
       << "pixelSpacing*MM:  Read / Phase          " << pixelSpacingReadMM << " / " << pixelSpacingPhaseMM << endl
       << "Slice Thickness / Gap (mm):             " << pixelSpacingSliceMM << " / " << sliceGapMM << endl
       << "isMotionCorrected / mcOrder:            " << isMotionCorrected << " / " << mcOrder << endl
       << "mcTranslation*MM:  X / Y / Z            " << mcTranslationXMM << " / " << mcTranslationYMM << " / " << mcTranslationZMM << endl
       << "mcRotation*MM:  X / Y / Z               " << mcRotationXRAD << " / " << mcRotationYRAD << " / " << mcRotationZRAD << endl
       << "repetitionTimeMS / repetitionDelayMS    " << repetitionTimeMS << " / " << repetitionDelayMS << endl
       << "currentTR / totalTR                     " << currentTR << " / " << totalTR << endl
       << "voxelToWorldMatrix                      " << voxelToWorldMatrix[0][0] << " " << voxelToWorldMatrix[0][1] << " " << voxelToWorldMatrix[0][2] << " " << voxelToWorldMatrix[0][3] << endl    
       << "                                        " << voxelToWorldMatrix[1][0] << " " << voxelToWorldMatrix[1][1] << " " << voxelToWorldMatrix[1][2] << " " << voxelToWorldMatrix[1][3] << endl    
       << "                                        " << voxelToWorldMatrix[2][0] << " " << voxelToWorldMatrix[2][1] << " " << voxelToWorldMatrix[2][2] << " " << voxelToWorldMatrix[2][3] << endl    
       << "                                        " << voxelToWorldMatrix[3][0] << " " << voxelToWorldMatrix[3][1] << " " << voxelToWorldMatrix[3][2] << " " << voxelToWorldMatrix[3][3] << endl;    
}

// Returns the size (in bytes) of the header
int32_t RtExternalImageInfo::getHeaderSize() const {
	return (int32_t)sizeof(RtExternalImageInfo);
}

// Returns the size (in bytes) per pixel, specified by datatype
int32_t RtExternalImageInfo::getBytesPerPix() const {
	if (strcmp(datatype, "int16_t\0")) {
		return sizeof(int16_t);
	} else {
		cout << "WARNING!!! TODO! assuming datatype is int16_t" << endl; // TODO(murfidev) fill in other supported datatypes
		return sizeof(int16_t);
	}
}

// Returns the size (in bytes) of the data blob
int32_t RtExternalImageInfo::getDataSize() const {
	return getBytesPerPix()*getNumVoxels();
}

// Returns the size (in voxels) of the data blob
int32_t RtExternalImageInfo::getNumVoxels() const {
	if (isMosaic)
		// If it's mosaiced then the number of voxels in the data blob
		// is potentially larger than the number of voxels in the volume.
		// Example: a 72x72x31 volume has 160704 voxels, but creates a 6x6
		// (ceil(sqrt(numSlices))) mosaic, and therefore sends 72x72x36 = 186624
		// voxels
		return (int32_t) numPixelsRead*numPixelsPhase*pow(getMosaicSize(),2);
	else
		return (int32_t) numPixelsRead*numPixelsPhase*numSlices;
}

// Returns mosaic grid size (in mosaic cells)
int32_t RtExternalImageInfo::getMosaicSize() const {
  if (isMosaic)
    return (int32_t) ceil(sqrt((double)numSlices));
  else
    return 1;
}

void* RtExternalImageInfo::serialize() const {
	char *data = (char*) calloc(getHeaderSize(),1);
	char *writeptr = data;
	
	memcpy(writeptr, magic, 5*sizeof(char));
    writeptr += 5*sizeof(char);

	memcpy(writeptr, &headerVersion, sizeof(int32_t));
    writeptr += sizeof(int32_t);
	
	memcpy(writeptr, scanType, 64*sizeof(char));
    writeptr += 64*sizeof(char);

	memcpy(writeptr, imageType, 16*sizeof(char));
    writeptr += 16*sizeof(char);
	
	memcpy(writeptr, note, 256*sizeof(char));
    writeptr += 256*sizeof(char);
	
	memcpy(writeptr, datatype, 16*sizeof(char));
    writeptr += 16*sizeof(char);
	
	memcpy(writeptr, &isLittleEndian, sizeof(bool));
    writeptr += sizeof(bool);

	memcpy(writeptr, &isMosaic, sizeof(bool));
    writeptr += sizeof(bool);
	
	memcpy(writeptr, &pixelSpacingReadMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &pixelSpacingPhaseMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &pixelSpacingSliceMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &sliceGapMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &numPixelsRead, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	memcpy(writeptr, &numPixelsPhase, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	memcpy(writeptr, &numSlices, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	for (int ii=0; ii<4; ii++) {
		for (int jj=0; jj<4; jj++) {
			memcpy(writeptr, &voxelToWorldMatrix[ii][jj], sizeof(float32_t));
			writeptr += sizeof(float32_t);
		}
	}

	memcpy(writeptr, &repetitionTimeMS, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	memcpy(writeptr, &repetitionDelayMS, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	memcpy(writeptr, &currentTR, sizeof(int32_t));
	writeptr += sizeof(int32_t);

	memcpy(writeptr, &totalTR, sizeof(int32_t));
	writeptr += sizeof(int32_t);
	
	memcpy(writeptr, &isMotionCorrected, sizeof(bool));
    writeptr += sizeof(bool);
	
	memcpy(writeptr, mcOrder, 5*sizeof(char));
    writeptr += 5*sizeof(char);
	
	memcpy(writeptr, &mcTranslationXMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &mcTranslationYMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &mcTranslationZMM, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &mcRotationXRAD, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &mcRotationYRAD, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	memcpy(writeptr, &mcRotationZRAD, sizeof(float64_t));
	writeptr += sizeof(float64_t);
	
	return (void*)data;
}
