/*=========================================================================
 *  RtConfigFmriRun.h is the header for class that controls xml configuration
 *  reading specific to a single run of an fmri experiment.
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

#ifndef RTCONFIGFMRIRUN_H
#define RTCONFIGFMRIRUN_H


#include"RtConfig.h"
#include"RtConfigFmriExperiment.h"

// class declaration
class RtConfigFmriRun : public RtConfig {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtConfigFmriRun() : RtConfig() {

  };

  // copy constructor (called often)
  RtConfigFmriRun(const RtConfigFmriRun &other) : RtConfig(other) {};

  // assignment operator (called often)
  void operator=(const RtConfigFmriRun &them) {
    parms = them.parms;
  };

  // destructor
  virtual ~RtConfigFmriRun() {};

  // sets some default configuration info
  virtual void setDefaults();

  // validate the configuration
  // checks for valid setup of different parts of the program
  // returns true for success
  virtual bool validateConfig();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

 protected:

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
