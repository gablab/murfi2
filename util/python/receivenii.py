from collections import namedtuple
import os
import socket
import struct
from time import sleep

import threading
import SocketServer

import nibabel as nb
import numpy as np

SocketServer.TCPServer.allow_reuse_address = True

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
    def __init__(self, callback, infoclient, *args, **keys):
            self.callback = callback
            self.infoclient = infoclient
            SocketServer.BaseRequestHandler.__init__(self, *args, **keys)

    def handle(self):
        self.callback(self.infoclient, self.request)
        '''
        cur_thread = threading.current_thread()
        response = "{}: {}".format(cur_thread.name, data)
        self.request.sendall(response)
        '''

def handler_factory(callback, infoclient):
    def createHandler(*args, **keys):
        return ThreadedTCPRequestHandler(callback, infoclient,  *args, **keys)
    return createHandler

def processDataCallback(infoclient, sock):
    infoclient.process_data(sock)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

class InfoClient(object):

    def __init__(self, infoclient_host=None, infoclient_port=None,
                 server_host=None, server_port=None,
                 save_location=None):
        self.host = server_host
        self.port = server_port
        self.infoclient_host = infoclient_host
        self.infoclient_port = infoclient_port

        self._is_running = None
        self._server = None
        self.datastore = []
        self.imagestore = []
        self.save_location = save_location

        self.ei = ExternalImage("ExternalImageHeader", struct_def)

    def stop(self):
        self._server.shutdown()
        self._is_running = None
        self._server = None

    def start(self):
        self._startserver()

    def check(self):
        if not self._is_running:
            raise RuntimeError('Server is not running')
        return self.datastore

    def send(self, message):
        """Send a stream of bytes to the infoclient in murfi
        """

        # Create a socket (SOCK_STREAM means a TCP socket)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        e = None
        try:
            # Connect to server and send data
            sock.connect((self.infoclient_host, self.infoclient_port))
            sock.sendall(message)
        except Exception, e:
            pass
        finally:
            sock.close()
        if e:
            print e
        if e and "Connection refused" in e:
            raise IOError(e)
        #print "Sent:     {}".format(message)

    def _startserver(self):
        if self._is_running:
            raise RuntimeError('Server already running')

        server = ThreadedTCPServer((self.host, self.port),
                                   handler_factory(processDataCallback, self))
        ip, port = server.server_address
        print "server running at %s on port %d" % (ip, port)
        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()
        self._is_running = True
        self._server = server

    def process_data(self, sock):
        in_bytes = sock.recv(1024)
        if '<rt:done>' in in_bytes:
            self.stop()
            return
        new_ei = self.ei.process_data(in_bytes, sock)
        if new_ei:
            if isinstance(new_ei, nb.Nifti1Image) and new_ei not in self.imagestore:
                self.imagestore.append(new_ei)
                index = len(self.imagestore) - 1
                uid = self.datastore[index].seriesUID
                filename = os.path.join(self.save_location,
                                        'img-%s-%04d.nii.gz' % (uid, index))
                new_ei.to_filename(filename)
                print "Saved to %s" % filename
            elif new_ei not in self.datastore:
                self.datastore.append(new_ei)
        else:
            self.stop()


