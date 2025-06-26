/*
 * Copyright (C) 2021 Weidmueller Interface GmbH & Co. KG
 * Roland Gaudig <roland.gaudig@weidmueller.com>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#include <errno.h>
#include <stdio.h>
#include <util.h>
#include <handler.h>
#include <blkid/blkid.h>
#include <fs_interface.h>

#if defined(CONFIG_EXT_FILESYSTEM)
static inline int ext_mkfs_short(const char *device_name, const char *fstype,
				 const char __attribute__ ((__unused__)) *name,
				 const char __attribute__ ((__unused__)) *options)
{
	return ext_mkfs(device_name, fstype, 0, NULL);
}
#endif

struct supported_filesystems {
	const char *fstype;
	int (*mkfs)(const char *device_name, const char *fstype,
		    const char *name, const char *options);
	int (*exists)(const char *device, const char *fstype);
};

int diskformat_fs_exists_default(const char *device, const char *fstype);

static struct supported_filesystems fs[] = {
#if defined(CONFIG_FAT_FILESYSTEM)
	{"vfat", fat_mkfs, diskformat_fs_exists_default},
#endif
#if defined(CONFIG_EXT_FILESYSTEM)
	{"ext2", ext_mkfs_short, diskformat_fs_exists_default},
	{"ext3", ext_mkfs_short, diskformat_fs_exists_default},
	{"ext4", ext_mkfs_short, diskformat_fs_exists_default},
#endif
#if defined(CONFIG_BTRFS_FILESYSTEM)
	{"btrfs", btrfs_mkfs, diskformat_fs_exists_default},
#endif
#if defined(CONFIG_LUKS2_VOLUME)
	{"luks2", luks2_format, luks2_exists},
#endif
};

/*
 * Checks if file system fstype already exists on device.
 * return 0 if not exists, 1 if exists, negative values on failure
 */

char *diskformat_fs_detect(const char *device)
{
	const char *value;
	char *s = NULL;
	size_t len;
	blkid_probe pr;

	pr = blkid_new_probe_from_filename(device);

	if (!pr) {
		ERROR("%s: failed to create libblkid probe",
			  device);
		return NULL;
	}

	while (blkid_do_probe(pr) == 0) {
		if (blkid_probe_lookup_value(pr, "TYPE", &value, &len)) {
			ERROR("blkid_probe_lookup_value failed");
			break;
		}

		if (len > 0) {
			s = strndup(value, len);
			TRACE("Found %s file system on %s", s, device);
			break;
		}
	}
	blkid_free_probe(pr);

	if (!s) {
		TRACE("Found no file system on %s", device);
	}

	return s;
}

int diskformat_fs_exists_default(const char *device, const char *fstype)
{
	bool ret = false;
	char *filesystem = diskformat_fs_detect(device);

	if (filesystem) {
		ret = !strcmp(fstype, filesystem);
	}

	free(filesystem);
	return ret;
}

int diskformat_fs_exists(const char *device, const char *fstype)
{
	int index;
	int ret;

	if (!device || !fstype) {
		ERROR("Uninitialized pointer as device/fstype argument");
		return -EINVAL;
	}

	for (index = 0; index < ARRAY_SIZE(fs); index++) {
		if (!strcmp(fs[index].fstype, fstype))
			break;
	}
	if (index >= ARRAY_SIZE(fs)) {
		ERROR("%s file system type not supported.", fstype);
		return -EINVAL;
	}

	ret = fs[index].exists(device, fstype);
	TRACE("device: %s, fstype: %s, exists: %i", device, fstype, ret);
	return ret;
}

int diskformat_mkfs(const char *device, const char *fstype, const char *name, const char *options)
{
	int index;
	int ret = 0;

	if (!device || !fstype) {
		ERROR("Uninitialized pointer as device/fstype argument");
		return -EINVAL;
	}

	for (index = 0; index < ARRAY_SIZE(fs); index++) {
		if (!strcmp(fs[index].fstype, fstype))
			break;
	}
	if (index >= ARRAY_SIZE(fs)) {
		ERROR("%s file system type not supported.", fstype);
		return -EINVAL;
	}

	TRACE("Creating %s file system on %s", fstype, device);
	ret = fs[index].mkfs(device, fstype, name, options);

	if (ret) {
		ERROR("creating %s file system on %s failed. %d",
		      fstype, device, ret);
		return -EFAULT;
	}

	return ret;
}

int diskformat_set_fslabel(const char *device, const char *fstype, const char *label)
{
#ifdef CONFIG_FAT_FILESYSTEM
	if (!strcmp(fstype, "vfat")) {
		if (fat_set_label(device, label)) {
			ERROR("%s: failed to set FAT label", device);
			return 1;
		}
		return 0;
	}
#endif
	/* failure by default */
	ERROR("%s: fslabel feature not supported", fstype);
	return 1;
}
