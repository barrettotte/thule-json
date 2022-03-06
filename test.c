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

    json_object* obj = root->v.t_object;
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

    json_object* obj = root->v.t_object;
    assert(obj != NULL);
    assert(strcmp(obj->key, "username") == 0);
    assert(obj->val->kind == JSON_VAL_STRING);
    assert(strcmp(obj->val->v.t_string, "barrettotte") == 0);
    assert(obj->next == NULL);
    assert(obj->prev == NULL);

    json_value_free(root);
}

void test_object_basic_2() {
    const char* src = "{\"myInt\": 123, \"myDouble\": 3.14}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->v.t_object;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myInt") == 0);
    assert(obj->val->kind == JSON_VAL_INT);
    assert(obj->val->v.t_int == 123);
    assert(obj->prev == NULL);
    
    obj = obj->next;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myDouble") == 0);
    assert(obj->val->kind == JSON_VAL_DOUBLE);
    assert(obj->val->v.t_double == 3.14);
    assert(obj->prev != NULL);
    assert(obj->next == NULL);

    json_value_free(root);
}

void test_object_basic_3() {
    const char* src = "{\"myTrue\": true, \"myFalse\": false, \"myNull\": null}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* obj = root->v.t_object;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myTrue") == 0);
    assert(obj->val->kind == JSON_VAL_BOOL);
    assert(obj->val->v.t_bool);
    assert(obj->prev == NULL);
    
    obj = obj->next;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myFalse") == 0);
    assert(obj->val->kind == JSON_VAL_BOOL);
    assert(!obj->val->v.t_bool);
    assert(obj->prev != NULL);
    assert(obj->next != NULL);

    obj = obj->next;
    assert(obj != NULL);
    assert(strcmp(obj->key, "myNull") == 0);
    assert(obj->val->kind == JSON_VAL_NULL);
    assert(obj->val->v.t_int == 0);
    assert(obj->prev != NULL);
    assert(obj->next == NULL);

    json_value_free(root);
}

void test_object_multilevel() {
    const char* src = "{\"user\":{\"username\":\"barrettotte\",\"email\":\"barrettotte@gmail.com\",\"age\":25}}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* rootObj = root->v.t_object;
    assert(rootObj != NULL);
    assert(strcmp(rootObj->key, "user") == 0);
    assert(rootObj->val->kind == JSON_VAL_OBJECT);

    json_object* userObj = rootObj->val->v.t_object;
    assert(userObj != NULL);
    assert(strcmp(userObj->key, "username") == 0);
    assert(userObj->val->kind == JSON_VAL_STRING);
    assert(strcmp(userObj->val->v.t_string, "barrettotte") == 0);
    assert(userObj->next != NULL);
    assert(userObj->prev == NULL);

    userObj = userObj->next;
    assert(userObj != NULL);
    assert(strcmp(userObj->key, "email") == 0);
    assert(userObj->val->kind == JSON_VAL_STRING);
    assert(strcmp(userObj->val->v.t_string, "barrettotte@gmail.com") == 0);
    assert(userObj->next != NULL);
    assert(userObj->prev != NULL);

    userObj = userObj->next;
    assert(userObj != NULL);
    assert(strcmp(userObj->key, "age") == 0);
    assert(userObj->val->kind == JSON_VAL_INT);
    assert(userObj->val->v.t_int == 25);
    assert(userObj->next == NULL);
    assert(userObj->prev != NULL);

    json_value_free(root);
}

void test_array_empty() {
    const char* src = "[]";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_ARRAY);
    assert(root->v.t_array->length == 0);
    json_value_free(root);
}

void test_array_basic_1() {
    const char* src = "[4]";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_ARRAY);
    json_array* arr = root->v.t_array;
    assert(arr->length == 1);
    assert(arr->items[0]->kind == JSON_VAL_INT);
    assert(arr->items[0]->v.t_int == 4);
    json_value_free(root);
}

void test_array_basic_2() {
    const char* src = "[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]";  // test realloc
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_ARRAY);
    json_array* arr = root->v.t_array;

    assert(arr->length == 16);
    for (int i = 0; i < 16; i++) {
        assert(arr->items[i]->kind == JSON_VAL_INT);
        assert(arr->items[i]->v.t_int == i);
    }
    json_value_free(root);
}

