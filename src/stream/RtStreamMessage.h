/******************************************************************************
 * RtStreamMessage.h is the header for a class that passes messages
 * between stream components.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-04
 * 
 *****************************************************************************/

#ifndef RTSTREAMMESSAGE_H
#define RTSTREAMMESSAGE_H

//#include<dirent.h>
#include"RtData.h"
#include"RtActivation.h"
#include"RtConductor.h"

#define MAX_MSGDATAS 10000

// class declaration
class RtStreamMessage  {
public:

  RtStreamMessage()  {
    numData = 0;
    curDataIndex = 0;
  }

  // add a peice of data to the message
  //  in 
  //   pointer to the data
  //  out
  //   success or failure
  bool addData(RtData *_data);

  // sets a data portion by index
  //  in
  //   data pointer
  //   index of data to set
  //  out
  //   sucess or failure
  bool setData(RtData *_data, unsigned int index);

  // sets the current data to the specified index
  //  in
  //   index of data to set as current
  //  out
  //   sucess or failure
  bool setCurrentData(unsigned int index);

  // sets the last data to be current
  //  out
  //   sucess or failure
  bool setLastDataAsCurrent();

  // get a data portion by index
  //  in
  //   index of data to get
  //  out
  //   pointer to the data or NULL, if index invalid
  RtData *getData(unsigned int index);

  // get a data portion by data id (returns the first found instance)
  //  in
  //   id of data to get
  //  out
  //   pointer to the data or NULL, if id doesnt exist
  RtData *getDataByID(const string &id);

  // get the current data (original data plus any desired processing up to
  // this point) 
  //  out 
  //   pointer to the current data or NULL, if none
  RtData *getCurrentData();

  // get the last added data
  //  out
  //   pointer to the last data or NULL, if none
  RtData *getLastData();


  // get data by data id and roi id (returns the first found instance)
  //  in
  //   dataid id of data
  //   roiid  id of roi
  //  out
  //   pointer to the data or NULL, if id doesnt exist
  RtData *getDataByIDAndRoiID(const string &dataid, const string &roiid);

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

  // index of the current data: data that represents the end result of
  // processing up this point
  unsigned int curDataIndex;

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


