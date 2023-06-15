#!/bin/bash

set -xe

docker build -t murfi:latest -f docker/Dockerfile .
sudo singularity build -F ~/bin/murfi.sif docker-daemon://murfi:latest
