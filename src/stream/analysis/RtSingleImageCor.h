/*=========================================================================
 *  RtSingleImageCor.h is the header for a class that estimates the
 *  activation at each voxel in a single image using a GLM-based
 *  correlation analysis based on Hinds, et al., 2008
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

#ifndef RTSINGLEIMAGEECOR_H
#define RTSINGLEIMAGEECOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtIncrementalGLM.h"
#include"RtLeastSquaresSolve.h"
#include"RtTypes.h"

#include<vector>
using namespace std;

// class declaration

class RtSingleImageCor : public RtIncrementalGLM {
 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtSingleImageCor();

  // destructor
  virtual ~RtSingleImageCor();

 protected:

  // deallocate internal data
  void freeEstErrSum();

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string, string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  void initEstimation(RtMRIImage &dat, RtMaskImage *mask);

  // start a logfile
  virtual void startDumpAlgoVarsFile();

  //// parameters of the activation estimation algorithm

  // whether to include the condition mean in the stat computation
  bool includeConditionMean;

  //// error estimation params

  // error norm method
  Norm errorNorm;

  // store the per pixel sum of absolute error for the single image model fit
  RtActivation *estErrSum;

  // number of timepoints
  unsigned int numTimepoints;

  // number of data actually in the error estimate so far
  int numDataPointsCount;

  // the amount of data to use in the estimation of the error
  unsigned int numDataPointsForErrEst;

  // whether errors should only be estimated in baseline
  bool onlyEstErrInBaseline;

  bool dumpAlgoVars;
  string dumpAlgoVarsFilename;
  std::ofstream dumpFile;
};

#endif
