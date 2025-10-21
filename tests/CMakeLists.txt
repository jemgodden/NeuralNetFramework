if(BUILD_TESTING)
    add_executable(nnffs_tests tests.cpp)
    target_link_libraries(nnffs_tests
            PRIVATE GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(nnffs_tests)
endif()
