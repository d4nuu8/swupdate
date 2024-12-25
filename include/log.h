/*
 * (C) Copyright 2024
 * Daniel Braunwarth <oss@braunwarth.dev>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#pragma once

#include <stdio.h>
#include "swupdate_status.h"

#define NOTIFY_BUF_SIZE	2048

/*
 * loglevel is used into TRACE / ERROR
 * for values > LASTLOGLEVEL, it is an encoded field
 * to inform the installer about a change in a subprocess
 */
typedef enum {
	OFF,
	ERRORLEVEL,
	WARNLEVEL,
	INFOLEVEL,
	TRACELEVEL,
	DEBUGLEVEL,
	LASTLOGLEVEL=DEBUGLEVEL
} LOGLEVEL;

/*
 * Following are used for notification from another process
 */

typedef enum {
	CANCELUPDATE=LASTLOGLEVEL + 1,
	CHANGE,
} NOTIFY_CAUSE;

enum {
	RECOVERY_NO_ERROR,
	RECOVERY_ERROR,
	RECOVERY_DWL,
};

typedef void (*notifier) (RECOVERY_STATUS status, int error, int level, const char *msg);

void notify(RECOVERY_STATUS status, int error, int level, const char *msg);
void notify_init(void);
void notifier_set_color(int level, char *col);

extern int loglevel;

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define swupdate_notify(status, format, level, arg...) do { \
	if (loglevel >= level) { \
		char tmpbuf[NOTIFY_BUF_SIZE]; \
		if (status == FAILURE) { \
			if (loglevel >= DEBUGLEVEL) \
				snprintf(tmpbuf, sizeof(tmpbuf), \
				     	"ERROR %s : %s : %d : " format, \
						__FILENAME__, \
					       	__func__, \
					       	__LINE__, \
						## arg); \
			else \
				snprintf(tmpbuf, sizeof(tmpbuf), \
					       	"ERROR : " format, ## arg); \
			notify(FAILURE, 0, level, tmpbuf); \
		} else {\
			snprintf(tmpbuf, sizeof(tmpbuf), \
				       	"[%s] : " format, __func__, ## arg); \
			notify(RUN, RECOVERY_NO_ERROR, level, tmpbuf); \
		} \
	} \
} while(0)

#define ERROR(format, arg...) \
	swupdate_notify(FAILURE, format, ERRORLEVEL, ## arg)

#define WARN(format, arg...) \
	swupdate_notify(RUN, format, WARNLEVEL, ## arg)

#define INFO(format, arg...) \
	swupdate_notify(RUN, format, INFOLEVEL, ## arg)

#define TRACE(format, arg...) \
	swupdate_notify(RUN, format, TRACELEVEL, ## arg)

#define DEBUG(format, arg...) \
	swupdate_notify(RUN, format, DEBUGLEVEL, ## arg)


int register_notifier(notifier client);
int syslog_init(void);
