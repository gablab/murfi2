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

#include"ace/Get_Opt.h"
#include"tinyxml/tinyxml.h"

#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>


// defaults
#define DEFAULT_SUBJECTSDIR  "/home/mri/subjects"
#define DEFAULT_CONFFILENAME "conf/example.xml"

// limits
#define MAX_PORT 65535

//*** constructors/destructors  ***//

// default constructor
RtConfig::RtConfig()
    : confFilename(DEFAULT_CONFFILENAME), conductor(NULL) {
  // nothing to do here
}

// default constructor
RtConfig::RtConfig(RtConductor &_conductor)
    : confFilename(DEFAULT_CONFFILENAME), conductor(&_conductor)  {
  // nothing to do here
}

// destructor
RtConfig::~RtConfig() {
  // nothing to do here
}

//*** config loading routines ***//

// parse command line args
bool RtConfig::parseArgs(int argc, char **args) {
  ACE_TRACE(("RtConfig::parseArgs"));

  // set up short options
  static const ACE_TCHAR options[] = ACE_TEXT (":f:h");
  ACE_Get_Opt cmdOpts(argc, args, options, 1, 0,
		      ACE_Get_Opt::PERMUTE_ARGS, 1);

  // set up long options
  cmdOpts.long_option("config",     'f', ACE_Get_Opt::ARG_REQUIRED);
  cmdOpts.long_option("help",       'h', ACE_Get_Opt::NO_ARG);

  // handle options
  for(int option; (option = cmdOpts ()) != EOF; ) {
    switch(option) {

    case 'f':
      confFilename = cmdOpts.opt_arg();
      break;

    case 'h':
      printUsage();
      exit(1);

    case ':':
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmdOpts.opt_opt()), -1);

    default:
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }
  }

  // parse the logfile
  if(!parseConfigFile()) {
    ACE_DEBUG((LM_ERROR, ACE_TEXT("failed to parse config file %s\n"),
	       confFilename));
    return false;
  }

  // get the executable name and add it to the config
  set("execName",args[0]);

  // dump config to screen if verbose
  if(get("info:terminal:verbose")==true) {
    dumpConfig(cout);
  }

  return true;
}

// parse config file
//  out: true (success) or false
bool RtConfig::parseConfigFile() {
  ACE_TRACE(("RtConfig::parseConfigFile"));

  return parms.LoadFile(confFilename);
}

// validate the configuration
// checks for valid setup of different parts of the program
bool RtConfig::validateConfig() {
  ACE_TRACE(("RtConfig::validateConfig"));

  bool valid = true;

  // check subject
  if(get("study:subject")==false) {
    cerr << "ERROR: no subject name specified" << endl;
    valid = false;
  }

  // check subjects directory
  if(get("study:subjectsDir")==false) {
    set("study:subjectsDir",DEFAULT_SUBJECTSDIR);
  }

  // check for existance of studyDir
  stringstream studyDir;
  studyDir << get("study:subjectsDir").str() << "/";
  studyDir << get("study:subject").str() << "/";
  string s1 = studyDir.str();

  string testfile = s1 +  "/testfile";

  ofstream fs(testfile.c_str());
  if(fs.fail()) {
    cerr << "ERROR: subject directory " << studyDir.str() << " is bad" << endl;
    valid = false;
  }
  else {

    set("study:dir",studyDir.str());
  }
  fs.close();


  // check logfile name
  if(get("info:log:disabled")==false) {
    if(get("info:log:filename")==false) {
      cerr << "WARNING: no logfile name specified" << endl;
      set("info:log:disabled",true);
    }
  }

  // check image receiver
  if(get("scanner:disabled")==false) {
    if(!isSet("scanner:receiveImages")) {
      set("scanner:receiveImages",true);
    }

    if((int) get("scanner:port") < 1 || (int) get("scanner:port") > MAX_PORT) {
      cerr << "WARNING: invalid port number for receiving scanner images"
	   << endl;
      set("scanner:receiveImages",false);
    }
  }

  return valid;
}

//*** config get/set parms ***/

// get a parm value.
//  in
//   name is a ':' delimited string spcifying a path into the XML configuration
//        for example preprocessor:module:name
//  out
//   string representing the value
RtConfigVal RtConfig::get(const char *name) {
  return get(string(name));
}

