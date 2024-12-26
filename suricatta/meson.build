sources = []

dependencies = []

if kconfig.has_key('CONFIG_SURICATTA')
	sources += 'suricatta.c'
endif

if kconfig.has_key('CONFIG_SURICATTA_HAWKBIT')
	sources += 'server_hawkbit.c'
endif

if kconfig.has_key('CONFIG_SURICATTA_LUA')
	sources += 'server_lua.c'

	dependencies += lua_dependency
endif

if kconfig.has_key('CONFIG_SURICATTA_GENERAL')
	sources += 'server_general.c'
endif

if sources.length() != 0
	core_libs += static_library(
		'suricatta',
		sources,
		include_directories: include_dirs,
		implicit_include_directories: false,
		dependencies: dependencies,
	)
endif
