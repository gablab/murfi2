/******************************************************************************
 * RtCode.h is the header for an abstract class that 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtCode.h"
#include"RtConductor.h"

// default constructor
RtCode::RtCode() {
  codeNum = -1;
}

// constructor w/ codenum
RtCode::RtCode(RtConductor *_conductor, int _codeNum) 
  : conductor(_conductor), codeNum(_codeNum) 
{  }

// destructor
RtCode::~RtCode() {
  
}


// set conductor
void RtCode::setConductor(RtConductor *_conductor) {
  conductor = _conductor;
}

// set the code number
void RtCode::setCodeNum(int _codeNum) {
  codeNum = _codeNum;
}

// get the code number
int RtCode::getCodeNum() {
  return codeNum;
}

// send the code to the handler
void RtCode::sendCode(RtData *data) {
  conductor->receiveCode(codeNum,data);
}

// log a message to the global logfile
void RtCode::log(const string &s) {
  conductor->log(s);
}

// log a message to the global logfile
void RtCode::log(stringstream &s) {
  conductor->log(s);
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


