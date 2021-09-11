#include "lfc/collections/str.h"

#include <stdlib.h>
#include <string.h>

#include "lfc/utils/hash.h"
#include "lfc/utils/mem.h"
#include "lfc/utils/panic.h"

void str_init(str_t* str) {
    str->capacity = STR_DEFAULT_CAPACITY;
    str->data = malloc_unwrap(sizeof(char), str->capacity, "[str_init] failed to alloc string data");
    str->len = 0;
}

void str_from(str_t* str, char* literal) {
    str->len = strlen(literal);
    str->capacity = str->len + STR_DEFAULT_CAPACITY;
    str->data = malloc_unwrap(sizeof(char), str->capacity, "[str_init] failed to alloc string data");
    memcpy(str->data, literal, str->len);
}

void str_free(str_t* str) {
    free(str->data);
}

void str_push(str_t* str, char c) {
    if (str->len + 1 > str->capacity) {
        str->capacity *= 2;
        str->data = malloc_unwrap(sizeof(char), str->capacity, "[str_push] failed to expand string");
    }

    str->data[str->len] = c;
    str->len += 1;
}

void str_pushstr(str_t* str, str_t* to_push) {
    if (str->len + to_push->len > str->capacity) {
        str->capacity = str->capacity * 2 + to_push->len;
        str->data = malloc_unwrap(sizeof(char), str->capacity, "[str_pushstr] failed to expand string");
    }

    memcpy(str->data + str->len, to_push->data, to_push->len);
    str->len += to_push->len;
}

char str_pop(str_t* str, char c) {
    if (str->len == 0) {
        panic(1, "[str_pop] can't pop from empty string");
    }

    // NOTE: is this too confusing to be worth it?
    return str->data[--str->len];
}

char str_get(str_t* str, size_t index) {
    if (index >= str->len) {
        panic(1, "[str_get] index %li out of bounds for string of length %li", index, str->len);
    }

    return str->data[index];
}

char* str_at(str_t* str, size_t index) {
    if (index >= str->len) {
        panic(1, "[str_at] index %li out of bounds for string of length %li", index, str->len);
    }

    return str->data + index;
}

uint8_t str_eq(str_t* str, str_t* other) {
    if (str->len != other->len) {
        return 0;
    }

    for (size_t i = 0; i < str->len; i++) {
        char c1 = str_get(str, i);
        char c2 = str_get(other, i);

        if (c1 != c2) {
            return 0;
        }
    }

    return 1;
}

uint8_t str_is_empty(str_t* str) {
    return str->len == 0;
}

uint8_t str_starts_with(str_t* str, char c) {
    return str->len > 0 && str_get(str, 0) == c;
}

uint8_t str_ends_with(str_t* str, char c) {
    return str->len > 0 && str_get(str, str->len - 1) == c;
}

size_t str_simple_hash(str_t* str) {
    return __strbase_simple_hash(str->data, str->len);
}
