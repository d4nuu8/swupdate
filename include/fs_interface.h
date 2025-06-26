/*
 * Copyright (C) 2021 Stefano Babic <stefano.babic@swupdate.org>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#pragma once

#include <stdbool.h>

char *diskformat_fs_detect(const char *device);
int diskformat_fs_exists(const char *device, const char *fstype);

int diskformat_mkfs(const char *device, const char *fstype, const char *name, const char *options);
int diskformat_set_fslabel(const char *device, const char *fstype, const char *label);

#if defined(CONFIG_FAT_FILESYSTEM)
extern int fat_mkfs(const char *device_name, const char *fstype,
	            const char *name, const char *options);
extern int fat_set_label(const char *device_name, const char *label);
#endif

#if defined (CONFIG_EXT_FILESYSTEM)
extern int ext_mkfs(const char *device_name, const char *fstype, unsigned long features,
		const char *volume_label);
#endif

#if defined (CONFIG_BTRFS_FILESYSTEM)
extern int btrfs_mkfs(const char *device_name, const char *fstype,
		      const char *name, const char *options);
#endif

#if defined(CONFIG_LUKS2_VOLUME)
extern int luks2_format(const char *device_name, const char *fstype,
	                const char *name, const char *options);
extern int luks2_exists(const char *device_name, const char *fstype);
#endif
