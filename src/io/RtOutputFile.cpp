/******************************************************************************
 * RtOutputfile.cpp defines a class that implements output operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";


#include"RtOutputFile.h"

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
  string fn = config.get("logFilename");

  // check the filename
  if(fn.empty()) {
    return false;
  }

  fp.open(fn.c_str(),fstream::out);

  // check 
  if(fp.fail()) {
    return false;
  }

  // write a header
  fp << "# realtime system log file" << endl
     << "# " << getVersionString() << endl
     << "# " << config.getVersionString() << endl
     << "# " << config.getConductorVersionString() << endl
     << "created ";
  RtOutput::printNow(fp);
  fp  << endl << endl;

  fp.flush();

  return true;
}

// close and clean up
bool RtOutputFile::close() {

  fp << "closed ";
  RtOutput::printNow(fp);
  fp  << endl;

  fp.close();

  return true;
}

// prints the current time to the file
void RtOutputFile::printNow() {
  RtOutput::printNow(fp);
}

// prints the configuration to the file
void RtOutputFile::writeConfig(RtConfig &config) {
  if(isOpen) {
    fp << "configuration:" << endl 
       << "--------------" << endl;
    config.dumpConfig(fp);
    fp << "--------------" << endl << endl;
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


