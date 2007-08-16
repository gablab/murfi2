/******************************************************************************
 * RtData.h declares a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATA_H
#define RTDATA_H


// class declaration
class RtData {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtData(); 

  // destructor
  virtual ~RtData();

private:

  //*** private data members  ***//
  
  unsigned int creationTime;

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


