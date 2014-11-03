"""Class to retreive and store ROI activations from murfi. See the
accompanying test_murfi_communicator.py for example usage.
"""

import socket
import re
import random

class MurfiActivationCommunicator:

    def __init__(self, ip, port, num_trs, roi_names, fake=False):

        self._ip = ip
        self._port = port
        self._num_trs = num_trs
        self._fake = fake

        self._rois = {}
        for roi_name in roi_names:
            self._rois[roi_name] = {
                'last_tr': -1,
                'activation': [float('NaN')] * self._num_trs
            }

        self._roi_query = \
            '<?xml version="1.0" encoding="UTF-8"?>' \
            '<info>' \
            '<get dataid=":*:*:*:__TR__:*:*:roi-weightedave:__ROI__:"></get>' \
            '</info>\n'

    def _send(self, mesg):
        if not self._fake:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((self._ip, self._port))
            sock.send(mesg)
            resp = sock.recv(4096)
            sock.close()
            return resp
        else:
            return str(random.gauss(0,1))

    def _ask_for_roi_activation(self, roi_name, tr):
        if tr >= self._num_trs:
            raise ValueError("Requested TR out of bounds")
            return

        to_send = self._roi_query
        to_send = to_send.replace('__TR__', str(tr + 1))
        to_send = to_send.replace('__ROI__', roi_name)
        resp = self._send(to_send)

        stripped = re.sub("<.*?>", "", resp)
        try:
            num = float(stripped)
        except ValueError:
            num = float('nan')

        return num

    def get_roi_activation(self, roi_name, tr=None):
        if roi_name not in self._rois:
            raise ValueError("No such roi %s" % roi_name)

        if tr is None:
            tr = self._rois[roi_name]['last_tr']

        if tr < 0 or tr >= self._num_trs:
            raise ValueError("Requested TR out of bounds")

        return self._rois[roi_name]['activation'][tr]

    def update(self):
        for roi_name, roi in self._rois.iteritems():
            if roi['last_tr'] >= self._num_trs:
                continue

            act = self._ask_for_roi_activation(roi_name, roi['last_tr'] + 1)
            while act == act:
                roi['last_tr'] += 1
                roi['activation'][roi['last_tr']] = act
                act = self._ask_for_roi_activation(roi_name, roi['last_tr'] + 1)
