/*=========================================================================
 *  RtModelFit.h is the header for a base class for any method for model
 *  estimation
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

#ifndef RTMODELFIT_H
#define RTMODELFIT_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>
#include<vnl/algo/vnl_convolve.h>

#include"RtStreamComponent.h"
#include"RtExperiment.h"
#include"RtDesignMatrix.h"
#include"RtDataImage.h"
#include"RtDataIDs.h"
#include"RtMaskImage.h"

#include"RtLimit.h"
#include"RtTypes.h"

#include<vector>
#include<fstream>
using namespace std;

// class declaration

class RtModelFit : public RtStreamComponent {
 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtModelFit();

  // destructor
  ~RtModelFit();

 protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb) = 0;

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string, string> &attr);

  // make sure config is right
  virtual bool validateComponentConfig();

  // configure this stream component
  //  in
  //   xml module node from which to read <option>s
  virtual bool init(TiXmlElement *module, RtConfig *config, RtConductor *_conductor);

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  virtual void initEstimation(const RtData &dat, RtMaskImage *mask);

  // template data id of the data volumes
  RtDataID templateDataID;

  // design matrix
  RtDesignMatrix design;

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


