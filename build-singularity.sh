#!/bin/bash

set -xe

docker build -t murfi -f docker/Dockerfile .
docker tag murfi:latest localhost:5000/murfi:latest
docker push localhost:5000/murfi
sudo singularity build -F ~/bin/murfi.sif docker-daemon://localhost:5000/murfi:latest
