/******************************************************************************
 * RtStreamComponent.h is the header for an abstract class for a
 * processing stream component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStreamComponent.h"
#include"RtPasser.h"
#include"ace/Mutex.h"

string RtStreamComponent::moduleString("generic-stream-component");
  
// default constructor
RtStreamComponent::RtStreamComponent() : super(), 
    persistent(false), putResultOnMessage(false) {
  componentID = moduleString;
  inputDataID = "data.mri.image";
  makeCurrentData = false;
  passer = NULL;
  outFilename = "";
}

// destructor
RtStreamComponent::~RtStreamComponent() {
  ofile.close();
}

//*** initialization routines  ***//

// configure this stream component
//  in
//   xml module node from which to read <option>s
void RtStreamComponent::init(TiXmlElement *module, RtConfig *config) {
  ACE_TRACE(("RtStreamComponent::init"));

  // process config info for cross module and global config info
  processConfig(*config);

  // store the conductor
  conductor = config->getConductor();

  string name;
  TiXmlElement *optionElmt;

  // iterate over options
  for(TiXmlNode *option = 0;
      (option = module->IterateChildren("option", option)); ) {
    if(option->Type() != TiXmlNode::ELEMENT) continue;

    optionElmt = (TiXmlElement*) option;

    if(TIXML_SUCCESS != optionElmt->QueryValueAttribute("name", &name)) {
      continue;
    }

    // figure out which option we have and process it
    processOption(name, optionElmt->GetText(), optionElmt);
  }

  // open outfile if name set
  if(outFilename != "") {
    ofile.open(outFilename.c_str(),ios_base::app);
    ofile << "started at ";
    printNow(ofile);
    ofile << endl;
  }

  if(!finishInit()) {
    cerr << "initialization failed" << endl;
    
  }
}

// process the configuration: only use this for cross module or global config
// that is not available in the xml node for this stream component
//  in 
//   config class
bool RtStreamComponent::processConfig(RtConfig &config) {
  return true;
}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtStreamComponent::processOption(const string &name, const string &text) {
  if(name == "input") {
    inputDataID = text;
    return true;
  }
  if(name == "makecurrent") {
    bool ret =  RtConfigVal::convert<bool>(makeCurrentData, text);
    return ret;
  }
  if(name == "makeavail") {
    bool ret =  RtConfigVal::convert<bool>(putResultOnMessage, text);
    return ret;
  }
  if(name == "makeoutput") {
    return conductor->addOutput(this);
  }
  if(name == "outputfile") {
    outFilename = text;
    return true;
  }

  return false;
}  

// adds an output to receive the data of this stream component
//  in
//   output to add
void RtStreamComponent::addOutput(RtOutput *out, const string &dataId) {
  if(out == NULL) return;

  if(passer == NULL) {
    ACE_NEW_NORETURN(passer, RtPasser(dataId));
  }

  passer->addOutput(out);
}

// adds outputs from a vector
//  in:
//   out: vector of output objects
//  out:
//   true (for success) or false
void RtStreamComponent::addVectorOfOutputs(vector<RtOutput*> &outs) {
  //cout << "adding " << outs.size() << " outputs to " << getID() << endl;
  for(vector<RtOutput*>::iterator i = outs.begin(); i != outs.end(); i++) {
    addOutput(*i);
  }
}

// finish initialization for prepare to run
bool RtStreamComponent::finishInit() {
  return true;
}

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

// pass any results to outputs
//  in: pointer to data to pass
void RtStreamComponent::passData(RtData* data) {
  if(passer != NULL) {
    passer->sendToOutputs(data);
  }
}

// sets the latest result of processing
//  in
//   data result
void RtStreamComponent::setResult(RtStreamMessage *msg, RtData *data) {
  ACE_Mutex mut;
  mut.acquire();

  if(putResultOnMessage) {
    msg->addData(data);

    if(makeCurrentData) {
      msg->setLastDataAsCurrent();
    }
  }

  passData(data);
  mut.release();
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


// gets the id for this stream component
string RtStreamComponent::getID() {
  return componentID;
}

// gets the data id for the input
string RtStreamComponent::getInputDataID() {
  return inputDataID;
}

// sets the data id for the input
void RtStreamComponent::setInputDataID(string id) {
  inputDataID = id;;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


