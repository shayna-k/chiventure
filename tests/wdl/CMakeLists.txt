set(TEST_EXE test-wdl)

add_executable(${TEST_EXE}
        test_attributes.c
        test_game.c
        test_item.c
        test_objstore.c
        test_room.c
        test_validation.c
        test_wdl_ctx.c
        test_wdz.c
        main.c)

target_link_libraries(${TEST_EXE} ${CRITERION_LIBRARY} ${JSONC_LIBRARIES} ${LIBZIP_LIBRARIES})

foreach(module ${CHIVENTURE_MODULES})
    target_link_libraries(${TEST_EXE} ${module})
endforeach(module)

target_include_directories(${TEST_EXE} PRIVATE ${LIBZIP_INCLUDE_DIR})
target_include_directories(${TEST_EXE} PRIVATE ${JSONC_INCLUDE_DIR})

add_test(NAME ${TEST_EXE}
         COMMAND ${TEST_EXE})
