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

  // construct from an external image info struct (siemens) and some image data
  RtMRIImage(RtExternalImageInfo &info, short *data = NULL);

  // construct from another image
  RtMRIImage(RtMRIImage &img);

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

  // print info about this image
  void printInfo(ostream &os);

  //********  methods for getting data from the image *******//

  // get the acquisition number
  string getCreationTime() const;

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

 protected:

  unsigned long slice; // slice index (zero based)

  double readFOV; // mm
  double phaseFOV; // mm

  // imaging parms
  bool swapReadPhase;
  string seriesInstanceUID;   // series id
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
