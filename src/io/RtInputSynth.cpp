/******************************************************************************
 * RtInputSynth.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id: RtInputSynth.cpp 383 2009-01-14 14:12:45Z alan $";

#include"RtInputSynth.h"
#include"RtDataIDs.h"
#include"RtExperiment.h"

#include<fstream>

using namespace std;

// destructor
RtInputSynth::~RtInputSynth() {

}

// configure and open
bool RtInputSynth::open(RtConfig &config) {
  RtInput::open(config);

  if(config.isSet("inputsynth:tr")) {
    tr = config.get("inputsynth:tr");
  }

  if(config.isSet("inputsynth:numTrs")) {
    numTrs = config.get("inputsynth:numTrs");
  }

  return true;
}

// close and clean up
//  out
//   success flag
bool RtInputSynth::close() {

  if(isOpen) {
    // other stuff
  }

  RtInput::close();

  return true;
}

// run the scanner input
int RtInputSynth::svc() {
  RtActivation *synth;
  stringstream infos;

  for(unsigned int curTr = 0; curTr < numTrs; curTr++) {

    synth = new RtActivation(0);
    synth->getDataID().setModuleID("inputsynth");
    synth->getDataID().setDataName("none");
    synth->getDataID().setTimePoint(curTr+1);

    sendCode(synth);

    // log that we received the image
    infos.str("");
    infos << "generatred synthetic data at tr " << curTr << endl;
    log(infos);
        
    cout << "generatred synthetic data at tr " << curTr << endl;

    usleep(1000000*tr);
  }
  return 0;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtInputSynth::getVersionString() {
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


