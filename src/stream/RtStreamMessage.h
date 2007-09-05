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

// class declaration
class RtStreamMessage  {
public:
  RtData *data;
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


