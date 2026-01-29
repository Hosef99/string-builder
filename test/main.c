#include <stdio.h>

#include "string_builder.h"

int main() {
    StringBuilder *sb = sb_create(16);
    char *test = "abcdefghijklmnop";
    sb_append_cstr_len(NULL, test + 6, 6);
    printf("test sb_string: %s\n", sb_peek(sb));
    sb_destroy(sb);
    return 0;
}
