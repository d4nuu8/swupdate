/*
 * (C) Copyright 2012-2023
 * Stefano Babic <stefano.babic@swupdate.org>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#pragma once

int emmc_write_bootpart(int fd, int bootpart);
int emmc_get_active_bootpart(int fd);
