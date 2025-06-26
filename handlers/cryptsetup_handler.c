/*
 * (C) Copyright 2025
 * Daniel Braunwarth <daniel@braunwarth.dev>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#include <stdio.h>
#include <errno.h>
#include <libcryptsetup.h>
#include "swupdate_image.h"
#include "handler.h"

#define MAX_CIPHER_LEN 32

void dcryptsetup_handler(void);

static int format_and_add_keyslots(const char *path)
{
	struct crypt_device *cd;
	int r = 0;

	r = crypt_init(&cd, path);
	if (r < 0) {
		ERROR("crypt_init() failed for %s.\n", path);
		goto error_no_free;
	}

	r = crypt_format(cd, CRYPT_LUKS2, "aes", "xts-plain64", NULL, NULL, 512 / 8, NULL);
	if (r < 0) {
		ERROR("crypt_format() failed on device %s\n", crypt_get_device_name(cd));
		goto error;
	}

	r = crypt_keyslot_add_by_volume_key(cd, CRYPT_ANY_SLOT, NULL, 0, "foo", 3);
	if (r < 0) {
		ERROR("Adding keyslot failed.\n");
		goto error;
	}

error:
	crypt_free(cd);
error_no_free:
	return r;
}

static int activate_and_check_status(const char *path, const char *device_name)
{
	struct crypt_device *cd;
	int r;

	r = crypt_init(&cd, path);
	if (r < 0) {
		ERROR("crypt_init() failed for %s.\n", path);
		goto error_no_free;
	}

	r = crypt_load(cd, CRYPT_LUKS, NULL);
	if (r < 0) {
		ERROR("crypt_load() failed on device %s.\n", crypt_get_device_name(cd));
		goto error;
	}

	r = crypt_activate_by_passphrase(cd, device_name, CRYPT_ANY_SLOT, "foo", 3, CRYPT_ACTIVATE_READONLY);
	if (r < 0) {
		ERROR("Device %s activation failed.\n", device_name);
		goto error;
	}

error:
	crypt_free(cd);
error_no_free:
	return r;
}

static int cryptsetup(struct img_type *img, void __attribute__ ((__unused__)) *data)
{
	int r = 0;

	if (!strlen(img->device)) {
		ERROR("cryptsetup handler requires setting \"device\" attribute");
		return -EINVAL;
	}

	const char *name = dict_get_value(&img->properties, "name");
	if (!name) {
		ERROR("cryptsetup handler requires setting \"name\" attribute");
		return -EINVAL;
	}

	r = format_and_add_keyslots(img->device);
	if (r < 0)
		goto error;

	r = activate_and_check_status(img->device, name);
	if (r < 0)
		goto error;

error:
	return r;
}

__attribute__((constructor))
void dcryptsetup_handler(void)
{
	register_handler("cryptsetup", cryptsetup,
			 PARTITION_HANDLER | NO_DATA_HANDLER, NULL);
}
