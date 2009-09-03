/******************************************************************************
 * RtEvent.cpp defines a class for an activation image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#include"RtEvent.h"

//*** constructors/destructors  ***//

// default constructor

RtEvent::RtEvent() : RtData() {
    ACE_TRACE(("RtMRIImage::RtEvent()"));

    dataID.setModuleID("event");

    tr = -1;
}

// destructor

RtEvent::~RtEvent() {
}

// set the tr for this event

void RtEvent::setTR(int _tr) {
    tr = _tr;
}

// get the tr for this event

int RtEvent::getTR() const {
    return tr;
}

// serialize as xml (dummy function)

TiXmlElement* RtEvent::serializeAsXML(TiXmlElement *requestElement) {
    return new TiXmlElement("event");
}

// unserialize xml (dummy function)

void RtEvent::unserializeXML(TiXmlElement* element) {

}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


