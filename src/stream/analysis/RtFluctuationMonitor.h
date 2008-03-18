/******************************************************************************
 * RtFluctuationMonitor.h is the header for a class that estimates the
 * activation at each voxel in a single image using a GLM-based
 * correlation analysis based on Hids, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#ifndef RTFLUCTUATIONMONITOR_H
#define RTFLUCTUATIONMONITOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtSingleImageCor.h"

#include<vector>
using namespace std;

// class declaration
class RtFluctuationMonitor : public RtSingleImageCor {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtFluctuationMonitor();

  // destructor
  ~RtFluctuationMonitor();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  //// parameters of the activation estimation algorithm

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


