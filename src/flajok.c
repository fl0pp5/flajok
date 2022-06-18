#include <ctype.h>
#include <string.h>

#include "flajok.h"

void
set_flag_value(flag_set_t *flag_set, raw_flag_t source, size_t index) {
    switch (flag_set->flags[index].type) {
        case INT_FLAG:
        case BOOL_FLAG:
            *(int *) flag_set->flags[index].value = strtol(source.value, NULL, 10);
            break;
        case STR_FLAG:
            strcpy(flag_set->flags[index].value, source.value);
            break;
        default:
            break;
    }
}

size_t
parse_one(flag_set_t *flag_set, const char *word, size_t start) {
    raw_flag_t raw_flag = {};
    size_t     word_len = strlen(word);
    size_t     i_word   = start;
    size_t     flag_id;
    size_t     i;
    char       ch;

    /* get minuses */
    if (word[i_word] == '-') {
        if (word[i_word + 1] == ' ' || isdigit(word[i_word + 1])) {

            fputs("invalid format", stderr);
            return -1;

        } else if (word[i_word + 1] == '-') {
            i_word++;
        }
        i_word++;
    }

    /* get name */
    for (i = 0; i_word < word_len; i_word++, i++) {
        ch = word[i_word];

        if (ch == ' ' || ch == '=') {
            i_word++;
            break;
        }

        raw_flag.name[i] = ch;
    }

    flag_id = get_flag_id(flag_set, raw_flag.name);

    /* check flag exists */
    if (flag_id == -1) {
        fprintf(stderr,"flag `%s` does not exists\n", raw_flag.name);
        return -1;
    }

    /* check if type is bool */
    if (flag_set->flags[flag_id].type == BOOL_FLAG) {
        return i_word;
    }

    /* else. get value */
    for (i = 0; i_word < word_len; i_word++, i++) {
        ch = word[i_word];

        if (isspace(ch) || ch == '\0') {
            raw_flag.value[i] = '\0';
            i_word++;
            break;
        }

        if (ch == '-' || !isalpha(ch) && !isdigit(ch) && !ispunct(ch)) {
            fprintf(stderr,"invalid symbol : `%c` \n", ch);
            return -1;
        }

        raw_flag.value[i] = ch;
    }

    set_flag_value(flag_set, raw_flag, flag_id);

    return i_word;
}

flag_set_t
make_flag_set() {
    return (flag_set_t) {
            .len=0,
            .cap=FLAG_SET_LEN,
    };
}

size_t
get_flag_id(flag_set_t *flag_set, const char *name) {
    size_t i;

    for (i = 0; i < flag_set->len; i++) {
        if (strcmp(flag_set->flags[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int
add_flag(flag_set_t *flag_set, flag_t flag) {
    if (get_flag_id(flag_set, flag.name) != -1) {
        return -1;
    }

    flag_set->flags[flag_set->len] = flag;

    flag_set->len++;

    return 1;
}

void
extract_word(int argc, char **argv, char word[]) {
    size_t i;

    for (i = 1; i < argc; i++) {
        strcat(word, argv[i]);
        strcat(word, " ");
    }

}

flag_set_t
flajok(int argc, char **argv, flag_t keys[], size_t keys_len) {
    char       src[ARGV_BUF_LEN] = {};
    flag_set_t flag_set          = make_flag_set();
    size_t     step              = 0;
    size_t     src_len;
    size_t     i;

    extract_word(argc, argv, src);
    src_len = strlen(src);

    for (i = 0; i < keys_len; i++) {
        if (add_flag(&flag_set, keys[i]) == -1) {
            return (flag_set_t) {
                    .len=-1,
            };
        }
    }

    while (step < src_len) {
        step = parse_one(&flag_set, src, step);

        if (step == -1) {
            return (flag_set_t) {
                    .len=-1,
            };
        }
    }

    return flag_set;
}
