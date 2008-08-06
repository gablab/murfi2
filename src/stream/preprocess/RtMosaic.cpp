/******************************************************************************
 * RtMosaic.cpp is the implementation of a class that converts a volume into
 * a flat mosaic representation
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-04
 *
 *****************************************************************************/

#include"RtMosaic.h"

string RtMosaic::moduleString("mosaic");

// default constructor
RtMosaic::RtMosaic() : RtStreamComponent() {
  componentID = moduleString;
}

// destructor
RtMosaic::~RtMosaic() {
  cout << "destroyed" << endl;
}

// process a single acquisition
int RtMosaic::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMosaic::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMosaic::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtMosaic::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new data image for the variance
  RtMRIImage *mosaic = new RtMRIImage(*img);
  mosaic->mosaic();

  // set the image id for handling
  mosaic->addToID("mosaic");

  setResult(msg,mosaic);

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

