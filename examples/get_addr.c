#include <string.h>

#include "../src/flajok.h"

/* e.g.
 * gcc -o getaddr get_addr.c
 * ./getaddr port=4040 --ip=192.12.55.10
 */

int
main(int argc, char **argv) {

    if (argc < 2) {
        return -1;
    }

    int port;
    char *ip;
    size_t i;

    flag_t keys[] = {
            (flag_t) {
                .name="ip",
                .type=STR_FLAG,
            },
            (flag_t) {
                .name="port",
                .type=INT_FLAG,
            },
    };
    size_t n = 2;
    flag_set_t flag_set = flajok(argc, argv, keys, n);

    if (flag_set.len == -1) {
        return -1;
    }

    for (i = 0; i < flag_set.len; i++) {
        if (strcmp(flag_set.flags[i].name, "port") == 0) {
            port = *(int *) flag_set.flags[i].value;
        } else {
            ip = flag_set.flags[i].value;
        }
    }

    printf("address: %s:%d\n", ip, port);
}