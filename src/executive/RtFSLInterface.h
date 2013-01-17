/*=========================================================================
 *  class to support interaction with the FSL tools
 *
 *  WARNING! platform specific code in here....
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

#ifndef RTFSLINTERFACE_H
#define RTFSLINTERFACE_H

#include<cstdio>
#include<cstdlib>
#include<string>
#include<map>
#include<iostream>

using namespace std;

// job management support
typedef enum {
  FSL_NO_SUCH_JOB = -1,
  FSL_JOB_RUNNING,
  FSL_JOB_FINISHED,
  FSL_JOB_ERROR,
  FSL_JOB_UNKNOWN_STATE
} FslJobStatus;

typedef unsigned int FslJobID;

class SystemCommand;

class RtFSLInterface {

 public:


  // execute a command string in the system environment
  static FslJobID runCommand(string command);

  // execute a command string in the system environment and block
  // until it finishes
  static FslJobStatus runCommandBlocking(string command);


  // perform a same-subject registration of two EPI volumes
  // in:
  //  target  image that will be registered to
  //  movable image that will be registered
  //  xfmFile file to save the transformation in (if empty a temp file
  //          is created)
  static FslJobID registerSameSubjEPI(string target, string movable,
                                      string xfmFile = string(tempnam(NULL,
                                                                      "rtreg")),
                                      bool block = false);



  // apply a pre-computed transformation to a file
  // in:
  //  target  image that was registered to
  //  movable image that was registered
  //  input   image to be transformed
  //  output  image filename to save to
  //  xfmFile file to save the transformation in (if empty a temp file
  //          is created)
  static FslJobID applyTransform(string target, string movable,
                                 string input,  string output,
                                 string xfmFile, bool block = false);


  // make a brain mask out of an epi volume
  // in:
  //  filename of the volume to base the mask off of
  static FslJobID makeBrainMask(string brainVolume,
                                string maskFilename,
                                string betOptions,
                                bool block = false);

  // run a block design analysis
  // in:
  //  filePrefix    full path plus file prefix of volumes to include
  //  runNum        series number of the images to include
  //  blockLength   number of seconds per block
  //  numConditions number of stimulus conditions (including rest)
  static FslJobID runAnalysis(string filePrefix, unsigned int runNum,
                              float blockLength, unsigned int numConditions);

  // get a job status
  static FslJobStatus getJobStatus(FslJobID jobID);

 private:

  static FslJobID getNextJobID();

  // map job ids to system command objects
  static map<FslJobID, SystemCommand*> cmdMap;
};

#endif
