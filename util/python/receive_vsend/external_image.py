from collections import namedtuple
import os
import struct
from time import sleep

import nibabel as nb
import numpy as np
import string

def mosaic(data):
    x, y, z = data.shape
    n = np.ceil(np.sqrt(z))
    X = np.zeros((n*x, n*y), dtype=data.dtype)
    for idx in range(z):
        x_idx = int(np.floor(idx/n)) * x
        y_idx = int(idx % n) * y
        # print x_idx, y_idx
        # print data.shape
        X[x_idx:x_idx + x, y_idx:y_idx + y] = data[..., idx]
        #import pylab
        #pylab.imshow(X, interpolation='nearest')
    return X

def demosaic(mosaic, x, y, z):
    data = np.zeros((x, y, z), dtype=mosaic.dtype)
    x,y,z = data.shape
    n = np.ceil(np.sqrt(z))
    dim = int(np.sqrt(np.prod(mosaic.shape)))
    mosaic = mosaic.reshape(dim, dim)
    for idx in range(z):
        x_idx = int(np.floor(idx/n)) * x
        y_idx = int(idx % n) * y
        data[..., idx] = mosaic[x_idx:x_idx + x, y_idx:y_idx + y]
    return data


class ExternalImage(object):

    # Definition of the Python equivalent of the C++ openheader datastructure.
    # See src/io/RtExternalImageInfo.h
    struct_def = [('magic', '5s'),
                  ('headerVersion', 'i'),
                  ('seriesUID','64s'),
                  ('scanType', '64s'),
                  ('imageType', '16s'),
                  ('note', '256s'),
                  ('dataType', '16s'),
                  ('isLittleEndian', '?'),
                  ('isMosaic', '?'),
                  ('pixelSpacingReadMM', 'd'),
                  ('pixelSpacingPhaseMM', 'd'),
                  ('pixelSpacingSliceMM', 'd'),
                  ('sliceGapMM', 'd'),
                  ('numPixelsRead', 'i'),
                  ('numPixelsPhase', 'i'),
                  ('numSlices', 'i'),
                  ('voxelToWorldMatrix', '16f'),
                  ('repetitionTimeMS', 'i'),
                  ('repetitionDelayMS', 'i'),
                  ('currentTR', 'i'),
                  ('totalTR', 'i'),
                  ('isMotionCorrected', '?'),
                  ('mcOrder', '5s'),
                  ('mcTranslationXMM', 'd'),
                  ('mcTranslationYMM', 'd'),
                  ('mcTranslationZMM', 'd'),
                  ('mcRotationXRAD', 'd'),
                  ('mcRotationYRAD', 'd'),
                  ('mcRotationZRAD', 'd'),
                  ]

    def __init__(self, typename, format_def=struct_def):
        self.names = []
        fmts = []
        for key, fmt in format_def:
            self.names.append(key)
            fmts.append(fmt)
        self.formatstr = ''.join(fmts)
        self.header_fmt = struct.Struct(self.formatstr)
        self.named_tuple_class = namedtuple(typename, self.names)
        self.hdr = None
        self.img = None
        self.num_bytes = None

    def hdr_from_bytes(self, byte_str):
        alist = list(self.header_fmt.unpack(byte_str))
        values = []
        for idx, key in enumerate(self.names):
            if key != 'voxelToWorldMatrix':
                val = alist.pop(0)
                if isinstance(val, basestring):
                    values.append(val.split(b'\0', 1)[0])
                else:
                    values.append(val)
            else:
                values.append([alist.pop(0) for i in range(16)])
        return self.named_tuple_class._make(tuple(values))

    def hdr_to_bytes(self, hdr_info):
        values = []
        for val in hdr_info._asdict().values():
            if isinstance(val, list):
                values.extend(val)
            else:
                values.append(val)
        return self.header_fmt.pack(*values)

    def create_header(self, img, idx, nt, mosaic):
        x ,y, z, t = img.shape
        sx, sy, sz, tr = img.get_header().get_zooms()
        affine = img.get_affine().flatten().tolist()
        EInfo = self.named_tuple_class
        infotuple = EInfo(magic='ERTI'.encode('ascii'),
                          headerVersion=1,
                          seriesUID='someuid',
                          scanType="EPI",
                          imageType='3D',
                          note='some note to leave',
                          dataType='int16_t',
                          isLittleEndian=True,
                          isMosaic=mosaic,
                          pixelSpacingReadMM=sx,
                          pixelSpacingPhaseMM=sy,
                          pixelSpacingSliceMM=sz,
                          sliceGapMM=0,
                          numPixelsRead=x,
                          numPixelsPhase=y,
                          numSlices=z,
                          voxelToWorldMatrix=affine,
                          repetitionTimeMS=tr*1000.,
                          repetitionDelayMS=0,
                          currentTR=idx,
                          totalTR=nt,
                          isMotionCorrected=True,
                          mcOrder='XYZT',
                          mcTranslationXMM=0.1,
                          mcTranslationYMM=0.2,
                          mcTranslationZMM=0.01,
                          mcRotationXRAD=0.001,
                          mcRotationYRAD=0.002,
                          mcRotationZRAD=0.0001)
        return infotuple

    def get_header_size(self):
        return self.header_fmt.size

    def get_image_size(self):
        return self.num_bytes

    def from_image(self, img, idx, nt, mosaic=True):
        hdrinfo = self.create_header(img, idx, nt, mosaic)
        if idx is not None:
            data = img.get_data()[..., idx]
        else:
            data = img.get_data()
        if mosaic:
            data = mosaic(data)
        data = data.flatten().tolist()
        num_elem = len(data)
        return self.hdr_to_bytes(hdrinfo), struct.pack('%dH' % num_elem,
                                                       *data)

    def make_img(self, in_bytes):
        h = self.hdr
        if h.dataType != 'int16_t':
            raise ValueError('Unsupported data type: %s' % h.dataType)

        data = struct.unpack('%dH' % (self.num_bytes / 2), in_bytes)
        if h.isMosaic:
            data = demosaic(np.array(data), h.numPixelsRead,
                            h.numPixelsPhase, h.numSlices)
        else:
            data = np.array(data).reshape((h.numPixelsRead, h.numPixelsPhase,
                                           h.numSlices))
        affine = np.array(h.voxelToWorldMatrix).reshape((4, 4))
        img = nb.Nifti1Image(data, affine)
        img_hdr = img.get_header()
        img_hdr.set_zooms((h.pixelSpacingReadMM,
                           h.pixelSpacingPhaseMM,
                           h.pixelSpacingSliceMM,
                           ))
        img_hdr.set_xyzt_units('mm', 'msec')
        img_hdr.set_dim_info()
        return img

    def process_header(self, in_bytes):
        magic = struct.unpack('4s', in_bytes[:4])[0]
        print magic
        if magic == 'ERTI' or magic == 'SIMU':
            # header
            self.hdr = self.hdr_from_bytes(in_bytes)
            h = self.hdr
            print "header received: TR=%d" % self.hdr.currentTR
            if self.hdr.isMosaic:
                nrows = int(np.ceil(np.sqrt(h.numSlices)))
                self.num_bytes = (2 * h.numPixelsRead *
                                  h.numPixelsPhase * nrows * nrows)
            else:
                self.num_bytes = (2 * h.numPixelsRead *
                                  h.numPixelsPhase * h.numSlices)
            print "Requires: %d bytes" % self.num_bytes
            return self.hdr
        else:
            raise ValueError("Unknown magic number %s" % magic)

    def process_image(self, in_bytes):
        self.img = self.make_img(in_bytes)
        return self.img
