/******************************************************************************
 * RtIntensityNorm.cpp is the implementation of a class that normalizes the
 * intensity of subsequent images in a timeseries to match the global signal
 * of the first.
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-03-15
 *
 *****************************************************************************/

#include"RtIntensityNorm.h"
#include"ace/Mutex.h"

string RtIntensityNorm::moduleString("intensity-norm");

// default constructor
RtIntensityNorm::RtIntensityNorm() : RtStreamComponent() {
  componentID = moduleString;
  meanIntensity = fmod(1.0,0.0); // assign nan

  makeBETMask = false;
  computingMask = false;
  maskScript = "/home/ohinds/projects/realtime/system/scripts/make_bet_mask.sh";
  betParms = "-f 0.5 -g 0 -n -m";

  maskIntensityThreshold = 0.7;

}

// destructor
RtIntensityNorm::~RtIntensityNorm() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtIntensityNorm::processOption(const string &name, const string &text,
				      const map<string,string> &attrMap) {

  // look for known options
  if(name == "betMask") {
    return RtConfigVal::convert<bool>(makeBETMask,text);
  }
  if(name == "maskScript") {
    maskScript = text;
    return true;
  }
  if(name == "betParms") {
    betParms = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// process a single acquisition
int RtIntensityNorm::process(ACE_Message_Block *mb) {
  double sum = 0;
  
  ACE_TRACE(("RtIntensityNorm::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtIntensityNorm::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtIntensityNorm::process: image passed is NULL\n"));
    return 0;
  }

  // look for first image, compute ROI and mean intensity
  if(isnan(meanIntensity)) {
    ACE_DEBUG((LM_DEBUG, "intensity normalization found first image\n"));

    // if we are getting a brain mask from bet write the first image and
    // wait for bet to finish before we read it back in and continue
    if(makeBETMask) {
      if(!computingMask) {
	// execute commands to make the mask PLATFORM SPECIFIC!!!!!
	system("mkdir -p mask");

	// lock, unmosaic, write, mosaic
	ACE_Mutex mutx;
	mutx.acquire(); {	
	  img->unmosaic();	
	  img->write("mask/first_img.nii");
	  img->mosaic();	
	}  mutx.release();

	string cmd = maskScript + " " 
	  + img->getFilename() + " "
	  + "mask/brain" + " " 
	  + betParms;
	cout << "executing: " << cmd << endl;
	system(cmd.c_str());

	computingMask = true;

	return 0;
      }
      else {
	// execute commands to read the mask PLATFORM SPECIFIC!!!!!
	string cmd = "if [ -f mask/brain_mask.nii -o -f mask/brain_mask.nii.gz ]; then exit 0; else exit 1; fi";

	cerr << "looking for mask" << endl;
	if(!system(cmd.c_str())) {
	  cerr << "found it" << endl;

	  computingMask = false;
	  mask.read("mask/brain_mask.nii");
	  mask.computeNumberOfOnVoxels();
	  mask.mosaic();
//	  mask.setFilename("/tmp/mosaic_mask.nii");
//	  mask.write("/tmp/mosaic_mask.nii");
	}
	else {
	  cerr << "didnt found it" << endl;
	  return 0;
	}
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
  }
  
  ACE_DEBUG((LM_DEBUG, "intensity normalizing image %d\n", 
	     img->getAcquisitionNum()));
  
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
  
  // subtract the difference for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    inorm->setPixel(i, (img->getPixel(i) - intensityDiff > 0)
		    ? (img->getPixel(i) - intensityDiff) : 0);
  }  

  // set the image id for handling
  inorm->addToID("intensity-norm");
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

