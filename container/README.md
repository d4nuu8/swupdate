# SWUpdate development container

[//]: # "Copyright (c) 2023 Daniel Braunwarth <daniel@braunwarth.dev>"
[//]: # "SPDX-License-Identifier: GPL-2.0-only"

This container is intended to build and run SWUpdate from Git sources.

To build and run the container [Podman](https://podman.io/) should be used.

The container is based on the official Debian Bookworm image in the slim
variant.

## Dependencies

To be able to build and use this container you need:

- [Make](https://www.gnu.org/software/make/)
- [Podman](https://podman.io/)

  See <https://podman.io/getting-started/> for information how to get started
  with Podman.

## Usage

### Build container image

To build the container image run:

```shell
make build
```

The resulting image is named `swupdate-devel`.

### Start container

To start the container run:

```shell
make start
```

The started container is named `swupdate-devel`. It is not possible to start
multiple container instances.

### Stop container

To stop the container run:

```shell
make stop
```

### Attach to running container

To attach to a running container run:

```shell
make attach
```

The default working directory is `/usr/src`. This is where the Git repository
is mounted to.

### Clean-up

To remove the container instance and image run:

```shell
make clean
```
