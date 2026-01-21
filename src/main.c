#include <stdio.h>
#include <stdlib.h>

#include "string_builder.h"

int main() {
    StringBuilder *sb = sb_create(16);
    sb_append_cstr(sb, "Testing");
    sb_append_cstr(sb, " a short message");
    StringBuilder *sb2 = sb_create(16);
    sb_append_cstr(sb2, " sb2's message is here");
    sb_join(sb, sb2);
    printf("sb_string: %s\n", sb_peek(sb));
    sb_destroy(sb);
}
