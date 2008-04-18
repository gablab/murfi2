/******************************************************************************
 * RtPasser.cpp implements a class that passes data from an
 * analysis stream to output
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtPasser.h"
#include"RtData.h"

string RtPasser::moduleString("passer");

// default constructor
RtPasser::RtPasser() : RtStreamComponent(), dataID("") {
  componentID = moduleString;
}

// string with id constructor
//  in
//   dataID is a string that constrains what type of data we send, if its
//   empty we send all the data
RtPasser::RtPasser(string _dataID) 
  : RtStreamComponent(), dataID(_dataID) {
  componentID = moduleString;
}

// char* with id constructor
//  in
//   dataID is a string that constrains what type of data we send, if its
//   empty we send all the data
RtPasser::RtPasser(char *_dataID) 
  : RtStreamComponent(), dataID(_dataID) {
  componentID = moduleString;
}

// destructor
RtPasser::~RtPasser() {}

//*** initialization routines  ***//
void RtPasser::addOutput(RtOutput *out, const string &dataId) {
  outputs.push_back(out);
}

// process a single acquisition
void RtPasser::sendToOutputs(RtData *d) {
  ACE_TRACE(("RtPasser::sendToOutputs"));

  // check if this data is our type
  if(d == NULL || !dataID.empty() && d->getID() != dataID) {	
    return;
  }

  //      ACE_DEBUG((LM_DEBUG, "passing image %d\n", ((RtDataImage*)msg->getLastData())->getAcquisitionNum()));


  // set to all outputs
  for(vector<RtOutput*>::iterator j = outputs.begin(); j != outputs.end(); j++) {
    //cout << "sending " << d->getID() << " to " << (*j)->getID() << endl;
    (*j)->setData(d);
  }

  return;
}

// processes as a stream component
int RtPasser::process(ACE_Message_Block* mb) {
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();
  RtData *img = msg->getCurrentData();
//  cout << "passer got msg " << msg 
//       << " with curdata " << img
//       << endl;

  sendToOutputs(img);
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


