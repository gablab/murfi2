#!/bin/bash

set -xe

args=${@:1}
build_args=( "${args[@]/#/--build-arg }" )

docker build -t murfi:latest ${build_args} -f docker/Dockerfile .

singularity build --fakeroot -F bin/murfi.sif docker-daemon://murfi:latest
