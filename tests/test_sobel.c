#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include "../include/sobel.h"

// Test: Mean filter on uniform image returns same value
static void test_mean_filter_uniform_image(void** state) {
    (void)state;  // Unused

    // Create a 5x5 uniform image (all pixels = 100)
    unsigned char input[25];
    unsigned char output[25];
    memset(input, 100, 25);
    memset(output, 0, 25);

    // Apply mean filter
    sobel(5, 5, input, output);

    // Interior pixels should be 100 (average of 9 pixels all valued 100)
    // Check center pixel
    assert_int_equal(output[1 * 5 + 1], 100);
    assert_int_equal(output[1 * 5 + 2], 100);
    assert_int_equal(output[2 * 5 + 1], 100);
}

// Test: Mean filter smooths a gradient
static void test_mean_filter_gradient(void** state) {
    (void)state;

    // Create a 5x5 gradient image (horizontal gradient)
    unsigned char input[25] = {0,   50,  100, 150, 200, 0,   50,  100, 150, 200, 0,   50, 100,
                               150, 200, 0,   50,  100, 150, 200, 0,   50,  100, 150, 200};
    unsigned char output[25];
    memset(output, 0, 25);

    // Apply mean filter
    sobel(5, 5, input, output);

    // Center pixel should be average of its 3x3 neighborhood
    // For position [2,2] (center): (100*3 + 150*3 + 50*3) / 9 = 100
    assert_int_equal(output[1 * 5 + 2], 100);
}

// Test: Mean filter edge handling (edges should be 0)
static void test_mean_filter_edge_handling(void** state) {
    (void)state;

    unsigned char input[25];
    unsigned char output[25];
    memset(input, 100, 25);
    memset(output, 255, 25);  // Fill with non-zero

    sobel(5, 5, input, output);

    // Left edge should be 0
    assert_int_equal(output[0 * 5 + 0], 0);
    assert_int_equal(output[1 * 5 + 0], 0);
    assert_int_equal(output[2 * 5 + 0], 0);

    // Right edge should be 0
    assert_int_equal(output[0 * 5 + 4], 0);
    assert_int_equal(output[1 * 5 + 4], 0);
    assert_int_equal(output[2 * 5 + 4], 0);
}

// Test: Zero image input
static void test_mean_filter_zero_image(void** state) {
    (void)state;

    unsigned char input[25];
    unsigned char output[25];
    memset(input, 0, 25);
    memset(output, 255, 25);

    sobel(5, 5, input, output);

    // All interior pixels should be 0
    assert_int_equal(output[1 * 5 + 1], 0);
    assert_int_equal(output[2 * 5 + 2], 0);
}

// Test: Maximum value handling (255)
static void test_mean_filter_max_value(void** state) {
    (void)state;

    unsigned char input[25];
    unsigned char output[25];
    memset(input, 255, 25);
    memset(output, 0, 25);

    sobel(5, 5, input, output);

    // All interior pixels should be 255
    assert_int_equal(output[1 * 5 + 1], 255);
    assert_int_equal(output[2 * 5 + 2], 255);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mean_filter_uniform_image),
        cmocka_unit_test(test_mean_filter_gradient),
        cmocka_unit_test(test_mean_filter_edge_handling),
        cmocka_unit_test(test_mean_filter_zero_image),
        cmocka_unit_test(test_mean_filter_max_value),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
