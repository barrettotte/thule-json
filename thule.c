#include <stdio.h>
#include "thule.h"

// check if character is whitespace
static bool is_whitespace(const char* c) {
    return *c == ' ' || *c == '\n' || *c == '\r' || *c == '\t';
}

// skip all whitespace
static void skip_whitespace(char* c) {
    while (is_whitespace(c)) {
        c++;
    }
}

// parse a JSON value
static uint8_t json_parse_val(char* c, const json_val_t* val) {
    uint8_t status = JSON_ERR_NONE;

    skip_whitespace(c);    
    switch (*c) {
        case '{':
            // TODO: object
            break;
        
        case '[':
            // TODO: array
            break;
        
        case '\"':
            // TODO: string
            break;

        case '0': case '1': case '2': case '3': case '4': 
        case '5': case '6': case '7': case '8': case '9':
        case '-':
            // TODO: number
            break;

        case 't':
        case 'f':
            // TODO: bool
            break;

        case 'n':
            // TODO: null
            break;

        default:
            status = JSON_ERR_VALSTART;
            break;
    }
    return status;
}

// parse json string (entry point)
uint8_t json_parse(const char* json_src, const json_val_t* root) {
    if (*json_src != '{' && *json_src != '[') {
        return JSON_ERR_ROOT;
    }
    return json_parse_val((char*) json_src, root);
}
