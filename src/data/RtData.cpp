/******************************************************************************
 * RtData.cpp defines a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtData.h"

  
// default constructor
RtData::RtData() {
  //dataID = "data";
  //roiID = "unset";
  persistent = false;
}

// destructor
RtData::~RtData() {
}

// gets the id string for this data
//  out
//   id string
RtDataID &RtData::getDataID() {
  return dataID;
}

// get the roiID
//string RtData::getRoiID() const {
//  return roiID;
//}

// append to the id string for this data
//  in
//   string to append to the dataID
//void RtData::addToID(string &s) {
//  dataID += ".";
//  dataID += s;
//}

// append to the id string for this data
//  in
//   string to append to the id
//void RtData::addToID(char *s) {
//  dataID.append(".");
//  dataID.append(s);
//}

// set the id string for this data
//  in
//   string to set the id to
void RtData::setDataID(const RtDataID &id) {
  dataID = id;
}

// set the roiID
//void RtData::setRoiID(const string &id) {
//  roiID = id;
//}

// get if we should delete this data after stream processing (not persistent)
bool RtData::getPersistent() const {
  return persistent;
}

// get if we should delete this data after stream processing (not persistent)
void RtData::setPersistent(bool p) {
  persistent = p;
}


// get the number of elements in this datatype
unsigned int RtData::getNumEl() const {
  return numEl;
}


// // try to obtain a lock for this data
// //  out
// //   if lock was obtained
// bool RtData::lockData(RtLocker *locker) {
//   ACE_TRACE(("RtData::lockData"));

//   if(lock == NULL) {
//     lock = locker;
//     return true;
//   }

//   return false;
// }

// // release a lock on this data
// //  out
// //   if lock was released or if data was not locked
// bool RtData::unlockData(RtLocker *locker) {
//   ACE_TRACE(("RtData::unlockData"));

//   if(lock == locker || lock == NULL) {
//     locker = NULL;
//     return true;
//   }

//   return false;
// }

// get the creation time
//ACE_Date_Time RtData::getCreationTime() const {
//  return creationTime;
//}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


