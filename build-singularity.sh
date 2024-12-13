#!/bin/bash

set -xe

docker build -t murfi:latest -f docker/Dockerfile .

singularity build --fakeroot -F bin/murfi.sif docker-daemon://murfi:latest
