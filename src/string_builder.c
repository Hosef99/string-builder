#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

    sb->buffer = NULL;
    sb->length = 0;
    sb->capacity = 0;
    
    return result;
}

char *sb_to_string_copy(StringBuilder *sb) {
    char *result = malloc(sb->length + 1);
    strncpy(result, sb->buffer, sb->length + 1);

    return result;
}

void sb_append_char(StringBuilder *sb, char ch) {
    if (!sb) return;

    if (sb->length + 2 >= sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity * GROWTH_RATE : 16;

        char *temp_buffer = realloc(sb->buffer, new_capacity);
        if (!temp_buffer) {
            return;
        }
        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    sb->buffer[sb->length++] = ch;
    sb->buffer[sb->length] = '\0';
}

void sb_append_cstr(StringBuilder *sb, const char *str) {
    if (!sb || !str) return;

    size_t string_length = strlen(str);
    if (string_length == 0) return;

    if (sb->length + string_length + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;
        while (sb->length + string_length + 1 > new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity); 
        if (!temp_buffer) return;

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buffer + sb->length, str, string_length);
    sb->length += string_length;
    sb->buffer[sb->length] = '\0';
}

void sb_append_cstr_len(StringBuilder *sb, const char *str, size_t len) {
    if (!sb || !str || len == 0) return;

    if (strlen(str) < len) return;

    if (sb->length + len + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;
        while (sb->length + len + 1 > new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity); 
        if (!temp_buffer) return;

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buffer + sb->length, str, len);
    sb->length += len;
    sb->buffer[sb->length] = '\0';
}

void sb_join(StringBuilder *sb_dest, StringBuilder *sb_src) {
    if (!sb_dest || !sb_src) return;
    if (sb_src->length == 0) return;

    if (sb_dest->length + sb_src->length + 1 >= sb_dest->capacity) {
        size_t new_capacity = sb_dest->capacity ? sb_dest->capacity : 16;

        while (sb_dest->length + sb_src->length + 1 >= new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb_dest->buffer, new_capacity); 
        if (!temp_buffer) return;

        sb_dest->buffer = temp_buffer;
        sb_dest->capacity = new_capacity;
    }

    memcpy(sb_dest->buffer + sb_dest->length, sb_src->buffer, sb_src->length);
    sb_dest->length += sb_src->length;
    sb_dest->buffer[sb_dest->length] = '\0';
}

void sb_append_format(StringBuilder *sb, const char *format, ...) {
    if (!sb) return;
    va_list args;

    va_start(args, format);
    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (needed < 0) return;

    char *buffer = malloc(needed + 1);

    va_start(args, format);
    vsnprintf(buffer, needed + 1, format, args);
    va_end(args);

    sb_append_cstr(sb, buffer);
    free(buffer);
}

void sb_append_cstr_escaped(StringBuilder *sb, const char *str) {
    if (!sb) return;

    size_t string_length = strlen(str);
    char *buffer = malloc(string_length);
    int pos = 0;
    while (*str) {
        if (*str == '\\') {
            str++;
            switch (*str) {
                case '\\':
                    buffer[pos++] = '\\';
                    str++;
                    break;
                case '\'':
                    buffer[pos++] = '\'';
                    str++;
                    break;
                case '\"':
                    buffer[pos++] = '\"';
                    str++;
                    break;
                case 'n': 
                    buffer[pos++] = '\n';
                    str++;
                    break;
                default: {
                    buffer[pos++] = '\\';
                    buffer[pos++] = *str++;
                    break;
                }
            }
            continue;
        }
        buffer[pos++] = *str++;
    }
    buffer[pos] = '\0';
    
    sb_append_cstr(sb, buffer);

    free(buffer);
}

void sb_append_cstr_escaped_len(StringBuilder *sb, const char *str, size_t len) {
    if (!sb) return;

    char *buffer = malloc(len);
    int pos = 0;
    while (*str && len--) {
        if (*str == '\\') {
            str++;
            switch (*str) {
                case '\\':
                    buffer[pos++] = '\\';
                    str++;
                    break;
                case '\'':
                    buffer[pos++] = '\'';
                    str++;
                    break;
                case '\"':
                    buffer[pos++] = '\"';
                    str++;
                    break;
                case 'n': 
                    buffer[pos++] = '\n';
                    str++;
                    break;
                default: {
                    buffer[pos++] = '\\';
                    buffer[pos++] = *str++;
                    len--;
                    break;
                }
            }
            continue;
        }
        buffer[pos++] = *str++;
    }
    buffer[pos] = '\0';
    
    sb_append_cstr(sb, buffer);

    free(buffer);
}

void sb_shrink_to_fit(StringBuilder *sb) {
    if (!sb) return;
    if (sb->length == sb->capacity) return;

    char *temp_buffer = realloc(sb->buffer, sb->length + 1);
    if (!temp_buffer) return;

    sb->buffer = temp_buffer;
}

void sb_clear(StringBuilder *sb) {
    if (!sb) return;
    free(sb->buffer);
    sb->buffer = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

size_t sb_length(const StringBuilder *sb) {
    if (!sb) return -1;
    return sb->length;
}

int sb_is_empty(const StringBuilder *sb) {
    if (!sb) return -1;
    return sb->length == 0;
}

const char *sb_peek(const StringBuilder *sb) {
    if (!sb) return NULL;
    return sb->buffer;
}

void sb_set_char(StringBuilder *sb, size_t index, char ch) {
    if (!sb) return;

    if (index >= sb->length) return;

    sb->buffer[index] = ch;
}

void sb_insert(StringBuilder *sb, size_t pos, const char *str) {
    if (!sb) return;
    
    if (pos > sb->length) return;

    size_t string_length = strlen(str);

    if (string_length == 0) return;

    size_t final_length = sb->length + string_length;

    if (final_length + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;

        while (final_length + 1 > new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity);
        if (!temp_buffer) return;

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memmove(sb->buffer + pos + string_length, sb->buffer + pos, string_length);
    
    memcpy(sb->buffer + pos, str, string_length);
    sb->length += string_length;
    sb->buffer[sb->length] = '\0';
}

void sb_delete(StringBuilder *sb, size_t pos, size_t len) {
    if (!sb) return;

    if (pos + len > sb->length) return;

    memmove(sb->buffer + pos, sb->buffer + pos + len, sb->length - pos - len);

    sb->length -= len;
    memset(sb->buffer + sb->length, 0, len);
    sb->buffer[sb->length] = '\0';
}

int sb_equal(StringBuilder *sb1, StringBuilder *sb2) {
    if (!sb1 || !sb2) return 0;

    if (sb1->length != sb2->length) return 0;

    return strcmp(sb1->buffer, sb2->buffer) == 0;
}
