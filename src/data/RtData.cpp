/******************************************************************************
 * RtData.cpp defines a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtData.h"

  
// default constructor
RtData::RtData() : id("data"), persistent(false), lock(NULL) {

}

// destructor
RtData::~RtData() {

  // notify our locker that we are being deleted
  if(lock != NULL) {
    lock->beingDeleted();
  }
  
}

// gets the id string for this data
//  out
//   id string
const string &RtData::getID() {
  return id;
}

// append to the id string for this data
//  in
//   string to append to the id
void RtData::addToID(string &s) {
  id += ".";
  id += s;
}

// append to the id string for this data
//  in
//   string to append to the id
void RtData::addToID(char *s) {
  id.append(".");
  id.append(s);
}


// get if we should delete this data after stream processing (not persistent)
bool RtData::getPersistent() {
  return persistent;
}

// get if we should delete this data after stream processing (not persistent)
void RtData::setPersistent(bool p) {
  persistent = p;
}

// try to obtain a lock for this data
//  out
//   if lock was obtained
bool RtData::lockData(RtLocker *locker) {
  ACE_TRACE(("RtData::lockData"));

  if(lock == NULL) {
    lock = locker;
    return true;
  }

  return false;
}

// release a lock on this data
//  out
//   if lock was released or if data was not locked
bool RtData::unlockData(RtLocker *locker) {
  ACE_TRACE(("RtData::unlockData"));

  if(lock == locker || lock == NULL) {
    locker = NULL;
    return true;
  }

  return false;
}

// get the creation time
ACE_Date_Time RtData::getCreationTime() {
  return creationTime;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


