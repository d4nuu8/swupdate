sources = []

dependencies = []

if kconfig.has_key('CONFIG_UBOOT')
	sources += 'uboot.c'
	dependencies += ubootenv_dep
endif

if kconfig.has_key('CONFIG_BOOTLOADER_NONE')
	sources += 'none.c'
endif

if kconfig.has_key('CONFIG_BOOTLOADER_GRUB')
	sources += 'grub.c'
	dependencies += cc.find_library('dl', required: true)
endif

if kconfig.has_key('CONFIG_BOOTLOADER_EBG')
	sources += 'ebg.c'

	if kconfig.has_key('CONFIG_BOOTLOADER_STATIC_LINKED')
		dependencies += dependency('libebgenv', required: true, static: true)
	endif
endif

if kconfig.has_key('CONFIG_BOOTLOADER_CBOOT')
	sources += 'cboot.c'

	if kconfig.has_key('CONFIG_BOOTLOADER_STATIC_LINKED')
		dependencies += dependency('tegra-boot-tools', required: true, static: true)
	endif
endif

if not kconfig.has_key('CONFIG_BOOTLOADER_STATIC_LINKED')
	dependencies += cc.find_library('dl', required: true)
endif

if sources.length() == 0
	sources += 'none.c'
endif

if sources.length() != 0
	core_libs += static_library(
		'bootloader',
		sources,
		include_directories: include_dirs,
		dependencies: dependencies,
		implicit_include_directories: false,
	)
endif
