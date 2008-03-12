/******************************************************************************
 * RtActivationSum.h is the header for a class that computes the sum over
 * all voxels in an activation map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTACTIVATIONSUM_H
#define RTACTIVATIONSUM_H

#include"RtStreamComponent.h"
#include"RtActivation.h"

// class declaration
class RtActivationSum : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtActivationSum();

  // destructor
  ~RtActivationSum();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // roiID for the activ ation we will sum
  string roiID;

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


