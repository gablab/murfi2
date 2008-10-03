/******************************************************************************
 * RtUnmosaic.cpp is the implementation of a class that converts
 * from a flat mosaic representation into a 3D volume
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-04
 *
 *****************************************************************************/

#include"RtUnmosaic.h"
#include"RtDataIDs.h"

string RtUnmosaic::moduleString(ID_UNMOSAIC);

// default constructor
RtUnmosaic::RtUnmosaic() : RtStreamComponent() {
  componentID = moduleString;
  dataName = ""; // unused
}

// destructor
RtUnmosaic::~RtUnmosaic() {
  cout << "destroyed" << endl;
}

// process a single acquisition
int RtUnmosaic::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtUnmosaic::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtUnmosaic::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtUnmosaic::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new data image for the variance
  RtMRIImage *unmosaic = new RtMRIImage(*img);
  unmosaic->getDataID().setFromInputData(*img,*this);

  unmosaic->unmosaic();

  // set the image id for handling
  //unmosaic->addToID("unmosaic");

  setResult(msg,unmosaic);

  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

