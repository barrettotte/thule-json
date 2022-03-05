#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "thule.h"

void test_object_empty() {
    const char* src = "{}\0";

    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->val.v_object;
    assert(obj != NULL);
    assert(obj->key == NULL);
    assert(obj->next == NULL);
    assert(obj->prev == NULL);
    // printf("%d\n", obj->val->kind);
    // assert(obj->val == NULL);

    json_value_free(root);
}

int main() {
    test_object_empty();

    return 0;
}
