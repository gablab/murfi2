/******************************************************************************
 * RtFluctuationMonitor.cpp is the implementation of a class that computes the
 * activation in a single image based on Hinds, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#include"RtFluctuationMonitor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"

string RtFluctuationMonitor::moduleString("fluctuationmonitor");


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtSingleImageCor() {
  id = moduleString;
}

// destructor
RtFluctuationMonitor::~RtFluctuationMonitor() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtFluctuationMonitor::processOption(const string &name, const string &text) {
  // look for known options
//  if(name == "baselineThreshold") {
//    RtConfigVal::convert<double>(tmp,text);    
//    baselineThreshold.put(0,tmp);
//    return true;
//  }

  return RtSingleImageCor::processOption(name, text);
}

// process a single acquisition
int RtFluctuationMonitor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtFluctuationMonitor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtFluctuationMonitor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor:process: data passed is NULL\n"));
    return 0;
  }

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {
    initEstimation(*dat);
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor::process: new data has wrong number of elements\n"));
    return -1;
  }

  // allocate a new data image for the correlation
  RtActivation *fluct = new RtActivation(*dat);
  fluct->initToZeros();

  //// element independent setup

  // build z
  for(unsigned int i = 0; i < numTrends; i++) {
    z->put(i,trends->get(numTimepoints-1,i));
  }
  z->put(numTrends,1); // regressor always 1

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
      fluct->setPixel(i,0.0);
      continue;
    }

    double z_hat = dat->getElement(i);
    for(unsigned int j = 0; j < numTrends+1; j++) {
      z_hat = z_hat - h->get(j)*c->get(i,j);
      c->put(i,j, f->get(j)*c->get(i,j) + g->get(j)*z_hat);
    }
    c->put(i,numTrends+1, sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)));
    if(numTimepoints > numTrends+1) {
      fluct->setPixel(i, (dat->getPixel(i) 
			  - c->get(i,0)*h->get(0) 
			  - c->get(i,1)*h->get(1)) 
		      / sqrt(c->get(i,numTrends+1)));
      //		    * C->get(numTrends, numTrends)/c->get(i,numTrends+1));

      sum += fluct->getPixel(i);

//      cor->setPixel(i, sqrt(numTimepoints-numTrends-1)
//		    * c->get(i,numTrends)/c->get(i,numTrends+1));
//      cout << sqrt(numTimepoints-numTrends-1)
//	* c->get(i,numTrends)/c->get(i,numTrends+1) << " ";
    }

//    if(i == 1 && numTimepoints == 2) {
//    if(i == 16*32*32 + 28*32 + 14) {
//      fprintf(stderr,"%d %d %f %f %f\n", numTimepoints, dat->getElement(i),
//	      conditions->get(numTimepoints-1,0), z_hat, cor->getPixel(i));
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

  cout << "sum " << sum << endl;

  // set the image id for handling
  fluct->addToID("voxel-fluctuationmonitor");
  fluct->setRoiID(roiID);

  setResult(msg,fluct);

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

