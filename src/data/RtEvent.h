/******************************************************************************
 * RtEvent.h declares a class for an event image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#ifndef RTEVENT_H
#define RTEVENT_H

#include"RtData.h"
#include"tinyxml/tinyxml.h"

#include "ace/Log_Msg.h"


// class declaration
class RtEvent : public RtData {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtEvent();

  // destructor
  virtual ~RtEvent();

  // serialize as xml (dummy function)
  virtual TiXmlElement* serializeAsXML();

  // set the tr for this event
  void setTR(int _tr);

  // get the tr for this event
  int getTR();

protected:

  int tr;
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


