# Find CMocka package
find_package(cmocka REQUIRED)

# Test for Sobel filter
add_executable(test_sobel test_sobel.c ../src/sobel.c)
target_link_libraries(test_sobel cmocka m)
target_include_directories(test_sobel PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add test to CTest
add_test(NAME SobelTests COMMAND test_sobel)

# Enable testing
enable_testing()
