/******************************************************************************
 * RtStream.cpp defines a class that controls execution of a single data
 * processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStream.h"
#include"RtCode.h"
#include"RtProcessor.h"

#include"RtDataIDs.h"

// default constructor

RtStream::RtStream() : super() {

}

// destructor

RtStream::~RtStream() {
    close();
}


//*** initialization routines  ***//

// set the conductor for this stream
//  in
//   conductor pointer

void RtStream::setStreamConductor(RtConductor *_conductor) {
    streamConductor = _conductor;
}


// initialize stream and prepare to run
//  out:
//   true (for success) or false

bool RtStream::configure(RtConfig &config) {

    Module *head = 0, *tail = 0;

    // create the head and tail by passing them up to the superclass
    ACE_NEW_RETURN(tail, Module(ACE_TEXT("end module"), 
				new RtEndTask(&openMsgs)), -1);
    super::open(NULL, head, tail);

    return addModules(config);
}

// exhaustive list of individual processing modules
#include"RtMosaic.h"
#include"RtUnmosaic.h"
#include"RtMaskLoad.h"
#include"RtMaskGen.h"
#include"RtDiff.h"
#include"RtMean.h"
#include"RtVar.h"
#include"RtIntensityNorm.h"
#include"RtPasser.h"
#include"RtSlideWinCor.h"
#include"RtIncrementalGLM.h"
#include"RtCurrentActivation.h"
#include"RtSingleImageCor.h"
#include"RtFluctuationMonitor.h"
#include"RtRoiFeedbackSynth.h"
#include"RtContrast.h"
#include"RtRoiCombine.h"
#include"RtRoiDifference.h"
#include"RtEventTriggerSynth.h"
#include"RtEventTriggerActivationSums.h"
#include"RtEventTriggerActivationDiff.h"

// build a single module
//  in
//   type: name of the module type to add
//   out: optional output to pass the result of this module to
//   text: optional text to be associated with the module

RtStreamComponent *RtStream::buildStreamComponent(const string &type,
                                                  const string &text) {
    RtStreamComponent *sc = NULL;

    // switch amongst module types
    // ALL MODULES MUST BE REGISTERED HERE
    // remember to modify RtDataIDs.h to add the new data id
    if (type == RtPasser::moduleString) { // for original data passer only
        ACE_NEW_NORETURN(sc, RtPasser());
    }
    else if (type == RtMosaic::moduleString) { // mosaic
        ACE_NEW_NORETURN(sc, RtMosaic());
    }
    else if (type == RtUnmosaic::moduleString) { // unmosaic
        ACE_NEW_NORETURN(sc, RtUnmosaic());
    }
    else if (type == RtMaskLoad::moduleString) { // load a mask
        ACE_NEW_NORETURN(sc, RtMaskLoad());
    }
    else if (type == RtMaskGen::moduleString) { // generate a mask
        ACE_NEW_NORETURN(sc, RtMaskGen());
    }
    else if (type == RtDiff::moduleString) { // voxel time difference
        ACE_NEW_NORETURN(sc, RtDiff());
    }
    else if (type == RtMean::moduleString) { // voxel time mean
        ACE_NEW_NORETURN(sc, RtMean());
    }
    else if (type == RtVar::moduleString) { // voxel time variance
        ACE_NEW_NORETURN(sc, RtVar());
    }
    else if (type == RtIntensityNorm::moduleString) { // intensity normalization
        ACE_NEW_NORETURN(sc, RtIntensityNorm());
    }
    else if (type == RtSlideWinCor::moduleString) { // voxel sliding window correlation
        ACE_NEW_NORETURN(sc, RtSlideWinCor());
    }
    else if (type == RtIncrementalGLM::moduleString) { // voxel incremental GLM
        ACE_NEW_NORETURN(sc, RtIncrementalGLM());
    }
    else if (type == RtCurrentActivation::moduleString) { // voxel current activation
        ACE_NEW_NORETURN(sc, RtCurrentActivation());
    }
    else if (type == RtSingleImageCor::moduleString) { // voxel single image correlation
        ACE_NEW_NORETURN(sc, RtSingleImageCor());
    }
    else if (type == RtFluctuationMonitor::moduleString) { // fluctuation monitor
        ACE_NEW_NORETURN(sc, RtFluctuationMonitor());
    }
    else if (type == RtContrast::moduleString) { // activation contrast
        ACE_NEW_NORETURN(sc, RtContrast());
    }
    else if (type == RtRoiCombine::moduleString) { // combine image activation
        ACE_NEW_NORETURN(sc, RtRoiCombine());
    }
    else if (type == RtRoiFeedbackSynth::moduleString) { // activation synth
        ACE_NEW_NORETURN(sc, RtRoiFeedbackSynth());
    }
    else if (type == RtRoiDifference::moduleString) { // activation roi diff
        ACE_NEW_NORETURN(sc, RtRoiDifference());
    }
    else if (type == RtEventTriggerSynth::moduleString) { // trigger synth
        ACE_NEW_NORETURN(sc, RtEventTriggerSynth());
    }
    else if (type == RtEventTriggerActivationSums::moduleString) { // trigger acts
        ACE_NEW_NORETURN(sc, RtEventTriggerActivationSums());
    }
    else if (type == RtEventTriggerActivationDiff::moduleString) { // trigger diff
        ACE_NEW_NORETURN(sc, RtEventTriggerActivationDiff());
    }
    else {
        cerr << "unknown module type: " << type << endl;
        cerr << "adding data passer instead" << endl;
        ACE_NEW_NORETURN(sc, RtPasser());
    }

    // set the conductor
    sc->setConductor(streamConductor);

    return sc;
}

