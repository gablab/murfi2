/******************************************************************************
 * RtIncrementalGLM.h is the header for a class that estimates the
 * activation at each voxel incrementally using Gentleman's method
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-01
 *
 *****************************************************************************/

#ifndef RTNCREMENTALGLM_H
#define RTINCREMENTALGLM_H

#include"RtActivationEstimator.h"
#include"RtLeastSquaresSolve.h"

// class declaration
class RtIncrementalGLM : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtIncrementalGLM();

  // destructor
  ~RtIncrementalGLM();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // get a datapoint by index
  //virtual unsigned short getDatapoint(RtData &data, unsigned int index) = 0;

  // if we should reinit
  bool needsInit;

  // number of data points to process
  unsigned int numData;

  // one solver for each voxel 
  RtLeastSquaresSolve **solvers;

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