void test_object_with_array() {
    const char* src = "{\"project\":{\"name\":\"thule-json\",\"languages\":[\"C\",\"Makefile\"]}}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* rootObj = root->v.t_object;
    assert(rootObj != NULL);
    assert(strcmp(rootObj->key, "project") == 0);
    assert(rootObj->val->kind == JSON_VAL_OBJECT);

    json_object* projectObj = rootObj->val->v.t_object;
    assert(projectObj != NULL);
    assert(strcmp(projectObj->key, "name") == 0);
    assert(projectObj->val->kind == JSON_VAL_STRING);
    assert(strcmp(projectObj->val->v.t_string, "thule-json") == 0);
    assert(projectObj->next != NULL);
    assert(projectObj->prev == NULL);

    projectObj = projectObj->next;
    assert(projectObj != NULL);
    assert(strcmp(projectObj->key, "languages") == 0);
    assert(projectObj->val->kind == JSON_VAL_ARRAY);
    assert(projectObj->next == NULL);
    assert(projectObj->prev != NULL);

    json_array* languages = projectObj->val->v.t_array;
    assert(languages->length == 2);
    assert(languages->items[0]->kind == JSON_VAL_STRING);
    assert(strcmp(languages->items[0]->v.t_string, "C") == 0);
    assert(languages->items[1]->kind == JSON_VAL_STRING);
    assert(strcmp(languages->items[1]->v.t_string, "Makefile") == 0);

    json_value_free(root);
}

void test_array_of_objects() {
    const char* src = "[{\"name\":\"thule-json\"},{\"name\":\"qr-asm\"},{\"name\":\"bootjack\"}]";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_ARRAY);

    json_array* arr = root->v.t_array;
    assert(arr->length == 3);

    char* names[] = {"thule-json", "qr-asm", "bootjack"};
    for (int i = 0; i < 3; i++) {
        json_object* project = arr->items[i]->v.t_object;
        assert(arr->items[i]->kind == JSON_VAL_OBJECT);
        assert(strcmp(project->key, "name") == 0);
        assert(project->val->kind == JSON_VAL_STRING);
        assert(strcmp(project->val->v.t_string, names[i]) == 0);
    }
    json_value_free(root);
}

void test_complex() {
    const char* src = "{\"profile\":{\"username\":\"barrettotte\"},\"projects\":[{\"name\":\"thule-json\",\"languages\":[\"C\",\"Makefile\"]},{\"name\":\"qr-asm\",\"languages\":[\"Assembly\"]}]}";
    json_value* root = json_parse(src);
    assert(root != NULL);
    assert(root->kind == JSON_VAL_OBJECT);

    json_object* rootObj = root->v.t_object;
    assert(rootObj != NULL);
    assert(strcmp(rootObj->key, "profile") == 0);
    assert(rootObj->val->kind == JSON_VAL_OBJECT);

    json_object* profile = rootObj->val->v.t_object;
    assert(profile != NULL);
    assert(strcmp(profile->key, "username") == 0);
    assert(strcmp(profile->val->v.t_string, "barrettotte") == 0);

    rootObj = rootObj->next;
    assert(rootObj != NULL);
    assert(strcmp(rootObj->key, "projects") == 0);
    assert(rootObj->val->kind == JSON_VAL_ARRAY);

    json_array* projects = rootObj->val->v.t_array;
    assert(projects->length == 2);

    // first project
    json_object* project = projects->items[0]->v.t_object;
    assert(projects->items[0]->kind == JSON_VAL_OBJECT);
    assert(project != NULL);
    assert(strcmp(project->key, "name") == 0);
    assert(project->val->kind == JSON_VAL_STRING);
    assert(strcmp(project->val->v.t_string, "thule-json") == 0);
    project = project->next;
    assert(strcmp(project->key, "languages") == 0);
    assert(project->val->kind == JSON_VAL_ARRAY);

    json_array* languages = project->val->v.t_array;
    assert(languages->length == 2);
    assert(languages->items[0]->kind == JSON_VAL_STRING);
    assert(strcmp(languages->items[0]->v.t_string, "C") == 0);
    assert(languages->items[1]->kind == JSON_VAL_STRING);
    assert(strcmp(languages->items[1]->v.t_string, "Makefile") == 0);

    // second project
    project = projects->items[1]->v.t_object;
    assert(projects->items[1]->kind == JSON_VAL_OBJECT);
    assert(project != NULL);
    assert(strcmp(project->key, "name") == 0);
    assert(project->val->kind == JSON_VAL_STRING);
    assert(strcmp(project->val->v.t_string, "qr-asm") == 0);
    project = project->next;
    assert(strcmp(project->key, "languages") == 0);
    assert(project->val->kind == JSON_VAL_ARRAY);

    languages = project->val->v.t_array;
    assert(languages->length == 1);
    assert(languages->items[0]->kind == JSON_VAL_STRING);
    assert(strcmp(languages->items[0]->v.t_string, "Assembly") == 0);

    json_value_free(root);
}

int main() {
    test_object_empty();
    test_object_basic_1();
    test_object_basic_2();
    test_object_basic_3();
    test_object_multilevel();

    test_array_empty();
    test_array_basic_1();
    test_array_basic_2();

    test_object_with_array();
    test_array_of_objects();
    test_complex();

    return 0;
}
