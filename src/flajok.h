#ifndef FLAJOK_FLAJOK_H
#define FLAJOK_FLAJOK_H
#include <stdlib.h>
#include <stdio.h>

#define NAME_BUF_LEN  64
#define VALUE_BUF_LEN 128
#define ARGV_BUF_LEN 1024

#define FLAG_SET_LEN  32

enum FlagType {
    INT_FLAG,
    STR_FLAG,
    BOOL_FLAG,
};

typedef struct {
    enum FlagType type;
    const char   *name;
    const char   *usage;
    const char   *default_value;
    char          value[VALUE_BUF_LEN];
} flag_t;


typedef struct {
    char name[NAME_BUF_LEN];
    char value[VALUE_BUF_LEN];
} raw_flag_t;


typedef struct {
    flag_t flags[FLAG_SET_LEN];
    size_t len;
    size_t cap;
} flag_set_t;

ssize_t
parse_one(flag_set_t *flag_set, const char *word, ssize_t start);

flag_set_t
make_flag_set();

int
add_flag(flag_set_t *flag_set, flag_t flag);

ssize_t
get_flag_id(flag_set_t *flag_set, const char *name);

flag_set_t
flajok(int argc, char **argv, flag_t keys[], size_t keys_len);

#endif //FLAJOK_FLAJOK_H
