/*=========================================================================
 *  RtConfig.h is the header for a virtual base class that controls xml
 *  configuration reading. this class must be extended for particular types
 *  of configuration (setDefaults() and validateConfig() must be implemented).
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

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
  RtConfig() { };

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
  //        for example preprocessor:module:name. an asterisk node name (eg
  //        'node1:*:node3' works as a wildcard and all children at that
  //        level will be searched.  WARNING: the first match is returned
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

  ///// templated sets must be defined in the header :(

  // set a parm value
  template<class T>
  bool set(const string &name, const T &tval) {
    string val;
    if(!RtConfigVal::convertToString<T>(val,tval)) {
      return false;
    }

    return set(name, val, &parms);
  }

  // set a parm value
  template<class T>
  bool set(const char *name, const T &tval) {
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
  bool set(const string &name, const string &value, TiXmlNode *node);

  // print a parm value
  void print(const char *name);

  //*** general ***//

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
  static const RtConfigVal unset;

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
