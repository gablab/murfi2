/******************************************************************************
 * RtConfigVar.h is for a single configuration variable
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONFIGVAL_H
#define RTCONFIGVAL_H

#define dTol 0.00000000001

#define fTol 0.00000000001f

#include "boost/filesystem.hpp"

#include<sstream>
#include<string>

using namespace std;
using namespace boost::filesystem;

// a single configuration value
class RtConfigVal {
  
public:

  //*** constructor ***/

  // leave blank
  RtConfigVal() : _set(false) { 
  }

  // assign it straight off
  RtConfigVal(string s) : val(s), _set(true) {
  }

  string str() {
    return val;
  }

  path filepath() {
    return path(val);
  }

  string getVal() {
    return val;
  }

  bool isSet() {
    return _set;
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
      && fabs(d - dval) < dTol;
  }

  const bool operator==(const float f) {
    float fval;
    return convert<float>(fval,val) 
      && fabsf(f - fval) < fTol;
  }

  const bool operator==(const long l) {
    long lval;
    return convert<long>(lval,val) 
      && l == lval;
  }

  const bool operator==(const int i) {
    int ival;
    return convert<int>(ival,val) 
      && i == ival;
  }

  const bool operator==(const bool b) {
    bool bval;
    bool ret = convert(bval,val) 
      && bval == b;
    return ret;
  }

  // outstream operator
  friend ostream &operator<<(ostream &os, const RtConfigVal &v) {
    os << v.val;
    return os;
  }

  // for conversion of string types to other types
  template <class T> inline static bool convert(T &t, const string& s) {
    istringstream iss(s);

    // hack for bool
    if(s == "true") {
      iss.str("1");
    }
    else if(s == "false") {
      iss.str("0");
    }

    return !(iss >> dec >> t).fail();
  }

  // for conversion of other types to strings
  template <class T> inline static bool convertToString(string& s, const T &t) {
    ostringstream oss;
    bool ret = !(oss << dec << t).fail();
    s = oss.str();
    return ret;
  }

  // for conversion of string types to bool
  inline static bool convert(bool &b, const string& s) {    
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

private:

  string val;
  bool _set;

  // tolerances for double and float comparison
  //static const float fTol = 0.00000000001f;
  //static const double dTol = 0.00000000001;

};

#endif
