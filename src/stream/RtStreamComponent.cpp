/******************************************************************************
 * RtStreamComponent.h is the header for an abstract class for a
 * processing stream component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStreamComponent.h"

string RtStreamComponent::moduleString("generic-stream-component");
  
// default constructor
RtStreamComponent::RtStreamComponent() : super(), persistent(false) {
  id = "RtStreamComponent";
}

// destructor
RtStreamComponent::~RtStreamComponent() {

}

//*** initialization routines  ***//

// initialize and run thread
//  out:
//   0 (for success) or -1 for failure
int RtStreamComponent::open(void *p) {
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("activating stream component\n")));
  return this->activate(THR_NEW_LWP | THR_JOINABLE, 1);
}


// send data when we're done
//  in 
//   msg: message to send
//   to:  timeout
int RtStreamComponent::put(ACE_Message_Block *msg, ACE_Time_Value *to) {
  ACE_TRACE(("RtStreamComponent::put"));

  return this->putq(msg,to);
}
  
// close a stream component
//  in
//   flags: flags to tell us who called
int RtStreamComponent::close(u_long flags) {
  ACE_TRACE(("RtStreamComponent::close"));

  if(flags == 1) { // stream close
    ACE_Message_Block *hangup = new ACE_Message_Block();
    hangup->msg_type(ACE_Message_Block::MB_HANGUP);
    if(this->putq (hangup) == -1) {
      hangup->release ();
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Task::close() putq")), -1);
    }
    
    return this->wait();
  }
  
  return 0;
}

// run the processing
int RtStreamComponent::svc() {
  ACE_TRACE(("RtStreamComponent::svc"));

  ACE_Message_Block *msg = 0;

  // wait for stuff to do
  while(1) {

    if(this->getq(msg) == -1) {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("getq returned -1\n")),-1);
    }

    // handle hangup
    if(msg->msg_type () == ACE_Message_Block::MB_HANGUP) {
      if(this->putq(msg) == -1) {
	ACE_ERROR((LM_ERROR, ACE_TEXT ("Task::svc() putq")));
	msg->release();
      }
      break;
    }

    // process normally
    //RtStreamMessage *strMsg = (RtStreamMessage*) msg->rd_ptr();
    if(this->process(msg) == -1) {
      msg->release();
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("process returned -1")), -1);
    }

    // send to the next component
    if(this->nextStep(msg) < 0) {
      ACE_ERROR((LM_ERROR, ACE_TEXT ("put_next failed")));
      msg->release();
      break;
    }
  }

  return 0;
}

// tells the next step to begin processing 
int RtStreamComponent::nextStep(ACE_Message_Block *mb) {
  ACE_TRACE(("RtStreamComponent::next_step"));
  return this->put_next(mb);
}


// get the version
//  out: char array that represents the cvs version
char *RtStreamComponent::getVersionString() {
  return VERSION;
}

// gets the string to match module names to during config
//string &RtStreamComponent::getModuleString() {
//  return moduleString;
//}

// sets whether this data should be kept around after the stream is done
void RtStreamComponent::setPersistent(bool p) {
  persistent = p;
}

// gets whether this data should be kept around after the stream is done
bool RtStreamComponent::getPersistent() {
  return persistent;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


