/******************************************************************************
 * RtStream.h is the header for a class that controls execution of a
 * single data processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSTREAM_H
#define RTSTREAM_H

#include<vector>
#include<stack>

#include"ace/Stream.h"

//#include"RtConductor.h"
#include"RtCode.h"
//#include"RtData.h"
//#include"RtStreamComponent.h"
//#include"RtStreamMessage.h"
#include"RtConfig.h"

class RtConductor;

using namespace std;

// class declaration
class RtStream : public ACE_Stream<ACE_MT_SYNCH>, public RtCode {

public:

  //*** constructors/destructors  ***//
  
  // constructor
  RtStream(); 

  // destructor
  virtual ~RtStream();

  //*** initialization routines  ***//
  
  // set the conductor for this stream
  //  in
  //   conductor pointer
  void setConductor(RtConductor *_conductor);

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  int configure(RtConfig &config);

  // adds all modules to the stream
  //  in
  //   config: configuration info
  virtual int addModules(RtConfig &config);

  //*** operation routines  ***//

  // accept new data received from an input
  //  in
  //   data: data 
  virtual void setInput(unsigned int code, RtData *data);

  // adds all the modules on the stack to the processing stream
  // the module stack is emtpty after this executes
  //  out
  //   -1 for error, 0 on success
  int pushAllModules();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:
  
  // superclass
  typedef ACE_Stream<ACE_MT_SYNCH> super;

  // processing module
  typedef ACE_Module<ACE_MT_SYNCH> Module;

  // pointer to conductor 
  RtConductor *conductor;

  // stack of moules to be added
  stack<Module*> addMod;
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


