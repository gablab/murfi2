/******************************************************************************
 * RtConfig.h is the header for a virtual base class that controls xml
 * configuration reading. this class must be extended for particular types
 * of configuration (setDefaults() and validateConfig() must be implemented). 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

// TODO: need these functions:
// 1) set all possible defaults
// 2) print out all possible options (have each module print its own options)
// 3) fold the small amount of duplicated functionality from RtExperiment
//    into this class

#ifndef RTCONFIG_H
#define RTCONFIG_H


#include<cctype>
#include<cmath>
#include<string>
#include<cstring>
#include"tinyxml/tinyxml.h"
#include"RtConfigVal.h"
#include<map>

using namespace std;

// for defaults


// class to wrap string for a single parm value
class RtConductor;

// class declaration
class RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfig() {}; 

  // copy constructor (called often)
  RtConfig(const RtConfig &other); 

  // destructor
  virtual ~RtConfig();

  //*** config loading routines ***//

  // parse xml config file
  bool parseConfigFile(const string &filename);

  // parse xml config string
  bool parseConfigStr(const string &xml);

  //*** config get/set parms ***/

  // get a parm value.
  //  in
  //   name is a ':' delimited string spcifying a path into the XML config
  //        for example processor:module:name
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

  // set a parm value
  template<class T>
  bool set(const string &name, T tval) {
    string val;
    if(!RtConfigVal::convertToString<T>(val,tval)) {
      return false;
    }

    return set(name, val, &parms);
  }

  // set a parm value
  template<class T>
  bool set(const char *name, T tval) {
    string s(name);
    return set(s,tval);
  }

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

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

  // print the name/value pairs to the screen
  void dumpConfig(ostream &os = cout);

  // utility function to build a map between attribute names and values
  static map<string,string> getAttributeMap(TiXmlElement &ele);

protected:

  // sets some default configuration info
  // MUST OVERRIDE THIS IN SUBCLASS
  virtual void setDefaults() = 0;

  // validate the configuration
  // checks for valid setup of different parts of the program
  // returns true for success
  // MUST OVERRIDE THIS IN SUBCLASS
  virtual bool validateConfig() = 0;

  // no value yet
  const RtConfigVal unset;

  // xml doc with all the config stuff
  TiXmlDocument parms;

  // for conversion of string types to other types
  template <class T> inline bool convert(T &t, const string& s);

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