# 616 bytes
struct_def = [('magic', '5s'), # char       magic[5];       // Must be "ERTI"
           ('headerVersion', 'i'), # int32_t    headerVersion;  // Version of this header
           ('seriesUID','64s'), # char       seriesUID[64];  // The DICOM unique ID of this series
           ('scanType', '64s'), # char       scanType[64];   // Type of scan, e.g., "MPRAGE" or "EPI"
           ('imageType', '16s'), # char       imageType[16];  // Valid types: {"2Dz", "2Dzt", "3D", "3Dt"}
                                 # // 2Dz: Single volume, 1 slice at a time
                                 # // 2Dzt: Multi-volume, 1 slice at a time
                                 # // 3D: Single volume all at once
                                 # // 3Dt: Multi-volume, 1 volume at a time
           ('note', '256s'), # char       note[256];      // Text. Suggested contents: scanner make &
                             # //   model, pulse sequence name, version of
                             # //   image-sending software used, etc.
           ('dataType', '16s'), # char       dataType[16];   // Of image data. Valid (OS-independent) types:
                             # // - char   , c_char   , uchar   , c_uchar
                             # // - int16_t, c_int16_t, uint16_t, c_uint16_t
                             # // - int32_t, c_int32_t, uint32_t, c_uint32_t
                             # // - float32_t, c_float32_t, float64_t, c_float64_t
           ('isLittleEndian', '?'), # bool       isLittleEndian; // true if image data uses little endian byte order
           ('isMosaic', '?'), #bool       isMosaic;       // true if 3D or 3Dt images are mosaiced.
           ('pixelSpacingReadMM', 'd'), # float64_t  pixelSpacingReadMM;  // Pixel dimension (mm) in each
           ('pixelSpacingPhaseMM', 'd'), # float64_t  pixelSpacingPhaseMM; //   acquisition direction: read, phase,
           ('pixelSpacingSliceMM', 'd'), # float64_t  pixelSpacingSliceMM; //   & slice.
           ('sliceGapMM', 'd'), # float64_t  sliceGapMM;          // Gap between slice excitations (mm)
           ('numPixelsRead', 'i'), # int32_t    numPixelsRead;   // Number of pixels in read-direction
           ('numPixelsPhase', 'i'), # int32_t    numPixelsPhase;  // Number of pixels in phase-direction
           ('numSlices', 'i'), # int32_t    numSlices;       // Number of slices
           ('voxelToWorldMatrix', '16f'), #float32_t  voxelToWorldMatrix[4][4];  // Voxel indices -> world coordinates
           ('repetitionTimeMS', 'i'), # int32_t    repetitionTimeMS;  // Time (ms) of one volume acquisition
           ('repetitionDelayMS', 'i'), # int32_t    repetitionDelayMS; // Time (ms) between acquisitions
           ('currentTR', 'i'), # int32_t    currentTR;         // Current TR number
           ('totalTR', 'i'), # int32_t    totalTR;           // Expected number of TRs
           ('isMotionCorrected', '?'), # bool       isMotionCorrected; // True if this data has MC applied
           ('mcOrder', '5s'), # char       mcOrder[5];        // Order of MC operations. Eg "TZYX" or "YXZT"
                              #  //   T: Translation
                              #  //   X, Y, Z: Rotation in X, Y, Z
           ('mcTranslationXMM', 'd'), # float64_t  mcTranslationXMM;  // Translation (mm) that was applied
           ('mcTranslationYMM', 'd'), # float64_t  mcTranslationYMM;  //   by motion correction along each
           ('mcTranslationZMM', 'd'), # float64_t  mcTranslationZMM;  //   axis
           ('mcRotationXRAD', 'd'), # float64_t  mcRotationXRAD;    // Rotation (radians) that was applied
           ('mcRotationYRAD', 'd'), # float64_t  mcRotationYRAD;    //   by motion correction along each
           ('mcRotationZRAD', 'd'), #float64_t  mcRotationZRAD;    //   axis
           ]

def do_mosaic(data):
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
    dim = np.sqrt(np.prod(mosaic.shape))
    mosaic = mosaic.reshape(dim, dim)
    for idx in range(z):
        x_idx = int(np.floor(idx/n)) * x
        y_idx = int(idx % n) * y
        data[..., idx] = mosaic[x_idx:x_idx + x, y_idx:y_idx + y]
    return data

