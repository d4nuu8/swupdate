/*
 * (C) Copyright 2012-2023
 * Stefano Babic <stefano.babic@swupdate.org>
 *
 * SPDX-License-Identifier:     GPL-2.0-only
 */

#pragma once

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#if defined(__linux__)
#include <linux/types.h>
#endif
#include <sys/types.h>
#include "globals.h"
#include "swupdate_status.h"
#include "swupdate_dict.h"
#include "compat.h"

#define AES_BLK_SIZE	16
#define AES_128_KEY_LEN	16
#define AES_192_KEY_LEN	24
#define AES_256_KEY_LEN	32

#define BOOTVAR_TRANSACTION "recovery_status"

struct img_type;
struct imglist;
struct hw_type;

extern int exit_code;

typedef enum {
	SERVER_OK,
	SERVER_EERR,
	SERVER_EBADMSG,
	SERVER_EINIT,
	SERVER_EACCES,
	SERVER_EAGAIN,
	SERVER_UPDATE_AVAILABLE,
	SERVER_NO_UPDATE_AVAILABLE,
	SERVER_UPDATE_CANCELED,
	SERVER_ID_REQUESTED,
} server_op_res_t;

enum compression_type {
  COMPRESSED_FALSE,
  COMPRESSED_TRUE,
  COMPRESSED_ZLIB,
  COMPRESSED_ZSTD,
};

typedef int (*writeimage) (void *out, const void *buf, size_t len);

struct swupdate_copy {
	/* input: either fdin is set or fdin < 0 and inbuf */
	int fdin;
	unsigned char *inbuf;
	/* data handler callback and output argument.
	 * out must point to a fd if seeking */
	writeimage callback;
	void *out;
	/* amount of data to copy */
	size_t nbytes;
	/* pointer to offset within source, must be set for fd */
	unsigned long *offs;
	/* absolute offset to seek in output (*out) if non-zero */
	unsigned long long seek;
	/* skip callback: only verify input */
	int skip_file;
	/* decompression to use */
	enum compression_type compressed;
	/* cpio crc checksum */
	uint32_t *checksum;
	/* sw-description sha256 checksum */
	unsigned char *hash;
	/* encryption */
	bool encrypted;
	const char *imgivt;
};

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define STRINGIFY(...) #__VA_ARGS__
#define PREPROCVALUE(s) STRINGIFY(s)
#define SETSTRING(p, v) do { \
	if (p) \
		free(p); \
	p = strdup(v); \
} while (0)


#define IS_STR_EQUAL(s,s1) (s && s1 && !strcmp(s,s1))
#define UNUSED __attribute__((__unused__))

#define LG_16 4
#define FROM_HEX(f) from_ascii (f, sizeof f, LG_16)
uintmax_t
from_ascii (char const *where, size_t digs, unsigned logbase);
int ascii_to_hash(unsigned char *hash, const char *s);
int ascii_to_bin(unsigned char *dest, size_t dstlen, const char *src);
void hash_to_ascii(const unsigned char *hash, char *s);
int IsValidHash(const unsigned char *hash);
bool is_hex_str(const char *ascii);

#ifndef typeof
#define typeof __typeof__
#endif
#define max(a, b) ({\
		typeof(a) _a = a;\
		typeof(b) _b = b;\
		_a > _b ? _a : _b; })

#define min(a, b) ({\
		typeof(a) _a = a;\
		typeof(b) _b = b;\
		_a < _b ? _a : _b; })

#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

bool strtobool(const char *s);

/*
 * Function to extract / copy images
 */
int copy_write(void *out, const void *buf, size_t len);
#if defined(__FreeBSD__)
int copy_write_padded(void *out, const void *buf, size_t len);
#endif
#if defined(__linux__)
/* strlcpy was originally developped in FreeBSD, not present in glibc */
size_t
strlcpy(char *dst, const char * src, size_t size);
#endif
int copyfile(struct swupdate_copy *copy);
int copyimage(void *out, struct img_type *img, writeimage callback);
int openfileoutput(const char *filename);
int mkpath(char *dir, mode_t mode);
int swupdate_file_setnonblock(int fd, bool block);

char **splitargs(char *args, int *argc);
char *mstrcat(const char **nodes, const char *delim);
char *swupdate_strcat(int n, ...);
char** string_split(const char* a_str, const char a_delim);
char *substring(const char *src, int first, int len);
char *string_tolower(char *s);
size_t snescape(char *dst, size_t n, const char *src);
void freeargs (char **argv);
int compare_versions(const char* left_version, const char* right_version);
int count_elem_list(struct imglist *list);
unsigned int count_string_array(const char **nodes);
void free_string_array(char **nodes);
long long get_output_size(struct img_type *img, bool strict);
bool img_check_free_space(struct img_type *img, int fd);
bool check_same_file(int fd1, int fd2);

/* location for libubootenv configuration file */
const char *get_fwenv_config(void);
void set_fwenv_config(const char *fname);

/* Decryption key functions */
int load_decryption_key(char *fname);
unsigned char *get_aes_key(void);
char get_aes_keylen(void);
unsigned char *get_aes_ivt(void);
int set_aes_key(const char *key, const char *ivt);

/* Getting global information */
int get_install_info(char *buf, size_t len);
sourcetype  get_install_source(void);
void get_install_swset(char *buf, size_t len);
void get_install_running_mode(char *buf, size_t len);
char *get_root_device(void);

/* Setting global information */
void set_version_range(const char *minversion,
			const char *maxversion,
			const char *current);

int size_delimiter_match(const char *size);
unsigned long long ustrtoull(const char *cp, char **endptr, unsigned int base);

const char* get_tmpdir(void);
const char* get_tmpdirscripts(void);

void swupdate_create_directory(const char* path);
#ifndef CONFIG_NOCLEANUP
int swupdate_remove_directory(const char* path);
#endif

int swupdate_mount(const char *device, const char *dir, const char *fstype);
int swupdate_umount(const char *dir);
char *swupdate_temporary_mount(tmp_mountpoint_t type, const char *device, const char *fstype);
int swupdate_temporary_umount(char *mountpoint);

/* Date / Time utilities */
char *swupdate_time_iso8601(struct timeval *tv);
