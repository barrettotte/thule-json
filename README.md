# thule-json

Another toy/minimal json parser that isn't convenient to use at all and is probably full of bugs.

I managed to get basic JSON parsing working and ensured I had no memory leaks via valgrind.
For a toy JSON parser, that's good enough for me.

## Usage

```c
// trivial example
#include "thule.h"

int main() {
    const char* src = "{\"username\": \"barrettotte\"}";
    json_value* root = json_parse(src);
    json_object* obj = root->v.t_object;

    printf("username => %s\n", obj->val->v.t_string);
    // username => barrettotte

    json_value_free(root);

    return 0;
}
```

```C
// slightly more complex example
#include "thule.h"

int main() {
    const char* src = "{\"profile\":{\"username\":\"barrettotte\"},\"projects\":[{\"name\":\"thule-json\",\"languages\":[\"C\",\"Makefile\"]},{\"name\":\"qr-asm\",\"languages\":[\"Assembly\"]}]}";
    json_value* root = json_parse(src);
    json_object* projects = root->v.t_object->next;                             // projects[]
    json_object* thule = projects->val->v.t_array->items[0]->v.t_object->next;  // projects[0]

    printf("projects[0].languages[0] => %s\n", thule->val->v.t_array->items[0]->v.t_string);
    // projects[0].languages[0] => C

    json_value_free(root);
    
    return 0;
}
```

TODO: more complex example (multi-level and type)

### Running Tests

- normal - `make`
- with memory leak check - `make valgrind`

## References

- [Phil Eaton - Writing a Simple JSON Parser](https://notes.eatonphil.com/writing-a-simple-json-parser.html)
- [RFC-8259 JSON Spec](https://datatracker.ietf.org/doc/html/rfc8259)
