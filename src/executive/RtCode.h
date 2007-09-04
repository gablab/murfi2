/******************************************************************************
 * RtCode.h is the header for an abstract class that 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCODE_H
#define RTCODE_H

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
  void sendCode();

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


