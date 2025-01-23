import click
from dataclasses import dataclass
import logging
import multiprocessing
import os
from pathlib import Path
import re
from subprocess import PIPE, Popen, TimeoutExpired
import sys
import time


logging.basicConfig()
logging.getLogger().setLevel(logging.INFO)


EXPECTED_OUTPUT = [
    "0",
    "0",
    "-0.285761",
    "-0.420583",
    "-0.248479",
    "0.129524",
    "0.416998",
    "1.51158",
    "3.1963",
    "3.37234",
    "2.99662",
    "3.26803",
    "3.77257",
    "4.08346",
    "4.04907",
    "3.83722",
    "2.68248",
    "1.20354",
    "0.463601",
    "0.672681",
    "1.66588",
    "2.18002",
    "2.39226",
    "2.3707",
    "2.35754",
    "2.22072",
    "2.19962",
    "2.22026",
    "1.24294",
    "-0.0540546",
    "-0.371192",
    "0.111927",
    "1.27062",
    "1.81215",
    "1.96511",
    "2.1515",
    "2.13323",
    "2.11847",
    "2.11763",
    "2.23493",
    "1.63971",
    "0.431847",
    "-0.166803",
    "0.237254",
    "1.57852",
    "2.14301",
    "2.36978",
    "2.5287",
    "2.34037",
    "2.69703",
    "3.15166",
    "3.18985",
    "1.73876",
    "0.111714",
    "-0.191012",
    "0.220048",
    "1.64457",
    "2.05697",
    "1.78853",
    "1.71655",
    "1.52912",
    "1.54522",
    "1.52188",
    "2.02795",
    "1.56991",
    "0.369011",
    "-0.161293",
    "-0.00211468",
    "1.11372",
    "2.07218",
    "2.55672",
    "2.46354",
    "2.50457",
    "2.48004",
    "2.10381",
    "1.7859",
    "1.13562",
    "0.206233",
    "-0.311939",
    "-0.189357",
    "0.902193",
    "1.28467",
    "1.45849",
    "1.89919",
    "2.44272",
    "2.84291",
    "2.525",
    "1.75201",
    "0.717464",
    "-0.0106586",
    "-0.118361",
    "0.106221",
    "1.58744",
    "2.46031",
    "2.25376",
    "1.96671",
    "2.20319",
    "2.18563",
    "2.09632",
    "2.34403",
    "1.67917",
    "0.169146",
    "-0.525884",
    "0.0150069",
    "1.27479",
    "2.04041",
    "2.43211",
    "2.41996",
    "2.26021",
    "2.06918",
    "2.19885",
    "2.22384",
    "0.928051",
    "-0.558896",
    "-0.76741",
    "-0.126883",
    "1.07235",
    "1.658",
    "1.89776",
    "2.00752",
    "2.14624",
    "2.14391",
    "2.57416",
    "2.74869",
    "1.64826",
    "1.17087",
    "1.30165",
    "1.19196",
    "1.98359",
    "2.12574",
    "2.39563",
    "2.83726",
    "2.90209",
    "3.16163",
    "2.82053",
    "2.43993",
    "1.55815",
    "0.0108201",
    "-0.542435",
    "-0.269729",
    "0.93676",
    "1.83753",
    "2.45327",
    "2.61687",
    "2.94547",
    "2.94481",
    "2.66748",
    "2.50198",
    "1.68628",
    "0.103873",
    "-0.44309",
    "-0.376338",
    "-0.750666",
    "-1.13615",
    "-1.45456",
    "-1.31343",
]


@dataclass
class RunParams:
    cmd: list[str]
    cwd: Path
    env: dict[str, str]
    sleep: int = 0


def run_cmd(params: RunParams) -> str:
    if params.sleep:
        logging.info(f"sleeping for {params.sleep} seconds")
        time.sleep(params.sleep)

    logging.info(f"running job: {params}")
    proc = Popen(params.cmd, cwd=params.cwd, env=params.env, stdout=PIPE, stderr=PIPE)

    finished = False
    try:
        out, err = proc.communicate(timeout=30)
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
            "conf/neurofeedback-preheader.xml" if include_pre_header else "conf/neurofeedback.xml",
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
            "servenii4d",
            "nii/0.nii.gz",
            os.uname().nodename,
            "15000",
            "0.1",
            "1" if include_pre_header else "0"
        ],
        cwd=str(Path("example_data")),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "PATH": str(Path("/") / "opt" / "murfi" / "util" / "scanner_sim") + ":" + os.environ["PATH"],
        },
        sleep=2,
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
        breakpoint()
        raise ValueError("TEST FAILURE: mismatch between expected and measured data")

    logging.info("passed")
    return 0


@click.command()
@click.option("--no-image", is_flag=True, help="Run from a non-sif environment locally")
def run(no_image):
    return regression_test(no_image, include_pre_header=True) or regression_test(no_image, include_pre_header=False)


if __name__ == "__main__":
    sys.exit(run())
