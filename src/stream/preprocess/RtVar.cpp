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
  componentID = moduleString;
  meanDataID = "data.image.mri.voxel-mean";
}

// destructor
RtVar::~RtVar() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtVar::processOption(const string &name, const string &text,
				      const map<string,string> &attrMap) {

  // look for known options
  if(name == "meanDataID") {
    meanDataID = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// process a single acquisition
int RtVar::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtVar::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtVar:process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtVar:process: image passed is NULL\n"));
    return 0;
  }

  // get the mean
  //cout << "getting mean data with id " << meanDataID << endl;
  RtMRIImage *mean = (RtMRIImage*)msg->getDataByID(meanDataID);

  if(mean == NULL) {
    cout << "RtVar::process: mean image not found" << endl;

    ACE_DEBUG((LM_INFO, "RtVar::process: mean image not found\n"));
    return 0;
  }


  if(numTimePoints == 0 || mean->getSeriesNum() != img->getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "var found first image\n"));

    varnum.setInfo(*img);

    for(unsigned int i = 0; i < varnum.getNumPix(); i++) {
      varnum.setPixel(i,0.0);
    }

    numTimePoints++;

    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "including image %d in the variance estimate\n", 
	     img->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != mean->getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtVar:process: last image is different size than this one\n"));
    return -1;    
  }
  
  // save the data
  numTimePoints++;


  // allocate a new data image for the variance
  RtMRIImage *var = new RtMRIImage(*img);
  
  // update the mean and variance numerator due to west (1979) for each voxel 
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) mean->getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
      + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    mean->setPixel(i, (unsigned short) newmean);
  
    double pixvarnum = varnum.getPixel(i);
    double newvarnum = pixvarnum 
      + (numTimePoints-1) * (thispix - pixmean) 
      * (thispix - pixmean) / (double) numTimePoints;
    varnum.setPixel(i,newvarnum);

    var->setPixel(i, (unsigned short) (newvarnum / (double)(numTimePoints-1)));
  }  

  // set the image id for handling
  var->addToID("voxel-variance");
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

