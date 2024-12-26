sources = []
dependencies = []

if kconfig.has_key('CONFIG_DISKFORMAT')
	sources += 'diskformat.c'

	dependencies += dependency('blkid', required: true)
endif

if kconfig.has_key('CONFIG_FAT_FILESYSTEM')
	sources += ['diskio.c', 'fat_fs.c', 'ff.c']

	dependencies += dependency('ext2fs', required: true)
	dependencies += dependency('uuid', required: true)
	dependencies += dependency('blkid', required: true)
endif

if kconfig.has_key('CONFIG_EXT_FILESYSTEM')
	sources += 'mke2fs.c'
endif

if kconfig.has_key('CONFIG_BTRFS_FILESYSTEM')
	sources += 'btrfs.c'

	dependencies += dependency('libbtrfsutil', required: true)
	dependencies += dependency('libudev', required: true)

	if kconfig.has_key('BTRFS_FILESYSTEM_USELIBMKFS')
		dependencies += dependency('mkfsbtrfs', required: true)
	endif
endif

if sources.length() != 0
	core_libs += static_library(
		'fs',
		sources,
		include_directories: include_dirs,
		dependencies: dependencies,
		implicit_include_directories: false,
	)
endif
