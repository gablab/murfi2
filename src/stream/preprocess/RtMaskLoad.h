/******************************************************************************
 * RtMaskLoad.h is the header for a stream module that loads a mask  
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-31
 *
 *****************************************************************************/

#ifndef RTMASKLOAD_H
#define RTMASKLOAD_H

#include"RtStreamComponent.h"

// class declaration
class RtMaskLoad : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtMaskLoad();

  // destructor
  ~RtMaskLoad();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // make sure we are configured properly
  virtual bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // data members
  string filename;
  string roiID;
  bool align;
  bool mosaic;
  bool unmosaic;
  bool flipLR;
  bool dynamic;
  bool save;

  // data
  RtMaskImage *maskLoad;
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


