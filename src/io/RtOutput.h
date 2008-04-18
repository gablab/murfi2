/******************************************************************************
 * RtOutput.h declares a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTOUTPUT_H
#define RTOUTPUT_H

#include"RtIO.h"
#include"RtConfig.h"
#include"RtData.h"
#include"ace/Date_Time.h"
#include<ostream>

// class declaration
class RtOutput : public RtIO {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutput(); 

  // destructor
  virtual ~RtOutput();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // hand of some data to be output
  virtual void setData(RtData *data);

  // set the id string
  void setID(const string &s);

  // add a string to the output id
  void addToID(string add);

  // get the id string
  string getID();

  // prints the current time 
  void printNow(ostream &os);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** private data members  ***//

  string outputID;
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


