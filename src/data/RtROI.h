/*=========================================================================
 *  RtROI.h declares a class to handle ROI functionality
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RTROI_H
#define RTROI_H

#include<vector>
#include"RtData.h"

using namespace std;

#define DEFAULT_NUM_ROI_VOXELS 128;

// class declaration
template<class T>
class RtROI : public RtData {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtROI();

  // constructor taking a go-by data image and (optionally) a number of
  // voxels to reserve space for. (send NULL for the image to just set
  // the num voxels)
  RtROI(RtDataImage<T> *_data,
        unsigned int numVoxels = DEFAULT_NUM_ROI_VOXELS);

  // construct from another ROI and a different type of data image.
  //
  // use this to apply an ROI from a structural to a functional image.
  // converts the voxel indices using the world coordinate transforms
  // from vox2ras field of the ROI's and the DataImage's data fields.
  template <class S>
  RtROI(const RtROI<S> &otherROI, RtDataImage<T> *_data);

  // destructor
  virtual ~RtROI();

  // operators



  // mods

  //
  // add a voxel to the ROI by 1D index
  //
  bool addVoxel(unsigned int i);

  //
  // add a voxel to the ROI by 2D index (mosaic)
  //
  bool addVoxel(unsigned int i, unsigned int j);

  //
  // add a voxel to the ROI by 3D index
  //
  bool addVoxel(unsigned int i, unsigned int j, unsigned int k);

  //
  // add a voxel to the ROI by 3D coordinates
  //
  bool addVoxel(double r, double a, double s);

  //
  // add voxels to the ROI in a radius around another voxel (1D index)
  //
  bool addVoxelsInRadius(unsigned int i, double radius);

  //
  // add voxels to the ROI in a radius around another voxel (2D index)
  //
  bool addVoxelsInRadius(unsigned int i, unsigned int j, double radius);

  //
  // add voxels to the ROI in a radius around another voxel (3D index)
  //
  bool addVoxelsInRadius(unsigned int i, unsigned int j,
                         unsigned int k, double radius);

  //
  // add voxels to the ROI in a radius around another voxel (3D coords)
  //
  bool addVoxelsInRadius(double r, double a, double s, double radius);

  //
  // remove a voxel from the ROI by 1D index
  //
  bool removeVoxel(unsigned int i);

  //
  // remove a voxel from the ROI by 2D index (mosaic)
  //
  bool removeVoxel(unsigned int i, unsigned int j);

  //
  // remove a voxel from the ROI by 3D index
  //
  bool removeVoxel(unsigned int i, unsigned int j, unsigned int k);

  //
  // remove a voxel from the ROI by 3D coordinates
  //
  bool removeVoxel(double r, double a, double s);

  //
  // remove voxels from the ROI in a radius around another voxel (1D index)
  //
  bool removeVoxelsInRadius(unsigned int i, double radius);

  //
  // remove voxels from the ROI in a radius around another voxel (2D index)
  //
  bool removeVoxelsInRadius(unsigned int i, unsigned int j, double radius);

  //
  // remove voxels from the ROI in a radius around another voxel (3D index)
  //
  bool removeVoxelsInRadius(unsigned int i, unsigned int j,
                            unsigned int k, double radius);
  //
  // remove voxels from the ROI in a radius around another voxel (3D coords)
  //
  bool removeVoxelsInRadius(double r, double, a, double s, double radius);

  //
  // sum the contents of the ROI
  //
  T sum();

  //
  // mean of the contents of the ROI
  //
  T mean();

  //
  // median of the contents of the ROI
  //
  T median();

  // gets and sets

  //
  // set the data
  //
  bool setData(RtDataImage<T> *_data);

  //
  // get the data
  //
  RtDataImage<T> getData();

  //
  // get the voxel indices
  //
  vector<unsigned int> &getVoxelIndices();

 protected:

  //*** data members  ***//

  // data image
  RtDataImage<T> *data;

  // list of voxel indices in this ROI
  vector<unsigned int> voxels;

};

// implementation (template class def must be in same file!)

// default constructor
RtROI::RtROI() : data(NULL) {
  voxels.reserve(DEFAULT_NUM_ROI_VOXELS);
}


// constructor taking a go-by data image and (optionally) a number of
// voxels to reserve space for. (send NULL for the image to just set
// the num voxels)
RtROI::RtROI(RtDataImage<T> *_data,
             unsigned int numVoxels = DEFAULT_NUM_ROI_VOXELS) {
  data = _data;
  voxels.reserve(numVoxels);
}

