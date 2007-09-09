/******************************************************************************
 * RtCode.h is the header for an abstract class that 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCODE_H
#define RTCODE_H

#include"RtData.h"
#include<sstream>
#include<string>

using namespace std;

// explicitly defined codes (not the only ones

#define START_CODE_STREAM 0
#define START_CODE_INPUTS 1
#define SCANNER_IMAGE_RECEIVED START_CODE_INPUTS
#define START_CODE_OUTPUTS 32768


class RtConductor;

class RtCode {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtCode();
  
  // constructor w/ codenum
  RtCode(RtConductor *_conductor, int _codeNum);

  // destructor
  virtual ~RtCode();

  // set conductor
  void setConductor(RtConductor *_conductor);

  // set the code number
  void setCodeNum(int _codeNum);

  // get the code number
  int getCodeNum();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() {
    return "$Id$";
  }

protected:

  // send the code to the handler
  void sendCode(RtData *data);

  // log a message to the global logfile
  void log(const string &s);

  // log a message to the global logfile
  void log(stringstream &s);

  // conductor instance to send codes to
  RtConductor *conductor;

  // code number
  int codeNum;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


