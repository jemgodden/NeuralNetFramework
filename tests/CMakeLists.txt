if(BUILD_TESTING)
    set(TESTS_EXECUTABLE_NAME nnf_tests)

    include_directories(src)

    file(GLOB_RECURSE SOURCES "*.cpp")

    add_executable(${TESTS_EXECUTABLE_NAME} ${SOURCES})

    target_link_libraries(${TESTS_EXECUTABLE_NAME}
            PUBLIC libnnf
            PRIVATE GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${TESTS_EXECUTABLE_NAME})
endif()
