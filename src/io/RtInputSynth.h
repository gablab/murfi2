/******************************************************************************
 * RtInputSynth.h declares a class that sythesizes input  operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-14 
 * 
 *****************************************************************************/

#ifndef RTINPUTSYNTH_H
#define RTINPUTSYNTH_H

#include"RtInput.h"
#include"RtConfig.h"

// controls input operations to receive scanner images
class RtInputSynth : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputSynth() : tr(2), numTrs(100) {
    addToID(":synth");
  }; 

  // destructor
  virtual ~RtInputSynth();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // set the handler that should receive new data
  //  bool setHandler(ACE_Handler &handler);

  // close and clean up
  bool close();

  // run the scanner input
  virtual int svc(); 

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

private:

  // time between "inputs" in seconds
  float tr;

  // number of inputs to generate
  unsigned int numTrs;
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

