project(
	'swupdate',
	'c',
	default_options: [
		'warning_level=1',
		'c_std=gnu99',
		'optimization=2',
		'prefix=/usr',
		'libdir=lib',
		'strip=true',
	],
	license: 'GPL-2.0-only',
	meson_version: '>= 0.61.0',
	version: files('meson.version'),
)

version_components = meson.project_version().split('.')
version_year = version_components[0]
version_month = version_components[1]
version_patch = version_components[2]
version_extra = run_command('./scripts/gen_extraversion', check: true).stdout().strip()

add_global_arguments('-DVERSION_YEAR=' + version_year, language: 'c')
add_global_arguments('-DVERSION_MONTH=' + version_month, language: 'c')
add_global_arguments('-DVERSION_PATCH=' + version_patch, language: 'c')
add_global_arguments('-DVERSION_EXTRA=' + version_extra, language: 'c')

add_global_arguments('-D_GNU_SOURCE', language: 'c')

cc = meson.get_compiler('c')
python = find_program('python3', 'python', required: true)
fs = import('fs')

# Parse Kconfig
run_command('mkdir', '--parents', 'include/generated/', check: true)
run_command(
	python,
	'./scripts/Kconfiglib/genconfig.py',
	'--header-path', 'include/generated/autoconf.h',
	check: true,
)
add_global_arguments(
	'-include', '../include/generated/autoconf.h',
	language: 'c',
)
keyval = import('keyval')
kconfig = keyval.load('.config')

include_dirs = include_directories('include')
install_headers(
	[
		'include/network_ipc.h',
		'include/swupdate_status.h',
		'include/progress_ipc.h',
	],
)

possible_cc_flags = [
	'-Wshadow',
	'-Wwrite-strings',
	'-Wundef',
	'-Wstrict-prototypes',
	'-Wunused',
	'-Wunused-parameter',
	'-Wunused-function',
	'-Wunused-value',
	'-Wmissing-prototypes',
	'-Wmissing-declarations',
	'-Wno-format-security',
	'-Wno-format-truncation',
	'-Wold-style-definition',
	'-finline-limit=0',
	'-fno-builtin-strlen',
	'-fomit-frame-pointer',
	'-ffunction-sections',
	'-fdata-sections',
	'-fno-guess-branch-probability',
	'-funsigned-char',
	'-falign-functions=1',
	'-falign-jumps=1',
	'-falign-labels=1',
	'-falign-loops=1',
	'-static-libgcc',
	'-fno-unwind-tables',
	'-fno-asynchronous-unwind-tables',
]

if kconfig.has_key('CONFIG_WERROR')
	possible_cc_flags += '-Werror'
endif

if kconfig.has_key('CONFIG_DEBUG')
	possible_cc_flags += '-g'

	if kconfig.has_key('CONFIG_DEBUG_PESSIMIZE')
		possible_cc_flags += '-O0'
	else
		possible_cc_flags += '-O2'
	endif
else
	possible_cc_flags += '-O2'
endif

add_project_arguments(
	cc.get_supported_arguments(possible_cc_flags),
	language: 'c',
)

if kconfig.has_key('CONFIG_LUA')
	lua_dependency = dependency(
		['lua', kconfig.get('CONFIG_LUAPKG').strip('"')],
		required: true,
	)

	embedded_lua_source = kconfig.get('CONFIG_EMBEDDED_LUA_HANDLER_SOURCE', '').strip(
		'""',
	)
	if embedded_lua_source != ''
		configure_file(
			copy: true,
			input: meson.global_source_root() / embedded_lua_source,
			output: embedded_lua_source,
		)
	endif
else
	lua_dependency = dependency('', required: false)
endif

ubootenv_dep = dependency('libubootenv', required: true, static: kconfig.has_key('CONFIG_BOOTLOADER_STATIC_LINKED'))
json_dep = dependency('json-c', required: true)
curl_dep = dependency('libcurl', required: true)

core_libs = []

# Libraries
subdir('bootloader')
subdir('containers')
subdir('corelib')
subdir('fs')
subdir('handlers')
subdir('ipc')
subdir('mongoose')
subdir('parser')
subdir('suricatta')

# Bindings
subdir('bindings')

# SWUpdate
subdir('core')

# Tools
subdir('tools')

# Tests
#subdir('test')
