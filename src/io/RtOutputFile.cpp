/******************************************************************************
 * RtOutputfile.cpp defines a class that implements output operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";


#include"RtOutputFile.h"
#include<iostream>
#include<sstream>

#include"ace/Mutex.h"

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
  if(config.get("info:log:disabled")==true) {
    return true;
  }
  
  // open the file for output
  string logname = config.get("info:log:filename");

  // check the filename
  if(logname.empty()) {
    return false;
  }
  else if(logname.substr(0,1) != "/") {
    stringstream fs;
    fs << config.get("study:dir") << "/"  << logname;
  
    logname = fs.str();
  }

  cout << "attempting to open logfile " << logname << endl;
  outfp.open(logname.c_str(), fstream::out | fstream::app);

  // check 
  if(outfp.fail()) {
    cerr << "ERROR: could not open log file " << logname << " for output" << endl;
    return false;
  }

  // write a header
  ACE_Mutex mutx;
  mutx.acquire();
  
  outfp << "################################################################"
	<< endl << endl
	<< "# realtime system log file" << endl
	<< "# " << getVersionString() << endl
	<< "# " << config.getVersionString() << endl
	<< "# " << config.getConductorVersionString() << endl << endl
	<< "created ";
  printNow();
  outfp  << endl << endl;

  outfp.flush();
  mutx.release();

  return true;
}

// write the string from a stringstream to the output file
void RtOutputFile::write(stringstream &ss) {
  write(ss.str());
}

// write a string to the output file
void RtOutputFile::write(const string &s) {
  if(!isOpen) {
    return;
  }

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << s;

  outfp.flush();

  mutx.release();
}

// close and clean up
bool RtOutputFile::close() {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << "closed ";
  RtOutput::printNow(outfp);
  outfp << endl;
  outfp << endl;

  outfp << "################################################################"
	<< endl << endl;

  outfp.close();

  isOpen = false;

  mutx.release();

  return true;
}

// prints the current time to the file
void RtOutputFile::printNow() {
  RtOutput::printNow(outfp);
}

// prints the configuration to the file
void RtOutputFile::writeConfig(RtConfig &config) {

  ACE_Mutex mutx;

  // make sure noone else writes while we are
  mutx.acquire();

  outfp << "configuration:" << endl 
	<< "--------------" << endl;
  config.dumpConfig(outfp);
  outfp << "--------------" << endl << endl;

  mutx.release();
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


