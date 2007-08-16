/******************************************************************************
 * RtStream.h is the header for a class that controls execution of a
 * single data processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSTREAM_H
#define RTSTREAM_H

#include<vector>

#include"RtStreamComponent.h"

using namespace std;

// class declaration
class RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtStream(); 

  // destructor
  ~RtStream();

  //*** initialization routines  ***//
  
  // adds a component to the processing pipeline
  //  in:
  //   component: component object
  //  out:
  //   true (for success) or false
  bool addComponent(RtStreamComponent &component);

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  bool init();

  //*** operation routines  ***//
  
  // begins stream processing
  //  out:
  //   true (for success) or false
  bool run(RtData &data);

private:

  //*** private data members  ***//

  // vector of components objects
  vector<RtStreamComponent&> components;

  // here we need a way to represent serial dependencies in processing
  

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


