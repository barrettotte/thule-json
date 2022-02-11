#ifndef __THULE_H
#define __THULE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUM,
    JSON_STR,
    JSON_ARR,
    JSON_OBJ
} json_type;

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
    union {
        char* val_str;
        int val_int;
        double val_dub;
        bool val_bool;
        json_arr_t* val_arr;
        json_obj_t* val_obj;
    } val;

    json_type type;
};

uint8_t json_parse(const char* json_src, const json_obj_t* root);

#endif