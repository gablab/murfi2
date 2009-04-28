/******************************************************************************
 * RtInput.h declares a class that implements input operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTINPUT_H
#define RTINPUT_H

//#include <sigc++/sigc++.h>

#include"ace/Task.h"

#include"RtIO.h"
#include"RtData.h"
#include"RtConfig.h"

// class declaration
class RtInput : public RtIO, public ACE_Task_Base {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInput(); 

  // destructor
  virtual ~RtInput();
  
  // set the activatable property
  void setActivatable(bool _activatable) { activatable = _activatable; }

  // get the activatable property
  bool isActivatable() { return activatable; }

  // opens an io object and initialize it
  //  out:
  //   true (for success) or false
  virtual bool open(RtConfig &config);

  // closes both the io object and task base
  //  out:
  //   true (for success) or false
  virtual bool close();

  // set the id string
  void setID(const string &s);

  // add a string to the input id
  void addToID(string add);

  // get the id string
  string getID();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** private data members  ***//

  // callback to receive the input
  //sigc::signal<void, RtData&> callback;
  
  string inputID;
  bool activatable;

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


