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

RtExternalImageInfo::RtExternalImageInfo() :
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
  imageType[0] = '\0';
  note[0] = '\0';
  dataType[0] = '\0';

  // identity
  memset(voxelToWorldMatrix, 0, sizeof(voxelToWorldMatrix));
  for(int i = 0; i < 4; i++) {
    voxelToWorldMatrix[i][i] = 1.;
  }

  mcOrder[0] = '\0';
}

//  Dumps all fields to STDOUT
void RtExternalImageInfo::displayImageInfo() const {
  cout << "-----------------------------" << endl;
  cout << "Dumping RtExternalImageInfo         " << endl;
  cout << "Data:"  << sizeof (RtExternalImageInfo) << " Bytes used" << endl;
  cout << "-----------------------------" << endl;
  cout << "Functor Code / Header Version:          "
       << magic << " / " << headerVersion << endl
       << "scanType:                               "
       << scanType << endl
       << "imageType / isMosaic:                   "
       << imageType << " / " << isMosaic << endl
       << "dataType / isLittleEndian:              "
       << dataType << " / " << isLittleEndian << endl
       << "note:                                   "
       << note << endl
       << "numPixels[Read / Phase / Slice]:        "
       << numPixelsRead << " / " << numPixelsPhase << " / " << numSlices << endl
       << "pixelSpacing[Read / Phase]:             "
       << pixelSpacingReadMM << " / " << pixelSpacingPhaseMM << endl
       << "Slice [Thickness / Gap]:                "
       << pixelSpacingSliceMM << " / " << sliceGapMM << endl
       << "isMotionCorrected / mcOrder:            "
       << isMotionCorrected << " / " << mcOrder << endl
       << "mcTranslation[X / Y / Z]:               "
       << mcTranslationXMM << " / " << mcTranslationYMM << " / "
       << mcTranslationZMM << endl
       << "mcRotationMM[X / Y / Z]:                "
       << mcRotationXRAD << " / " << mcRotationYRAD << " / "
       << mcRotationZRAD << endl
       << "repetitionTimeMS / repetitionDelayMS    "
       << repetitionTimeMS << " / " << repetitionDelayMS << endl
       << "currentTR / totalTR                     "
       << currentTR << " / " << totalTR << endl
       << "voxelToWorldMatrix                      "
       << voxelToWorldMatrix[0][0] << " " << voxelToWorldMatrix[0][1] << " "
       << voxelToWorldMatrix[0][2] << " " << voxelToWorldMatrix[0][3] << endl
       << "                                        "
       << voxelToWorldMatrix[1][0] << " " << voxelToWorldMatrix[1][1] << " "
       << voxelToWorldMatrix[1][2] << " " << voxelToWorldMatrix[1][3] << endl
       << "                                        "
       << voxelToWorldMatrix[2][0] << " " << voxelToWorldMatrix[2][1] << " "
       << voxelToWorldMatrix[2][2] << " " << voxelToWorldMatrix[2][3] << endl
       << "                                        "
       << voxelToWorldMatrix[3][0] << " " << voxelToWorldMatrix[3][1] << " "
       << voxelToWorldMatrix[3][2] << " " << voxelToWorldMatrix[3][3] << endl;
}

// Returns the size (in bytes) of the header
size_t RtExternalImageInfo::getHeaderSize() {
  return sizeof(RtExternalImageInfo);
}

// Returns the size (in bytes) per pixel, specified by dataType
int32_t RtExternalImageInfo::getBytesPerPix() const {
  if (strcmp(dataType, "int16_t\0")) {
    return sizeof(int16_t);
  } else {
    cout << "WARNING!!! TODO! assuming dataType is int16_t" << endl; // TODO(murfidev) fill in other supported dataTypes
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
