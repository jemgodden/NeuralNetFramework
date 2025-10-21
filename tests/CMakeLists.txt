if(BUILD_TESTING)
    set(TESTS_EXECUTABLE_NAME nnf_tests)

    include_directories(src)

    add_executable(${TESTS_EXECUTABLE_NAME} tests.cpp)

    target_link_libraries(${TESTS_EXECUTABLE_NAME}
            PUBLIC libnnf
            PRIVATE GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${TESTS_EXECUTABLE_NAME})
endif()
