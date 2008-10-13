/******************************************************************************
 * RtEventTriggerActivationSums.h is the header for a class to trigger an event based on
 * activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 
 *
 *****************************************************************************/

#ifndef RTEVENTTRIGGERACTIVATIONSUMS_H
#define RTEVENTTRIGGERACTIVATIONSUMS_H

#include"RtEventTrigger.h"

// class declaration
class RtEventTriggerActivationSums : public RtEventTrigger {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTriggerActivationSums();

  // destructor
  ~RtEventTriggerActivationSums();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // idss for the activation sums
  string posActivationSumModuleID;
  string negActivationSumModuleID;
  string posActivationSumDataName;
  string negActivationSumDataName;
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


