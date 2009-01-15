/******************************************************************************
 * RtEventTrigger.h is the header for a class to trigger an event based on
 * activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 
 *
 *****************************************************************************/

#ifndef RTEVENTTRIGGER_H
#define RTEVENTTRIGGER_H

#include"RtStreamComponent.h"

// class declaration
class RtEventTrigger : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTrigger();

  // destructor
  ~RtEventTrigger();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb) = 0;

  // roiIDs for the activation sums
  string posRoiID;
  string negRoiID;

  // flags or whether to monitor rois
  bool monitorPos;
  bool monitorNeg;

  // whether we should trigger on good and bad activation directions
  bool triggerBothDirections;

  // thresholds for triggering
  double posThresh;
  double negThresh;

  int afterTriggerSkipTRs;
  int initialSkipTRs;
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


