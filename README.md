# Flexi library
A simple dynamic array library in C

---

## Features

- Support for 1 dimensional integer arrays and 2 dimensional string arrays

- Methods applicable on the above, ``append``, ``pop``, ``remove_index``, ``replace_index``,
  ``print``

## Usage

There are 2 structs for int and string arrays respectively ``ArrInt``, ``ArrString``.
Declare the struct and initialize the array:
```C
ArrString arr;
FlexiInit(&arr, "STRING");  // "INT" for ints
```
Access to methods with ``arr.head.<MethodName>``, e.g. :
```C
arr.head.append(&arr, "Hello World!");
arr.head.replace_index(&arr, 0, "Goodbye World!");
```
After you are finished using the array call the ``destroy`` method:
```C
arr.head.destroy(&arr);
```
This is important to free up memory allocated by malloc/realloc.

### Notes

- all methods take first argument as a reference to your array struct
- when passing ints to the ``append``/``replace_index`` functions you will get a
warning about int -> pointer conversion( this is safe, ignore it)
- Upon request or in the case that I need it, I will add support for other types
- Feedback and/or questions are welcome, p.s. You probably shouldn't use this
library, I am still learning C :).
