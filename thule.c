#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thule.h"

/***** private *****/

typedef enum {
    JSON_TOK_ERROR,
    JSON_TOK_COLON,
    JSON_TOK_COMMA,
    JSON_TOK_LBRACKET,
    JSON_TOK_RBRACKET,
    JSON_TOK_LSQBRACE,
    JSON_TOK_RSQBRACE,
    JSON_TOK_TRUE,
    JSON_TOK_FALSE,
    JSON_TOK_NULL,
    JSON_TOK_INT,
    JSON_TOK_DOUBLE,
    JSON_TOK_STRING,
    JSON_TOK_EOF
} json_token_kind;

typedef struct {
    json_token_kind kind;
    size_t length;
    const char* start;
} json_token;

typedef struct {
    const char* src;
    size_t scan_pos;
    size_t line;
    size_t token_start;
    json_token curr_token;
    json_token next_token;
    bool eof;
} json_parser;

static json_array* parse_array(json_parser* parser);
static json_object* parse_object(json_parser* parser);

// match string with current lexer position
static bool match(json_parser* parser, const char* s) {
    while (isalpha(parser->src[parser->scan_pos])) {
        parser->scan_pos++;
    }
    for (int i = 0; s[i]; i++) {
        if (s[i] != parser->src[parser->token_start + i]) {
            return false;
        }
    }
    return true;
}

// consume all whitespace characters
static void skip_ws(json_parser* parser) {
    while(1) {
        switch (parser->src[parser->scan_pos]) {
            case '\n': parser->line++;  // fallthrough
            case '\r':
            case '\t':
            case ' ':  parser->scan_pos++; break;
            default:   return;
        }
    }
}

// print token kind enum
void print_token_kind(json_token_kind kind) {
    switch(kind) {
        case JSON_TOK_ERROR:    printf("error"); break;
        case JSON_TOK_COLON:    printf(":"); break;
        case JSON_TOK_COMMA:    printf(","); break;
        case JSON_TOK_LBRACKET: printf("{"); break;
        case JSON_TOK_RBRACKET: printf("}"); break;
        case JSON_TOK_LSQBRACE: printf("["); break;
        case JSON_TOK_RSQBRACE: printf("]"); break;
        case JSON_TOK_TRUE:     printf("true"); break;
        case JSON_TOK_FALSE:    printf("false"); break;
        case JSON_TOK_NULL:     printf("null"); break;
        case JSON_TOK_INT:      printf("int"); break;
        case JSON_TOK_DOUBLE:   printf("double"); break;
        case JSON_TOK_STRING:   printf("string"); break;
        case JSON_TOK_EOF:      printf("eof"); break;
    }
}

// create new token of specified kind
static json_token new_token(json_parser* parser, json_token_kind kind) {
    json_token tok;
    tok.start = parser->src + parser->token_start;
    tok.length = parser->scan_pos - parser->token_start;
    tok.kind = kind;
    return tok;
}

// lex a numeric literal
static json_token lex_number(json_parser* parser) {
    json_val_kind kind = JSON_TOK_INT;

    while (isdigit(parser->src[parser->scan_pos])) {
        parser->scan_pos++;
    }
    if (parser->src[parser->scan_pos] == '.') {
        kind = JSON_TOK_DOUBLE;
        parser->scan_pos++;

        while (isdigit(parser->src[parser->scan_pos])) {
            parser->scan_pos++;
        }
    }
    return new_token(parser, kind);
}

// lex a string literal
static json_token lex_string(json_parser* parser) {
    char c = parser->src[parser->scan_pos];

    while (c != '\0' && c != '"') {
        parser->scan_pos++;
        c = parser->src[parser->scan_pos];
    }
    if (c == '"') {
        parser->scan_pos++;
    }
    return new_token(parser, JSON_TOK_STRING);
}

