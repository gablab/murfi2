/******************************************************************************
 * RtConfig.h is the header for a class that controls configuration of
 * a a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONFIG_H
#define RTCONFIG_H


#include<string>
#include<map>

using namespace std;

// clases for individual configuration modules

// class declaration
class RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfig(); 

  // destructor
  ~RtConfig();

  //*** config loading routines ***//

  // parse sommand line args
  bool parseArgs(int argc, char **args);

  // parse config file
  bool parseConfigFile();

  //*** config get/set parms ***/

  // get a parm value
  string getParm(string name);

  // set a parm value (string)
  void setParm(string name, string val);

  // set a parm value (int)
  void setParm(string name, int val);

  // set a parm value (double)
  void setParm(string name, double val);

  // set a parm value (bool)
  void setParm(string name, bool val);

private:

  // map of config params
  map<string,string> parms;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


