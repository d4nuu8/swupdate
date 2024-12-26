if kconfig.has_key('CONFIG_LUA')
	core_libs += shared_library(
		'swupdate',
		['auxiliar.c', 'lua_swupdate.c', 'lua_compat_shared.c'],
		name_prefix: 'lua_',
		include_directories: include_dirs,
		dependencies: lua_dependency,
		link_with: ipc_lib,
		implicit_include_directories: false,
		install: true,
		install_dir: get_option('datadir') / 'lua' / lua_dependency.get_variable(
			'major_version',
			default_value: lua_dependency.get_variable(
				'abiver',
				default_value: '',
			),
		),
		version: '0.1',
	)
endif
