#!/bin/sh
# Copyright (c) Siemens AG, 2021
#
# Authors:
#  Michael Adler <michael.adler@siemens.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#
# SPDX-License-Identifier:	GPL-2.0-only
set -eu

REPO_ROOT="$(git rev-parse --show-toplevel)"
BUILD_DIR=$(mktemp --directory)

find configs -type f | while read -r fname; do
    echo "*** Testing config: $fname"
    rm -rf "${BUILD_DIR}"
    "${REPO_ROOT}/scripts/Kconfiglib/defconfig.py" "$fname"
    meson setup "${BUILD_DIR}"
    meson compile -C "${BUILD_DIR}"
    meson test -C "${BUILD_DIR}"
done
