/******************************************************************************
 * RtRoiFeedbackSynth.h is the header for a class that synthesizes roi
 * activations
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-14
 *
 *****************************************************************************/

#ifndef RTROIFEEDBACKSYNTH_H
#define RTROIFEEDBACKSYNTH_H

#include"RtStreamComponent.h"

// class declaration
class RtRoiFeedbackSynth : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiFeedbackSynth();

  // destructor
  ~RtRoiFeedbackSynth();

protected:

  // process an option
  //  in
  //   name of the option to process
  //   val  text of the option node
  bool processOption(const string &name,
		     const string &text,
		     const map<string,string> &attrMap);

  // validate config
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  unsigned int numTimepoints;
  float mean;
  float sd;

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
