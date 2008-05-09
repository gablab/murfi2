/******************************************************************************
 * RtIO.h declares a base class for input and output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTIO_H
#define RTIO_H

#include"RtCode.h"
#include"RtConfig.h"
#include"ace/Date_Time.h"

// class declaration
class RtIO : public RtCode {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtIO(); 

  // destructor
  virtual ~RtIO();

  //*** initialization routines  ***//
  
  // opens an io object and initialize it
  //  out:
  //   true (for success) or false
  virtual bool open(RtConfig &config);

  // closes an io object
  //  out:
  //   true (for success) or false
  virtual bool close();
  
  // set the id string
  void setID(const string &s);

  // get the id string
  string getID();

  // prints the current time 
  void printNow(ostream &os);

  // gets the version
  //  out:
  //   cvs version string for this class
  virtual char *getVersionString();
  
protected:

  bool isOpen;
  ACE_Date_Time now;

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


