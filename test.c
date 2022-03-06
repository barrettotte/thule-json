#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thule.h"

void test_object_empty() {
    const char* src = "{}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->v_object;
    assert(obj != NULL);
    assert(obj->key == NULL);
    assert(obj->next == NULL);
    assert(obj->prev == NULL);
    assert(obj->val == NULL);

    json_value_free(root);
}

void test_object_basic_1() {
    const char* src = "{\"username\": \"barrettotte\"}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->v_object;
    assert(obj != NULL);
    assert(strcmp(obj->key, "username") == 0);
    assert(obj->val->kind == JSON_VAL_STRING);
    assert(strcmp(obj->val->v_string, "barrettotte") == 0);
    assert(obj->next == NULL);
    assert(obj->prev == NULL);

    json_value_free(root);
}

void test_object_basic_2() {
    const char* src = "{\"myInt\": 123, \"myDouble\": 3.14}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->v_object;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myInt") == 0);
    assert(obj->val->kind == JSON_VAL_INT);
    assert(obj->val->v_int == 123);
    assert(obj->prev == NULL);
    
    obj = obj->next;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myDouble") == 0);
    assert(obj->val->kind == JSON_VAL_DOUBLE);
    assert(obj->val->v_double == 3.14);
    assert(obj->prev != NULL);
    assert(obj->next == NULL);

    json_value_free(root);
}

int main() {
    test_object_empty();
    printf("===============\n");
    test_object_basic_1();
    printf("===============\n");
    test_object_basic_2();

    return 0;
}
