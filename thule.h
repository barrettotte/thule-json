#ifndef __THULE_H
#define __THULE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    JSON_ERR_NONE,
    JSON_ERR_ROOT,
    JSON_ERR_VALSTART,
} json_error;

typedef enum {
    JSON_VAL_NULL,
    JSON_VAL_BOOL,
    JSON_VAL_NUM,
    JSON_VAL_STR,
    JSON_VAL_ARR,
    JSON_VAL_OBJ
} json_val_kind;

typedef struct json_val json_val;
typedef struct json_arr json_arr;
typedef struct json_obj json_obj;

struct json_arr {
    json_val* items;
    size_t length;
    size_t capacity;
};

struct json_obj {
    char* key;
    json_val* val;
    json_obj* next;
    json_obj* prev;
};

struct json_val {
    json_val_kind kind;
    union {
        char* val_str;
        int val_int;
        double val_dub;
        bool val_bool;
        json_arr* val_arr;
        json_obj* val_obj;
    } val;
};

/* functions */

void json_parse(const char* src, json_val* root);
void json_free(const json_val* root);

#endif