// construct from another ROI and a different type of data image.
//
// use this to apply an ROI from a structural to a functional image.
// converts the voxel indices using the world coordinate transforms
// from vox2ras field of the ROI's and the DataImage's data fields.
//
// note: should only be used for constructing an ROI from a volume of
// same size or smaller voxels than the target. more complicated
// processing is required to fill smaller target voxels
template <class S>
RtROI::RtROI(const RtROI<S> &otherROI, RtDataImage<T> *_data) {
  double r,a,s;

  data = _data;

  if(data == NULL || otherROI.getData() == NULL) {
    cerr << "can't construct ROI from NULL data" << endl;
    return;
  }

  // get the other ROI's voxel indices and loop through them, adding a
  // voxel for ours for each.
  vector<unsigned int> otherVox = otherROI.getVoxelIndices();
  for(vector<unsigned int>::iterator i = otherVox.begin();
      i != otherVox.end(); i++) {
    otherROI.getData()->getRASfrom1D(i,r,a,s);
    addVoxel(r,a,s);
  }
}

// destructor
virtual RtROI::~RtROI() {

}

// mods

//
// add a voxel to the ROI by 1D index
//
bool RtROI::addVoxel(unsigned int i) {
  // TODO: disallow duplicate entries

  voxels.push_back(i);
  return true;
}

//
// add a voxel to the ROI by 2D index (mosaic)
//
bool RtROI::addVoxel(unsigned int i, unsigned int j) {
  if(data == NULL) {
    cerr << "data can't be NULL from conversion of indices from 2D to 1D"
         << endl;
    return false;
  }

  return addVoxel(data.get1Dfrom2D(i,j));
}

//
// add a voxel to the ROI by 3D index
//
bool RtROI::addVoxel(unsigned int i, unsigned int j, unsigned int k) {
  if(data == NULL) {
    cerr << "data can't be NULL from conversion of indices from 2D to 1D"
         << endl;
    return false;
  }

  return addVoxel(data.get1Dfrom3D(i,j,k));
}

//
// add a voxel to the ROI by 3D coordinates
//
bool RtROI::addVoxel(double r, double a, double s) {
  if(data == NULL) {
    cerr << "data can't be NULL from conversion of indices from 2D to 1D"
         << endl;
    return false;
  }

  return addVoxel(data.get1DfromRAS(r,a,s));
}

//
// add voxels to the ROI in a radius around another voxel (1D index)
//
bool RtROI::addVoxelsInRadius(unsigned int i, double radius) {
  return false;
}

//
// add voxels to the ROI in a radius around another voxel (2D index)
//
bool RtROI::addVoxelsInRadius(unsigned int i, unsigned int j, double radius) {
  return false;
}

//
// add voxels to the ROI in a radius around another voxel (3D index)
//
bool RtROI::addVoxelsInRadius(unsigned int i, unsigned int j,
                              unsigned int k, double radius) {
  return false;
}

//
// add voxels to the ROI in a radius around another voxel (3D coords)
//
bool RtROI::addVoxelsInRadius(double r, double a, double s, double radius) {
  return false;
}

//
// remove a voxel from the ROI by 1D index
//
bool RtROI::removeVoxel(unsigned int i) {
  return false;
}

//
// remove a voxel from the ROI by 2D index (mosaic)
//
bool RtROI::removeVoxel(unsigned int i, unsigned int j) {
  return false;
}

//
// remove a voxel from the ROI by 3D index
//
bool RtROI::removeVoxel(unsigned int i, unsigned int j, unsigned int k) {
  return false;
}

//
// remove a voxel from the ROI by 3D coordinates
//
bool RtROI::removeVoxel(double r, double a, double s) {
  return false;
}

//
// remove voxels from the ROI in a radius around another voxel (1D index)
//
bool RtROI::removeVoxelsInRadius(unsigned int i, double radius) {
  return false;
}

//
// remove voxels from the ROI in a radius around another voxel (2D index)
//
bool RtROI::removeVoxelsInRadius(
    unsigned int i, unsigned int j, double radius) {
  return false;
}

//
// remove voxels from the ROI in a radius around another voxel (3D index)
//
bool RtROI::removeVoxelsInRadius(unsigned int i, unsigned int j,
                                 unsigned int k, double radius) {
  return false;
}
//
// remove voxels from the ROI in a radius around another voxel (3D coords)
//
bool RtROI::removeVoxelsInRadius(double r, double, a, double s, double radius) {
  return false;
}

//
// sum the contents of the ROI
//
template <class T>
T RtROI::sum() {
  T t = 0;

  for(vector<unsigned int>::iterator i = voxels.begin(); i != voxels.end();
      i++) {
    t += data.getPixel(*i);
  }

  return t;
}

//
// mean of the contents of the ROI
//
T RtROI::mean() {
  return sum()/voxels.size();
}

//
// median of the contents of the ROI
//
T RtROI::median() {
  // sort
  // return
  return 0;
}

// gets and sets

//
// set the data
//
bool RtROI::setData(RtDataImage<T> *_data) {
  data = _data;
}

//
// get the data
//
RtDataImage<T> RtROI::getData() {
  return data;
}


#endif
