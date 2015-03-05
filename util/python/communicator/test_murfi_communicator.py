#!/usr/bin/python

import sys
import time

from murfi_activation_communicator import MurfiActivationCommunicator


def main(argv):

    roi_names = ['pcc', 'brain-pcc']
    communicator = MurfiActivationCommunicator('localhost',
                                               15001, 100,
                                               roi_names)

    while True:
        communicator.update()

        for roi in roi_names:
            try:
                print "%s: %f" % (roi, communicator.get_roi_activation(roi))
            except:
                pass

        time.sleep(1)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
