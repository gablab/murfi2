/******************************************************************************
 * RtIntensityNorm.cpp is the implementation of a class that normalizes the
 * intensity of subsequent images in a timeseries to match the global signal
 * of the first.
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-03-15
 *
 *****************************************************************************/

#include"RtIntensityNorm.h"
#include"RtExperiment.h"
#include"RtConfigFmriExperiment.h"
#include"ace/Mutex.h"
#include"RtDataIDs.h"

string RtIntensityNorm::moduleString(ID_SPATIALINTENSITYNORM);

#define DEFAULT_BET_PARMS "-f 0.5 -g 0 -n -m"
#define DEFAULT_MASK_FILENAME "mask/brain_mask.nii"

// default constructor
RtIntensityNorm::RtIntensityNorm() : RtStreamComponent() {
  componentID = moduleString;
  dataName = NAME_SPATIALINTENSITYNORM_IMG;

  meanIntensity = fmod(1.0,0.0); // assign nan

  makeBETMask = false;
  computingMask = false;
  betParms = DEFAULT_BET_PARMS;
  maskFilename = DEFAULT_MASK_FILENAME;

  maskIntensityThreshold = 0.7;

}

// destructor
RtIntensityNorm::~RtIntensityNorm() {
  //cout << "destroyed" << endl;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtIntensityNorm::processOption(const string &name, const string &text,
				      const map<string,string> &attrMap) {

  // look for known options
  if(name == "maskFilename") {
    maskFilename = text;
    return true; 
  }
  if(name == "betMask") {
    return RtConfigVal::convert<bool>(makeBETMask,text);
  }
  if(name == "betParms") {
    betParms = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// process a single acquisition
int RtIntensityNorm::process(ACE_Message_Block *mb) {
  static bool needsInit = true; 
  double sum = 0;
  
  ACE_TRACE(("RtIntensityNorm::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtIntensityNorm::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtIntensityNorm::process: image passed is NULL\n"));
    return 0;
  }

  // look for first image, compute ROI and mean intensity
  if(needsInit) {
    ACE_DEBUG((LM_DEBUG, "intensity normalization found first image\n"));

    mask.setFilename(maskFilename);

    // if we are getting a brain mask from bet write the first image and
    // wait for bet to finish before we read it back in and continue
    if(makeBETMask) {
      if(!computingMask) {
	// execute commands to make the mask
	maskJobID = RtFSLInterface::makeBrainMask(
	        getConfig().getSeriesRefVolFilename(
				    img->getDataID().getSeriesNum()), 
		mask.getFilename(),
		betParms);
	
	computingMask = true;

	return 0;
      }
      else if(computingMask 
	      && RtFSLInterface::getJobStatus(maskJobID) == FSL_JOB_FINISHED) {

	computingMask = false;
	mask.load();
	mask.computeNumberOfOnVoxels();
	//mask.mosaic();
	//	  mask.setFilename("/tmp/mosaic_mask.nii");
	//	  mask.write("/tmp/mosaic_mask.nii");
      }
      else if(computingMask 
	      && RtFSLInterface::getJobStatus(maskJobID) == FSL_JOB_ERROR) {	
	computingMask = false;
	cerr << "RtIntensityNorm: error computing brain mask. guessing by threshold" << endl;
	mask.initByMeanIntensityThreshold(*img, maskIntensityThreshold);
      }
      else {
	cerr << "not done computing mask..." << endl;
	return 0;
      }
    }
    else {
      mask.initByMeanIntensityThreshold(*img, maskIntensityThreshold);
    }

    // iterate over pixels accumulating intensity
    sum = 0;
    for(unsigned int i = 0; i < img->getNumPix(); i++) {
      if(mask.getPixel(i)) {
	sum += img->getPixel(i);
      }
    }

    // store the average intensity in mask
    meanIntensity = sum/mask.getNumberOfOnVoxels();

    cout << "found mean intensity of " << meanIntensity << "=" 
	 << sum << "/" << mask.getNumberOfOnVoxels() << endl;

    needsInit = false;
  }
  
//  ACE_DEBUG((LM_DEBUG, "intensity normalizing image %d\n", 
//	     img->getAcquisitionNum()));
  
  // compute the mean intensity in the mask for this image

  // iterate over pixels accumulating intensity
  sum = 0;
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    if(mask.getPixel(i)) {
      sum += img->getPixel(i);
    }
  }

  // norm factor
  short intensityDiff 
    = (short) rint(sum/mask.getNumberOfOnVoxels() - meanIntensity);
  
  // allocate a new data image for normalized image
  RtMRIImage *inorm = new RtMRIImage(*img);
  
  inorm->getDataID().setFromInputData(*img,*this);
  inorm->getDataID().setDataName(dataName);

  // subtract the difference for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    inorm->setPixel(i, (img->getPixel(i) - intensityDiff > 0)
		    ? (img->getPixel(i) - intensityDiff) : 0);
  }  

  // set the image id for handling
  //inorm->addToID("intensity-norm");
  setResult(msg,inorm);

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

