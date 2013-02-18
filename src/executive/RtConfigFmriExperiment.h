/*=========================================================================
 *  RtConfigFmri.h is the header for class that controls xml configuration
 *  reading specific to an fmri experiment.
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

#ifndef RTCONFIGFMRIEXPERIMENT_H
#define RTCONFIGFMRIEXPERIMENT_H


#include"RtConfig.h"

// class declaration
class RtConfigFmriExperiment : public RtConfig {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtConfigFmriExperiment() : RtConfig() {};

  // copy constructor (called often)
  RtConfigFmriExperiment(const RtConfigFmriExperiment &other);

  // destructor
  virtual ~RtConfigFmriExperiment() {};

  // sets some default configuration info
  virtual void setDefaults();

  // validate the configuration
  // checks for valid setup of different parts of the program
  // returns true for success
  virtual bool validateConfig();


  //******* computed values must have special functions

  // get the filename for the transformation file that takes the
  // experiment reference volume into the space of the current series
  string getSeriesXfmFilename(unsigned int series);

  // get the filename of a file transformed by using the series xfm on
  // an input file
  string getSeriesXfmOutputFilename(unsigned int series, string input);

  // get the filename for the epi volume that is the reference space
  // for a single series
  string getSeriesRefVolFilename(unsigned int series);

  // get the filename for the mask volume for a series
  string getSeriesMaskFilename(unsigned int series, string roiID);

  // determine if the reference volume already exists
  // true if reference volume exists
  bool getStudyRefVolExists();

  // build a filename for a volume in this experiment
  //  in
  //   series number
  //   timepoint
  //  out
  //   absolute file string
  string getVolFilename(int _seriesNum,int _timepoint);

  // build a filename for a design matrix in this experiment
  //  in
  //   series number
  //  out
  //   absolute file string
  string getDesignFilename(int _seriesNum);
};


#endif
