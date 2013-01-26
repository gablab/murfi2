/*=========================================================================
 *  RtIncrementalGLM.h is the header for a class that estimates the
 *  activation at each voxel incrementally using Gentleman's method
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

#ifndef RTINCREMENTALGLM_H
#define RTINCREMENTALGLM_H

#include"RtModelFit.h"
#include"RtLeastSquaresSolve.h"
#include"util/timer/timer.h"

// class declaration
class RtIncrementalGLM : public RtModelFit {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtIncrementalGLM();

  // destructor
  virtual ~RtIncrementalGLM();

 protected:

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual void initEstimation(const RtData &dat, RtMaskImage *mask);

  // make sure we are configured properly
  virtual bool validateComponentConfig();

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // start a logfile
  virtual void startDumpAlgoVarsFile();

  // one solver for each voxel
  unsigned int numSolvers;
  RtLeastSquaresSolve **solvers;

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
