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
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // data ID for the activation we will sum
  string activationID;

  // roiID for the activation we will sum
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


