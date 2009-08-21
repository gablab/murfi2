/******************************************************************************
 * RtDataViewer.h is the header for a call to view data in the datastore
 * after all processing has occured
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-08-21
 *
 *****************************************************************************/

#ifndef RTDATAVIEWER_H
#define RTDATAVIEWER_H

#include"RtStreamComponent.h"
#include<vector>
using namespace std;

// class declaration
class RtDataViewer : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtDataViewer();

  // destructor
  ~RtDataViewer();

protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition (pure virtual)
  virtual int process(ACE_Message_Block *mb);

  vector<string> dataIds;
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