class ExternalImage(object):

    def __init__(self, typename, format_defn):
        self.names = []
        fmts = []
        for key, fmt in format_defn:
            self.names.append(key)
            fmts.append(fmt)
        self.formatstr = ''.join(fmts)
        self.struct = struct.Struct(self.formatstr)
        self.named_tuple_class = namedtuple(typename, self.names)
        self.hdr = None
        self.img = None
        self.num_bytes = None

    def hdr_from_bytes(self, byte_str):
        alist = list(self.struct.unpack(byte_str))
        values = []
        for idx, key in enumerate(self.names):
            if key != 'voxelToWorldMatrix':
                val = alist.pop(0)
                if isinstance(val, basestring):
                    values.append(val.rstrip(b'\0'))
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
        return self.struct.pack(*values)

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

    def from_image(self, img, idx, nt, mosaic=True):
        hdrinfo = self.create_header(img, idx, nt, mosaic)
        if idx is not None:
            data = img.get_data()[..., idx]
        else:
            data = img.get_data()
        if mosaic:
            data = do_mosaic(data)
        data = data.flatten().tolist()
        num_elem = len(data)
        return self.hdr_to_bytes(hdrinfo), struct.pack('%dH' % num_elem,
                                                       *data)

    def make_img(self, in_bytes):
        h = self.hdr
        if h.dataType != 'int16_t':
            raise ValueError('Unsupported data type')
        num_elem = self.num_bytes/2 #h.numPixelsRead * h.numPixelsPhase * h.numSlices
        data = struct.unpack('%dH' % num_elem, in_bytes)
        if h.isMosaic:
            data = demosaic(np.array(data), h.numPixelsRead, h.numPixelsPhase,
                             h.numSlices )
        else:
            data = np.array(data).reshape((h.numPixelsRead, h.numPixelsPhase,
                                           h.numSlices))
        affine = np.array(h.voxelToWorldMatrix).reshape((4, 4))
        img = nb.Nifti1Image(data, affine)
        img_hdr = img.get_header()
        img_hdr.set_zooms((h.pixelSpacingReadMM,
                           h.pixelSpacingPhaseMM,
                           h.pixelSpacingSliceMM))
        return img

    def process_data(self, in_bytes, sock):
        magic = struct.unpack('4s', in_bytes[:4])[0]
        if  magic == 'ERTI':
            # header
            self.hdr = self.hdr_from_bytes(in_bytes)
            h = self.hdr
            print "header received: TR=%d" % self.hdr.currentTR
            if self.hdr.isMosaic:
                nrows = int(np.ceil(np.sqrt(h.numSlices)))
                self.num_bytes = 2 * h.numPixelsRead * h.numPixelsPhase * nrows * nrows
            else:
                self.num_bytes = 2 * h.numPixelsRead * h.numPixelsPhase * h.numSlices
            print "Requires: %d bytes" % self.num_bytes
            return self.hdr
        else:
            # data
            msg = in_bytes
            while (self.num_bytes is None):
                sleep(.1)
            MSGLEN = self.num_bytes
            if MSGLEN is None:
                raise ValueError('MSGLEN should not be none')
            while len(msg) < MSGLEN:
                chunk = sock.recv(MSGLEN-len(msg))
                if chunk == '':
                    raise RuntimeError("socket connection broken")
                msg = msg + chunk
            print "data received: TR=%d bytes=%d" % (self.hdr.currentTR,
                                                     len(msg))
            self.img = self.make_img(msg)
            self.num_bytes = None
            return self.img

def serve_files(filenames, tr):
    ic = InfoClient('localhost', 15000, 'localhost', 15001)
    for idx, filename in enumerate(filenames):
        img = nb.load(filename)
        if len(img.shape) == 4:
            for i in range(img.shape[3]):
                hdr_bytes, data_bytes = ic.ei.from_image(img, i,
                                                         nt=img.shape[3],
                                                         mosaic=True)
                ic.send(hdr_bytes)
                sleep(.05)
                ic.send(data_bytes)
                sleep(tr)
        else:
            hdr_bytes, data_bytes = ic.ei.from_image(img, idx,
                                                     nt=len(filename),
                                                     mosaic=True)
            ic.send(hdr_bytes)
            sleep(.05)
            ic.send(data_bytes)
            sleep(tr)
    ic.send('<rt:done>')


def receive(save_location):
    ic = InfoClient('localhost', 15001, 'localhost', 15000,
                    save_location=save_location)
    ic.start()
    print "started"
    while(ic._is_running):
        sleep(5)
    print "stopped"

