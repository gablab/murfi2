/******************************************************************************
 * RtDiff.h is the header for a class that computes the difference
 * between two images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtDiff.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

// default constructor
RtDiff::RtDiff() : RtStreamComponent() {
  last = NULL;
}

// destructor
RtDiff::~RtDiff() {}

// process a single acquisition
int RtDiff::process(ACE_Message_Block *mb) {
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtDataImage *img = (RtDataImage*)msg->getLastData();

  if(img == NULL) {
    ACE_DEBUG((LM_INFO, "RtDiff:process: image passed is NULL\n"));
    return -1;
  }

  if(last == NULL) {
    ACE_DEBUG((LM_DEBUG, "differencing found first image\n"));

    last = img;
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "differencing images %d and %d\n", 
	     img->getAcquisitionNum(), last->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != last->getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtDiff:process: last image is different size than this one\n"));
    return -1;    
  }

  // allocate a new data image for the difference
  RtDataImage *diff = new RtDataImage(img->getInfo());

  // compute the absolute difference
  for(int i = 0; i < img->getNumPix(); i++) {
    diff->setPixel(i, absdiff(img->getPixel(i),last->getPixel(i)));
  }  

  // set the image id for handling
  diff->addToID("voxel_difference");

  // store this as last image
  last = img;

  // add out new data to the message
  msg->addData(diff);

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


