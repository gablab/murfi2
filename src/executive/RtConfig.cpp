/******************************************************************************
 * RtConfig.cpp defines a class that controls configuration of a a
 * real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtConfig.h"

#include<sstream>
#include<iostream>
#include<fstream>

  //*** constructors/destructors  ***//
  
// default constructor
RtConfig::RtConfig() {
  // nothing to do here
}

// destructor
RtConductor::~RtConfig() {
  // nothing to do here
}

//*** config loading routines ***//

// parse command line args
bool RtConfig::parseArgs(int argc, char **args) {
  int i, eqind;
  string pair, name, val;

  // get the executable name
  parms["execName"] = args[0];

  // loop over args and parse them into name/value pairs
  for(i = 1; i < argc; i++) {
    
    // make it a string and find the equals sign delimiter
    pair.assign(args[i]);
    eqind = pair.find("=");

    // equals not found, error
    if(eqind == string::npos) {
      RtConductor::usage();
      exit(1);
    }

    // split the string into name and val and assign it
    name = pair.substr(1,eqind-1);
    val  = pair.substr(eqind+1);

    parms[name] = value;

    if(VERBOSE) {
      cout << name << "=" << value << endl;
    }
  }

  return true;
}

// parse config file
bool RtConfig::parseConfigFile() {
  return true;
}

//*** config get/set parms ***/

// get a parm value
string RtConfig::getParm(string name) {
  map<string,string>::iterator p = parms.find(name);
  
  if(p == parms.end()) {
    return "";
  }

  return p->second; 
}

// set a parm value (string)
void RtConfig::setParm(string name, string val) {
  stringstream ss;
  string s;
  ss << val;
  ss >> parms[name];
}

// set a parm value (int)
void RtConfig::setParm(string name, int val) {
  stringstream ss;
  string s;
  ss << val;
  ss >> parms[name];
}

// set a parm value (double)
void RtConfig::setParm(string name, double val) {
  stringstream ss;
  string s;
  ss << val;
  ss >> parms[name];
}

// set a parm value (bool)
void RtConfig::setParm(string name, bool val) {
  stringstream ss;
  string s;
  ss << val;
  ss >> parms[name];  
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


