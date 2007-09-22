/******************************************************************************
 * RtVar.h is the implementation of a class that computes the variance
 * of a set of images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtVar.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtVar::moduleString("voxel-variance");

// default constructor
RtVar::RtVar() : RtStreamComponent() {
  numTimePoints = 0;
  mean.addToID("voxel_variance");
  id = moduleString;
}

// destructor
RtVar::~RtVar() {
  cout << "destroyed" << endl;
}

// process a single acquisition
int RtVar::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtVar::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtDataImage *img = (RtDataImage*)msg->getCurrentData();

  cout << "voxel-variance got msg " << msg 
       << " with curdata " << img
       << endl;

  if(img == NULL) {
    cout << "RtVar:process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtVar:process: image passed is NULL\n"));
    return 0;
  }

  if(numTimePoints == 0 || mean.getSeriesNum() != img->getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "var found first image\n"));

    mean = (*img);
    varnum.setImage(img->getInfo());

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
  
  // update the mean and variance numerator due to west (1979) for each voxel 
  for(int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) mean.getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
      + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    mean.setPixel(i, (unsigned short) newmean);
  
    int pixvarnum = (int) varnum.getPixel(i);
    int newvarnum = pixvarnum 
      + (numTimePoints-1) * (thispix - pixmean) 
      * (int) rint((thispix - pixmean)/(double)numTimePoints);
    varnum.setPixel(i, (unsigned short) newvarnum);

    // consider using an incremental approach
    unsigned short v = 0;
    for(vector<RtDataImage*>::iterator j = hist.begin(); j != hist.end(); j++){
      v += absdiff((*j)->getPixel(i), mean.getPixel(i))
	* absdiff((*j)->getPixel(i), mean.getPixel(i));
    }

    v = (unsigned short) rint(v/(double) numTimePoints);

    cout << newvarnum/(numTimePoints) << " ";
    if(!i%20) {
      cout << endl;
    }

    var->setPixel(i, (unsigned short) newvarnum/(numTimePoints));
    //var->setPixel(i, mean.getPixel(i));
    //var->setPixel(i, varnum.getPixel(i));
  }  

  // set the image id for handling
  var->addToID("voxel_variance");
//
//  string fn("/tmp/voxvar");
//  fn += img->getAcquisitionNum();
//  fn += ".dat";
//  var->write(fn);
//
  setResult(msg,var);

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

