/*=========================================================================
 *  RtDataViewer.cpp is a class for viewing the contents of the data store
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

#include "RtDataViewer.h"
#include "RtDataID.h"
#include "RtDataIDs.h"

string RtDataViewer::moduleString(ID_DATAVIEWER);

// default constructor
RtDataViewer::RtDataViewer() : RtStreamComponent() {
  componentID = moduleString;
}

// destructor
RtDataViewer::~RtDataViewer() {}

// process an option
//  in
//   name of the option to process
//   val  text of the option node
bool RtDataViewer::processOption(const string &name, const string &text,
                                 const map<string,string> &attrMap) {

  if(name == "dataid") {
    dataIds.push_back(text);
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtDataViewer::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtDataViewer::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtDataViewer::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current time
  unsigned int time = msg->getCurrentData()->getDataID().getTimePoint();

  // debug
  getDataStore().getAvailableData();

  RtDataID id;
  // find all data requested
  for(vector<string>::iterator i = dataIds.begin(); i != dataIds.end(); i++) {
    id.setFromString(*i);

    if(id.getTimePoint() != DATAID_NUM_UNSET_VALUE
       && id.getTimePoint() != DATAID_NUM_WILDCARD_VALUE) {
      id.setTimePoint(time);
    }

    cout << "RtDataViewer::process: searching for " << id
         << " from " << *i << endl;

    // find the data with the right ID
    RtData *dat = getDataStore().getData(id);

    if(dat == NULL) {
      cout << "RtDataViewer::process: could not find " << id << endl;
      if(logOutput) {
        stringstream logs("");
        logs << "RtDataViewer::process: could not find " << id << endl;
        log(logs);
      }
    }
    else {
      cout << "RtDataViewer::process: found " << dat->getDataID() << endl;
      if(logOutput) {
        stringstream logs("");
        logs << "RtDataViewer::process: found "
             << dat->getDataID() << endl;
        log(logs);
      }
    }
  }

  return 0;
}
