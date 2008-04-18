/******************************************************************************
 * RtOutput.cpp defines a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtOutput.h"

// default constructor
RtOutput::RtOutput() {
  outputID = "output";
} 

// destructor
RtOutput::~RtOutput() {

}

// init
bool RtOutput::open(RtConfig &config) {
  if(!RtIO::open(config)) {
    return false;
  }

  return true;
}

// hand off some data to be output
void RtOutput::setData(RtData *data) {
  // 
}

// set the id string
void RtOutput::setID(const string &s) {
  outputID = s;
}

// add a string to the output id
void RtOutput::addToID(string add) {
  outputID += add;
}

// get the id string
string RtOutput::getID() {
  return outputID;
}

// prints the current time 
void RtOutput::printNow(ostream &os) {
  now.update();

  os << now.year() << "/" << now.month() << "/" << now.day() << " "
     << now.hour() << ":" << now.minute() << ":" << now.second()
     << "." << now.microsec();
}
  
// gets the version
//  out:
//   cvs version string for this class
char *RtOutput::getVersionString() {
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


