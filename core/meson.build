sources = [
	'semver.c',
	'strlcpy.c',
	'syslog.c',
	'bootloader.c',
	'state.c',
	'network_utils.c',
	'hw-compatibility.c',
	'install_from_file.c',
	'notifier.c',
	'pctl.c',
	'progress_thread.c',
	'swupdate_dict.c',
	'artifacts_versions.c',
	'cpio_utils.c',
	'handler.c',
	'installer.c',
	'network_thread.c',
	'parser.c',
	'parsing_library.c',
	'stream_interface.c',
	'swupdate_vars.c',
	'util.c',
]

dependencies = [json_dep, ubootenv_dep]

if kconfig.has_key('CONFIG_GUNZIP')
	dependencies += dependency('zlib', required: true)
endif

if kconfig.has_key('CONFIG_PKCS11')
	dependencies += dependency('p11-kit-1', required: true)
endif

if kconfig.has_key('CONFIG_ZSTD')
	dependencies += dependency('libzstd', required: true)
endif

if kconfig.has_key('CONFIG_SYSTEMD')
	dependencies += dependency('libsystemd', required: true)
endif

dependencies += lua_dependency

swupdatelib = static_library(
	'swupdate',
	sources,
	include_directories: include_dirs,
	dependencies: dependencies,
	link_with: core_libs,
	link_args: link_args,
	implicit_include_directories: false,
)

executable(
	'swupdate',
	'swupdate.c',
	include_directories: include_dirs,
	dependencies: dependencies,
	link_with: swupdatelib,
	install: true,
)
