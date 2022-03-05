# thule-json

Another toy/minimal json parser that isn't convenient to use at all and is probably full of bugs.

I managed to get basic JSON parsing working and ensured I had no memory leaks via valgrind.
For a toy JSON parser, that's good enough for me.

## Usage

```c
#include "thule.h"

```

### Running Tests

- `make; ./thule_test`

## References

- [Phil Eaton - Writing a Simple JSON Parser](https://notes.eatonphil.com/writing-a-simple-json-parser.html)
- [RFC-8259 JSON Spec](https://datatracker.ietf.org/doc/html/rfc8259)
