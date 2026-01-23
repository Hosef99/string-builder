#include <stdio.h>

#include "string_builder.h"

int main() {
    StringBuilder *sb = sb_create(16);
    char *test = "012345678\\n9012345";
    sb_append_json_escaped_len(sb, test + 6, 6);
    printf("test sb_string: %s\n", sb_peek(sb));
    sb_destroy(sb);
    return 0;
}
