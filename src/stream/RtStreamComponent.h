/******************************************************************************
 * RtStreamComponent.h is the header for an abstract class for a
 * processing stream component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSTREAMCOMPONENT_H
#define RTSTREAMCOMPONENT_H

#include"ace/Task.h"

#include"RtStream.h"
#include"RtStreamMessage.h"
#include"RtConfig.h"
#include"RtData.h"

// class declaration
class RtStreamComponent : public ACE_Task<ACE_MT_SYNCH> {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtStreamComponent();

  // destructor
  virtual ~RtStreamComponent();

  //*** initialization routines  ***//

  // initialize and run thread
  //  out:
  //   0 (for success) or -1 (failure)
  virtual int open(void * = 0);

  // send data when we're done
  //  in 
  //   msg: message to send
  //   to:  timeout
  virtual int put(ACE_Message_Block *msg, ACE_Time_Value *to);

  // close a stream component
  //  in
  //   flags: flags to tell us who called
  virtual int close(u_long flags);
  
  // run the stream
  virtual int svc();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


protected:

  typedef ACE_Task<ACE_MT_SYNCH> super;

  // call the next processing step
  virtual int nextStep(ACE_Message_Block *mb);

  // pure virtual for implementation of real processing
  virtual int process(RtStreamMessage *msg) = 0;

};


class RtEndTask : public RtStreamComponent {
protected:
  virtual int process(RtStreamMessage*) {
    return 0;
  }

  virtual int nextStep(ACE_Message_Block *) {
    return 0;
  }
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


