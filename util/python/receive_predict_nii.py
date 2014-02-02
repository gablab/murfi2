#!/usr/bin/env python

import argparse
from collections import namedtuple
import os
import socket
import sys
from time import sleep

import threading
import SocketServer

import external_image
import nibabel as nb
import numpy as np

from sklearn.externals import joblib

SocketServer.TCPServer.allow_reuse_address = True

import matplotlib.pyplot as plt
mask = nb.load('rtsatra/merged_brain_mask.nii.gz').get_data()
clf2 = joblib.load('rtsatra/model.pkl')
img = plt.imread('AF02_AN/morph%03d.png' % 0)
plt.clf()
plt.imshow(img)
plt.title('make her angry')
plt.show(block=False)

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

def process_data_callback(infoclient, sock):
    print "received info"
    infoclient.process_data(sock)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

class ImageReceiver(object):

    def __init__(self, args):
        self.host = args.host
        self.port = args.port

        self._is_running = None
        self._server = None
        self.imagestore = []
        self.save_location = args.save_directory
        self.current_uid = None
        self.current_series_hdr = None
        self.save_4d = args.four_dimensional
        self.stop_after_one_series = args.single_series

        self.ei = external_image.ExternalImage("ExternalImageHeader")

    def stop(self):
        self._server.shutdown()
        self._is_running = None
        self._server = None

        if self.save_4d:
            self.save_imagestore()

        print "image receiver stopped"

    def start(self):
        self._startserver()

    def check(self):
        if not self._is_running:
            raise RuntimeError('Server is not running')
        return self.imagestore

    def _startserver(self):
        if self._is_running:
            raise RuntimeError('Server already running')

        server = ThreadedTCPServer((self.host, self.port),
                                   handler_factory(process_data_callback, self))
        ip, port = server.server_address
        print "image receiver running at %s on port %d" % (ip, port)
        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()
        self._is_running = True
        self._server = server

    def process_data(self, sock):
        in_bytes = sock.recv(self.ei.get_header_size())

        if len(in_bytes) != self.ei.get_header_size():
            raise ValueError(
                "Header data wrong size: expected %d bytes, got %d" %
                (self.ei.get_header_size(), len(in_bytes))
                )
        print "processing %d header data bytes" % len(in_bytes)

        hdr = self.ei.process_header(in_bytes)
        print hdr

        # validation
        if self.current_uid != hdr.seriesUID:
            #assert hdr.currentTR == 1
            self.current_uid = hdr.seriesUID
            self.current_series_hdr = hdr

        img_data = ""
        while len(img_data) < self.ei.get_image_size():
            in_bytes = sock.recv(4096)
            img_data += in_bytes

        '''Uncommenting the following line will make things work
        '''
        img_data = img_data[:self.ei.get_image_size()]

        if len(img_data) != self.ei.get_image_size():
            raise ValueError(
                "Image data wrong size: expected %d bytes, got %d" %
                (self.ei.get_image_size(), len(img_data))
                )
        print "processing %d image data bytes" % len(img_data)

        new_ei = self.ei.process_image(img_data)
        if new_ei:
            if (isinstance(new_ei, nb.Nifti1Image) and
                new_ei not in self.imagestore):
                self.imagestore.append(new_ei)
                if not self.save_4d:
                    self.save_nifti(new_ei)
                TRs = len(self.imagestore)
                if TRs % 2 == 0:
                    data = new_ei.get_data()
                    probs = clf2.predict_proba(data[mask>0][None, :])
                    picture_index = int(100*probs[0][0])
                    img = plt.imread('AF02_AN/morph%03d.png' % picture_index)
                    print picture_index
                    plt.clf()
                    plt.imshow(img)
                    if int(TRs/30.) % 2 == 0:
                        plt.title('make her angry: %d' % picture_index)
                    else:
                        plt.title('make her calm: %d' % picture_index)
                    plt.draw()

            if hdr.currentTR + 1 == hdr.totalTR:
                if self.save_4d:
                    self.save_imagestore()
                    self.imagestore = []
                if self.stop_after_one_series:
                    self.stop()
        else:
            self.stop()

    def save_nifti(self, img):
        if len(img.get_shape()) == 3 or img.get_shape()[3] == 1:
            index = len(self.imagestore) - 1
            if index % 2 == 0:
                imgtype = 'pro'
            else:
                imgtype = 'ret'
            filename = os.path.join(
                self.save_location,
                'img-%s-%s-%05d.nii.gz' % (self.current_uid, imgtype, index))
        else:
            filename = os.path.join(self.save_location,
                                    'img-%s.nii.gz' % self.current_uid)
        img.to_filename(filename)
        print "Saved to %s" % filename

    def save_imagestore(self):
        if len(self.imagestore) == 0:
            return

        base_shape = self.imagestore[0].get_shape()
        new_shape = (base_shape[0], base_shape[1], base_shape[2],
                     len(self.imagestore))
        new_data = np.zeros(new_shape)
        for i in xrange(new_shape[3]):
            assert self.imagestore[i].get_shape() == \
                self.imagestore[0].get_shape()
            new_data[:,:,:,i] = self.imagestore[i].get_data()

        new_img = nb.Nifti1Image(new_data, self.imagestore[0].get_affine())
        new_img.get_header().set_zooms((
                self.current_series_hdr.pixelSpacingReadMM,
                self.current_series_hdr.pixelSpacingPhaseMM,
                self.current_series_hdr.pixelSpacingSliceMM,
                self.current_series_hdr.repetitionTimeMS +
                self.current_series_hdr.repetitionDelayMS))
        self.save_nifti(new_img)

def parse_args(args):
    parser = argparse.ArgumentParser()
    parser.add_argument("-H", "--host", default="localhost",
                        help="Name of the host to run the image receiver on.")
    parser.add_argument("-p", "--port", type=int, default="15000",
                        help="Port to run the image receiver on.")
    parser.add_argument("-d", "--save_directory", default=".",
                        help="Directory to save images to.")
    parser.add_argument("-f", "--four_dimensional", action="store_true",
                        help="Store each image series as a single 4D file.")
    parser.add_argument("-s", "--single_series", action="store_true",
                        help="Shut down the receiver after one entire series "
                        "has been read.")
    return parser.parse_args()

def main(argv):
    args = parse_args(argv)
    receiver = ImageReceiver(args)
    receiver.start()
    while(receiver._is_running):
        sleep(1)

if __name__ == "__main__":
    sys.exit(main(sys.argv))
