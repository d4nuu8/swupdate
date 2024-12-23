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

    # test('test_crypt', cyrpt_test)
endif

if kconfig.has_key('CONFIG_HASH_VERIFY')
    hash_test = executable(
        'test_hash',
        'test_hash.c',
        include_directories: include_dirs,
        dependencies: cmocka_dependency,
        link_with: swupdatelib,
    )

    # test('test_hash', hash_test)
endif

if kconfig.has_key('CONFIG_SIGALG_RAWRSA') and kconfig.has_key('CONFIG_SIGNED_IMAGES')
    verify_test = executable(
        'test_verify',
        'test_verify.c',
        include_directories: include_dirs,
        dependencies: [cmocka_dependency, lua_dependency],
        link_with: swupdatelib,
    )

    # test('test_veriyf', verify_test)
endif

if kconfig.has_key('CONFIG_SURICATTA_HAWKBIT')
    hawkbit_test = executable(
        'test_hawkbit',
        ['test_json.c', 'test_server_hawkbit.c'],
        include_directories: include_dirs,
        dependencies: cmocka_dependency,
        link_with: swupdatelib,
    )

    # test('test_hawkbit', hawkbit_test)
endif
