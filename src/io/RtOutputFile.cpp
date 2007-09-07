/******************************************************************************
 * RtOutputfile.cpp defines a class that implements output operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";


#include"RtOutputFile.h"
#include<iostream>

// default constructor
RtOutputFile::RtOutputFile() : RtOutput() {
}

// destructor
RtOutputFile::~RtOutputFile() {
}

// open and start accepting input
bool RtOutputFile::open(RtConfig &config) {

  if(!RtOutput::open(config)) {
    return false;
  }

  // if logging is off, return
  if(config.get("noLogging")==true) {
    return true;
  }
  
  // open the file for output
  string fn, logname = config.get("logFilename");

  // check the filename
  if(fn.empty()) {
    return false;
  }
  else if(fn.substr(0,1) != "/") {
    fn = (char*) config.get("studyDir");
    fn += logname;
  }

  cout << "attempting to open logfile " << fn << endl;
  outfp.open(fn.c_str(),fstream::out);

  // check 
  if(outfp.fail()) {
    cerr << "ERROR: could not open log file " << fn << " for output" << endl;
    return false;
  }

  // write a header
  outfp << "# realtime system log file" << endl
	<< "# " << getVersionString() << endl
	<< "# " << config.getVersionString() << endl
	<< "# " << config.getConductorVersionString() << endl
	<< "created ";
  printNow();
  outfp  << endl << endl;

  outfp.flush();

  return true;
}

// close and clean up
bool RtOutputFile::close() {

  outfp << "closed ";
  RtOutput::printNow(outfp);
  outfp  << endl;

  outfp.close();

  return true;
}

// prints the current time to the file
void RtOutputFile::printNow() {
  RtOutput::printNow(outfp);
}

// prints the configuration to the file
void RtOutputFile::writeConfig(RtConfig &config) {
  if(isOpen) {
    outfp << "configuration:" << endl 
	  << "--------------" << endl;
    config.dumpConfig(outfp);
    outfp << "--------------" << endl << endl;
  }
}

// gets the version
//  out:
//   cvs version string for this class
char *RtOutputFile::getVersionString() {
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


