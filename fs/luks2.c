/*
 * Copyright (C) 2024 Daniel Braunwarth, daniel.braunwarth@kuka.com
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "pctl.h"
#include "fs_interface.h"

#define LUKS_FORMAT_CMD "echo \"\" | cryptsetup luksFormat --batch-mode"
#define LUKS_OPEN_CMD "echo \"\" | cryptsetup open --batch-mode"
#define IS_LUKS_CMD "cryptsetup isLuks --batch-mode --type LUKS2"

int luks2_format(const char *device_name, __attribute__((unused)) const char *fstype,
		 const char *name, const char *options)
{
	char *cmd;
	int ret = 0;

	if (asprintf(&cmd, LUKS_FORMAT_CMD " %s %s\n", options, device_name) == -1) {
		ERROR("Error allocating memory");
		return -ENOMEM;
	}

	ret = run_system_cmd(cmd);
	free(cmd);
	if (ret)
		return ret;

	if (asprintf(&cmd, LUKS_OPEN_CMD " %s %s\n", device_name, name) == -1) {
		ERROR("Error allocating memory");
		return -ENOMEM;
	}

	ret = run_system_cmd(cmd);
	free(cmd);
	return ret;
}

int luks2_exists(const char *device_name, __attribute__((unused)) const char *fstype)
{
	char *cmd;
	int ret = 0;

	if (asprintf(&cmd, IS_LUKS_CMD " %s\n", device_name) == -1) {
		ERROR("Error allocating memory");
		return -ENOMEM;
	}

	ret = !run_system_cmd(cmd);
	free(cmd);
	return ret;
}
