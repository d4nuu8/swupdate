sources = [
	'emmc_utils.c',
	'multipart_parser.c',
	'parsing_library_libjson.c',
	'server_utils.c',
	'swupdate_config.c',
]

c_args = []

link_args = []

dependencies = []

if kconfig.has_key('CONFIG_DOWNLOAD')
	sources += 'downloader.c'
endif

if kconfig.has_key('CONFIG_MTD')
	sources += 'mtd-interface.c'

	dependencies += cc.find_library('mtd', required: true)
	dependencies += cc.find_library('ubi', required: true)
endif

if kconfig.has_key('CONFIG_LUA')
	sources += ['lua_interface.c', 'lua_compat.c']

	dependencies += lua_dependency

	embedded_lua_source = kconfig.get('CONFIG_EMBEDDED_LUA_HANDLER_SOURCE', '').strip(
		'""',
	)
	if embedded_lua_source != ''
		link_args += [
			'-Wl,--format=binary',
			'-znoexecstack',
			'-Wl,' + embedded_lua_source,
			'-Wl,--format=default',
		]

		embededde_lua_source = embedded_lua_source.replace('"', '').replace(
			'.',
			'_',
		).replace(
			'/',
			'_',
		)

		c_args += [
			'-DEMBEDDED_LUA_SRC_START=_binary_' + embededde_lua_source + '_start',
			'-DEMBEDDED_LUA_SRC_END=_binary_' + embededde_lua_source + '_end',
		]
	endif
endif

if kconfig.has_key('CONFIG_SSL_IMPL_OPENSSL')
	dependencies += dependency('libcrypto', required: true)
	dependencies += dependency('libssl', required: true)

	if kconfig.has_key('CONFIG_HASH_VERIFY')
		sources += 'verify_signature.c'
	endif

	if kconfig.has_key('CONFIG_ENCRYPTED_IMAGES')
		if kconfig.has_key('CONFIG_PKCS11')
			sources += 'swupdate_decrypt_pkcs11.c'
		else
			sources += 'swupdate_decrypt_openssl.c'
		endif
	endif

	if kconfig.has_key('CONFIG_SIGALG_RAWRSA') or kconfig.has_key(
		'CONFIG_SIGALG_RSAPSS',
	)
		sources += 'swupdate_rsa_verify.c'
	endif

	if kconfig.has_key('CONFIG_SIGALG_CMS')
		sources += 'swupdate_cms_verify.c'
	endif
endif

if kconfig.has_key('CONFIG_SSL_IMPL_WOLFSSL')
	dependencies += dependency('wolfssl', required: true)

	c_args += '-DOPENSSL_ALL'

	if kconfig.has_key('CONFIG_SIGALG_CMS')
		sources += 'swupdate_pkcs7_verify.c'
	endif
endif

if kconfig.has_key('CONFIG_SSL_IMPL_MBEDTLS')
	# TODO Use dependency() if Mbed TLS >= 3.6.0 is commonly available
	dependencies += cc.find_library('mbedcrypto', required: true)
	dependencies += cc.find_library('mbedtls', required: true)
	dependencies += cc.find_library('mbedx509', required: true)

	if kconfig.has_key('CONFIG_HASH_VERIFY')
		sources += 'verify_signature_mbedtls.c'
	endif

	if kconfig.has_key('CONFIG_ENCRYPTED_IMAGES')
		if kconfig.has_key('CONFIG_PKCS11')
			sources += 'swupdate_decrypt_pkcs11.c'
		else
			sources += 'swupdate_decrypt_mbedtls.c'
		endif
	endif

	if kconfig.has_key('CONFIG_SIGALG_RAWRSA') or kconfig.has_key(
		'CONFIG_SIGALG_RSAPSS',
	)
		sources += 'swupdate_rsa_verify_mbedtls.c'
	endif
endif

if kconfig.has_key('CONFIG_PKCS11')
	dependencies += dependency('wolfssl', required: true)
	dependencies += dependency('p11-kit-1', required: true)
endif

if kconfig.has_key('CONFIG_SIGALG_GPG')
	sources += 'swupdate_gpg_verify.c'
endif

if kconfig.has_key('CONFIG_LIBCONFIG')
	sources += ['swupdate_settings.c', 'parsing_library_libconfig.c']

	dependencies += cc.find_library('config', required: true)
endif

if kconfig.has_key('CONFIG_CHANNEL_CURL')
	sources += 'channel_curl.c'

	dependencies += curl_dep
endif

core_libs += static_library(
	'corelib',
	sources,
	c_args: c_args,
	include_directories: include_dirs,
	dependencies: dependencies,
	link_args: link_args,
	implicit_include_directories: false,
)
