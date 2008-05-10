#include<nifti1_io.h>
#include<iostream>
#include<vector>

using namespace std;

void build_hdr(nifti_image *hdr) {
  // info
  hdr->iname_offset = 348;
  hdr->swapsize = 0;
  hdr->analyze75_orient = a75_transverse_unflipped;
  
  // filename
  hdr->fname = (char*) malloc(256*sizeof(char));
  strcpy(hdr->fname,"/tmp/tmp.nii");
  hdr->iname = (char*) malloc(256*sizeof(char));
  strcpy(hdr->iname,"/tmp/tmp.nii");

  vector<unsigned int> dims;
  dims.push_back(192);
  dims.push_back(192);
  dims.push_back(1);

  // dims
  hdr->ndim = (dims.size() >= 4) ? dims.size() : 4;
  hdr->dim[0] = hdr->ndim;
  for(int ind = 1; ind < 8; ind++) {
    hdr->dim[ind] = 1;
    hdr->pixdim[ind] = 1;
  }

  for(unsigned int ind = 1; ind <= dims.size(); ind++) {
    hdr->dim[ind] = dims[ind-1];
    hdr->pixdim[ind] = 1.0f;
  }

  // crappy
  switch(hdr->ndim) {
  case 7:
    hdr->nw = hdr->dim[7];
    hdr->dw = hdr->pixdim[7];
  case 6:
    hdr->nv = hdr->dim[6];
    hdr->dv = hdr->pixdim[6];
  case 5:
    hdr->nu = hdr->dim[5];
    hdr->du = hdr->pixdim[5];
  case 4:
    hdr->nt = hdr->dim[4];
    hdr->dt = hdr->pixdim[4];
  case 3:
    hdr->nz = hdr->dim[3];
    hdr->dz = hdr->pixdim[3];
  case 2:
    hdr->ny = hdr->dim[2];
    hdr->dy = hdr->pixdim[2];
  case 1:
    hdr->nx = hdr->dim[1];
    hdr->dx = hdr->pixdim[1];
    break;
  default:
    break;
  }

  // img data info
  hdr->nvox = hdr->dim[1]*hdr->dim[2]*hdr->dim[3];
  hdr->nbyper = 2;

  // data type
  //hdr->datatype = DT_SIGNED_SHORT;
  hdr->datatype = DT_UINT16;

  hdr->scl_slope = 0;
  hdr->scl_inter = 0;
  hdr->slice_start = 0;
  hdr->slice_end = 0;

  // nifti type (NIFTI-1 (1 file)
  hdr->nifti_type = 1;

  hdr->xyz_units = NIFTI_UNITS_MM;
  hdr->time_units = NIFTI_UNITS_MSEC;

  hdr->qform_code = 1;
  hdr->quatern_b = 0;
  hdr->quatern_c = 0;
  hdr->quatern_d = 0;
  hdr->qoffset_x = 0;
  hdr->qoffset_y = 0;
  hdr->qoffset_z = 0;
  hdr->num_ext = 0;
  hdr->ext_list = NULL;
}

int main() {
  // fill the nifti struct with our data
  nifti_image *img = (nifti_image*) malloc(sizeof(nifti_image));
  build_hdr(img);

  // allocate and copy data

  // make artificial data
  unsigned short *data = new unsigned short[img->nbyper*img->nvox];
  //short *data = new short[img->nbyper*img->nvox];
  for(int i=0; i < img->dim[1]*img->dim[2]*img->dim[3]; i++) {
    data[i] = i;
  }

  img->data = (void*) malloc(img->nbyper*img->nvox);
  memcpy(img->data, data, img->nbyper*img->nvox);

  // debugging
  nifti_image_infodump(img);

  // write the file
  nifti_image_write(img);

  nifti_image_free(img);  

  delete data;
  
  return 0;
}
