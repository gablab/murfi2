import click
import os
from subprocess import Popen, PIPE
import sys


@click.command(help="Build a singularity image from a docker image")
@click.option(
    "--target",
    default=["docker", "singularity"],
    help="Build targets",
    type=click.Choice(["base", "docker", "singularity"]),
    multiple=True,
)
@click.option("--tag", default="latest", help="Tag for the docker and singularity images")
@click.option("--base-tag", default="latest", help="Base image tag to use to build the murfi docker image")
@click.option("--build-arg", multiple=True, help="Args to pass to docker build, e.g, MAKE_TARGET=debug")
def main(tag, build_arg, target, base_tag):
    """
    Build the murfi docker and singularity images.
    """
    build_args = ""
    if build_arg:
        build_args = " ".join([f"--build-arg {arg}" for arg in build_arg])
    build_args += f" --build-arg BASE_IMAGE_TAG={base_tag}"

    if "base" in target:
        docker_cmd = ["docker", "build", "-t", f"murfi-base:{tag}", "-f", "docker/Dockerfile.base", "."]
        print(docker_cmd)
        proc = Popen(docker_cmd, stdout=PIPE, stderr=PIPE)
        out, err = proc.communicate()
        if proc.returncode != 0:
            print(f"Docker base image build failed with error: {err.decode()}")
            print(f"stdout: {out.decode()}")
            print(f"stderr: {err.decode()}")
            raise Exception("Docker base image build failed")

    if "docker" in target:
        docker_cmd = ["docker", "build", "-t", f"murfi:{tag}"] + build_args.split() + ["-f", "docker/Dockerfile", "."]
        print(docker_cmd)
        proc = Popen(docker_cmd, stdout=PIPE, stderr=PIPE)
        out, err = proc.communicate()
        if proc.returncode != 0:
            print(f"Docker build failed with error: {err.decode()}")
            print(f"stdout: {out.decode()}")
            print(f"stderr: {err.decode()}")
            raise Exception("Docker build failed")

    if "singularity" in target:
        os.makedirs("bin", exist_ok=True)
        singularity_cmd = ["singularity", "build", "--fakeroot", "-F", "bin/murfi.sif", f"docker-daemon://murfi:{tag}"]
        print(singularity_cmd)
        proc = Popen(singularity_cmd, stdout=PIPE, stderr=PIPE)
        out, err = proc.communicate()
        if proc.returncode != 0:
            print(f"Singularity build failed with error: {err.decode()}")
            print(f"stdout: {out.decode()}")
            print(f"stderr: {err.decode()}")
            raise Exception("Singularity build failed")


if __name__ == "__main__":
    main()