// lex next token from source
static json_token lex(json_parser* parser) {
    if (parser->src[parser->scan_pos] == '\0') {
        parser->eof = true;
        return new_token(parser, JSON_TOK_EOF);
    }
    skip_ws(parser);
    parser->token_start = parser->scan_pos;

    const char c = parser->src[parser->scan_pos++];
    switch (c) {
        case ':': return new_token(parser, JSON_TOK_COLON);
        case ',': return new_token(parser, JSON_TOK_COMMA);
        case '{': return new_token(parser, JSON_TOK_LBRACKET);
        case '}': return new_token(parser, JSON_TOK_RBRACKET);
        case '[': return new_token(parser, JSON_TOK_LSQBRACE);
        case ']': return new_token(parser, JSON_TOK_RSQBRACE);
        case 't': return new_token(parser, match(parser, "true")  ? JSON_TOK_TRUE  : JSON_TOK_ERROR);
        case 'f': return new_token(parser, match(parser, "false") ? JSON_TOK_FALSE : JSON_TOK_ERROR);
        case 'n': return new_token(parser, match(parser, "null")  ? JSON_TOK_NULL  : JSON_TOK_ERROR);
        case '"': return lex_string(parser);
    }
    if (isdigit(c)) {
        return lex_number(parser);
    }
    printf("bad char: %c\n", c);
    return new_token(parser, JSON_TOK_ERROR);
}

// consume current token and fetch the next
static void consume(json_parser* parser) {
    parser->curr_token = parser->next_token;
    parser->next_token = lex(parser);
}

// assert next token's type and consume it
static bool asserted_consume(json_parser* parser, json_token_kind kind) {
    if (parser->next_token.kind != kind) {
        printf("token kind assert failed: expected ");
        print_token_kind(kind);
        printf(", actual: ");
        print_token_kind(parser->next_token.kind);
        printf("\n");
        exit(1);
    }
    consume(parser);
    return true;
}

// initialize parser state
static void parser_init(json_parser* parser, const char* src) {
    parser->src = src;
    parser->scan_pos = 0;
    parser->line = 1;
    parser->next_token = lex(parser);
    parser->eof = false;
}

// parse numeric literal
static char* parse_number(json_parser* parser) {
    size_t len = parser->curr_token.length;
    char* i = malloc(sizeof(char) * len + 1);
    i[len] = '\0';
    memcpy(i, parser->curr_token.start, len);
    return i;
}

// parse integer literal
static long parse_integer(json_parser* parser) {
    char* literal = parse_number(parser);
    long val = strtol(literal, NULL, 10);
    free(literal);
    return val;
}

// parse double literal
static double parse_double(json_parser* parser) {
    char* literal = parse_number(parser);
    double val = strtod(literal, NULL);
    free(literal);
    return val;
}

// parse string literal
static char* parse_string(json_parser* parser) {
    size_t len = parser->curr_token.length - 2;
    char* s = malloc(sizeof(char) * len + 1);
    memcpy(s, parser->curr_token.start + 1, len);
    s[len] = '\0';
    return s;
}

// parse a JSON node
static json_value* parse_val(json_parser* parser) {
    json_value* node = malloc(sizeof(json_value));
    node->kind = JSON_VAL_UNINIT;
    consume(parser);

    switch(parser->curr_token.kind) {
        case JSON_TOK_LSQBRACE:
            node->kind = JSON_VAL_ARRAY;
            node->v.t_array = parse_array(parser);
            break;
        case JSON_TOK_LBRACKET:
            node->kind = JSON_VAL_OBJECT;
            node->v.t_object = parse_object(parser);
            break;
        case JSON_TOK_INT:
            node->kind = JSON_VAL_INT;
            node->v.t_int = parse_integer(parser);
            break;
        case JSON_TOK_DOUBLE:
            node->kind = JSON_VAL_DOUBLE;
            node->v.t_double = parse_double(parser);
            break;
        case JSON_TOK_STRING:
            node->kind = JSON_VAL_STRING;
            node->v.t_string = parse_string(parser);
            break;
        case JSON_TOK_TRUE:
            node->kind = JSON_VAL_BOOL;
            node->v.t_bool = true;
            break;
        case JSON_TOK_FALSE:
            node->kind = JSON_VAL_BOOL;
            node->v.t_bool = false;
            break;
        case JSON_TOK_NULL:
            node->kind = JSON_VAL_NULL;
            node->v.t_int = 0;
            break;
        default:
            node->kind = JSON_VAL_ERROR;
            node->v.t_int = JSON_ERR_BADTOKEN;
            break;
    }
    return node;
}

