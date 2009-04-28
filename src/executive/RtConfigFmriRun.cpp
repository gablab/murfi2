/******************************************************************************
 * RtConfigFmriRun.cpp defines a class that controls configuration of
 * a single real-time fMRI experimental run.
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-11
 *
 *****************************************************************************/

static char *VERSION = "$Id: RtConfig.cpp 268 2008-10-13 19:54:35Z ohinds $";

#include"RtConfigFmriRun.h"
#include"RtLimit.h"

#include"ace/Trace.h"

// defaults
static const unsigned int DEFAULT_SCANNERPORT = 15000;
static const string DEFAULT_IMAGETYPE = "epi";
static const string DEFAULT_STUDYREFNAME(  "study_ref");

// set default config info
void RtConfigFmriRun::setDefaults() {

  set("scanner:disabled",false);
  set("scanner:receiveImages",true);
  set("scanner:port", DEFAULT_SCANNERPORT);

  set("scanner:saveImages",true);
  set("scanner:onlyReadMoco",true);
  set("scanner:unmosaic",true);

}

// validate the configuration
// checks for valid setup of different parts of the program
// returns true for success
bool RtConfigFmriRun::validateConfig() {
  //ACE_TRACE(("RtConfigFmriRun::validateConfig"));

  bool valid = true;
  stringstream ss;

  // check image receiver
  if(get("scanner:disabled")==false) {
    if((int) get("scanner:port") < 1 
       || (int) get("scanner:port") > MAX_TCPIP_PORT_NUM) {
      cerr << "WARNING: invalid port number for receiving scanner images"
	   << endl;
      set("scanner:receiveImages",false);
    }
  }

  // check optional acquisition parameters
  if(!isSet("scanner:imageType")==false) {
    set("scanner:imageType",DEFAULT_IMAGETYPE);
  }

  // check required acquisition parameters
  if(!isSet("scanner:tr")) {
    cerr << "ERROR: tr must be set!" << endl;
  }

  if(!isSet("scanner:measurements")) {
    cerr << "ERROR: number of measurements must be set!" << endl;
  }

  if(!isSet("scanner:matrixSize")) {
    cerr << "ERROR: matrix size must be set!" << endl;
  }

  if(!isSet("scanner:slices")) {
    cerr << "ERROR: slices must be set!" << endl;
  }

  if(!isSet("scanner:voxdim1")) {
    cerr << "ERROR: voxdim1 must be set!" << endl;
  }

  if(!isSet("scanner:voxdim2")) {
    cerr << "ERROR: voxdim2 must be set!" << endl;
  }

  if(!isSet("scanner:voxdim3")) {
    cerr << "ERROR: voxdim3 must be set!" << endl;
  }

  // study reference volume
  if(!isSet("study:xfm:referenceVol")) {
    path p;
    p.operator=(get("study:xfm:directory").filepath()
		/ (DEFAULT_STUDYREFNAME + "." 
		   + get("study:volumeFormat").str()));
    cout << "using default study reference volume name " << p.string() << endl;
    set("study:xfm:referenceVol",p.string());
  }

  return valid;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtConfigFmriRun::getVersionString() {
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


