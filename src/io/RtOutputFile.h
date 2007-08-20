/******************************************************************************
 * RtOutputfile.h declares a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTOUTPUTFILE_H
#define RTOUTPUTFILE_H

#include"RtOutput.h"
#include<fstream>

using namespace std;

// class declaration
class RtOutputFile : public RtOutput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutputFile(); 

  // destructor
  virtual ~RtOutputFile();

  // open and start accepting input
  bool open(RtConfig &config);

  // close and clean up
  bool close();

  // write to the file 
  template<class T>
  RtOutputFile &operator<<(const T &t) {
    if(isOpen) {
      fp << t;
    }
    return *this;
  }

  // outstream operator
  void writeConfig(RtConfig &config);

  // prints the current time 
  void printNow();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


private:

  //*** private data members  ***//
  ofstream fp;

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


