from dataclasses import dataclass
from functools import partial
import multiprocessing
import os
from pathlib import Path
import re
import subprocess
import sys


EXPECTED_OUTPUT = [
    "0",
    "0",
    "-0.102086",
    "-0.1576",
    "-0.133939",
    "0.196203",
    "0.52309",
    "0.867423",
    "1.21708",
    "1.2192",
    "1.68033",
    "1.2418",
    "0.337687",
    "-0.481095",
    "-0.375066",
    "-0.779692",
    "-0.561479",
    "0.309684",
    "0.852785",
    "0.987761",
    "1.12479",
    "0.899453",
    "0.386156",
    "-0.419694",
    "-0.826397",
    "-0.604402",
    "0.103188",
    "0.956852",
    "1.18064",
    "1.05952",
    "1.32797",
    "1.52359",
    "0.989894",
    "0.0933749",
    "-0.0383255",
    "0.140214",
    "0.788264",
    "1.64787",
    "1.96616",
    "2.05713",
    "2.13521",
    "2.25166",
    "2.24832",
    "1.39547",
    "0.594089",
    "0.2222",
    "0.705044",
    "1.27745",
    "1.48216",
    "1.73497",
    "1.77609",
    "1.74525",
    "1.19823",
    "0.743697",
    "0.190449",
    "-0.093813",
    "0.411284",
    "1.46412",
    "1.6875",
    "1.71024",
    "1.74226",
    "2.06581",
    "1.65887",
    "0.923489",
    "0.345185",
    "-0.19525",
    "0.125972",
    "0.891021",
    "1.1097",
    "1.23406",
    "1.4214",
    "1.72352",
    "1.30804",
    "0.370558",
    "-0.426449",
    "-0.60596",
    "0.206878",
    "1.18601",
    "1.39624",
    "1.34044",
    "1.3882",
    "1.32285",
    "0.54259",
    "-0.223585",
    "-0.535269",
]


@dataclass
class RunParams:
    cmd: list[str]
    cwd: Path
    env: dict[str, str]


def run_cmd(params: RunParams) -> str:
    proc = subprocess.Popen(params.cmd, cwd=params.cwd, env=params.env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = proc.communicate()
    return out


def regression_test():
    sif_path = Path(os.getcwd()) / "bin" / "murfi.sif"

    murfi_params = RunParams(
        cmd=[
            "apptainer",
            "exec",
            sif_path,
            "murfi",
            "-f",
            "scripts/neurofeedback.xml",
        ],
        cwd=str(Path("example_data")),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "MURFI_SUBJECTS_DIR": ".",
            "MURFI_SUBJECT_NAME": ".",
        },
    )

    serve_params = RunParams(
        cmd=[
            "apptainer",
            "exec",
            sif_path,
            "bash",
            "servedata.sh",
            "100",
            "15000",
            os.uname().nodename,
            "2",
        ],
        cwd=str(Path("example_data") / "scripts"),
        env={
            "QT_QPA_PLATFORM": "offscreen",
        },
    )

    print("running test...")
    pool = multiprocessing.Pool(2)
    out = pool.map(run_cmd, [murfi_params, serve_params])

    # get results by TR
    tr_data = re.compile(r"RtRoiCombine: active at tr (\d+) result is: ([-\d.]+)").findall(out[0].decode())
    if len(EXPECTED_OUTPUT) != len(tr_data):
        print("ERROR: new and old data have different lengths (%d != %d)" % (len(tr_data), len(EXPECTED_OUTPUT)))
        return 1

    expected = list(zip(map(str, range(1, len(EXPECTED_OUTPUT) + 1)), EXPECTED_OUTPUT))
    if expected != tr_data:
        raise ValueError("TEST FAILURE: mismatch between expected and measured data")

    print("passed")
    return 0


if __name__ == "__main__":
    sys.exit(regression_test())
