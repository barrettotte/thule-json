#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../thule.h"

void test_parse_object() {
    const char* src = "{\"username\": \"barrettotte\", \"age\": 25, \"test\": null}";
    json_val_t* root = malloc(sizeof(json_val_t));
    const uint8_t result = json_parse(src, root);

    assert(result == JSON_ERR_NONE);
    free(root);
}

void test_ERR_ROOT() {
    const char* src = "x";
    json_val_t* root = malloc(sizeof(json_val_t));
    const uint8_t result = json_parse(src, root);

    assert(result == JSON_ERR_ROOT);
    free(root);
}

int main() {
    // successful parses
    test_parse_object();

    // parse errors
    test_ERR_ROOT();

    return 0;
}
