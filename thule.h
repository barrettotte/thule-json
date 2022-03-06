#ifndef __THULE_H
#define __THULE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    JSON_ERR_BADTOKEN
} json_error;

typedef enum {
    JSON_VAL_UNINIT,
    JSON_VAL_NULL,
    JSON_VAL_BOOL,
    JSON_VAL_INT,
    JSON_VAL_DOUBLE,
    JSON_VAL_STRING,
    JSON_VAL_ARRAY,
    JSON_VAL_OBJECT,
    JSON_VAL_ERROR
} json_val_kind;

typedef struct json_value json_value;
typedef struct json_array json_array;
typedef struct json_object json_object;

struct json_array {
    json_value** items;
    size_t length;
    size_t capacity;
};

struct json_object {
    char* key;
    json_value* val;
    json_object* next;
    json_object* prev;
};

struct json_value {
    json_val_kind kind;
    union {
        char*        t_string;
        long         t_int;
        double       t_double;
        bool         t_bool;
        json_array*  t_array;
        json_object* t_object;
    } v;
};

json_value* json_parse(const char* src);
void json_value_free(json_value* node);
void json_value_print(json_value* root);

#endif