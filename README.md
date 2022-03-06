# thule-json

Another minimal and unsafe json parser.

I managed to get general JSON parsing working and ensured I had no memory leaks/issues via valgrind.
For a toy JSON parser this is good enough for me. 

This is unsafe because I'm missing error handling/reporting.

## Usage

```c
// trivial example

#include "thule.h"

int main() {
    const char* src = "{\"username\": \"barrettotte\"}";
    json_value* root = json_parse(src);
    json_object* obj = root->v_object;

    printf("username => %s\n", obj->val->v_string);
    // username => barrettotte

    json_value_free(root);
    return 0;
}
```

```c
// slightly more complex example

#include "thule.h"

int main() {
    const char* src = "{\"profile\":{\"username\":\"barrettotte\"},\"projects\":[{\"name\":\"thule-json\",\"languages\":[\"C\",\"Makefile\"]},{\"name\":\"qr-asm\",\"languages\":[\"Assembly\"]}]}";
    json_value* root = json_parse(src);
    json_object* projects = root->v_object->next;                           // projects[]
    json_object* thule = projects->val->v_array->items[0]->v_object->next;  // projects[0]

    printf("projects[0].languages[0] => %s\n", thule->val->v_array->items[0]->v_string);
    // projects[0].languages[0] => C

    json_value_free(root);
    return 0;
}
```

### Running Tests

- normal - `make`
- with memory leak check - `make valgrind`

## References

- [Phil Eaton - Writing a Simple JSON Parser](https://notes.eatonphil.com/writing-a-simple-json-parser.html)
- [RFC-8259 JSON Spec](https://datatracker.ietf.org/doc/html/rfc8259)
