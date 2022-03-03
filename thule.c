#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "thule.h"

/***** private *****/

typedef enum {
    TOK_ERROR,
    TOK_COLON,
    TOK_COMMA,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_LSQBRACE,
    TOK_RSQBRACE,
    TOK_TRUE,
    TOK_FALSE,
    TOK_NULL,
    TOK_INT,
    TOK_DOUBLE,
    TOK_STRING,
    TOK_EOF
} json_token_kind;

typedef struct {
    json_token_kind kind;
    size_t length;
    const char* start;
} json_token;

typedef struct {
    const char* src;
    size_t pos;
    size_t line;
    size_t tok_start;
    bool eof;
} json_lexer;

typedef struct {
    const char* src;
    json_lexer* lexer;
    json_token* current;
    json_token* next;
} json_parser;

// initialize Json lexer
static void lexer_init(json_lexer *lexer, const char *src) {
    lexer->src = (char*) src;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->eof = false;
}

// create new token of specified kind
static json_token* new_token(json_lexer* lexer, json_token_kind kind) {
    json_token* tok = malloc(sizeof(json_token));
    tok->start = lexer->src + lexer->tok_start;
    tok->length = lexer->pos - lexer->tok_start;
    tok->kind = kind;
    return tok;
}

// match string with current lexer position
static bool match(json_lexer* lexer, const char* s) {
    while (isalpha(lexer->src[lexer->pos])) {
        lexer->pos++;
    }
    for (int i = 0; s[i]; i++) {
        if (s[i] != lexer->src[lexer->tok_start + i]) {
            return false;
        }
    }
    return true;
}

// consume all whitespace characters
static void skip_ws(json_lexer* lexer) {
    while(1) {
        switch (lexer->src[lexer->pos]) {
            case '\n':  lexer->line++;  // fallthrough
            case ' ':
            case '\r':
            case '\t':  lexer->pos++;  break;
            default:    return;
        }
    }
}

// lex next token, advancing through source
static json_token* lex(json_lexer* lexer) {
    if (lexer->src[lexer->pos] == '\0') {
        lexer->eof = true;
        return new_token(lexer, TOK_EOF);
    }
    skip_ws(lexer);
    lexer->tok_start = lexer->pos;

    const char c = lexer->src[lexer->pos++];
    switch (c) {
        case ':':  return new_token(lexer, TOK_COLON);
        case ',':  return new_token(lexer, TOK_COMMA);
        case '{':  return new_token(lexer, TOK_LBRACKET);
        case '}':  return new_token(lexer, TOK_RBRACKET);
        case '[':  return new_token(lexer, TOK_LSQBRACE);
        case ']':  return new_token(lexer, TOK_RSQBRACE);
        case 't':  return new_token(lexer, match(lexer, "true")  ? TOK_TRUE  : TOK_ERROR);
        case 'f':  return new_token(lexer, match(lexer, "false") ? TOK_FALSE : TOK_ERROR);
        case 'n':  return new_token(lexer, match(lexer, "null")  ? TOK_NULL  : TOK_ERROR);
    }
    // TODO: numbers / string
    if (c == '"') {

    }

    return new_token(lexer, TOK_ERROR);
}

// initialize parser's variables
static void parser_init(json_parser* parser, const char* src) {
    parser->src = src;
    lexer_init(parser->lexer, src);
    parser->next = lex(parser->lexer);
}

// deallocate parser's memory
static void parser_free(json_parser* parser) {
    // TODO:  whatever else
    free(parser);
}

// parse a JSON value
static void parse_val(json_parser* parser, json_val* node) {
    
}

/***** public *****/

// parse json string (entry point)
void json_parse(const char* src, json_val* root) {
    json_parser* parser = malloc(sizeof(parser));
    
    parser_init(parser, src);
    parse_val(parser, root);
    
    parser_free(parser);
}
