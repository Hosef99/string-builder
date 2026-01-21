#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_builder.h"

#define GROWTH_RATE 2

StringBuilder *sb_create(size_t initial_capacity) {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    sb->buffer = malloc(initial_capacity);
    sb->length = 0;
    sb->capacity = initial_capacity;
    return sb;
}

void sb_destroy(StringBuilder *sb) {
    if (!sb) return;

    free(sb->buffer);
    free(sb);
}

char *sb_to_string(StringBuilder *sb) {
    char *result = realloc(sb->buffer, sb->length + 1);

    result[sb->length] = '\0';

    sb->buffer = NULL;
    sb->length = 0;
    sb->capacity = 0;
    
    return result;
}

char *sb_to_string_copy(StringBuilder *sb) {
    char *result = malloc(sb->length + 1);
    strncpy(result, sb->buffer, sb->length);
    result[sb->length] = '\0';

    return result;
}

void sb_append_char(StringBuilder *sb, char ch) {
    if (!sb) return;

    if (sb->length + 1 >= sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity * GROWTH_RATE : 16;

        char *temp_buffer = realloc(sb->buffer, new_capacity);
        if (!temp_buffer) {
            return;
        }
        printf("Allocated with size %ld\n", new_capacity);
        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    sb->buffer[sb->length++] = ch;
}

void sb_append_cstr(StringBuilder *sb, const char *str) {
    if (!sb || !str) return;

    size_t string_length = strlen(str);
    if (string_length == 0) return;

    if (sb->length + string_length >= sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;
        while (sb->length + string_length >= new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity); 
        if (!temp_buffer) return;

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buffer + sb->length, str, string_length);
    sb->length += string_length;
}

void sb_join(StringBuilder *sb_dest, StringBuilder *sb_src) {
    if (!sb_dest || !sb_src) return;
    if (sb_src->length == 0) return;

    if (sb_dest->length + sb_src->length >= sb_dest->capacity) {
        size_t new_capacity = sb_dest->capacity ? sb_dest->capacity : 16;

        while (sb_dest->length + sb_src->length >= new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb_dest->buffer, new_capacity); 
        if (!temp_buffer) return;

        sb_dest->buffer = temp_buffer;
        sb_dest->capacity = new_capacity;
    }

    memcpy(sb_dest->buffer + sb_dest->length, sb_src->buffer, sb_src->length);
    sb_dest->length += sb_src->length;
}

const char *sb_peek(const StringBuilder *sb) {
    return sb->buffer;
}
