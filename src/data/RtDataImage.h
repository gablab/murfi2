/******************************************************************************
 * RtDataImage.h declares a base class for a generic image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

#ifndef RTDATAIMAGE_H
#define RTDATAIMAGE_H

#include<iostream>
#include"ace/Date_Time.h"
#include"ace/Mutex.h"
#include<iostream>
#include<vector>
#include<iomanip>
#include<cmath>
#include<vnl/vnl_matrix_fixed.h>
#include<gsl/gsl_vector.h>

#include"RtData.h"
#include"nifti1_io.h"

#include"debug_levels.h"

using namespace std;

/*
  Notes about conversion of this type to use itk::Image internally
  IF WE SHOULD REALLY DO THIS.
  
  - probably use itk::Image<T,4> to represent 4D images, where T will be short
    or double
 
  - use smartpointers

  - use itk::ImportImageFilter to convert from scanner data array

  - got to use iterators to tranverse data sequentially

  - use itk::PointSet to represent masks (make sure to insert and retrieve
    using iterators)

  - 
 */

// helper for shared memory locking

class RtMutex : public ACE_Mutex {
public:

    void operator=(RtMutex &other) {
    }
};

// class declaration

template<class T>
class RtDataImage : public RtData {
#ifdef USE_FRONTEND
    // We need method to access data in frontend
    friend class FrMaskEditor;
    friend class FrDocumentReader;
#endif

public:

    const static unsigned int MAGIC_NUMBER = 0;

    //*** constructors/destructors  ***//

    // default constructor
    RtDataImage();

    // constructor with a filename to read the image from
    RtDataImage(const string &filename);

    // constructor with a number of
    RtDataImage(unsigned int);

    // construct from raw bytes -- BE CAREFUL WITH THIS
    //RtDataImage(char *bytes, unsigned int len);

    // construct from an image info struct and (possibly blank) data
    //RtDataImage(RtDataImageInfo &info, T *data = NULL);

    // construct from another image
    RtDataImage(RtDataImage &img);

    // destructor
    virtual ~RtDataImage();

    // lock the data (read/write)
    virtual void lock();

    // unlock the data
    virtual void unlock();

    // save the image to a file (already set filename)
    //  out
    //   success or failure
    virtual bool save();

    // write an image to a file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool write(const string &filename);

    // write an image to a raw file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool writeRaw(const string &filename);

    // write an image to a nifti file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool writeNifti(const string &filename);

    // write the info (all but data) to a stream
    //  in
    //   stream to write to
    //  out
    //   success or failure
    virtual bool writeInfo(ostream &os);

    // write the data to a stream
    //  in
    //   stream to write to
    //  out
    //   success or failure
    virtual bool writeData(ostream &os);

    // load the image to a file (already set filename)
    //  out
    //   success or failure
    virtual bool load();

    // read an image header from a file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool readHeader(const string &filename);

    // read an image from a file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool read(const string &filename);

    // read an image from a raw file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool readRaw(const string &filename);

    // read the header from a nifti1 image file
    // WARNING! sets header info but does not read the pixels, can create
    // a mismatch between data and info
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    bool readNiftiHeader(const string &_filename);

    // read an image from a nifti file
    //  in
    //   filename: string filename
    //  out
    //   success or failure
    virtual bool readNifti(const string &filename);

    // read info from a stream
    //  in
    //   input stream to read from
    //  out
    //   success or failure
    virtual bool readInfo(istream &is);

    // read data from a stream
    //  in
    //   input stream to read from
    //  out
    //   success or failure
    virtual bool readData(istream &is);

    // set info from a nifti image header
    //  in
    //   nifti image struct
    //  out
    //   success or failure
    bool setInfo(nifti_image *hdr);

    // copy info from our image into a nifti image header
    //  in
    //   nifti image struct
    //  out
    //   success or failure
    bool copyInfo(nifti_image *hdr);

    // print info about this image
    //  in
    //   output stream to write to
    virtual void printInfo(ostream &os);

    // serialize the data as xml for transmission or saving to a file
    virtual TiXmlElement *serializeAsXML(TiXmlElement *requestElement);

    // unserialize the xml data
    virtual void unserializeXML(TiXmlElement* element);

    // flip each slice LR
    bool flipLR();

    // convert to a mosaic representation
    bool mosaic();

    // convert from a mosaic representation
    bool unmosaic();

    // get a copy of the data in mosaiced format
    //  out
    //   copy: pointer to the data that will be created
    T* getMosaicedCopy();

    // test for mosaic image format
    bool isMosaic();

    // guess whether the image is mosaiced based on the dimensions
    bool seemsMosaic();

    // get the width of a potential mosaiced version of this data
    unsigned int getMosaicedWidth();

