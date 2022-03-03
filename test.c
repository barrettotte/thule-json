#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "thule.h"

void test_parse_object() {
    const char* src = "{\"username\": \"barrettotte\", \"age\": 25, \"test\": null}";
    json_val* root = malloc(sizeof(json_val));
    json_parse(src, root);
    
    assert(root != NULL);

    json_free(root);
}

int main() {
    test_parse_object();

    return 0;
}
