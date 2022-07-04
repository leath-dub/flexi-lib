#include "../flexi.h"

/*
 * This file is a test file for testing aspects of the library.
 * feel free to use these as guide for usage of library
 *
 */

void main(void) {
    ArrString arr;
    FlexiInit(&arr);

    for (size_t i = 0; i < 100; ++i)
        arr.head.append(&arr, "Hello World!");
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.remove_index(&arr, 10);
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.replace_index(&arr, 20, "REPLACED");
    arr.head.print(&arr);
    printf("len: %d, cap: %d\n", arr.head.length, arr.head.capacity);

    arr.head.destroy(&arr);
}