// adds a single module to the stack
//  in
//   stream component for this module

void RtStream::addSingleModule(RtStreamComponent *sc) {
    Module *mod;
    ACE_NEW_NORETURN(mod, Module(ACE_TEXT(sc->getID().c_str()), sc));
    addMod.push(mod);
}

// adds modules to the stream
//  in
//   config: configuration info

bool RtStream::addModules(RtConfig &config) {
    ACE_TRACE(("RtStream::addModules"));

    // add the processing module
    Module *procMod;
    RtProcessor *proc = new RtProcessor();
    ACE_NEW_RETURN(procMod, Module(ACE_TEXT("processing module"), proc), -1);

    proc->setStreamConductor(streamConductor);
    proc->configure(config);

    if (this->push(procMod) == -1) {
        cerr << "ERROR: failed to add processing modules" << endl;
        return false;
    }

    return true;
}

// adds all 'module' nodes that are children of the passed node as modules
// for the stream
//  in
//   elmt: xml element

void RtStream::addModulesFromNode(TiXmlElement *elmt, RtConfig *config) {
    string name;
    RtStreamComponent *sc;
    TiXmlElement *childElmt;

    // build the outputs at the top level
    vector<string> topLevelOutNames;
    buildOutputNames(elmt, topLevelOutNames);
    if (!topLevelOutNames.empty()) {
        sc = buildStreamComponent(RtPasser::moduleString);
        addOutputsToComponent(sc, topLevelOutNames);
        addSingleModule(sc);
        cout << "added stream " << sc->getID() << endl;
    }

    TiXmlNode *child = 0;
    while ((child = elmt->IterateChildren("module", child))) {
        if (child->Type() != TiXmlNode::ELEMENT) continue;

        childElmt = (TiXmlElement*) child;

        // send
        if (TIXML_SUCCESS == childElmt->QueryValueAttribute("name", &name)) {
            // add the component
            sc = buildStreamComponent(name);
            if (sc == NULL) {
                cout << "failed to build stream: " << name << endl;
                continue;
            }

            // allow the stream component to configure itself
            if (!sc->init(childElmt, config, streamConductor)) {
                cout << "failed to configure stream: " << name << endl;
                //delete sc;
                continue;
            }

            // build the outputs
            vector<string> outNames;
            buildOutputNames(childElmt, outNames);
            addOutputsToComponent(sc, outNames);

            // create and add the module to the stack
            addSingleModule(sc);
            cout << "added stream " << sc->getID() << endl;
        }
    }

}


// adds outputs to a stream component (needs to be here so that we have
// access to the conductor to get pointers to the outputs)
//  in 
//   sc: stream component to add the outputs to

void RtStream::addOutputsToComponent(RtStreamComponent *sc,
                                     vector<string> &outNames) {
    for (vector<string>::iterator i = outNames.begin(); i != outNames.end(); i++) {
        vector<RtOutput*> outs = streamConductor->getAllOutputsWithName(*i);
        //cout<< "adding " << outs.size() << " outputs " << *i << " to " << sc->getID() << endl;
        sc->addVectorOfOutputs(outs);
    }
}


// build a vector of ids of outputs from an xml node's children
//  in
//   module element
//  out 
//   vector of string output ids

void RtStream::buildOutputNames(TiXmlElement *module, vector<string> &names) {
    TiXmlNode *child = 0;
    while ((child = module->IterateChildren("output", child))) {
        if (child->Type() != TiXmlNode::ELEMENT) continue;

        // extract the text from this element
        // CONSIDER USING CHILD ITERATORS HERE
        string outid = "output:";
        outid += ((TiXmlElement*) child)->GetText();
        names.push_back(outid);
    }
}

// accept new data received from an input
//  in
//   data: data
void RtStream::setInput(unsigned int code, RtData *data) {
  ACE_TRACE(("RtStream::setInput"));

  // take action based on the kind of new input
  switch (code) {
   case SCANNER_IMAGE_RECEIVED:

     if(data == NULL) {
       cout << "refusing to start a stream with NULL data" << endl;
       break;
     }

//     if(DEBUG_MASK & TIMER) {
//       cout << "RtStream start: " << getExperimentElapsedTime() << endl;
//     }

     // if this code is from a new scanner image, start the processing
     ACE_Message_Block *mb;
     ACE_NEW_NORETURN(mb, ACE_Message_Block(sizeof (RtStreamMessage)));
     
     RtStreamMessage *msg;
     ACE_NEW_NORETURN(msg, RtStreamMessage());
     
     msg->init(streamConductor);
     msg->addData(data);
     
     memcpy(mb->wr_ptr(), msg, sizeof (RtStreamMessage));
     mb->wr_ptr(sizeof (RtStreamMessage));
     
     openMsgs.insert(mb);
  
     //cerr << "got data " << data << " and put it in a msg " << msg << endl;
     
     // RtStreamMessage *m = (RtStreamMessage*) mb->rd_ptr();
     //cerr << "retrieve data from msg gives " << m->getCurrentData() << endl;
     this->put(mb);

     break;
  }
}


// adds all the modules on the stack to the processing stream
// the module stack is emtpty after this executes
//  out
//   -1 for error, 0 on success

bool RtStream::pushAllModules() {
    ACE_TRACE(("RtStream::pushAllModules"));

    // add all the modules from the stack
    while (!addMod.empty()) {
        if (this->push(addMod.top()) == -1) {
            cerr << "failed to add a module" << endl;
            return false;
        }

        addMod.pop();
    }

    return true;
}



// gets the version
//  out:
//   cvs version string for this class

char *RtStream::getVersionString() {
    return VERSION;
}



/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


