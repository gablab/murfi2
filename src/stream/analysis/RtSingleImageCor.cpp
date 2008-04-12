/******************************************************************************
 * RtSingleImageCor.cpp is the implementation of a class that computes the
 * activation in a single image based on Hinds, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#include"RtSingleImageCor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"

string RtSingleImageCor::moduleString("singleimcor");

#define DUMP 0
#ifdef DUMP
#include<fstream>
static ofstream dumpfile("/tmp/sungle_image_dump.txt");
#endif

// default constructor
RtSingleImageCor::RtSingleImageCor() : RtActivationEstimator() {
  id = moduleString;

  z = f = g = h = NULL;

  baselineThreshold.set_size(1);
  baselineThreshold.put(0,0.0);
}

// destructor
RtSingleImageCor::~RtSingleImageCor() {
  deleteBaselineMeans();

  if(z != NULL) {
    delete z;
  }

  if(f != NULL) {
    delete f;
  }

  if(g != NULL) {
    delete g;
  }

  if(h != NULL) {
    delete h;
  }

  cout << "destroyed" << endl;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtSingleImageCor::processOption(const string &name, const string &text) {
  double tmp;

  // look for known options
  if(name == "baselineThreshold") {
    RtConfigVal::convert<double>(tmp,text);    
    baselineThreshold.put(0,tmp);
    return true;
  }

  return RtActivationEstimator::processOption(name, text);
}


// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtSingleImageCor::initEstimation(RtMRIImage &image) {
  RtActivationEstimator::initEstimation(image);

  // create images to store baseline estimates
  initBaselineMeans(&image);

  // get the number of datapoints we must process
  numData = image.getNumEl();

  //// initialize all subsidiary variables
  C = new vnl_matrix<double>(numTrends+1,numTrends+2);
  C->fill(0.0);
  C->fill_diagonal(1e-7);

  c = new vnl_matrix<double>(numData,numTrends+2);
  c->fill(0.0);
  for(unsigned int i = 0; i < numData; i++) {
    c->put(i,numTrends+1,1e-7);
  }

  f = new vnl_vector<double>(numTrends+1);
  f->fill(0.0);

  g = new vnl_vector<double>(numTrends+1);
  g->fill(0.0);

  h = new vnl_vector<double>(numTrends+1);
  h->fill(0.0);

  z = new vnl_vector<double>(numTrends+1);
  z->fill(0.0);


  needsInit = false;
}

// initialize the baseline mean estimation parameters to match an image
// in 
//  mri image to be used as a template to allocate space for running means
void RtSingleImageCor::initBaselineMeans(RtMRIImage *img) {
  // clean up existing versions
  deleteBaselineMeans();

  // allocate vectors and initialize them
  for(unsigned int i = 0; i < numConditions; i++) {
    baselineMeans.push_back(new RtActivation(*img));
  }

  numBaselineTimepoints.set_size(numConditions);
  numBaselineTimepoints.fill(0);
}

// deallocate all baseline mean related images
void RtSingleImageCor::deleteBaselineMeans() {
  for(unsigned int i = 0; i < baselineMeans.size(); i++) {
    if(baselineMeans[i] != NULL) {
      delete baselineMeans[i];
    }
  }
}

// process a single acquisition
int RtSingleImageCor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtSingleImageCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtSingleImageCor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtSingleImageCor:process: data passed is NULL\n"));
    return 0;
  }

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {
    initEstimation(*dat);
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtSingleImageCor::process: new data has wrong number of elements\n"));
    return -1;
  }

  // allocate a new data image for the correlation
  RtActivation *cor = new RtActivation(*dat);
  cor->initToZeros();

  // set threshold
  if(numTimepoints > numTrends+1) {
    cor->setThreshold(getTStatThreshold(1));
    cout << "single image est: using t threshold of " 
	 << cor->getThreshold() << endl;
  }

  //// element independent setup

  // decide if we are in an "on" or "off" stimulus condition for each condition
  static vnl_vector<unsigned int> isBaseline(numConditions);
  isBaseline.fill(0);

  // decide whether we are in an on or off condition
  for(unsigned int j = 0; j < numConditions; j++) {
    // when in baseline condition, set the flag and increment the 
    // number of timepoints this baseline condition has
    if(conditions->get(numTimepoints-1,j) < baselineThreshold.get(j)) { 
      isBaseline.put(j,1);
      numBaselineTimepoints.put(j,numBaselineTimepoints.get(j)+1);
    }
  }

  // build z
  for(unsigned int i = 0; i < numTrends; i++) {
    z->put(i,trends->get(numTimepoints-1,i));
  }
  z->put(numTrends,conditions->get(numTimepoints-1,0));

  double b_new, b_old = 1;

  // update the element independent entries of C
  for(unsigned int j = 0; j < numTrends+1; j++) {
    h->put(j, z->get(j)/C->get(j,j));
    b_new = sqrt(pow(b_old,2)+pow(h->get(j),2));
    f->put(j, b_new/b_old);
    g->put(j, h->get(j)/(b_new*b_old));
    b_old = b_new;

    for(unsigned int k = 0; k < numTrends+1; k++) {
      z->put(k, z->get(k) - h->get(j)*C->get(k,j));
      C->put(k,j, f->get(j)*C->get(k,j) + g->get(j)*z->get(k));
    }

  }

  double sum = 0;
  //// compute t map for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      cor->setPixel(i,fmod(1.0,0.0)); // assign nan
      continue;
    }

    double z_hat = dat->getElement(i);


//    #ifdef DUMP
//    //if(dat->getPixel(i) > 4090) {
//      dumpfile << c->get(i,numTrends+1) << " " << pow(c->get(i,numTrends+1),2) << " " << pow(z_hat/b_old,2) << " " << sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)) << endl;
//      //}
//    #endif


    // decide whether we are in an on or off condition
    // NOTE: by testing after the mask we are assuming a constant mask over time
    for(unsigned int j = 0; j < numConditions; j++) {
      if(isBaseline.get(j)) { // off
	// check whether this is the first image of the baseline
	if(numBaselineTimepoints.get(j) == 1) {	  
	  // replace the mean with the voxel intensity for first timepoint
	  baselineMeans[j]->setPixel(i, z_hat);
	}
	else {
	  // sum the voxel intensity within this baseline condition
	  baselineMeans[j]->setPixel(i, baselineMeans[j]->getPixel(i) + z_hat);
	}
//
//    if(i == 16*32*32 + 28*32 + 14) {
//      fprintf(stderr,"%f ", baselineMeans[j]->getPixel(i)/(numBaselineTimepoints.get(j) > 0 ? numBaselineTimepoints.get(j) : 1));
//    }    
      }
      else {
	// update the mean
	if(numBaselineTimepoints.get(j) > 0) {
	  baselineMeans[j]->setPixel(i, 
	      baselineMeans[j]->getPixel(i)/(numBaselineTimepoints.get(j)));
	}

//    if(i == 16*32*32 + 28*32 + 14) {
//      fprintf(stderr,"%f ", baselineMeans[j]->getPixel(i));
//    }



	// change z_hat to be the mean of the last baseline in the on condition
	// TODO: model trends within the on block (maybe this already happens)	
	z_hat = baselineMeans[j]->getPixel(i);
      }
    }

    for(unsigned int j = 0; j < numTrends+1; j++) {
      z_hat = z_hat - h->get(j)*c->get(i,j);

      c->put(i,j, f->get(j)*c->get(i,j) + g->get(j)*z_hat);
    }

    #ifdef DUMP
    //if(dat->getPixel(i) > 4090) {
      dumpfile << c->get(i,numTrends+1) << " " << pow(c->get(i,numTrends+1),2) << " " << pow(z_hat/b_old,2) << " " << sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)) << endl;
      //}
    #endif

    c->put(i,numTrends+1, sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)));
    // compute the correlation after we have enough timepoints to
    if(numTimepoints > numTrends+1) {
      cor->setPixel(i, (dat->getPixel(i) 
			- c->get(i,0)*h->get(0) 
			- c->get(i,1)*h->get(1))
		    * C->get(numTrends, numTrends)/c->get(i,numTrends+1));

      sum += cor->getPixel(i);

//      cor->setPixel(i, sqrt(numTimepoints-numTrends-1)
//		    * c->get(i,numTrends)/c->get(i,numTrends+1));
//      cout << sqrt(numTimepoints-numTrends-1)
//	* c->get(i,numTrends)/c->get(i,numTrends+1) << " ";
    }

//    if(i == 1 && numTimepoints == 2) {
//    if(i == 16*32*32 + 28*32 + 14) {
#ifdef DUMP
    dumpfile << numTimepoints << " " << i << " " << dat->getPixel(i)
	     << " " << c->get(i,0) << " " << h->get(0) 
	     << " " << c->get(i,1) << " " << h->get(1) 
             << " " << C->get(numTrends, numTrends) 
	     << " " << c->get(i,numTrends+1)
	     << " " <<  cor->getPixel(i) << endl;
    dumpfile.flush();
#endif
//
//      cout << "z: "; printVnlVector(*z); cout << endl;
//      cout << "f: "; printVnlVector(*f); cout << endl;
//      cout << "g: "; printVnlVector(*g); cout << endl;
//      cout << "h: "; printVnlVector(*h); cout << endl;
//      cout << "C: " << endl;
//      C->print(cout);
//      cout << "c: "; printVnlVector(c->get_row(i)); cout << endl;
//
//      //if(i > 40) {
//	int trash;
//	cin >> trash;
//      //}
//    }
  }
//  cout << endl;

  // set the image id for handling
  cor->addToID("voxel-singleimcor");
  cor->setRoiID(roiID);

  // in the first non-baseline condition reset the number of baseline timepoints
  for(unsigned int j = 0; j < numTrends+1; j++) {
    if(numBaselineTimepoints.get(j) > 0 && !isBaseline.get(j)) {
      numBaselineTimepoints.put(j,0);
    }
  }

  setResult(msg,cor);

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

