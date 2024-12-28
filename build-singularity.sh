#!/bin/bash

set -xe

args=${@:1}
if [[ ${args} != "" ]]; then
    build_args=( "${args[@]/#/--build-arg }" )
else
    build_args=""
fi

docker build -t murfi:latest ${build_args} -f docker/Dockerfile .

mkdir -p bin
singularity build --fakeroot -F bin/murfi.sif docker-daemon://murfi:latest
