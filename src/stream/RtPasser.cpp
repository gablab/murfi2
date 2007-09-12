/******************************************************************************
 * RtPasser.cpp implements a class that passes data from an
 * analysis stream to output
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtPasser.h"

// default constructor
RtPasser::RtPasser() : RtStreamComponent(), dataID("") {
  id = "RtPasser";
}

// string with id constructor
//  in
//   dataID is a string that constrains what type of data we send, if its
//   empty we send all the data
RtPasser::RtPasser(string _dataID) : RtStreamComponent(), dataID(_dataID) {
  id = "RtPasser";
}

// char* with id constructor
//  in
//   dataID is a string that constrains what type of data we send, if its
//   empty we send all the data
RtPasser::RtPasser(char *_dataID) : RtStreamComponent(), dataID(_dataID) {
  id = "RtPasser";
}

// destructor
RtPasser::~RtPasser() {}

//*** initialization routines  ***//
void RtPasser::addOutput(RtOutput *out) {
  outputs.push_back(out);
}

// process a single acquisition
int RtPasser::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtPasser::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // look for our kind of data
  for(unsigned int i = 0; i < msg->getNumData(); i++) {

    RtData *d = msg->getData(i);

    // check if its our type
    if(!dataID.empty() && d->getID() != dataID) {	
      continue;
    }

    //      ACE_DEBUG((LM_DEBUG, "passing image %d\n", ((RtDataImage*)msg->getLastData())->getAcquisitionNum()));


    for(vector<RtOutput*>::iterator j = outputs.begin(); j != outputs.end(); j++) {
      (*j)->setData(d);
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


