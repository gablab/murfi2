/*=========================================================================
 *  RtDataStore.h declares a class for data storage and retreival
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

#ifndef RTDATASTORE_H
#define RTDATASTORE_H

#include"RtDataID.h"
#include"RtDataListener.h"
#include"RtData.h"
#include"ace/Mutex.h"

#include"RtOutput.h"
#include<vector>
#include<map>
#include<set>
using namespace std;

// class declaration
class RtDataStore : public RtOutput  {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtDataStore();

  // destructor
  virtual ~RtDataStore();

  // add listener to be notified when new data arrives
  virtual void addListener(RtDataListener *lis);

  //*** data methods ***//

  // hand off some data to be output
  virtual void setData(RtData *data);

  // get data by id
  //
  // NOTE: this function is much more efficient if the requested data is
  // specified fully, i.e. there are no wildcards in the dataID (see
  // RtDataID.h) avoid using wildcards if possible
  virtual RtData *getData(RtDataID id);

  // get latest data
  virtual RtData *getLatestData(RtDataID id);

  // get a list of the available data in the data store
  // note: this returns a list of the unique data ids excepting timepoint.
  set<RtDataID>::const_iterator getAvailableData();

 protected:

  // list of outputs to be notified when new data arrives
  vector<RtDataListener*> notifyList;

  // hash map to store pointers to acquired data
  map<RtDataID,RtData*,RtDataIDCompare> store;

  // list of the data ids we currently have in the data
  // store (excepting timepoiint)
  set<RtDataID> availableData;

  // set data ids to availableData
  void setAvailableData(RtDataID);

  // latest tr for which dataStore has data
  unsigned int latestTR;

  // create mutex for datastore lockdown
  ACE_Mutex mut;

};

#endif
