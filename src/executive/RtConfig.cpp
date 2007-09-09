/******************************************************************************
 * RtConfig.cpp defines a class that controls configuration of a a
 * real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConfig.h"
#include"RtConductor.h"


#include<sstream>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>

#define MAX_LINE_CHARS 1000

// defaults
#define DEFAULT_SUBJECTSDIR "/home/mri/subjects"
#define DEFAULT_CONFILENAME "conf/example.conf"


//*** constructors/destructors  ***//

RtConfigVal unset;

// default constructor
RtConfig::RtConfig() : conductor(NULL){
  // nothing to do here
}

// default constructor
RtConfig::RtConfig(RtConductor &_conductor) : conductor(&_conductor) {
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
  string::size_type eqind;
  string pair, name, val;

  // get the executable name
  parms["execName"] = args[0];

  if(argc > 1 &&
     (  !strcmp(args[1],"-h")
     || !strcmp(args[1],"-help")
     || !strcmp(args[1],"--help")
     || !strcmp(args[1],"-?")
     )
     ) {
    printUsage();
    exit(1);
  }

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
  
  if(!parseConfigFile()) {
    return false;
  }

  // dump config to screen if verbose
  if(get("verbose")==true) {
    dumpConfig(cout);
  }

  return true;
}

// parse config file
//  out: true (success) or false
bool RtConfig::parseConfigFile() {

  string name, val;
  string fn = get("confFilename");
  char trash[MAX_LINE_CHARS];
  char next;

  if(fn.empty()) {
    fn =  DEFAULT_CONFILENAME;
  }

  // try to open the config file for reading
  ifstream cf(fn.c_str());
  if(cf.fail()) {
    return false;
  }

  while(!cf.eof()) {
    next = cf.peek();

    // check for comments
    if(next == '#' || next == '\n') {
      cf.getline(trash,MAX_LINE_CHARS);
      continue;
    }

    // get the name/value pair
    cf >> name;
    cf >> val;

    // discard the rest of the line
    cf.getline(trash,MAX_LINE_CHARS);

    parms[name] = val;
  }

  cf.close();


  return true;
}

// validate the configuration
// checks for valid setup of different parts of the program
bool RtConfig::validateConfig() {
  
  bool valid = true;

  // check subject
  if(get("subject")==false) {
    cerr << "ERROR: no subject name specified" << endl;
    valid = false;
  }

  // check subjects directory
  if(get("subjectsDir")==false) {
    set("subjectsDir",DEFAULT_SUBJECTSDIR);
  }

  // check for existance of studyDir
  stringstream studyDir;
  studyDir << get("subjectsDir").str() << "/";
  studyDir << get("subject").str() << "/";
  string s1 = studyDir.str();

  string testfile = s1 +  "/testfile";

  ofstream fs(testfile.c_str());
  if(fs.fail()) {
    cerr << "ERROR: subject directory " << studyDir.str() << " is bad" << endl;
    valid = false;
  }
  else {

    set("studyDir",studyDir.str());
  }
  fs.close();


  // check logfile name
  if(get("disableLogOutput")==false) {
    set("logOutput",true);
    if(get("logFilename")==false) {
      cerr << "WARNING: no logfile name specified" << endl;
      set("logOutput",false);
    }
  }

  // check image receiver
  if(get("disableReceiveScannerImages")==false) {
    set("receiveScannerImages",true);
    if((int) get("scannerPort") < 1 || (int) get("scannerPort") > 65535) {
      cerr << "WARNING: invalid port number for receiving scanner images" 
	   << endl;
      set("receiveScannerImages",false);
    }
  }

  return valid;
}

//*** config get/set parms ***/

// get a parm value
RtConfigVal &RtConfig::get(const char *name) {
  if(name == NULL) {
    return unset;
  }

  map<string,RtConfigVal>::iterator p = parms.find(name);

  if(p == parms.end()) {
    //    return RtConfigVal::UNSET;
    return unset;
  }

  return p->second;
}
//
//// get a parm value
//string RtConfig::get(const char *name) {
//  if(name == NULL) {
//    return unset;
//  }
//
//  map<string,RtConfigVal>::iterator p = parms.find(name);
//
//  if(p == parms.end()) {
//    return unset;
//  }
//
//  return ((RtConfigVal) p->second).getVal();
//}

// set a parm value
template<class T>
void RtConfig::set(const string name, T tval) {
  parms[name] = tval;
}

// set a parm value
template<class T>
void RtConfig::set(const char *name, T tval) {
  string s(name);
  set(s,tval);
}

// print a parm value
void RtConfig::print(const char *name) {
  if(name == NULL) {
    cout << "name is null";
    return;
  }

  map<string,RtConfigVal>::iterator p = parms.find(name);

  if(p == parms.end()) {
    cout << "unset";
    return;
  }

  cout << p->second;
}

// set the conductor
//  in: _conductor is a pointer to a conductor
void RtConfig::setConductor(RtConductor *_conductor) {
  conductor = _conductor;
}

// get the conductor
//  out: pointer to the conductor
RtConductor *RtConfig::getConductor() {
  return conductor;
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
void RtConfig::dumpConfig(ostream &os) {
  map<string,RtConfigVal>::iterator i;
  for(i=parms.begin(); i != parms.end(); i++) {
    os << i->first << "=" << i->second << endl;
  }
}


// gets the version
//  out:
//   cvs version string for this class
char *RtConfig::getVersionString() {
  return VERSION;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtConfig::getConductorVersionString() {
  if(conductor == NULL) {
    return "null";
  }
  return conductor->getVersionString();
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


