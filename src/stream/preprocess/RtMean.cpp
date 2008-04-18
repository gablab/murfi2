/******************************************************************************
 * RtMean.h is the implementation of a class that computes the mean
 * of a set of images incrementally
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtMean.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtMean::moduleString("voxel-mean");

// default constructor
RtMean::RtMean() : RtStreamComponent() {
  componentID = moduleString;
  numTimePoints = 0;
}

// destructor
RtMean::~RtMean() {
  cout << "destroyed" << endl;
}

// process a single acquisition
int RtMean::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMean::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMean::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtMean::process: image passed is NULL\n"));
    return 0;
  }

  if(numTimePoints == 0 || storemean.getSeriesNum() != img->getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "mean found first image\n"));
    storemean = (*img);
  }

  // validate sizes
  if(img->getNumPix() != storemean.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtMean::process: storemean is different size than this one\n"));
    return -1;
  }

  // save the data
  numTimePoints++;


  // allocate a new data image for the variance
  RtMRIImage *mean = new RtMRIImage(*img);

  // update the mean and variance numerator due to west (1979) for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) storemean.getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
      + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    storemean.setPixel(i, (unsigned short) newmean);
    mean->setPixel(i, (unsigned short) newmean);
  }

  // set the image id for handling
  mean->addToID("voxel-mean");
  //cout << "mean id " << mean->getID() << endl;
  setResult(msg,mean);

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

