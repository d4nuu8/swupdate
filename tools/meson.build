executable(
	'swupdate-client',
	'swupdate-client.c',
	include_directories: include_dirs,
	link_with: ipc_lib,
	install: true,
)

executable(
	'swupdate-ipc',
	'swupdate-ipc.c',
	include_directories: include_dirs,
	dependencies: [json_dep, curl_dep],
	link_with: ipc_lib,
	install: true,
)

executable(
	'swupdate-progress',
	'swupdate-progress.c',
	include_directories: include_dirs,
	link_with: ipc_lib,
	install: true,
)
