/******************************************************************************
 * RtSmoothRoi.cpp smooths data over an Roi
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtSmoothRoi.h"

#include "RtElementAccess.h"
#include"RtDataIDs.h"
#include"util/timer/timer.h"
#include<cmath>

// wow, i can't beleive i had to do this. why isn't log in a namespace??
inline static double logarithm(double d) { return log(d); }

string RtSmoothRoi::moduleString(ID_SMOOTHROI);

// default constructor
RtSmoothRoi::RtSmoothRoi() : RtStreamComponent() {
  componentID = moduleString;
  fwhm = -1;
  autoFwhm = true;
}

// destructor
RtSmoothRoi::~RtSmoothRoi() {}

// validate the configuration
bool RtSmoothRoi::validateComponentConfig() {
  autoFwhm = fwhm < 0;

  return true;
}

// process a single acquisition
int RtSmoothRoi::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtSmoothRoi::process"));

  timer tim;
  if(printTiming) {
    tim.start();
  }

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get image
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();
  if(img == NULL) {
    ACE_DEBUG((LM_INFO, "RtSmoothRoi:process: image passed is NULL\n"));

    return 0;
  }

  // get mask
  RtMaskImage *mask = getMaskFromMessage(*msg);
  if(mask == NULL) {
    cerr << "ERROR: could not find the mask for roi smooth" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtSmoothRoi::process: could not find the mask for roi smooth at tr "
	   << img->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }

  // set the fwhm if we have to
  if(autoFwhm) {
    fwhm = 2*img->getPixDim(0);
    autoFwhm = false;
  }

  RtMRIImage *smoothed = new RtMRIImage(*img);
  smoothed->getDataID().setModuleID(ID_SMOOTHROI);
  smoothed->getDataID().setDataName(NAME_SMOOTHROI);
  smoothed->getDataID().setRoiID(mask->getDataID().getRoiID());
  smoothed->initToNans();

  double sigmasquared = pow(fwhm / sqrt(8*logarithm(2.0)), 2);

  // copy voxel data into a vector
  vnl_vector<double> roiVox(mask->getNumberOfOnVoxels());
  RtElementAccess ac(img, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  unsigned int i = 0;
  for(vector<unsigned int>::iterator it = indices.begin(); 
      it != indices.end(); i++, it++) {
    roiVox[i] = static_cast<double>(img->getPixel(*it));
  }

  //// create convolution matrix
  vnl_matrix<double> convMat(roiVox.size(), roiVox.size());

  // compute pairwise voxel distances
  // apply gaussian equation
  i = 0;
  for(vector<unsigned int>::iterator it = indices.begin(); 
      it != indices.end(); i++, it++) {
    unsigned int j = 0;
    unsigned int x,y,z;
    img->get3Dfrom1D(*it,x,y,z);

    for(vector<unsigned int>::iterator it2 = indices.begin(); 
	it2 != indices.end(); j++, it2++) {
      unsigned int X,Y,Z;
      img->get3Dfrom1D(*it2,X,Y,Z);

      double dist = exp(-
	( pow((static_cast<int>(x)-static_cast<int>(X))/img->getPixDim(0),2) 
	+ pow((static_cast<int>(y)-static_cast<int>(Y))/img->getPixDim(1),2) 
	+ pow((static_cast<int>(z)-static_cast<int>(Z))/img->getPixDim(2),2)
	) / 2 / sigmasquared);

      convMat.put(i,j,dist);
      convMat.put(j,i,dist);
    }
  }

  // normalize columns
  i = 0;
  for(vector<unsigned int>::iterator it = indices.begin(); 
      it != indices.end(); i++, it++) {
    vnl_vector<double> col = convMat.get_column(i);
    col /= col.sum();
    convMat.set_column(i,col);
  }

  // convolve
  vnl_vector<double> sroiVox = roiVox * convMat;

  // copy vector back to volume
  i = 0;
  for(vector<unsigned int>::iterator it = indices.begin(); 
      it != indices.end(); i++, it++) {
    smoothed->setPixel(*it,static_cast<short>(sroiVox[i]));
  }
    
  setResult(msg, smoothed);

  if(printTiming) {
    tim.stop();
    cout << "RtSmoothRoi process at tr " 
	 << img->getDataID().getTimePoint()
	 << " elapsed time: " << tim.elapsed_time()*1000 << "ms"  << endl;
  }

  if(print) {
    cout << "RtSmoothRoi: done at tr " 
	 << img->getDataID().getTimePoint() << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtSmoothRoi: done at tr " 
	 << img->getDataID().getTimePoint() << endl;
    log(logs);
  }


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


