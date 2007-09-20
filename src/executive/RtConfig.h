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
#include<cstring>
#include"tinyxml/tinyxml.h"
#include"RtConfigVal.h"

using namespace std;

// class to wrap string for a single parm value
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

  // get a parm value.
  //  in
  //   name is a ':' delimited string spcifying a path into the XML config
  //        for example preprocessor:module:name
  //  out
  //   string representing the value
  RtConfigVal get(const char *name);

  // get a parm value. start searching from a specified node
  //  in
  //   name is a ':' delimited str spcifying a path into the XML configuration
  //        for example preprocessor:module:name
  //   node is a xml node to start looking at
  //  out
  //   string representing the value
  RtConfigVal get(const string &name, TiXmlNode *node = NULL);

  // get an xml node in the config. start searching from a specified node
  //  in
  //   name is a ':' delimited str spcifying a path into the XML configuration
  //        for example preprocessor:module:name
  //   node is a xml node to start looking at
  //  out
  //   string representing the value
  TiXmlNode *getNode(const string &name, TiXmlNode *node = NULL);

  // determine if there is a value set for a particular config variable
  //  in
  //   the variable name to check
  //  out
  //   true if the var has been set
  bool isSet(const string &name);

  // get a parm value
  //string get(const char *name);

  // set parm
  template<class T>
  bool set(const char *name, T tval);

  // set a parm value
  template<class T>
  bool set(const string name, T tval);


  // sets a parm value starting from a specified xml node
  // children are created appropritately
  //  in
  //   name:  ':' delimited string representing the xml node attribute to set
  //   value: the value to set the attribute to
  //   node:  the xml node to start from
  //  out: success or failure
  bool set(const string name, const string value, TiXmlNode *node);

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
  void dumpConfig(ostream &os = cout);

  // validate the configuration
  // checks for valid setup of different parts of the program
  bool validateConfig();

private:

  const RtConfigVal unset;

  // xml doc with all the config stuff
  TiXmlDocument parms;

  // name of the xml config file to read
  string confFilename;

  // pointer to the conductor
  RtConductor *conductor;

  // for conversion of string types to other types
  template <class T> inline bool convert(T &t, const string& s);

  // prints the usage info for the realtime system
  void printUsage();

  //**************************************************//
  // stuff to support printing to a stream
  // taken from the tinyXml examples
  // 

  TiXmlAttribute *getElementAttribute(TiXmlElement *elmt, const string &name);
  const static unsigned int  NUM_INDENTS_PER_SPACE=2;
  const char *getIndent(unsigned int numIndents);
  // same as getIndent but no "+" at the end
  const char * getIndentAlt(unsigned int numIndents);
  void dumpNode(TiXmlNode* pParent, ostream &os, unsigned int indent = 0);
  int dumpNodeAttribs(TiXmlElement* pElement,ostream &os,unsigned int indent);
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


