/******************************************************************************
 * RtDataStore.cpp defines a class for data storage and retreival
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/
// not ready for prime time
#ifdef 0

static char *VERSION = "$Id$";

#include"RtDataStore.h"

#define INITIAL_NOTIFY_LIST_LEN 128;

RtDataStore::RtDataStore() {
  outputNotifyList.reserve(INITIAL_NOTIFY_LIST_LEN);
}

// destructor
RtDataStore::~RtDataStore() {

}

// add an output to be notified when new data arrives
void RtDataStore::addOutputForNotify(const RtOutput *out) {
  outputNotifyList.push_back(out);
}

//*** data methods ***//

// hand off some data to be output
void RtDataStore::setData(RtData *data) {
  // save the pointer to the data
  store[data.getDataID()] = data;

  // notify listeners
  for(vector<RtOutput*>::iterator i = outputNotifyList.begin(); 
      i != outputNotifyList.end(); i++) {
    (*i)->setData(data);
  }
}

// get data by id
RtData *RtDataStore::getData(const RtDataID &id) {
  vector<RtOutput*>::iterator_const i = store.find(id);

  // if not found
  if(i == store.end()) {
    return NULL;
  }

  return (*i).second
}

// get the version
//  out: char array that represents the cvs version
char RtDataStore::*getVersionString() {
  return VERSION;
}

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


