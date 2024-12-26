/*
 * (C) Copyright 2013-2023
 * Stefano Babic <stefano.babic@swupdate.org>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#pragma once

#include "swupdate_image.h"

/*
 * this is used to indicate if a file
 * in the .swu image is required for the
 * device, or can be skipped
 */
typedef enum {
	COPY_FILE,
	SKIP_FILE,
	INSTALL_FROM_STREAM
} swupdate_file_t;

void free_image(struct img_type *img);