    // get the height of a potential mosaiced version of this data
    unsigned int getMosaicedHeight();

    //********  methods for getting data from the image *******//

    // get the unique data id
    RtDataID &getDataID();

    // initialize to all zeros
    void initToZeros();

    // initialize to all nans
    void initToNans();

    // get dimensions
    vector<int> &getDims();

    // get dimensions
    int getDim(int i);

    // get voxel dimensions
    vector<double> &getPixDims();

    // get voxel dimensions
    double getPixDim(int i);

    // get vxl2ras
    vnl_matrix_fixed<double, 4, 4 > &getVxl2Ras();

    // get ras2ref
    vnl_matrix_fixed<double, 4, 4 > &getRas2Ref();

    // get number of pix
    unsigned int getNumPix();

    // get number of elements
    unsigned int getNumEl();

    // get number of bytes per pixel
    unsigned short getBytesPerPix();

    // get pixel value
    T getPixel(unsigned int i);

    // get element value
    T getElement(unsigned int i);

    // get data size
    unsigned int getImgDataLen();

    // get a copy of the image data
    T *getDataCopy();

    // get a const reference to the image data
    const T *getDataConst();

    //*** sets  ***//

    // set pixel dimensions
    void setPixDim(unsigned int i, double dim);

    // set pixel value
    void setPixel(unsigned int i, T v);

    // set element value
    void setElement(unsigned int i, T v);

    // sets the min and max pixel value for this data image
    void setMinMax();

    void setFilename(string _filename);

    string getFilename() const;


    //************** transforms ******************//

    // transform 1D index into RAS world coords
    void getRASfrom1D(unsigned int i, double &r, double &a, double &s);

    // transform 2D index into RAS world coords
    void getRASfrom2D(unsigned int i, unsigned int j,
                      double &r, double &a, double &s);

    // transform 3D index into RAS world coords
    void getRASfrom3D(unsigned int i, unsigned int j, unsigned int k,
                      double &r, double &a, double &s);

    // transform 2D index into 1D index
    unsigned int get1Dfrom2D(unsigned int i, unsigned int j);

    // transform 3D index into 1D index
    unsigned int get1Dfrom3D(unsigned int i, unsigned int j, unsigned int k);

    // transform RAS world coords into 1D index
    unsigned int get1DfromRAS(double r, double a, double s);

    // transform 1D index into 2D index
    void get2Dfrom1D(unsigned int i1D, unsigned int &i2D, unsigned int &j2D);

    // transform 3D index into 2D index
    void get2Dfrom3D(unsigned int i1D, unsigned int j1D, unsigned int k1D,
                     unsigned int &i2D, unsigned int &j2D);

    // transform 1D index into 3D index
    void get3Dfrom1D(unsigned int i1D,
                     unsigned int &i3D, unsigned int &j3D, unsigned int &k3D);

    // transform 2D index into 3D index
    void get2Dfrom2D(unsigned int i1D, unsigned int j1D,
                     unsigned int &i3D, unsigned int &j3D, unsigned int &k3D);

    //************** time conversion functions ******************//

    // convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
    static ACE_Date_Time siemensTime2ACE_Date_Time(const char *tc);

    // convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
    static string ACE_Date_Time2SiemensTime(const ACE_Date_Time &t);

protected:

    // image data
    T *data;

    // for managing shared memory access
    RtMutex mutex;

    // magic number for this image type
    unsigned int magicNumber;

    // filename to save/load the image to/from
    string filename;

    //// transformations on read

    bool alignOnRead; // whether to transform this image into series
    // reference space before loading it. assumes that
    // this image is in study reference space and may
    // severly slow down initial loading.
    bool mosaicOnRead; // automatically mosaic loaded images
    bool unMosaicOnRead; // automatically unmosaic loaded images
    bool flipLROnRead; // automatically flip loaded images along LR axis

    // dimensions of the image data
    vector<int> dims;
    vector<double> pixdims;

    // info about the pixels
    unsigned long imgDataLen;
    unsigned long numPix;
    unsigned short bytesPerPix;

    // transformation matrix: voxels to RAS space
    vnl_matrix_fixed<double, 4, 4 > vxl2ras;

    // transformation matrix: RAS to reference space
    vnl_matrix_fixed<double, 4, 4 > ras2ref;

    // fields for storing min and max values
    bool minMaxSet;
    T minVal, maxVal;

    // for transforming between mosaic representation
    bool isMosaiced;
    unsigned short matrixSize;
    unsigned short numSlices;
    double sliceThick;
    double sliceGap;

private:
    // convert to a mosaic representation
    bool mosaicUnlocked();

    // convert from a mosaic representation
    bool unmosaicUnlocked();

};


// definition of template class here due to gcc template compilation limitations
#include"RtDataImage.tpp"


#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


