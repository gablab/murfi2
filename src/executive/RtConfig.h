/******************************************************************************
 * RtConfig.h is the header for a class that controls configuration of
 * a a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONFIG_H
#define RTCONFIG_H


#include<cctype>
#include<cmath>
#include<string>
#include<algorithm>
#include<string>
#include<cstring>
#include<sstream>
#include<map>

using namespace std;

// class to wrap string for a single parm value
class RtConfigVal;
class RtConductor;

// class declaration
class RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfig(); 

  // constructor with conductor reference
  RtConfig(RtConductor &_conductor); 

  // destructor
  virtual ~RtConfig();


  //*** config loading routines ***//

  // parse sommand line args
  bool parseArgs(int argc, char **args);

  // parse config file
  bool parseConfigFile();


  //*** config get/set parms ***/

  // get a parm value
  RtConfigVal &get(const char *name);

  // get a parm value
  //string get(const char *name);

  // set parm
  template<class T>
  void set(const char *name, T tval);

  // set parm
  template<class T>
  void set(const string, T tval);

  // print a parm value
  void print(const char *name);

  //*** general ***//

  // set the conductor
  //  in: _conductor is a pointer to a conductor
  void setConductor(RtConductor *_conductor);

  // get the conductor
  //  out: pointer to the conductor
  RtConductor *getConductor();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

  // get the version string for the conductor reference
  //  out: char array that represents the cvs version for the conductor
  char *getConductorVersionString();


  // print the name/value pairs to the screen
  void dumpConfig(ostream &os);

  // validate the configuration
  // checks for valid setup of different parts of the program
  bool validateConfig();

private:

  // map of config params
  map<string,RtConfigVal> parms;

  // pointer to the conductor
  RtConductor *conductor;

  // prints the usage info for the realtime system
  void printUsage();
};

// a single configuration value
class RtConfigVal {
  
public:

  //*** constructor ***/

  // leave blank
  RtConfigVal() { 
    val = "";
  }

  // assign it straight off
  inline RtConfigVal(string s) {
    val=s;
    //transform(val.begin(),val.end(),val.begin(),(int(*)(int))tolower); 
  }

  string str() {
    return val;
  }

  string getVal() {
    return val;
  }

  //*** operators ***//

  // logical not
  const bool operator!() {
    if(
       val == "0"
       || val == "false"
       || val == ""
       ) {
      return true;
    }
    return false;
  }

  // assignment lvalue
  template<class T>
  RtConfigVal &operator=(const T t) {
    stringstream ss;
    ss << t;
    ss >> val;
    return *this;
  }

  // assignment rvalue
  template<class T>
  operator T()  {
    T t;
    convert<T>(t,val);
    return t;
  }

  // string assignment rvalue
  operator string()  {
    return val;
  }

  // string assignment lvalue
  string operator=(RtConfigVal &config)  {
    return config.val;
  }

  // comparison 
  const bool operator==(const string s) {
    return val == s;
  }

  const bool operator==(const double d) {
    double dval;
    return convert<double>(dval,val) 
      & fabs(d - dval) < dTol;
  }

  const bool operator==(const float f) {
    float fval;
    return convert<float>(fval,val) 
      & fabsf(f - fval) < fTol;
  }

  const bool operator==(const long l) {
    long lval;
    return convert<long>(lval,val) 
      & l == lval;
  }

  const bool operator==(const int i) {
    int ival;
    return convert<int>(ival,val) 
      & i == ival;
  }

  const bool operator==(const bool b) {
    bool bval;
    bool ret = convert(bval,val) 
      & bval == b;
    return ret;
  }

  // outstream operator
 friend ostream &operator<<(ostream &os, const RtConfigVal &v) {
    os << v.val;
    return os;
  }

private:

  string val;

  // tolerances for double and float comparison
  static const float fTol = 0.00000000001f;
  static const double dTol = 0.00000000001;

  // for conversion of string types to other types
  template <class T> inline bool convert(T &t, const string& s) {
    istringstream iss(s);
    return !(iss >> dec >> t).fail();
  }

  // for conversion of string types to bool
  inline bool convert(bool &b, const string& s) {    
    if(
       s == "0"
       || s == "false"
       || s == ""
       ) {
      b = false;
    }
    else {
      b = true;
    }

    return true;
  }
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


