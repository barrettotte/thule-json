#include "thule.h"
#include <stdio.h>  // TODO: remove

// parse json string (entry point)
uint8_t json_parse(const char* json_src, const json_obj_t* root) {
    uint8_t status = 0;
    char* c = (char*) json_src;

    // TODO: remove
    while (*c != '\0') {
        printf("%c", *c++);
    }
    printf("\n");
    // TODO: remove

    return status;
}

