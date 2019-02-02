# murfi in a container

Murfi can be used a Docker container, and by extension a Singularity container. To build the murfi Docker image, run

```
docker build --tag murfi --file Dockerfile ..
```

or

```
docker build --tag murfi --file source.Dockerfile ..
```

from this directory.

The former command builds murfi on a `debian:jessie` image. Murfi's GUI in this image may not work with your system, because the version of OpenGL in the container must be similar to the version of OpenGL on your host. Debian Jessie has a relatively old version of OpenGL. This image installs all dependencies from the `apt` respositories, and some dependencies are not available in newer releases.

The latter command builds almost all of murfi's dependencies from source, including dependencies that are not present in the `apt` sources of newer Debian/Ubuntu releases. Building this Docker image will take longer than building the first, but results should be more reliable. If you receive a `GLXBadContext` error, you should try building with a different base image. The default base image is `ubuntu:18.04`.


## Running in a container

Note: These instructions are incomplete and should be updated soon.

To run the murfi GUI in Docker, execute the following command

```
docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix:ro -e DISPLAY=unix$DISPLAY murfi
```

Singularity offers stronger support for GUIs, so if you are using a Linux system, it could be beneficial to convert a murfi Docker image to a Singularity container and run murfi using Singularity.
