project(
	'swupdate',
	'c',
	version: '2024.05.02',
	default_options: [
		'warning_level=1',
		'c_std=gnu99',
		'optimization=2',
		'warning_level=1',
		'prefix=/usr',
		'libdir=lib',
		'strip=true',
	],
)

add_global_arguments('-DSWU_VER="1.0.0"', language: 'c')
add_global_arguments('-D_GNU_SOURCE', language: 'c')

cc = meson.get_compiler('c')
python = find_program('python3', 'python', required: true)

# Parse Kconfig
run_command(
	python,
	'./scripts/KConfiglib/genconfig.py',
	'--header-path', 'include/generated/autoconf.h',
	check: true,
)
add_global_arguments('-include', '../include/generated/autoconf.h', language: 'c')
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
add_project_arguments(cc.get_supported_arguments(possible_cc_flags), language: 'c')

if kconfig.has_key('CONFIG_LUA')
	lua_dependency = dependency(['lua', kconfig.get('CONFIG_LUAPKG').strip('"')], required: true)

	embedded_lua_source = kconfig.get('CONFIG_EMBEDDED_LUA_HANDLER_SOURCE', '').strip('""')
    if embedded_lua_source != ''
        configure_file(copy: true, input: meson.source_root() / embedded_lua_source, output: embedded_lua_source)
	endif
endif

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
subdir('test')
