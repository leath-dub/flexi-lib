#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    AR_UNASSIGNED,
    AR_INT,
    AR_STRING,
    AR_COUNT
} ArrType;

typedef struct {
    char *name[32];
    ArrType type;
    size_t length;
    size_t capacity;
    void (*create)(void *, size_t);
    void (*destroy)(void *);
    void (*append)(void *, const void *);
    void (*pop)(void *);
    void (*remove_index)(void *, size_t);
    void (*replace_index)(void *, size_t, const void *);
    void (*print)(void *);
} ArrHead;

typedef struct {
    ArrHead head;
    int *arr;
} ArrInt;

typedef struct {
    ArrHead head;
    char *arr;
} ArrChar;

typedef struct {
    ArrHead head;
    char **arr;
} ArrString;

// debug macros
#define DEBUG 1
#define log(MSG) printf("log: "MSG"\n")
#define err(MSG) fprintf(stderr, "error in line %d: "MSG"\n", __LINE__)

static void array_create(void *head, size_t size);
static void array_destroy(void *head);

static void array_append(void *head, const void *item);
static void array_pop(void *head);
static void array_remove_index(void *head, size_t index);
static void array_replace_index(void *head, size_t index, const void *item);

static void array_print(void *head);

static void array_panic(void *head);

void FlexiInit(void *head, const char *type);

void array_create(void *head, size_t size) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)head;
        array->arr = malloc(sizeof(int) * size);
        if (array->arr == NULL) {
            err("malloc failed");
            array_panic(ptr);
        }
        ptr->length = 0;
        ptr->capacity = size;
#ifdef DEBUG
        log("integer array created");
#endif
    } else if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head;
        array->arr = malloc(sizeof(char *) * size);
        if (array->arr == NULL) {
            err("malloc failed");
            array_panic(ptr);
        }
        /* to avoid any undefined behaviour when freeing memory on destroy
         * we allocate one type size of area at each index in the array
         */
        for (size_t i = 0; i < size; i++) {
            array->arr[i] = malloc(sizeof(char));
            if (array->arr[i] == NULL) {
                err("malloc failed");
                array_panic(ptr);
            }
        }
        // Length and capaciy of 1 dim array of pointers
        ptr->length = 0;
        ptr->capacity = size;
#ifdef DEBUG
        log("string array created");
#endif
    } else if (ptr->type == AR_UNASSIGNED) {
        err("Unassigned typing to array, assign `array_struct`.head.type enum");
    }
}
#define array_create(A) array_create(A, 8)

void array_destroy(void *head) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)head;
        free(array->arr);
#ifdef DEBUG
        log("integer array destroyed");
#endif
    } else if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head;
        for (size_t i = 0; i < ptr->capacity; i++)
            free(array->arr[i]);
        free(array->arr);
#ifdef DEBUG
        log("string array destroyed");
#endif
    }
}

void array_append(void *head, const void *item) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)head;
        // dynamic resize
        if ((ptr->capacity - ptr->length) <= 0) {
            ptr->capacity *= 2;
            int *data = realloc(array->arr,
                sizeof(int) * ptr->capacity);
            if (data == NULL) {
                err("realloc failed");
                array_panic(ptr);
            } array->arr = data;
        }
        // appending item
        int casted = (intptr_t)item;
        array->arr[ptr->length] = casted;
        ptr->length++;
    }
    if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head; // cast head for child
        // dynamic resize
        if ((ptr->capacity - ptr->length) <= 0) {
            ptr->capacity *= 2;
            char **data = realloc(array->arr,
                sizeof(char *) * ptr->capacity);
            if (data == NULL) {
                err("realloc failed");
                array_panic(ptr);
            } array->arr = data;
        }
        // appending item
        char *casted = (char *)item;
        size_t str_len;
        for (str_len = 0; casted[str_len] != '\0'; ++str_len);
        char *data = realloc(array->arr[ptr->length],
            sizeof(char) * ++str_len);
        array->arr[ptr->length] = data;
        memcpy(array->arr[ptr->length], casted, str_len * sizeof(char));
        ptr->length++;
    }
}

void array_pop(void *head) {
    ArrHead *ptr = (ArrHead *)head;
    ptr->length--;
}

void array_remove_index(void *head, size_t index) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)ptr;
        while ((index + 1) < (ptr->length)) {
            (array->arr)[index] = (array->arr)[index + 1];
            ++index;
        }
        ptr->length--;
    } else if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head;
        while ((index + 1) < (ptr->length)) {
            size_t str_len;
            // get length of string infront of target
            for (str_len = 0;
                array->arr[index + 1][str_len] != '\0';
            ++str_len);
            char *data = realloc(array->arr[index],
                sizeof(char) * ++str_len);
            if (data == NULL) {
                err("realloc failed");
                array_panic(ptr);
            }
            memcpy(array->arr[index], array->arr[index + 1], str_len);
            ++index;
        }
        ptr->length--;
    }
}

void array_replace_index(void *head, size_t index, const void *item) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)ptr;
        array->arr[index] = (intptr_t)item;
    } else if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head;
        char *str = (char *)item;
        size_t str_len;
        for (str_len = 0; str[str_len] != '\0'; ++str_len);
        char *data = realloc(array->arr[index],
            sizeof(char) * ++str_len);
        if (data == NULL) {
            err("realloc failed");
            array_panic(ptr);
        }
        array->arr[index] = data;
        memcpy(array->arr[index], item, str_len);
    }
}

void array_print(void *head) {
    ArrHead *ptr = (ArrHead *)head;
    if (ptr->type == AR_INT) {
        ArrInt *array = (ArrInt *)head;
        printf("[");
        if (ptr->length == 0) {
            printf("]\n");
        } else {
            for (size_t i = 0; i < ptr->length - 1; ++i)
                printf("%d, ", array->arr[i]);
            printf("%d]\n", array->arr[ptr->length - 1]);
        }
    } else if (ptr->type == AR_STRING) {
        ArrString *array = (ArrString *)head;
        printf("[");
        if (ptr->length == 0) {
            printf("]\n");
        } else {
            for (size_t i = 0; i < ptr->length - 1; ++i)
                printf("\"%s\", ", array->arr[i]);
            printf("\"%s\"]\n", array->arr[ptr->length - 1]);
        }
    }
}

void FlexiInit(void *head, const char *type) {
    ArrHead *ptr = (ArrHead *)head;
    if (type == "INT") ptr->type = AR_INT;
    if (type == "STRING") ptr->type = AR_STRING;
    ptr->create = array_create;
    ptr->destroy = array_destroy;
    ptr->append = array_append;
    ptr->pop = array_pop;
    ptr->remove_index = array_remove_index;
    ptr->replace_index = array_replace_index;
    ptr->print = array_print;
    array_create(ptr);
}

void array_panic(void *head) {
    ArrHead *ptr = (ArrHead *)head;
    ptr->destroy(ptr);
    exit(1);
}
