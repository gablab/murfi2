/*=========================================================================
 *  RtElementAccess.h declares a class that can retreive and set elements in
 *  RtDataImages of double or short template type
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

#ifndef RTELEMENTACCESS_H
#define RTELEMENTACCESS_H

#include"RtData.h"
#include"RtMaskImage.h"
#include<vector>
#include<limits>
#include"RtTypes.h"

using namespace std;

// class declaration
class RtElementAccess {

 public:

  // setup to access elements of this image and compute the indices of
  // non-NaN elements (O(n))
  RtElementAccess(RtData *_data);

  // setup to access elements of this image within a mask (O(1))
  // NOTE: calling this with a NULL mask will result in no indices being
  // computed, which can be used if element index mapping is handled outside
  // this class
  RtElementAccess(RtData *_data, RtMaskImage *_mask);

  // destructor
  virtual ~RtElementAccess();

  // get an element (double)
  double getDoubleElement(unsigned int index);

  // get an element (short)
  short getShortElement(unsigned int index);

  // set element (double)
  void setElement(unsigned int index, double val);

  // set element (short)
  void setElement(unsigned int index, short val);

  // get the element indices (from mask if there is one)
  vector<unsigned int> getElementIndices();

 protected:

  template<class T>
  void buildElementIndices();

  //*** data members  ***//

  RtData *data;
  RtMaskImage *mask;

  // if there is no mask we store our own (non-NaN) indices
  vector<unsigned int> elementIndices;
};

#endif
