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

#ifndef RTMRIIMAGE_H
#define RTMRIIMAGE_H

#include<vector>
#include<iostream>
#include"ace/Date_Time.h"

#include"RtDataImage.h"
#include"RtMotion.h"
#include"RtExternalSenderImageInfo.h"

#include"debug_levels.h"

using namespace std;

class RtMRIImage : public RtDataImage<short> {

 public:

  const static unsigned int MAGIC_NUMBER = 0x5081;

  //*** constructors/destructors  ***//

  RtMRIImage();
  virtual ~RtMRIImage();

  // construct from an external image info struct and some image data
  RtMRIImage(RtExternalImageInfo &info, short *data = NULL);

  // construct from another image
  RtMRIImage(RtMRIImage &img);

  // construct by reading a nifti file
  RtMRIImage(const string &filename, int series=0, int timepoint=0);

  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool writeInfo(ostream &os);

  //  in
  //   filename: string filename
  //  out
  //   success or failure
  bool readInfo(istream &is);

  // set info struct
  //  in
  //   _info: struct to copy
  void setInfo(const RtExternalImageInfo &_info);

  // set info from nifti header
  //  in
  //   _info: nifti header to copy from
  void setInfoFromNiiHeader(const nifti_1_header &_info);

  // print info about this image
  void printInfo(ostream &os);

  //********  methods for getting data from the image *******//

  // get the acquisition number
  string getCreationTime() const;

  // get the total number of repetitions expected
  int getTotalRepetitions() const;

  // set the repetition time
  void setRepetitionTime(double tr);

  // set the matrix size
  void setMatrixSize(unsigned int ms);

  // get the matrix size
  unsigned int getMatrixSize();

  // set the number of slices
  void setNumSlices(unsigned int ns);

  // get the number of slices
  unsigned int getNumSlices();

  // get a smart contrast level
  float getAutoContrast();

  // get a smart brightness level
  float getAutoBrightness();

  // set whether this is a moco image
  void setMoco(bool);

  // set the motion parameters
  void setTranslationX(float64_t x);
  void setTranslationY(float64_t y);
  void setTranslationZ(float64_t z);
  void setRotationX(float64_t x);
  void setRotationY(float64_t y);
  void setRotationZ(float64_t z);

  // get whether this is a moco image
  bool getMoco();

  // get motion parameters
  float64_t getTranslationX();
  float64_t getTranslationY();
  float64_t getTranslationZ();
  float64_t getRotationX();
  float64_t getRotationY();
  float64_t getRotationZ();

 protected:

  double readFOV; // mm
  double phaseFOV; // mm

  // imaging parms
  string seriesInstanceUID;   // series id
  double tr;                  // repetition time (ms)
  int totalRepetitions;

  // actual acquision info parms
  ACE_Date_Time time;                // acquisition time
  ACE_Date_Time refFrameTime;        // acquisition time of frame of reference

  // scanner online post-processing parms
  bool distCorrect2D;           // 2d distortion correction
  bool moco;                    // motion correction
  float64_t  mcTranslationXMM;  // Translation (mm) that was applied
  float64_t  mcTranslationYMM;  //   by motion correction along each
  float64_t  mcTranslationZMM;  //   axis

  float64_t  mcRotationXDeg;    // Rotation (degrees) that was applied
  float64_t  mcRotationYDeg;    //   by motion correction along each
  float64_t  mcRotationZDeg;    //   axis

  // received data parms
  bool fromScanner;
};



#endif
