#!/usr/bin/env python

# TODO figure out what changed between
# ed2f3e8eb003b9fa840444a415b7a7c2d58e053a and 0543da7 that broke TR
# 85.

import multiprocessing
import os
import pickle
import re
import subprocess
import sys

def run(v):

    cmd = v[0]
    cwd = v[1]
    proc = subprocess.Popen(cmd, cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = proc.communicate()
    return out


def main():
    if os.path.basename(os.getcwd()) != "regression":
        print "this script must be run from murfi/test/regression"
        sys.exit(1)

    print "running test..."

    runs = []
    runs += [[["../bin/murfi",
               "-f" "scripts/neurofeedback_nodisplay.xml"],
              "../../example_data"]]
    runs += [[["./servedata.sh", "100", "15000", "localhost", "2"],
              "../../example_data/scripts"]]

    pool = multiprocessing.Pool(2)
    out = pool.map(run, runs)

    # get results by TR
    tr_data = []
    tr_searcher = re.compile(
        r"RtRoiCombine: active at tr (\d+) result is: ([-\d.]+)")
    for line in out[0].split("\n"):
        tr_match = tr_searcher.search(line)
        if tr_match is not None:
            tr = int(tr_match.groups()[0]) - 1
            assert tr == len(tr_data), "out of order tr outputs: %d != %d" % (
                tr, len(tr_data))
            tr_data.append(float(tr_match.groups()[1]))

    regr_tr_data = pickle.load(open("neurofeedback.rgr"))
    if len(regr_tr_data) != len(tr_data):
        print "ERROR: new and old data have different lengths (%d != %d)" % (
            len(tr_data), len(regr_tr_data))
        return 1

    success = True
    for i in xrange(0, len(tr_data)):
        if tr_data[i] != regr_tr_data[i]:
            success = False
            print "ERROR: new and old are different at TR %d (%f != %f)" % (
                i + 1, tr_data[i], regr_tr_data[i])

    if success:
        print "passed"
    else:
        print "TEST FAILED"

    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())
