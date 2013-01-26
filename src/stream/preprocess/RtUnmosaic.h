/*=========================================================================
 *  RtUnmosaic.h is the header for a class that converts from a flat mosaiced
 *  representation to a 3D volume
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

#ifndef RTUNMOSAIC_H
#define RTUNMOSAIC_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtUnmosaic : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtUnmosaic();

  // destructor
  virtual ~RtUnmosaic();

 protected:

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

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
