# SWUpdate development container
# Copyright (c) 2023 Daniel Braunwarth <daniel@braunwarth.dev>
#
# SPDX-License-Identifier: GPL-2.0-only

build:
	podman image build --network=host --tag swupdate-devel .

start:
	podman container list | grep swupdate-devel || \
	podman container run \
	    --interactive \
		--tty \
		--rm \
		--volume $(shell git rev-parse --show-toplevel):/usr/src \
		--workdir /usr/src \
		--network=host \
		--env=COLORTERM \
		--env=TERM \
		swupdate-devel \
		/usr/bin/zsh

stop:
	podman container stop --ignore swupdate-devel

clean: stop
	podman image rm --force swupdate-devel || true

.PHONY: build start stop clean atach
