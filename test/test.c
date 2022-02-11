#include <stdlib.h>
#include "../thule.h"

int main() {
    char* src = "{ \"hello\": \"world\" }\0";
    json_obj_t* root = malloc(sizeof(json_obj_t));

    uint8_t result = json_parse(src, root);
    free(root);

    return result;
}