// parse JSON array
static json_array* parse_array(json_parser* parser) {
    json_array* arr = malloc(sizeof(json_array));
    arr->capacity = 8;
    arr->length = 0;
    arr->items = malloc(sizeof(json_value) * arr->capacity); // init

    // populate array
    while (!parser->eof && parser->next_token.kind != JSON_TOK_RSQBRACE) {
        // resize if needed
        if (arr->length >= arr->capacity) {
            arr->capacity *= 2;
            arr->items = realloc(arr->items, sizeof(json_value) * arr->capacity);
        }
        json_value* item = parse_val(parser);
        arr->items[arr->length++] = item;

        if (parser->next_token.kind == JSON_TOK_COMMA) {
            consume(parser);
        } else {
            break; // end of array items
        }
    }
    // shrink to only what was actually used
    if (arr->length == 0) {
        free(arr->items);
        arr->items = NULL;
    } else {
        arr->items = realloc(arr->items, sizeof(json_value) * arr->length);
    }
    asserted_consume(parser, JSON_TOK_RSQBRACE);
    return arr;
}

// allocate a new JSON object node
static json_object* new_object() {
    json_object* obj = malloc(sizeof(json_object));
    obj->key = NULL;
    obj->next = NULL;
    obj->prev = NULL;
    obj->val = NULL;
    return obj;
}

// parse JSON object
static json_object* parse_object(json_parser* parser) {
    json_object* head = new_object();
    json_object* prev = NULL;
    json_object* curr = head;

    while (!parser->eof && parser->next_token.kind != JSON_TOK_RBRACKET) {
        asserted_consume(parser, JSON_TOK_STRING); // key start
        curr->key = parse_string(parser);
        asserted_consume(parser, JSON_TOK_COLON);  // key end

        curr->val = parse_val(parser);
        curr->prev = prev;
        prev = curr;

        if (parser->next_token.kind == JSON_TOK_RBRACKET) {
            break;  // end of object
        }
        // next field
        asserted_consume(parser, JSON_TOK_COMMA);
        curr->next = new_object();
        curr = curr->next;
    }
    asserted_consume(parser, JSON_TOK_RBRACKET);
    return head;
}

/***** public *****/

// parse JSON source (entry point)
json_value* json_parse(const char* src) {
    json_parser parser;
    parser_init(&parser, src);
    return parse_val(&parser);
}

// deallocate JSON node's heap memory
void json_value_free(json_value* node) {
    if (node->kind == JSON_VAL_STRING) {
        free(node->v.t_string);
    } else if (node->kind == JSON_VAL_ARRAY) {
        json_array* arr = node->v.t_array;

        for (int i = 0; i < arr->length; i++) {
            json_value* item = arr->items[i];
            json_value_free(item);
        }
        if (arr->items != NULL) {
            free(arr->items);
        }
        free(arr);
    } else if (node->kind == JSON_VAL_OBJECT) {
        json_object* obj = node->v.t_object;
        json_object* prev = NULL;

        while (obj != NULL) {
            obj->prev = NULL;
            if (obj->key != NULL) {
                free(obj->key);
                obj->key = NULL;
            }
            if (obj->val != NULL) {
                json_value_free(obj->val);
                obj->val = NULL;
            }
            prev = obj;
            obj = obj->next;
            if (prev != NULL) {
                free(prev);
            }
        }
    }
    free(node);
}

// print JSON tree
void json_value_print(json_value* node) {
    switch (node->kind) {
        case JSON_VAL_OBJECT: printf("Object{}"); break;
        case JSON_VAL_ARRAY:  printf("Array[%zu]", node->v.t_array->length); break;
        case JSON_VAL_STRING: printf("\"%s\"", node->v.t_string); break;
        case JSON_VAL_INT:    printf("%ld", node->v.t_int); break;
        case JSON_VAL_DOUBLE: printf("%f", node->v.t_double); break;
        case JSON_VAL_BOOL:   printf("%s", node->v.t_bool ? "true" : "false"); break;
        case JSON_VAL_NULL:   printf("null"); break;
        default:              printf("unknown JSON value"); break;
    }
}
