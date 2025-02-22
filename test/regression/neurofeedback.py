import click
from dataclasses import dataclass
import logging
import multiprocessing
import os
from pathlib import Path
import re
from subprocess import PIPE, Popen, TimeoutExpired
import sys


logging.basicConfig()
logging.getLogger().setLevel(logging.DEBUG)


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
    logging.info(f"running job: {params}")
    proc = Popen(params.cmd, cwd=params.cwd, env=params.env, stdout=PIPE, stderr=PIPE)

    finished = False
    try:
        out, err = proc.communicate(timeout=60)
        finished = True
    except TimeoutExpired:
        proc.kill()
        out, err = proc.communicate()

    logging.info(f"{'' if finished else 'did not'} finish: {params}")
    logging.debug(f"stdout: {out}")
    logging.debug(f"stderr: {err}")
    return out


def regression_test(no_image, include_pre_header):
    base_exec_path = Path(os.getcwd()) / "bin" / "murfi"
    base_cmd = [] if no_image else ["singularity", "exec", base_exec_path.with_suffix(".sif")]

    murfi_exec = [base_exec_path] if no_image else base_cmd + ["murfi"]
    murfi_params = RunParams(
        cmd=murfi_exec + [
            "-f",
            "scripts/neurofeedback-preheader.xml" if include_pre_header else "scripts/neurofeedback.xml",
        ],
        cwd=str(Path("example_data")),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "MURFI_SUBJECTS_DIR": ".",
            "MURFI_SUBJECT_NAME": ".",
        },
    )

    serve_params = RunParams(
        cmd=base_cmd + [
            "bash",
            "servedata.sh",
            "100",
            "15000",
            os.uname().nodename,
            "2",
            "1" if include_pre_header else "0"
        ],
        cwd=str(Path("example_data") / "scripts"),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "PATH": str(Path("/") / "opt" / "murfi" / "util" / "scanner_sim") + ":" + os.environ["PATH"],
        },
    )

    logging.info("running test...")
    pool = multiprocessing.Pool(2)
    out = pool.map(run_cmd, [murfi_params, serve_params])

    # get results by TR
    tr_data = re.compile(r"RtRoiCombine: active at tr (\d+) result is: ([-\d.]+)").findall(out[0].decode())
    if len(EXPECTED_OUTPUT) != len(tr_data):
        logging.error("ERROR: new and old data have different lengths (%d != %d)" % (len(tr_data), len(EXPECTED_OUTPUT)))
        return 1

    expected = list(zip(map(str, range(1, len(EXPECTED_OUTPUT) + 1)), EXPECTED_OUTPUT))
    if expected != tr_data:
        raise ValueError("TEST FAILURE: mismatch between expected and measured data")

    logging.info("passed")
    return 0


@click.command()
@click.option("--no-image", is_flag=True, help="Run from a non-sif environment locally")
def run(no_image):
    return regression_test(no_image, include_pre_header=True) or regression_test(no_image, include_pre_header=False)


if __name__ == "__main__":
    sys.exit(run())
