#!/usr/bin/env python

import multiprocessing
import os
import subprocess
import sys

def run(v):

    cmd = v[0]
    cwd = v[1]
    proc = subprocess.Popen(cmd, cwd=cwd)
    out, err = proc.communicate()
    return proc.returncode


def main():
    if os.path.basename(os.getcwd()) != "regression":
        print "this script must be run from murfi/test/regression"
        sys.exit(1)

    print "running test..."

    runs = []
    runs += [[["../bin/murfi",
               "-f" "scripts/neurofeedback_nodisplay.xml"],
              "../../example_data"]]
    runs += [[["./servedata.sh", "200", "15000", "localhost", "2"],
              "../../example_data/scripts"]]

    pool = multiprocessing.Pool(2)
    pool.map(run, runs)

if __name__ == "__main__":
    main()
