sources = []
dependencies = []

if kconfig.has_key('CONFIG_DOCKER')
	sources += 'docker.c'
endif

if sources.length() != 0
	core_libs += static_library(
		'containers',
		sources,
		include_directories: include_dirs,
		dependencies: dependencies,
		implicit_include_directories: false,
	)
endif
