import click
from dataclasses import dataclass
import logging
import multiprocessing
import os
from pathlib import Path
import re
from regression_data import EXPECTED_OUTPUT
import shutil
from subprocess import PIPE, Popen, TimeoutExpired
import sys
import time

logging.basicConfig()
logging.getLogger().setLevel(logging.INFO)


DICOM_PATH = "tmp" / "murfi_input"


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
        out, err = proc.communicate(timeout=60)
        finished = True
    except TimeoutExpired:
        proc.kill()
        out, err = proc.communicate()

    logging.info(f"{'' if finished else 'did not '}finish: {params}")
    logging.debug(f"stdout: {out}")
    logging.debug(f"stderr: {err}")
    return out


def regression_test(no_image, sender="vsend", include_pre_header=False):
    base_exec_path = Path(os.getcwd()) / "bin" / "murfi"
    base_cmd = [] if no_image else ["singularity", "exec", base_exec_path.with_suffix(".sif")]

    murfi_exec = [base_exec_path] if no_image else base_cmd + ["murfi"]
    conf_file = Path("conf") / (f"neurofeedback-{sender}" + ("-preheader" if include_pre_header else "") + ".xml")
    murfi_params = RunParams(
        cmd=murfi_exec + [
            "-f",
            str(conf_file),
        ],
        cwd=str(Path("murfi_example_data")),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "MURFI_SUBJECTS_DIR": ".",
            "MURFI_SUBJECT_NAME": ".",
        },
    )

    if sender == "vsend":
        cmd = base_cmd + [
            "servenii4d",
            "nii/0.nii.gz",
            os.uname().nodename,
            "15000",
            "0.1",
            "1" if include_pre_header else "0"
        ]
    elif sender == "dicom":
        cmd = base_cmd + [
            "servedicoms",
            "dcm",
            str(DICOM_PATH),
            "200"
        ]

    serve_params = RunParams(
        cmd=cmd,
        cwd=str(Path("murfi_example_data")),
        env={
            "QT_QPA_PLATFORM": "offscreen",
            "PATH": str(Path("/") / "opt" / "murfi" / "util" / "scanner_sim") + ":" + os.environ["PATH"],
        },
        sleep=2,
    )

    logging.info("running tests...")
    pool = multiprocessing.Pool(2)
    out = pool.map(run_cmd, [murfi_params, serve_params])

    # get results by TR
    tr_data = re.compile(r"RtRoiCombine: active at tr (\d+) result is: ([-\d.]+)").findall(out[0].decode())
    if len(EXPECTED_OUTPUT) != len(tr_data):
        logging.error("ERROR: new and old data have different lengths (%d != %d)" % (len(tr_data), len(EXPECTED_OUTPUT)))
        return 1

    expected = list(zip(map(str, range(1, len(EXPECTED_OUTPUT) + 1)), EXPECTED_OUTPUT))
    if expected != tr_data:
        raise ValueError(
            f"TEST FAILURE: mismatch between expected and measured data\nexpected: {expected}\nmeasured: {tr_data}")

    logging.info("passed")
    return 0


@click.command()
@click.option("--no-image", is_flag=True, help="Run from a non-sif environment locally")
@click.option("--target", required=False, help="Run a specific test. Options: [vsend, vsend-preheader, dicom]")
def run(no_image, target):
    def prepare_dicom_dir():
        try:
            shutil.rmtree(DICOM_PATH)
        except FileNotFoundError:
            pass
        os.makedirs(DICOM_PATH)

    if target:
        if target == "vsend":
            return regression_test(no_image, sender="vsend", include_pre_header=False)
        elif target == "vsend-preheader":
            return regression_test(no_image, sender="vsend", include_pre_header=True)
        elif target == "dicom":
            prepare_dicom_dir()
            return regression_test(no_image, sender="dicom")
        else:
            logging.error(f"Unknown target: {target}")
            return 1
    else:
        prepare_dicom_dir()
        return (
            regression_test(no_image, sender="vsend", include_pre_header=True) or
            regression_test(no_image, sender="vsend", include_pre_header=False) or
            regression_test(no_image, sender="dicom")
        )


if __name__ == "__main__":
    sys.exit(run())
