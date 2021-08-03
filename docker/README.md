# murfi in a container

A container is an executable that houses all of a program's dependencies, and it standardizes the installation process for that program. Some of murfi's dependencies are not available in current `apt` or `yum` repositories, so their installation would be nontrivial. The murfi container, however, contains all of murfi's dependencies and makes it easy to install and run on many Linux systems. If you want to share your research with another group and need them to install murfi, you can simply share the murfi container with that group instead of having to install murfi and its dependencies manually.

Murfi can be used in a Docker container and, by extension, in a Singularity container.  To build the murfi Docker image, first install Docker (instructions for [CentOS](https://docs.docker.com/install/linux/docker-ce/centos/), [Debian](https://docs.docker.com/install/linux/docker-ce/debian/), [Fedora](https://docs.docker.com/install/linux/docker-ce/fedora/), [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/), [other systems](https://docs.docker.com/install/linux/docker-ce/binaries/)). Then, build the murfi Docker container with the command

```bash
docker build --tag murfi2 --file source.Dockerfile ..
```

or

```bash
docker build --tag murfi2 --file Dockerfile ..
```

in this directory.

If you run into an error about GPG keys, please make sure that you have enough 
space for docker to build. On some platforms docker (e.g. OSX) uses a reserved
space and this gets filled up as you use docker. A complete clearing of this can 
be done with `docker system prune -a`. However, this will wipe out everything
in docker.

The former command is recommended. That command builds almost all of murfi's dependencies from source, including dependencies that are not present in the `apt` sources of newer Debian/Ubuntu releases. Building this Docker image will take longer than building with the latter command, but the container should be more reliable. If you receive a `GLXBadContext` error, you should try building with a base image that is similar to your host. The default base image is `ubuntu:18.04`.

The latter command builds murfi on a `debian:jessie` image. Murfi's GUI in this image may not work with your system, because the version of OpenGL in the container must be similar to the version of OpenGL on your host. Debian Jessie has a relatively old version of OpenGL. This image installs all dependencies from the `apt` repositories, and some dependencies are not available in newer releases.

## Recommendations for using the container

Docker is a popular container builder and runner, but because murfi uses a graphical user interface and network, it can be tricky to run murfi using Docker. For this reason, it is recommended to run murfi using [Singularity](https://github.com/sylabs/singularity). To install Singularity, please follow their [installation guide](https://www.sylabs.io/guides/3.0/user-guide/installation.html). Once Singularity is installed, it can be called from the command line as `singularity`.

To make a Singularity container of murfi, first build the Docker image as described above. Then, convert the Docker image to a Singularity container:

```bash
sudo singularity build murfi2.sif docker-daemon://murfi2:latest
```

Please note the use of `sudo` here. Root access is necessary to interact with the Docker daemon.

## Downloading a dataset to run the example

Before running murfi, please download the example data and set environment variables.

```bash
curl -fsSL https://dl.dropbox.com/s/1vvrz2g4tbzoh5c/murfi_example_data.tgz | tar -xzv
export MURFI_SUBJECTS_DIR="$PWD"
export MURFI_SUBJECT_NAME=murfi_example_data
```

## Running the example using the Docker container

Using the docker image, you can run murfi in a windowed setting or in
the background.

TODO


## Running the example using the Singularity container

Next, you can run murfi. `murfi2.sif` is the path to the Singularity container, which is a file. The `murfi` bit after the path to the container is the command that is executed within the container. Everything after `murfi` is arguments that are passed to `murfi`.

```bash
singularity exec murfi2.sif murfi -f murfi_example_data/scripts/neurofeedback.xml
```

Now, the murfi graphical user interface should appear. In a new terminal window, run the following to serve data.

```bash
cd murfi_example_data/scripts
singularity exec --bind $PWD/.. ../../murfi2.sif ./servedata.sh 3000 15000 127.0.0.1
```

The data should now be visible in the murfi window. Please note the `--bind` argument in the command above. This mounts a directory onto the container. Singularity will mount your home directory and the current working directory by default, but other directories have to be mounted explicitly. We used the `--bind` argument to mount the parent directory, which contains the example data. Without this explicit bind, the container would not have access to the data.

One can also alias the command to run containerized `murfi` simply with `murfi`:

```bash
alias murfi='singularity exec /ABSOLUTE/PATH/TO/murfi2.sif murfi'
```

With this alias, the command

```bash
singularity exec murfi2.sif murfi -f murfi_example_data/scripts/neurofeedback.xml
```

can be replaced with

```bash
murfi -f murfi_example_data/scripts/neurofeedback.xml
```
