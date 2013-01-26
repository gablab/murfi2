/*=========================================================================
 *  RtDataStore.cpp defines a class for data storage and retreival
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

#include"RtDataStore.h"

#define INITIAL_NOTIFY_LIST_LEN 128

RtDataStore::RtDataStore() {
  notifyList.reserve(INITIAL_NOTIFY_LIST_LEN);
}

// destructor
RtDataStore::~RtDataStore() {

}

// add an output to be notified when new data arrives
void RtDataStore::addListener(RtDataListener *lis) {
  notifyList.push_back(lis);
}

//*** data methods ***//

// hand off some data to be output
void RtDataStore::setData(RtData *data) {

  mut.acquire();

  // special check for processing done
  if(data->getDataID().getModuleID() == "end-task") {
    // update value of latestTR
    if (data->getDataID().getTimePoint() != DATAID_NUM_UNSET_VALUE &&
        data->getDataID().getTimePoint() != DATAID_NUM_WILDCARD_VALUE) {

      latestTR = data->getDataID().getTimePoint();
    }

    mut.release();
    return;
  }

  // don't allow wildcards on insertion
  RtDataID insert = data->getDataID();
  if(insert.hasWildcards()) {
    insert.eliminateWildcards();
  }

  // put data into datastore with its dataID as the key
  store[data->getDataID()] = data;

  // add to availableData (needs a hard copy of the dataID)
  setAvailableData(data->getDataID());

  mut.release();

  //debug
  //  set<RtDataID>::const_iterator it = getAvailableData();
  //endebug

  // notify listeners
  for(vector<RtDataListener*>::iterator i = notifyList.begin();
      i != notifyList.end(); i++) {
    (*i)->notify(data->getDataID());
  }
}

void RtDataStore::setAvailableData(RtDataID dataID) {

  // remove timepoint if one is set
  if(dataID.getTimePoint() != DATAID_NUM_UNSET_VALUE) {
    dataID.setTimePoint(DATAID_NUM_WILDCARD_VALUE);
  }

  // put data id into available data (insert will only add a unique value)
  // TODO will a position iterator add any efficiency here?
  availableData.insert(dataID);

}

// get data by id
//
// NOTE: this function is much more efficient if the requested data is
// specified fully, i.e. there are no wildcards in the dataID (see RtDataID.h)
// avoid using wildcards if possible
RtData *RtDataStore::getData(RtDataID dataID) {

  // fill in wildcards (search through available data makes this inefficient)
  if(dataID.hasWildcards()) {
    // find corresponding full id by an exhaustive search through available
    // data
    for(set<RtDataID>::const_iterator id = availableData.begin();
        id != availableData.end(); id++) {
      RtDataID avail = (*id);

      // handle potential timepoint-less data
      unsigned int tp = avail.getTimePoint();
      if(dataID.getTimePoint() != DATAID_NUM_UNSET_VALUE) {
        avail.setTimePoint(DATAID_NUM_WILDCARD_VALUE);
      }
      else {
        avail.setTimePoint(DATAID_NUM_UNSET_VALUE);

      }

      if (avail == dataID) { // == respects wildcards
        // copy, preserving timepoint if not a timepointless datum
        unsigned int origTp = dataID.getTimePoint();
        dataID = avail;

        if(tp != DATAID_NUM_UNSET_VALUE) {
          dataID.setTimePoint(origTp);
        }
        else {
          dataID.setTimePoint(tp);
        }

        break;
      }
    }

    // finally, handle wildcard in timepoint field by using the latest timepoint
    if (dataID.getTimePoint() == DATAID_NUM_WILDCARD_VALUE) {
      dataID.setTimePoint(latestTR);
    }

    if(dataID.hasWildcards()) {
      cerr << "filling in wildcards failed. no match found." << endl;
      return NULL;
    }
  }

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

// get latest data by id
RtData *RtDataStore::getLatestData(RtDataID dataID) {

  // set timepoint in dataID
  dataID.setTimePoint(latestTR);

  return getData(dataID);
}

// get available data
set<RtDataID>::const_iterator RtDataStore::getAvailableData() {
  set<RtDataID>::const_iterator it = availableData.begin();
  return it;
}