// get a parm value. start searching from a specified node
//  in
//   name is a ':' delimited string spcifying a path into the XML configuration
//        for example preprocessor:module:name
//   node is a xml node to start looking at
//  out
//   string representing the value
RtConfigVal RtConfig::get(const string &name, TiXmlNode *node) {
  ACE_TRACE((ACE_TEXT("RtConfig::get(string, node)")));

  if(node == NULL) {
    node = &parms;
  }

  // make sure we are an element if we are not text
  if(node->Type() != TiXmlNode::ELEMENT && node != &parms) {
    return unset;
  }

  TiXmlElement *elmt = (TiXmlElement*) node;

  // find the next ':'
  size_t delind = name.find(':');

  string childName(name.substr(0,delind));
  string rest;
  if(delind == string::npos) { // found no more delim cases, try to return val
    string val;

    // if no children found, check attributes if there are no more ':'s
    if(node != &parms
       && TIXML_SUCCESS == elmt->QueryValueAttribute(childName,&val)) {
      return RtConfigVal(val);
    }

    // search children for text elements to return
    TiXmlNode *text = 0;
    while((text = elmt->IterateChildren(childName, text))) {
      if(text->Type() == TiXmlNode::TEXT) {
	return RtConfigVal(text->ValueStr());
      }
    }

  }
  else if(delind == name.length()-1) {
    // we havent taken all delimiters, but there is no more remaining name,
    // so look for text children
    TiXmlNode *me;
    while((me = node->IterateChildren(node->FirstChild()))) {
      if(me->Type() == TiXmlNode::TEXT) {
	return RtConfigVal(me->ValueStr());
      }
    }
    return unset;
  }
  else {
    rest = name.substr(delind+1);
  }

  // look for child with name
  TiXmlNode *child = 0;
  while((child = elmt->IterateChildren(childName, child))) {
    string val = get(rest,child);

    // FIGURE OUT WHAT TO DO IF WE'RE EMPTY (COULD MEAN TO BE EMPTY)
    if(!val.empty()) {
      return RtConfigVal(val);
    }
  }

  return unset;
}

// get an xml node in the config. start searching from a specified node
//  in
//   name is a ':' delimited str spcifying a path into the XML configuration
//        for example preprocessor:module:name
//   node is a xml node to start looking at
//  out
//   node representing the value, or NULL if none does
TiXmlNode *RtConfig::getNode(const string &name, TiXmlNode *node) {
  ACE_TRACE((ACE_TEXT("RtConfig::get(string, node)")));

  if(node == NULL) {
    node = &parms;
  }

  // make sure we are an element if we are not text
  if(node->Type() != TiXmlNode::ELEMENT && node != &parms) {
    return NULL;
  }

  // if the name string is empty we return the current node
  if(name.empty()) {
    return node;
  }

  TiXmlElement *elmt = (TiXmlElement*) node;

  // find the next ':'
  size_t delind = name.find(':');

  string childName(name.substr(0,delind));
  string rest;
  if(delind == string::npos) {
    // found no more delim cases, try to return a text child

    TiXmlNode *text = 0;
    while((text = elmt->IterateChildren(childName, text))) {
      if(text->Type() == TiXmlNode::TEXT) {
	return text;
      }
    }

  }
  else if(delind == name.length()-1) {
    // we havent taken all delimiters, but there is no more remaining name,
    // so look for text children of any name
    TiXmlNode *me;
    while((me = node->IterateChildren(node->FirstChild()))) {
      if(me->Type() == TiXmlNode::TEXT) {
	return me;
      }
    }
    return NULL;
  }
  else {
    rest = name.substr(delind+1);
  }

  // look though each child with name, return the first non NULL
  TiXmlNode *child = 0;
  while((child = elmt->IterateChildren(childName, child))) {
    TiXmlNode *ret = getNode(rest,child);
    if(ret != NULL) {
      return ret;
    }
  }

  return NULL;
}

// determine if there is a value set for a particular config variable
//  in
//   the variable name to check
//  out
//   true if the var has been set
bool RtConfig::isSet(const string &name) {
  return get(name,&parms).isSet();
}


