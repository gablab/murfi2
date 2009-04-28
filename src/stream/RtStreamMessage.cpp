/******************************************************************************
 * RtStreamMessage.cpp defines a class that passes messages between stream
 * components.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-04
 *
 *****************************************************************************/

#include"RtStreamMessage.h"

// add a piece of data to the message
//  in
//   pointer to the data
//   bool for setting data as current data
//  out
//   success or failure
bool RtStreamMessage::addData(RtData *_data, bool _makeCurrentData) {
  ACE_TRACE(("RtStreamMessage::addData"));

  mut.acquire();
  
  if(numData >= MAX_MSGDATAS) {
    return false;
  }

  ACE_DEBUG((LM_DEBUG, "adding data to msg: %d\n", numData));

  data[numData++] = _data;
  
  if(_makeCurrentData) {
    this->setLastDataAsCurrent();
  }
  
  mut.release();
  
  return true;
}

// sets a data portion by index
//  in
//   data pointer
//   index of data to set
//  out
//   sucess or failure
bool RtStreamMessage::setData(RtData *_data, unsigned int index) {
  if(index > numData) {
    return false;
  }

  data[index] = _data;

  return true;
}


// sets the current data to the specified index
//  in
//   index of data to set as current
//  out
//   sucess or failure
bool RtStreamMessage::setCurrentData(unsigned int index) {
  if(index > numData) {
    return false;
  }

  curDataIndex = index;

  return true;
}

// sets the last data to be current
//  out
//   sucess or failure
bool RtStreamMessage::setLastDataAsCurrent() {
  curDataIndex = numData-1;
  return true;
}

// get the current data (original data plus any desired processing up to
// this point)
//  out
//   pointer to the current data or NULL, if none
RtData *RtStreamMessage::getCurrentData() const {
  return getData(curDataIndex);
}

// get the last added data
//  out
//   pointer to the last data or NULL, if none
RtData *RtStreamMessage::getLastData() const {
  if(numData == 0) {
    return NULL;
  }

  ACE_DEBUG((LM_DEBUG, "retreiving last data: %d: %x\n", numData, this));

  return data[numData-1];
}

// get a data portion by index
//  in
//   index of data to get
//  out
//   pointer to the data or NULL, if index invalid
RtData *RtStreamMessage::getData(unsigned int index) const {
  if(index >= numData) {
    return NULL;
  }

  ACE_DEBUG((LM_DEBUG, "getting data: %d\n", index));

  return data[index];
}

// get a data portion by module id, data name, and roi id (returns
// the first found instance). note that any of these can be "" to
// match any string for that field
//  in
//   module id
//   data name
//   roi id
//  out
//   pointer to the data or NULL, if such data doesnt exist
RtData *RtStreamMessage::getData(const string &moduleId,
				 const string &dataName,
				 const string &roiId) const {
  RtDataID idTemplate(DATAID_UNSET_VALUE,DATAID_UNSET_VALUE,DATAID_UNSET_VALUE,
		      "", moduleId, dataName, roiId);
  return getData(idTemplate);
}

// get a data portion by a template data id. uses RtDataID::operator==
//  in
//   template id
//  out
//   pointer to the data or NULL, if such data doesnt exist
RtData *RtStreamMessage::getData(const RtDataID &idTemplate) const {
  //cout << "serching for " << idTemplate << endl;

  for(unsigned int i = 0; i < numData; i++) {
    if(DEBUG_LEVEL & MODERATE) {
      cout << data[i] << " " << data[i]->getDataID() << endl;
    }
    if(idTemplate == data[i]->getDataID()) {
       return data[i];
    }
  }

  return NULL;
}

// get the number of data objects we currently have
unsigned int RtStreamMessage::getNumData() const {
  return numData;
}


// get pointer to our conductor
RtConductor *RtStreamMessage::getConductor() const {
  return conductor;
}

// set the pointer to our conductor and zero our numData
void RtStreamMessage::init(RtConductor *_conductor) {
  numData = 0;
  conductor = _conductor;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


