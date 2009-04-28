/******************************************************************************
 * RtEventTriggerSynth.h is the header for a class to
 * simulated a event trigger sequence
 *
 * Oliver Hinds <ohinds@mit.edu> 
 *
 *****************************************************************************/

#ifndef RTEVENTTRIGGERSYNTH_H
#define RTEVENTTRIGGERSYNTH_H

#include"RtEventTrigger.h"

// class declaration
class RtEventTriggerSynth : public RtEventTrigger {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTriggerSynth();

  // destructor
  ~RtEventTriggerSynth();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // validate config
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // average number of trs between triggers
  float meanISI;
  float sdISI;
  
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