// sets a parm value starting from a specified xml node
// children are created appropritately
//  in
//   name:  ':' delimited string representing the xml node attribute to set
//   value: the value to set the attribute to
//   node:  the xml node to start from
//  out: success or failure
bool RtConfig::set(const string name, const string value,
		      TiXmlNode *node) {
  if(node == NULL) {
    return "";
  }

  size_t nextdelim = name.find(':');

  TiXmlElement *ele = (TiXmlElement*) node;

  // check if we have consumed all of the ':' things in the name
  if(nextdelim == string::npos) {
    if(node == &parms) { // if top level add an element with text
      TiXmlElement newel(name);
      TiXmlText txt(value);
      newel.InsertEndChild(txt);
      node->InsertEndChild(newel);
    }
    else { // otherwise add an attribute to the existing node
      ele->SetAttribute(name,value);
    }

    return true; // termination condition
  }

  // find the node with the next name piece
  string nextNode = name.substr(0,nextdelim);
  string nextName = name.substr(nextdelim+1);

  // get the child with the appropriate name, or create it
  TiXmlNode *child = ele->FirstChild(nextNode);

  if(child == NULL) {
    child = new TiXmlElement(nextNode);
  }

  // recursive set
  return set(nextName, value, child);
}

// set a parm value
template<class T>
bool RtConfig::set(const string name, T tval) {
  string val;
  if(!RtConfigVal::convertToString<T>(val,tval)) {
    return false;
  }

  return set(name, val, &parms);
}

// set a parm value
template<class T>
bool RtConfig::set(const char *name, T tval) {
  string s(name);
  return set(s,tval);
}

// print a parm value
void RtConfig::print(const char *name) {
  if(name == NULL) {
    cout << "name is null";
    return;
  }

  cout << get(name).str();
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

// for conversion of string types to other types
template <class T> inline bool RtConfig::convert(T &t, const string& s) {
  istringstream iss(s);
  return !(iss >> dec >> t).fail();
}


// prints the usage info for the realtime system
void RtConfig::printUsage() {
  int w = 15;

  cout << "usage: " << endl << get("execName")
       << " [-f conffile]" << endl << endl
       << "---------------------------------------------" << endl
       << "some useful flags:" << endl
       << setiosflags(ios::left)
       << setw(w) << endl
       << "---------------------------------------------" << endl;
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

// print the name/value pairs to the screen
void RtConfig::dumpConfig(ostream &os) {
  dumpNode(&parms, os);
}

//**************************************************//
// tinyxml support stuff
// some taken from the tinyXml examples
//

TiXmlAttribute *RtConfig::getElementAttribute(TiXmlElement *elmt,
					      const string &name) {
  if(elmt == NULL) {
    return NULL;
  }

  TiXmlAttribute* attr = elmt->FirstAttribute();
  while(attr) {
    if(name == attr->Name()) {
      return attr;
    }
  }

  return NULL;
}

const char *RtConfig::getIndent(unsigned int numIndents) {
  static const char *pINDENT="                                      + ";
  static const unsigned int LENGTH=strlen(pINDENT);
  unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
  if( n > LENGTH ) n = LENGTH;

  return &pINDENT[LENGTH-n];
}

// same as getIndent but no "+" at the end
const char * RtConfig::getIndentAlt(unsigned int numIndents) {
  static const char *pINDENT="                                        ";
  static const unsigned int LENGTH=strlen(pINDENT);
  unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
  if(n > LENGTH) n = LENGTH;

  return &pINDENT[LENGTH-n];
}

int RtConfig::dumpNodeAttribs(TiXmlElement* pElement, ostream &os,
				 unsigned int indent) {
  if(!pElement) return 0;

  TiXmlAttribute* pAttrib=pElement->FirstAttribute();
  int i=0;
  const char* pIndent=getIndent(indent);
  while(pAttrib) {
    os << pIndent << pAttrib->Name() << '=' << pAttrib->ValueStr() << endl;
    i++;
    pAttrib=pAttrib->Next();
  }
  //if(i) os << endl;
  return i;
}

void RtConfig::dumpNode(TiXmlNode* pParent, ostream &os,
			   unsigned int indent) {
  if(!pParent) return;

  TiXmlNode* pChild;
  TiXmlText* pText;
  int t = pParent->Type();

  // ignore certain types
  if(t == TiXmlNode::COMMENT
     || t == TiXmlNode::UNKNOWN
     || t == TiXmlNode::DECLARATION) {
    return;
  }

  os << getIndent(indent);

  switch(t) {
  case TiXmlNode::DOCUMENT:
    //
    break;

  case TiXmlNode::ELEMENT:
    os << pParent->Value() << endl;
    dumpNodeAttribs(pParent->ToElement(), os, indent+1);
    break;

  case TiXmlNode::TEXT:
    pText = pParent->ToText();
    os << pText->Value() << endl;;
    break;

  default:
    break;
  }

  for(pChild = pParent->FirstChild(); pChild != 0;
      pChild = pChild->NextSibling()) {
    dumpNode(pChild, os, indent+1);
  }
}

//**************************************************//

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


