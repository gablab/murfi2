/******************************************************************************
 * RtVar.h is the header for a class that computes the variance over a
 * set of images. this is an implementation of the west (1979) algorithm for
 * incremental variance computation. 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTVAR_H
#define RTVAR_H

#include"RtStreamComponent.h"
#include"RtActivation.h"

// class declaration
class RtVar : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtVar();

  // destructor
  ~RtVar();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  //virtual bool processOption(const string &name, const string &text);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  unsigned int numTimePoints;

  // saves the mean image up to t-1
  //RtMRIImage mean;

  // saves the variance numerator image up to t-1
  RtActivation varnum;
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


