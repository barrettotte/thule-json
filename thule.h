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

typedef struct json_val json_val_t;
typedef struct json_arr json_arr_t;
typedef struct json_obj json_obj_t;

struct json_arr {
    json_val_t* items;
    size_t length;
    size_t capacity;
};

struct json_obj {
    char* key;
    json_val_t* val;
    json_obj_t* next;
    json_obj_t* prev;
};

struct json_val {
    json_val_kind kind;
    union {
        char* val_str;
        int val_int;
        double val_dub;
        bool val_bool;
        json_arr_t* val_arr;
        json_obj_t* val_obj;
    } val;
};

/* functions */

uint8_t json_parse(const char* json_src, const json_val_t* root);

#endif