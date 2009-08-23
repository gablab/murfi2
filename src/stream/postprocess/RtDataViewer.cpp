/******************************************************************************
 * RtDataViewer.cpp is a class for viewing the contents of the data store
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-08-21
 *
 *****************************************************************************/

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
  //getDataStore().getAvailableData();

  RtDataID id;
  // find all data requested
  for(vector<string>::iterator i = dataIds.begin(); i != dataIds.end(); i++) {
    id.setFromString(*i);
    
    if(id.getTimePoint() != DATAID_NUM_UNSET_VALUE) {
      id.setTimePoint(time);
    }

    cout << "RtDataViewer::process: searching for " << id << " from " << *i << endl;

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
	logs << "RtDataViewer::process: found " << dat->getDataID() << endl;
	log(logs);
      }
    }
  }

  return 0;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


