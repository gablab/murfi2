/******************************************************************************
 * RtPasser.h is the header for a class that passes data from an
 * stream to output
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTPASSER_H
#define RTPASSER_H

#include"RtOutput.h"
#include"RtData.h"
#include"RtStreamComponent.h"

// class declaration
class RtPasser: public RtStreamComponent  {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  // all data that 
  RtPasser();

  // string with id constructor
  //  in
  //   dataID is a string that constrains what type of data we send, if its
  //   empty we send all the data
  //RtPasser(string _dataID);

  // id constructor
  //  in
  //   idTemplate is a an instance of RtDataID where the fields are used as a
  //   template to match incomming data to decide if it should be send to
  //   the outputs or not. Empty fields are not matched and filled fields are
  RtPasser(const RtDataID &idTemplate);

  // char* with id constructor
  //  in
  //   dataID is a string that constrains what type of data we send, if its
  //   empty we send all the data
  //RtPasser(char *_dataID);

  // destructor
  ~RtPasser();

  //*** initialization routines  ***//
  void addOutput(RtOutput *out);

  // process a single acquisition
  void sendToOutputs(RtData *d);

protected:

  // processes as a stream component
  int process(ACE_Message_Block*);

  // vector of outputs to pass the data to
  vector<RtOutput*> outputs;

  // only pass data of this id
  RtDataID passDataID;
  
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


