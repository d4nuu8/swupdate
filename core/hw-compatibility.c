/*
 * (C) Copyright 2023
 * Stefano Babic, stefano.babic@swupdate.org.
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#include <errno.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "hw-compatibility.h"
#include "notifier.h"

#define HWID_REGEXP_PREFIX	"#RE:"

/*
 * The HW revision of the board *MUST* be inserted
 * in the sw-description file
 */
#ifdef CONFIG_HW_COMPATIBILITY
/**
 * hwid_match - try to match a literal or RE hwid
 * @rev: literal or RE specification
 * @hwrev: current HW revision
 *
 * Return: 0 if match found, non-zero otherwise
 */
static int hwid_match(const char* rev, const char* hwrev)
{
	int ret, prefix_len;
	char errbuf[256];
	regex_t re;
	const char *re_str;

	prefix_len = strlen(HWID_REGEXP_PREFIX);

	/* literal revision */
	if (strncmp(rev, HWID_REGEXP_PREFIX, prefix_len)) {
		ret = strcmp(rev, hwrev);
		goto out;
	}

	/* regexp revision */
	re_str = rev+prefix_len;

	if ((ret = regcomp(&re, re_str, REG_EXTENDED|REG_NOSUB)) != 0) {
		regerror(ret, &re, errbuf, sizeof(errbuf));
		ERROR("error in regexp %s: %s", re_str, errbuf);
		goto out;
	}

	if ((ret = regexec(&re, hwrev, 0, NULL, 0)) == 0)
		TRACE("hwrev %s matched by regexp %s", hwrev, re_str);
	else
		TRACE("no match of hwrev %s with regexp %s", hwrev, re_str);

	regfree(&re);
out:
	return ret;
}

int check_hw_compatibility(struct hw_type *hwt, struct hwlist *hardware)
{
	struct hw_type *hw;
	int ret;

	ret = get_hw_revision(hwt);
	if (ret < 0)
		return -1;

	TRACE("Hardware %s Revision: %s", hwt->boardname, hwt->revision);
	LIST_FOREACH(hw, hardware, next) {
		if (hw &&
		    (!hwid_match(hw->revision, hwt->revision))) {
			TRACE("Hardware compatibility verified");
			return 0;
		}
	}

	return -1;
}
#else
int check_hw_compatibility(struct hw_type __attribute__ ((__unused__)) *hwt, struct hwlist __attribute__ ((__unused__)) *hardware)
{
	return 0;
}
#endif

/*
 * This function is strict bounded with the hardware
 * It reads some GPIOs to get the hardware revision
 */
int get_hw_revision(struct hw_type *hw)
{
	FILE *fp;
	int ret;
	char *b1, *b2;
#ifdef CONFIG_HW_COMPATIBILITY_FILE
#define HW_FILE CONFIG_HW_COMPATIBILITY_FILE
#else
#define HW_FILE "/etc/hwrevision"
#endif

	if (!hw)
		return -EINVAL;

	/*
	 * do not overwrite if it is already set
	 * (maybe from command line)
	 */
	if (strlen(hw->boardname))
		return 0;

	memset(hw->boardname, 0, sizeof(hw->boardname));
	memset(hw->revision, 0, sizeof(hw->revision));

	/*
	 * Not all boards have pins for revision number
	 * check if there is a file containing theHW revision number
	 */
	fp = fopen(HW_FILE, "r");
	if (!fp)
		return -1;

	ret = fscanf(fp, "%ms %ms", &b1, &b2);
	fclose(fp);

	if (ret != 2) {
		TRACE("Cannot find Board Revision");
		if(ret == 1)
			free(b1);
		return -1;
	}

	if ((strlen(b1) > (SWUPDATE_GENERAL_STRING_SIZE) - 1) ||
		(strlen(b2) > (SWUPDATE_GENERAL_STRING_SIZE - 1))) {
		ERROR("Board name or revision too long");
		ret = -1;
		goto out;
	}

	strlcpy(hw->boardname, b1, sizeof(hw->boardname));
	strlcpy(hw->revision, b2, sizeof(hw->revision));

	ret = 0;

out:
	free(b1);
	free(b2);

	return ret;
}
