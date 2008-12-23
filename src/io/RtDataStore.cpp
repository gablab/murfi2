/******************************************************************************
 * RtDataStore.cpp defines a class for data storage and retreival
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-13
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtDataStore.h"

#define INITIAL_NOTIFY_LIST_LEN 128

RtDataStore::RtDataStore() {
  outputNotifyList.reserve(INITIAL_NOTIFY_LIST_LEN);
}

// destructor
RtDataStore::~RtDataStore() {

}

// add an output to be notified when new data arrives
void RtDataStore::addOutputForNotify(RtOutput *out) {
  outputNotifyList.push_back(out);
}

//*** data methods ***//

// hand off some data to be output
void RtDataStore::setData(RtData *data) {
  
  mut.acquire();

  // put data into datastore with its dataID as the key
  store[data->getDataID()] = data;
  
  mut.release();
  
  // add to availableData (needs a hard copy of the dataID)
  setAvailableData(data->getDataID());
  
  //debug
//  set<RtDataID>::const_iterator it = getAvailableData();
  //endebug
  
  // notify listeners
  for(vector<RtOutput*>::iterator i = outputNotifyList.begin();
      i != outputNotifyList.end(); i++) {
    (*i)->setData(data);
  }
}

void RtDataStore::setAvailableData(RtDataID dataID) {
  
  // remove timepoint
  dataID.setTimePoint(DATAID_UNSET_VALUE);
  
  // put data id into available data (insert will only add a unique value)
  // TODO will a position iterator add any efficiency here?
  availableData.insert(dataID);
  
}

// get data by id
RtData *RtDataStore::getData(RtDataID &dataID) {
  
  // iterator for map
  map<RtDataID,RtData*,RtDataIDCompare>::const_iterator it;
    
  mut.acquire();
  it = store.find(dataID);

  // if not found
  if(it == store.end()) {
    mut.release();
    return NULL;
  }
  mut.release();

  return (*it).second;
}

// get available data
set<RtDataID>::const_iterator RtDataStore::getAvailableData() {
  
  set<RtDataID>::const_iterator it = availableData.begin();

//debug
//  cout << "availableData contains:";
//  for ( it=availableData.begin() ; it != availableData.end(); it++ )
//    cout << " " << *it;
//
//  cout << endl;
//endebug

  return it;
}

// get the version
//  out: char array that represents the cvs version
char *RtDataStore::getVersionString() {
  return VERSION;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


