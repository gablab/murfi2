/*=========================================================================
 *  RtContrast.h is the header for a class that estimates the
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

#ifndef RTCONTRAST_H
#define RTCONTRAST_H

#include"RtStreamComponent.h"
#include"RtDesignMatrix.h"
#include"RtMaskImage.h"

// helper to hold contrast names and weights
class RtContrastVecEntry {

public:

  RtContrastVecEntry() : weight(0) {};
  RtContrastVecEntry(const string &_name, double _weight = 0.0) 
    : name(_name), weight(_weight) {};

  string name;
  double weight;
};

// class declaration
class RtContrast : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtContrast();

  // destructor
  ~RtContrast();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // configure this stream component
  //  in
  //   xml module node from which to read <option>s
  virtual bool init(TiXmlElement *module, RtConfig *config, RtConductor *_conductor);


  // initialize the contrast for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual bool initContrast(RtMRIImage &image);

  // initialize the contrast 

  // contrast vector and name
  vector<RtContrastVecEntry> contrastVector;
  string contrastName;

  // for tstat conversion
  double contrastCov;

  // ids for reading the betas and residuals
  string estimationModuleID;

  // design matrix for this estimation
  RtDesignMatrix *designMatrix;

  // temp vars for the input
  vnl_vector<double> inputContrastVec;
  vector<string> inputConditionNames;
  unsigned int numInputConditions;

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


