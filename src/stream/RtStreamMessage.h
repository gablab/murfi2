/******************************************************************************
 * RtStreamMessage.h is the header for a class that passes messages
 * between stream components.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-04
 * 
 *****************************************************************************/

#ifndef RTSTREAMMESSAGE_H
#define RTSTREAMMESSAGE_H

#include"RtData.h"
#include"RtConductor.h"

#define MAX_MSGDATAS 10000

// class declaration
class RtStreamMessage  {
public:

  RtStreamMessage()  {
    numData = 0;
  }


  // add a peice of data to the message
  //  in 
  //   pointer to the data
  //  out
  //   success or failure
  bool addData(RtData *_data);

  // get a data portion by index
  //  in
  //   index of data to get
  //  out
  //   pointer to the data or NULL, if index invalid
  RtData *getData(unsigned int index);

  // sets a data portion by index
  //  in
  //   data pointer
  //   index of data to set
  //  out
  //   sucess or failure
  bool setData(RtData *_data, unsigned int index);

  // get the last added data
  //  out
  //   pointer to the last data or NULL, if none
  RtData *getLastData();

  // get number of data objects currently stored
  unsigned int getNumData();

  // get pointer to our conductor
  RtConductor *getConductor();

  // set the pointer to our conductor
  void init(RtConductor *_conductor);

protected:

  // pointers to data
  RtData *data[MAX_MSGDATAS];

  // how many data objs we have
  unsigned int numData;

  // pointer to conductor
  RtConductor *conductor;

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


