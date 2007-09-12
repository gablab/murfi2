/******************************************************************************
 * RtVar.h is the implementation of a class that computes the variance
 * of a set of images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtVar.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

// default constructor
RtVar::RtVar() : RtStreamComponent() {
  id = "RtVar";
  numTimePoints = 0;
}

// destructor
RtVar::~RtVar() {}

// process a single acquisition
int RtVar::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtVar::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtDataImage *img = (RtDataImage*)msg->getCurrentData();

  if(img == NULL) {
    ACE_DEBUG((LM_INFO, "RtVar:process: image passed is NULL\n"));
    return -1;
  }

  if(numTimePoints == 0) {
    ACE_DEBUG((LM_DEBUG, "var found first image\n"));

    mean = (*img);
    numTimePoints++;

    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "including image %d in the variance estimate\n", 
	     img->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != mean.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtVar:process: last image is different size than this one\n"));
    return -1;    
  }
  
  // save the data
  hist.push_back(img);
  numTimePoints++;

  // allocate a new data image for the variance
  RtDataImage *var = new RtDataImage(img->getInfo());
  
  // update the mean and compute the variance for each voxel 
  for(int i = 0; i < img->getNumPix(); i++) {
    mean.setPixel(i, 
	(unsigned short) rint((numTimePoints-1)/(double)numTimePoints 
      * mean.getPixel(i) + img->getPixel(i)/(double)numTimePoints));
    
    // consider using an incremental approach
    unsigned short v = 0;
    for(vector<RtDataImage*>::iterator j = hist.begin(); j != hist.end(); j++){
      v += absdiff((*j)->getPixel(i), mean.getPixel(i))
	* absdiff((*j)->getPixel(i), mean.getPixel(i));
    }

    v = (unsigned short) rint(v/(double) numTimePoints);

    var->setPixel(i, v);
  }  

  // set the image id for handling
  var->addToID("voxel_variance");

//  string fn("/tmp/voxvar");
//  fn += img->getAcquisitionNum();
//  fn += ".dat";
//  var->write(fn);

  // add the variance to the message
  msg->addData(var);

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

