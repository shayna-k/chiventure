set(TEST_EXE test-npc)

add_executable(${TEST_EXE}
        test_dialogue.c
        test_npc.c
        test_rooms_npc.c
        main.c)

target_link_libraries(${TEST_EXE} ${CRITERION_LIBRARY})

foreach(module ${CHIVENTURE_MODULES})
    target_link_libraries(${TEST_EXE} ${module})
endforeach(module)

add_test(NAME ${TEST_EXE}
         COMMAND ${TEST_EXE})

target_link_libraries(${TEST_EXE} ${LUA_LIBRARIES})
target_include_directories(${TEST_EXE} PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(${TEST_EXE} PRIVATE tests/) 