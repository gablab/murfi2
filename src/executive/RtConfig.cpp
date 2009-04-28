/******************************************************************************
 * RtConfig.cpp defines a class that controls configuration of a a
 * real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConfig.h"

#include"ace/Get_Opt.h"
#include"tinyxml/tinyxml.h"

#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>

const RtConfigVal RtConfig::unset;

//*** constructors/destructors  ***//

// copy constructor (called often)
RtConfig::RtConfig(const RtConfig &other) 
  : parms(other.parms) {
  // nothing to do here
}

// destructor
RtConfig::~RtConfig() {
  // nothing to do here
}

//*** config loading routines ***//

// parse xml config file
//  out: true (success) or false
bool RtConfig::parseConfigFile(const string &filename) {
  ACE_TRACE(("RtConfig::parseConfigFile"));

  setDefaults();

  // try to read file
  if(!parms.LoadFile(filename)) {
    cout << parms.ErrorDesc() << endl;
    parms.Clear();
    parms.ClearError();
    return false;
  }

  return validateConfig();
}

// parse xml config string
//  out: true (success) or false
bool RtConfig::parseConfigStr(const string &xml) {
  ACE_TRACE(("RtConfig::parseConfigStr"));

  setDefaults();

  // try to parse string
  parms.Parse(xml.c_str());
  if(parms.Error()) {
    cout << parms.ErrorDesc() << endl;
    parms.Clear();
    parms.ClearError();
    return false;
  }

  return validateConfig();
}

//*** config get/set parms ***/

// get a parm value.
//  in
//   name is a ':' delimited string spcifying a path into the XML configuration
//        for example processor:module:name
//  out
//   string representing the value
RtConfigVal RtConfig::get(const char *name) {
  return get(string(name));
}

// get a parm value. start searching from a specified node
//  in
//   name is a ':' delimited string spcifying a path into the XML configuration
//        for example processor:module:name
//   node is a xml node to start looking at
//  out
//   string representing the value
RtConfigVal RtConfig::get(const string &name, TiXmlNode *node) {
  ACE_TRACE((ACE_TEXT("RtConfig::get(string, node)")));

  // if no node specified, make it the top level
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
  string rest, optionName;
  if(delind == string::npos) { // found no more delim cases, try to return val
    const char *val;

    // if no children found, check attributes if there are no more ':'s
    if(node != &parms && NULL != (val = elmt->Attribute(childName.c_str()))) {
      return RtConfigVal(val);
    }

    // search children for text elements to return
    TiXmlNode *text = 0;

    // find either a node with the child name or an "option" with the right
    // "name" attribute
    while((text = elmt->IterateChildren(childName, text))) {
      if(text->Type() == TiXmlNode::TEXT) {
	return RtConfigVal(text->ValueStr());
      }
    }

    while((text = elmt->IterateChildren("option", text))) {
      if(text->ToElement() != NULL 
	 && TIXML_SUCCESS 
	   == text->ToElement()->QueryValueAttribute("name",&optionName)
	 && optionName == childName) {
	
	return RtConfigVal(text->ToElement()->GetText());
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

  // look for child with name or "option" child with correct "name" attribute
  TiXmlNode *child = 0;
  while((child = elmt->IterateChildren(childName, child))) {
    string val = get(rest,child);

    // FIGURE OUT WHAT TO DO IF WE'RE EMPTY (COULD MEAN TO BE EMPTY)
    if(!val.empty()) {
      return RtConfigVal(val);
    }
  }

  while((child = elmt->IterateChildren("option", child))) {
    if(child->ToElement() != NULL 
       && TIXML_SUCCESS 
         == child->ToElement()->QueryValueAttribute("name",&optionName)
       && optionName == childName) {
      string val = get(rest,child);
	
      // FIGURE OUT WHAT TO DO IF WE'RE EMPTY (COULD MEAN TO BE EMPTY)
      if(!val.empty()) {
	return RtConfigVal(val);
      }
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

  // if no node specified, make it the top level
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
  bool searchAllChildren = (childName == "*"); 
  TiXmlNode *child = 0;
  while((child = (searchAllChildren 
		  ? elmt->IterateChildren(child)
		  : elmt->IterateChildren(childName, child)
		  ))) {
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
bool RtConfig::set(const string &name, const string &value,
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
    node->LinkEndChild(child);
  }

  // recursive set
  return set(nextName, value, child);
}

// print a parm value
void RtConfig::print(const char *name) {
  if(name == NULL) {
    cout << "name is null";
    return;
  }

  cout << get(name).str();
}


//*** private functions ***//

// for conversion of string types to other types
template <class T> inline bool RtConfig::convert(T &t, const string& s) {
  istringstream iss(s);
  return !(iss >> dec >> t).fail();
}


// gets the version
//  out:
//   cvs version string for this class
char *RtConfig::getVersionString() {
  return VERSION;
}

// print the name/value pairs to the screen
void RtConfig::dumpConfig(ostream &os) {
  dumpNode(&parms, os);
}

//**************************************************//
// tinyxml support stuff
// some taken from the tinyXml examples
//

// get the attribute with a certain name
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

// build a map between attribute names and values
map<string,string> RtConfig::getAttributeMap(TiXmlElement &ele) {
  map<string,string> attrMap;
  
  // find all attributes
  for(TiXmlAttribute *attr = ele.FirstAttribute(); attr; attr = attr->Next()) {
    string name = attr->Name();
    attrMap[name] = attr->ValueStr();
  }
  
  return attrMap;
}

// get the indent for printing 
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


