/******************************************************************************
 * RtRoiFeedbackSynth.cpp computes the mean over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoiFeedbackSynth.h"
#include "RtDataIDs.h"

#include"boost/random/normal_distribution.hpp"
#include"boost/random/lagged_fibonacci.hpp"


string RtRoiFeedbackSynth::moduleString(ID_ROIFEEDBACKSYNTH);

// default constructor
RtRoiFeedbackSynth::RtRoiFeedbackSynth() : RtRoi2Feedback() {
  componentID = moduleString;

  mean = 0;
  sd = 1;
}

// destructor
RtRoiFeedbackSynth::~RtRoiFeedbackSynth() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoiFeedbackSynth::processOption(const string &name, 
					   const string &text,
					   const map<string,string> &attrMap) {
  if(name == "mean") {
    return RtConfigVal::convert<float>(mean,text);
  }
  else if(name == "sd") {
    return RtConfigVal::convert<float>(sd,text);
  }

  return RtRoi2Feedback::processOption(name, text, attrMap);
}  

// process a single acquisition
int RtRoiFeedbackSynth::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiFeedbackSynth::process"));

  static boost::normal_distribution<float> rnd(mean, sd);
  static boost::lagged_fibonacci19937 engine;
  numTimepoints++;

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // create a one element activation image
  RtActivation *act = new RtActivation(1);

  // setup the data id
  act->getDataID().setModuleID(componentID);
  act->getDataID().setDataName(NAME_ROIVAL);
  act->getDataID().setRoiID(roiID);
  act->getDataID().setTimePoint(numTimepoints);

  act->setPixel(0, rnd.operator()<boost::lagged_fibonacci19937>((engine)));
  
  setResult(msg, act);

  // log the activation
  stringstream logs("");
  logs << "activation synth: " << numTimepoints << ":" << roiID  
       << " " << act->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << tr << " " << act->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & TEMP) {
    cout << "activation synth: " << numTimepoints << " " << act->getPixel(0) << endl;
    cout.flush();
  }


  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


