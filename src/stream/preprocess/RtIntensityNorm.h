/*=========================================================================
 *  RtIntensityNorm.h is the header for a class that normalizes the intensity
 *  of subsequent images in a timeseries to match the global signal of the
 *  first.
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

#ifndef RTINTENSITYNORM_H
#define RTINTENSITYNORM_H

#include"RtStreamComponent.h"
#include"RtMaskImage.h"
#include"RtMRIImage.h"
#include"RtDataImage.h"

#include <float.h>

// class declaration
class RtIntensityNorm : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtIntensityNorm();

  // destructor
  ~RtIntensityNorm();

protected:

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // initialize processing
  bool initINorm(RtMRIImage *img);

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);

  double meanIntensity; // intensity to normalize to
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
