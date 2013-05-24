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
#ifndef RtExternalImageInfo_h
#define RtExternalImageInfo_h

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

typedef float  float32_t;
typedef double float64_t;

using namespace std;

const char EXTERNALSENDER_MAGIC[5] = "ERTI";  // ERTI = External RealTime Info
const int EXTERNALSENDER_VERSION = 4;

struct RtExternalImageInfo {
 public:
  RtExternalImageInfo();

  void displayImageInfo() const;     // print info about a received image
  void* serialize() const;           // create header for sending an image
  static size_t getHeaderSize();
  int32_t getBytesPerPix() const;
  int32_t getDataSize() const;
  int32_t getNumVoxels() const;
  int32_t getMosaicSize() const;

  char       magic[5];       // Must be "ERTI"
  int32_t    headerVersion;  // Version of this header
  char       seriesUID[64];  // The DICOM unique ID of this series
  char       scanType[64];   // Type of scan, e.g., "MPRAGE" or "EPI"
  char       imageType[16];  // Valid types: {"2Dz", "2Dzt", "3D", "3Dt"}
                             // 2Dz: Single volume, 1 slice at a time
                             // 2Dzt: Multi-volume, 1 slice at a time
                             // 3D: Single volume all at once
                             // 3Dt: Multi-volume, 1 volume at a time
  char       note[256];      // Text. Suggested contents: scanner make &
                             //   model, pulse sequence name, version of
                             //   image-sending software used, etc.
  char       dataType[16];   // Of image data. Valid (OS-independent) types:
                             // - char   , c_char   , uchar   , c_uchar
                             // - int16_t, c_int16_t, uint16_t, c_uint16_t
                             // - int32_t, c_int32_t, uint32_t, c_uint32_t
                             // - float32_t, c_float32_t, float64_t, c_float64_t
  bool       isLittleEndian; // true if image data uses little endian byte order
  bool       isMosaic;       // true if 3D or 3Dt images are mosaiced.

  float64_t  pixelSpacingReadMM;  // Pixel dimension (mm) in each
  float64_t  pixelSpacingPhaseMM; //   acquisition direction: read, phase,
  float64_t  pixelSpacingSliceMM; //   & slice.
  float64_t  sliceGapMM;          // Gap between slice excitations (mm)

  int32_t    numPixelsRead;   // Number of pixels in read-direction
  int32_t    numPixelsPhase;  // Number of pixels in phase-direction
  int32_t    numSlices;       // Number of slices

  float32_t  voxelToWorldMatrix[4][4];  // Voxel indices -> world coordinates

  int32_t    repetitionTimeMS;  // Time (ms) of one volume acquisition
  int32_t    repetitionDelayMS; // Time (ms) between acquisitions
  int32_t    currentTR;         // Current TR number
  int32_t    totalTR;           // Expected number of TRs

  bool       isMotionCorrected; // True if this data has MC applied
  char       mcOrder[5];        // Order of MC operations. Eg "TZYX" or "YXZT"
                                //   T: Translation
                                //   X, Y, Z: Rotation in X, Y, Z

  float64_t  mcTranslationXMM;  // Translation (mm) that was applied
  float64_t  mcTranslationYMM;  //   by motion correction along each
  float64_t  mcTranslationZMM;  //   axis

  float64_t  mcRotationXRAD;    // Rotation (radians) that was applied
  float64_t  mcRotationYRAD;    //   by motion correction along each
  float64_t  mcRotationZRAD;    //   axis

};
#endif
