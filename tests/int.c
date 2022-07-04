#include "../flexi.h"

/*
 * This file is a test file for testing aspects of the library.
 * feel free to use these as guide for usage of library
 *
 */

void main(void) {
    ArrInt arr;
    FlexiInit(&arr, "INT");

    for (size_t i = 0; i < 100; ++i)
        arr.head.append(&arr, i);
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.remove_index(&arr, 10);
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.replace_index(&arr, 20, 666);
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.destroy(&arr);
}
