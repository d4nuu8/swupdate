if kconfig.has_key('CONFIG_WEBSERVER') and kconfig.has_key('CONFIG_MONGOOSE')
	sources = ['mongoose.c', 'mongoose_multipart.c', 'mongoose_interface.c']

	c_args = [
		'-DMG_ENABLE_DIRLIST=1',
		'-DMG_ENABLE_EPOLL=1',
		'-DMG_ENABLE_POSIX_FS=1',
		'-DMG_ENABLE_LOG=0',
		'-DMG_ENABLE_MD5=1',
		'-DMG_MAX_RECV_SIZE=262144',
	]

	dependencies = []

	if kconfig.has_key('CONFIG_MONGOOSEIPV6')
		c_args += ['-DMG_ENABLE_IPV6=1']
	endif

	if kconfig.has_key('CONFIG_MONGOOSESSL')
		if kconfig.has_key('CONFIG_SSL_IMPL_OPENSSL')
			dependencies += dependency('libcrypto', required: true)
			dependencies += dependency('libssl', required: true)

			c_args += ['-DMG_TLS=2']
		endif

		if kconfig.has_key('CONFIG_SSL_IMPL_WOLFSSL')
			dependencies += dependency('wolfssl', required: true)

			c_args += ['-DMG_TLS=5', '-DOPENSSL_ALL']
		endif

		if kconfig.has_key('CONFIG_SSL_IMPL_MBEDTLS')
			c_args += ['-DMG_TLS=1']
		endif
	endif

	core_libs += static_library(
		'mongoose',
		sources,
		c_args: c_args,
		include_directories: include_dirs,
		implicit_include_directories: false,
	)
endif
