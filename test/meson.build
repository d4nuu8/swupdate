cmocka_dependency = dependency('cmocka', required: true)

util_test = executable(
	'test_util',
	'test_util.c',
	include_directories: include_dirs,
	dependencies: cmocka_dependency,
	link_with: swupdatelib,
)
test('test_util', util_test)

if not kconfig.has_key('CONFIG_PKCS11') and kconfig.has_key('CONFIG_ENCRYPTED_IMAGES')
	cyrpt_test = executable(
		'test_crypt',
		'test_crypt.c',
		include_directories: include_dirs,
		dependencies: cmocka_dependency,
		link_with: swupdatelib,
	)
	test('test_crypt', cyrpt_test)
endif

if kconfig.has_key('CONFIG_HASH_VERIFY')
	hash_test = executable(
		'test_hash',
		'test_hash.c',
		include_directories: include_dirs,
		dependencies: cmocka_dependency,
		link_with: swupdatelib,
	)
	test('test_hash', hash_test)
endif

if kconfig.has_key('CONFIG_SIGALG_RAWRSA') and kconfig.has_key('CONFIG_SIGNED_IMAGES')
	rsa_private = custom_target(
		'test_verify_rsa_private',
		output: 'signing-secret.pem',
		command: ['openssl', 'genrsa', '-out', '@OUTPUT@'],
	)

	rsa_public = custom_target(
		'test_verify_rsa_public',
		input: rsa_private,
		output: 'signing-pubkey.pem',
		command: [
			'openssl',
			'rsa',
			'-in', '@INPUT@',
			'-out', '@OUTPUT@',
			'-outform', 'PEM',
			'-pubout',
		],
	)

	rsa_signed = configure_file(
		copy: true,
		input: 'data/to-be-signed',
		output: 'to-be-signed',
	)

	rsa_signature = custom_target(
		'test_verify_rsa_signature',
		input: [rsa_private, rsa_signed],
		output: 'signature',
		command: ['openssl', 'dgst', '-sha256', '-sign', '@INPUT0@', '@INPUT1@'],
		capture: true,
		install: true,
		install_dir: 'data',
	)

	verify_test = executable(
		'test_verify',
		['test_verify.c', rsa_public, rsa_signature],
		include_directories: include_dirs,
		dependencies: [cmocka_dependency, lua_dependency],
		link_with: swupdatelib,
	)
	test('test_veriyf', verify_test)
endif

if kconfig.has_key('CONFIG_SURICATTA_HAWKBIT')
	sources = ['test_server_hawkbit.c']

	link_args = run_command('scripts/generate_linker_flags', sources, check: true).stdout().split()

	hawkbit_test = executable(
		'test_hawkbit',
		sources,
		include_directories: include_dirs,
		dependencies: cmocka_dependency,
		link_with: swupdatelib,
		link_args: link_args,
	)
	test('test_hawkbit', hawkbit_test)

	sources = [
		'test_json.c',
	]

	link_args = run_command('scripts/generate_linker_flags', sources, check: true).stdout().split()

	json_test = executable(
		'test_json',
		sources,
		include_directories: include_dirs,
		dependencies: cmocka_dependency,
		link_with: swupdatelib,
		link_args: link_args,
	)

	test('test_json', json_test)
endif
