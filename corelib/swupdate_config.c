/*
 * (C) Copyright 2016
 * Stefano Babic, stefano.babic@swupdate.org.
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#include "swupdate_config.h"

struct swupdate_cfg swcfg;

struct swupdate_cfg *get_swupdate_cfg(void) {
	return &swcfg;
}
