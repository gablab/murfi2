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
#include<iomanip>
#include<fstream>

  //*** constructors/destructors  ***//
  
RtConfigVal unset;

// default constructor
RtConfig::RtConfig() {
  // nothing to do here
}

// destructor
RtConfig::~RtConfig() {
  // nothing to do here
}

//*** config loading routines ***//

// parse command line args
bool RtConfig::parseArgs(int argc, char **args) {
  int i;
  unsigned int eqind;
  string pair, name, val;

  // get the executable name
  parms["execName"] = args[0];

  // loop over args and parse them into name/value pairs
  for(i = 1; i < argc; i++) {
    
    // make it a string and find the equals sign delimiter
    pair.assign(args[i]);
    eqind = pair.find("=");

    // equals not found, error
    if(eqind == string::npos || eqind == 0) {
      printUsage();
      exit(1);
    }

    // split the string into name and val and assign it
    set(pair.substr(0,eqind), pair.substr(eqind+1));
  }

  // dump config to screen if verbose
  if(get("verbose")==true) {
    dumpConfig();
  }

  return true;
}

// parse config file
bool RtConfig::parseConfigFile() {
  return true;
}

//*** config get/set parms ***/

// get a parm value
RtConfigVal &RtConfig::get(string name) {
  map<string,RtConfigVal>::iterator p = parms.find(name);
  
  if(p == parms.end()) {
    //    return RtConfigVal::UNSET;
    return unset;
  }

  return p->second; 
}

// set a parm value
template<class T>
void RtConfig::set(string name, T tval) {
  parms[name] = tval;
}

//*** private functions ***//

// prints the usage info for the realtime system
void RtConfig::printUsage() {
  int w = 15;

  cout << "usage: " << endl << get("execName") 
       << " [name1=val1 name2=val2 ...]" << endl << endl
       << "---------------------------------------------" << endl
       << "some useful flags:" << endl
       << setiosflags(ios::left) 
       << setw(w) << "conf:"     << "configuration filename" << endl
       << setw(w) << "verbose:"  << "enable/disable verbose mode" << endl
       << "---------------------------------------------" << endl;
}

// print the name/value pairs to the screen
void RtConfig::dumpConfig() {
  map<string,RtConfigVal>::iterator i;
  for(i=parms.begin(); i != parms.end(); i++) {
    cout << i->first << "=" << i->second << endl;
  }
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


