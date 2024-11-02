sources = ['network_ipc-if.c', 'network_ipc.c', 'progress_ipc.c']

ipc_lib = shared_library(
	'swupdate',
	sources,
	include_directories: include_dirs,
	install: true,
	version: '0.1',
)
core_libs += ipc_lib
