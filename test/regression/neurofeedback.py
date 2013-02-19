#!/usr/bin/env python

import multiprocessing
import os
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

    # read the regression standard data


if __name__ == "__main__":
    main()
