/*=========================================================================
 *  RtElementAccess.cpp defines a class that can retreive and set elements in
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

#include"RtElementAccess.h"


// setup to access elements of this image
RtElementAccess::RtElementAccess(RtData *_data)
  : data(_data),
    mask(NULL) {
  if(data == NULL || data->getElementType() == RT_UNKNOWN_TYPE) { // cant
    return;
  }

  // build the indices
  switch(data->getElementType()) {
    case RT_DOUBLE_TYPE:
      buildElementIndices<double>();
      break;

    case RT_SHORT_TYPE:
      buildElementIndices<short>();
      break;

    default:
      return;
  }
}

// setup to access elements of this image
RtElementAccess::RtElementAccess(RtData *_data, RtMaskImage *_mask = NULL)
    : data(_data), mask(_mask) { }

// destructor
RtElementAccess::~RtElementAccess() {

}

// get an element (double)
double RtElementAccess::getDoubleElement(unsigned int index) {
  // get based on type
  switch(data->getElementType()) {
    case RT_DOUBLE_TYPE:
      return static_cast<RtDataImage<double>*>(data)->getElement(index);
      break;

    case RT_SHORT_TYPE:
      return static_cast<RtDataImage<short>*>(data)->getElement(index);
      break;

    default:
      return numeric_limits<double>::quiet_NaN();
  }
}

// get an element (short)
short RtElementAccess::getShortElement(unsigned int index) {
  // get based on type
  switch(data->getElementType()) {
    case RT_DOUBLE_TYPE:
      return static_cast<short>(
          rint(static_cast<RtDataImage<double>*>(data)->getElement(index)));
      break;

    case RT_SHORT_TYPE:
      return static_cast<RtDataImage<short>*>(data)->getElement(index);
      break;

    default:
      return numeric_limits<short>::quiet_NaN();
  }
}

// set anelement (double)
void RtElementAccess::setElement(unsigned int index, double val) {
  // set based on type
  switch(data->getElementType()) {
    case RT_DOUBLE_TYPE:
      return static_cast<RtDataImage<double>*>(data)->setElement(index, val);
      break;

    case RT_SHORT_TYPE:
      return static_cast<RtDataImage<short>*>(data)->setElement(
          index, static_cast<short>(rint(val)));
      break;

    default:
      break;
  }
}

// set an element (short)
void RtElementAccess::setElement(unsigned int index, short val) {
  // set based on type
  switch(data->getElementType()) {
    case RT_DOUBLE_TYPE:
      return static_cast<RtDataImage<double>*>(data)->setElement(index, val);
      break;

    case RT_SHORT_TYPE:
      return static_cast<RtDataImage<short>*>(data)->setElement(index, val);
      break;

    default:
      break;
  }
}

// get the element indices (from mask if there is one)
vector<unsigned int> RtElementAccess::getElementIndices() {
  if(mask == NULL) {
    return elementIndices;
  }

  return mask->getOnVoxelIndices();
}

// build the element indices for a particular datatype by finding
// non-NaN and non-infinity entries
template<class T>
void RtElementAccess::buildElementIndices() {
  // cast the data to the appropriate type
  RtDataImage<T> *img = static_cast<RtDataImage<T>*>(data);
  elementIndices.reserve(img->getNumEl());

  // go through pixels looking for non-nan entries
  for(unsigned int i = 0; i < img->getNumEl(); i++) {
    if(!isnan(img->getElement(i))) {
      elementIndices.push_back(i);
    }
  }
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
