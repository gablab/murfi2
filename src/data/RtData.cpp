/******************************************************************************
 * RtData.cpp defines a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtData.h"

  
// default constructor
RtData::RtData() : numEl(0), elType(RT_UNKNOWN_TYPE) {}

// destructor
RtData::~RtData() {
}

// gets the id string for this data
//  out
//   id string
RtDataID &RtData::getDataID() {
  return dataID;
}

// gets the id string for this data
//  out
//   id string
RtDataID RtData::getDataID() const {
  return dataID;
}

// set the id string for this data
//  in
//   string to set the id to
void RtData::setDataID(const RtDataID &id) {
  dataID = id;
}


// get the number of elements in this datatype
unsigned int RtData::getNumEl() const {
  return numEl;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


