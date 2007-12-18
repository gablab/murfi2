/******************************************************************************
 * RtAccumCor.cpp is the implementation of a class that computes the
 * instantaneous activation in an Image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtAccumCor.h"
#include"RtMRIImage.h"

string RtAccumCor::moduleString("accumcor");

// default constructor
RtAccumCor::RtAccumCor() : RtActivationEstimator() {
  id = moduleString;

  needsInit = true;
}

// destructor
RtAccumCor::~RtAccumCor() {
  cout << "destroyed" << endl;
}

// process a configuration option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtAccumCor::processOption(const string &name, const string &text) {
  // look for known options
  if(name == "something") {

    return true;
  }

  return RtStreamComponent::processOption(name, text);
}  

// process a single acquisition
int RtAccumCor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtAccumCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtAccumCor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtAccumCor:process: data passed is NULL\n"));
    return 0;
  }

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {

    // get the number of datapoints we must process
    numData = dat->getNumEl();

    // initialize the trend vectors
    //buildTrends();

    // initialize all subsidiary variables
    C.set_size(numTrends+1,numTrends+2);
    C.fill_diagonal(10e-7);

    c.set_size(numData,numTrends+2);
    for(unsigned int i = 0; i < numData; i++) {
      c.put(i,numTrends+1,10e-7);
    }

    f.set_size(numTrends+2);
    f.fill(0.0);

    g.set_size(numTrends+2);
    g.fill(0.0);

    h.set_size(numTrends+2);
    h.fill(0.0);

    z.set_size(numTrends+2);
    z.fill(0.0);

    needsInit = false;
    return 0;
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtAccumCor::process: new data has wrong number of elements\n"));
    return -1;    
  }

  // allocate a new data image for the correlation
  RtActivation *cor = new RtActivation(*dat);
  //cor->setScaleIsInverted(true);
  cor->setThreshold(2.3);

  //// element independent setup

  // build z
  for(unsigned int i = 0; i < numTrends; i++) {
    z.put(i,trends.get(numTimepoints,i));
  }
  z.put(numTrends,conditions.get(1,numTimepoints));
  
  double b_new, b_old = 1;

  // update the element independent entries of C
  for(unsigned int j = 0; j < numTrends+1; j++) {
    h.put(j, z.get(j)/C.get(j,j));
    b_new = sqrt(b_old*b_old+h[j]*h[j]);
    f.put(j, b_new/b_old);
    g.put(j, h[j]/(b_new*b_old));
    b_old = b_new;
    
    for(unsigned int k = 0; k < numTrends+1; k++) {
      z.put(k, z[k] - h[j]*C.get(k,j));
      C.put(k,j, f[j]*C.get(k,j) + g[j]*z[k]);
    }

  }


  // compute for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    double z_hat = dat->getElement(i);
	  
    for(unsigned int j = 0; j < numTrends+1; j++) {
      z_hat = z_hat - h[j]*c.get(i,j);
      c.put(i,j, f[j]*c.get(i,j) + g[j]*z_hat);
    }	  
    c.put(i,numTrends+2, sqrt(pow(c.get(i,numTrends+2),2)+pow(z_hat/b_old,2)));
//    p(vi,vj,vk) = c(vi,vj,vk,L+1)/sqrt(c(vi,vj,vk,L+2)^2+c(vi,vj,vk,L+1)^2);
//    a(vi,vj,vk) = c(vi,vj,vk,L+1)/C(L+1,L+1);
	  
    if(numTimepoints > numTrends+1) {
      cor->setPixel(i, sqrt(numTimepoints-numTrends-1) 
		    * c.get(i,numTrends+1)/c.get(i,numTrends+2));
    }
  }  
  cout << endl;

  // set the image id for handling
  cor->addToID("accumcor");
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

