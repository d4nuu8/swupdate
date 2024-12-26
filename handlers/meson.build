sources = ['dummy_handler.c', 'handler_helpers.c']

dependencies = []

if kconfig.has_key('CONFIG_ARCHIVE')
	sources += 'archive_handler.c'

	dependencies += dependency('libarchive', required: true)
endif

if kconfig.has_key('CONFIG_BOOTLOADERHANDLER')
	sources += 'boot_handler.c'
endif

if kconfig.has_key('CONFIG_BTRFS_FILESYSTEM')
	sources += 'btrfs_handler.c'
endif

if kconfig.has_key('CONFIG_COPY')
	sources += 'copy_handler.c'
endif

if kconfig.has_key('CONFIG_CFI')
	sources += 'flash_handler.c'
endif

if kconfig.has_key('CONFIG_DELTA')
	sources += ['delta_handler.c', 'delta_downloader.c', 'zchunk_range.c']

	dependencies += dependency('zck', required: true)
endif

if kconfig.has_key('CONFIG_EMMC_HANDLER')
	sources += 'emmc_csd_handler.c'
endif

if kconfig.has_key('CONFIG_DISKFORMAT_HANDLER')
	sources += 'diskformat_handler.c'
endif

if kconfig.has_key('CONFIG_DISKPART')
	sources += 'diskpart_handler.c'

	dependencies += dependency('fdisk', required: true)
endif

if kconfig.has_key('CONFIG_UNIQUEUUID')
	sources += 'uniqueuuid_handler.c'

	dependencies += dependency('blkid', required: true)
endif

if kconfig.has_key('CONFIG_CFIHAMMING1')
	sources += 'flash_hamming1_handler.c'
endif

if kconfig.has_key('CONFIG_LUASCRIPTHANDLER')
	sources += 'lua_scripthandler.c'
endif

if kconfig.has_key('CONFIG_RAW')
	sources += 'raw_handler.c'
endif

if kconfig.has_key('CONFIG_RDIFFHANDLER')
	sources += 'rdiff_handler.c'

	dependencies += cc.find_library('rsync', required: true)
endif

if kconfig.has_key('CONFIG_READBACKHANDLER')
	sources += 'readback_handler.c'
endif

if kconfig.has_key('CONFIG_REMOTE_HANDLER')
	sources += 'remote_handler.c'

	dependencies += dependency('libczmq', required: true)
endif

if kconfig.has_key('CONFIG_SHELLSCRIPTHANDLER')
	sources += 'shell_scripthandler.c'
endif

if kconfig.has_key('CONFIG_SSBLSWITCH')
	sources += 'ssbl_handler.c'
endif

if kconfig.has_key('CONFIG_SWUFORWARDER_HANDLER')
	sources += ['swuforward_handler.c', 'swuforward-ws.c']

	dependencies += dependency('libwebsockets', required: true)
	dependencies += dependency('liburiparser', required: true)
endif

if kconfig.has_key('CONFIG_UBIVOL')
	sources += 'ubivol_handler.c'
endif

if kconfig.has_key('CONFIG_UCFWHANDLER')
	sources += 'ucfw_handler.c'

	dependencies += dependency('libgpiod', required: true)
endif

if kconfig.has_key('CONFIG_DOCKER')
	sources += 'docker_handler.c'
endif

dependencies += lua_dependency

if sources.length() != 0
	core_libs += static_library(
		'handlers',
		sources,
		include_directories: include_dirs,
		implicit_include_directories: false,
		dependencies: dependencies,
	)
endif
