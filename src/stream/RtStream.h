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

#include"ace/Stream.h"

#include"RtCode.h"
#include"RtData.h"
#include"RtStreamComponent.h"
#include"RtStreamMessage.h"

using namespace std;

// class declaration
class RtStream : public ACE_Stream<ACE_MT_SYNCH>, public RtCode {

public:

  //*** constructors/destructors  ***//
  
  // constructor
  RtStream(RtConductor *_conductor); 

  // destructor
  virtual ~RtStream();

  //*** initialization routines  ***//
  
  // set the conductor for this stream
  //  in
  //   conductor pointer
  void setConductor(RtConductor *_conductor);

  // adds a component to the processing pipeline
  //  in:
  //   component: component object
  //  out:
  //   true (for success) or false
  //bool addComponent(RtStreamComponent &component);

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  virtual bool open(RtConfig &config);

  //*** operation routines  ***//

  // accept new data received from an input
  //  in
  //   data: data 
  virtual void setInput(unsigned int code, RtData *data);

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

  // begins stream processing
  //  out:
  //   true (for success) or false
  //virtual bool run(RtData &data);

  // vector of components objects
  //vector<RtStreamComponent> components;

  // here we need a way to represent serial dependencies in processing
  // maybe a graph?

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


