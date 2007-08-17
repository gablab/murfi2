/******************************************************************************
 * RtOutputfile.h declares a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTOUTPUTFILE_H
#define RTOUTPUTFILE_H

#include"RtOutput.h"

// class declaration
class RtOutputFile : public RtOutput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutputFile(); 

  // destructor
  virtual ~RtOutputFile();

  // open and start accepting input
  bool open();

  // close and clean up
  bool close();


private:

  //*** private data members  ***//


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


