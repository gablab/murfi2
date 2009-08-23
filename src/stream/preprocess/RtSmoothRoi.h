/******************************************************************************
 * RtSmoothROI.h is the header for a class that smooths input data over an ROI
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTSMOOTHROI_H
#define RTSMOOTHROI_H

#include"RtStreamComponent.h"
#include"RtMRIImage.h"

// class declaration
class RtSmoothRoi : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtSmoothRoi();

  // destructor
  ~RtSmoothRoi();

protected:


  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // validate the configuration
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  double fwhm;
  bool autoFwhm;

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


