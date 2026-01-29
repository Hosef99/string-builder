#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "error.h"
#include "string_builder.h"

#define GROWTH_RATE 2

StringBuilder *sb_create(size_t initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 16;
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    sb->buffer = malloc(initial_capacity);
    sb->length = 0;
    sb->capacity = initial_capacity;
    return sb;
}

void sb_destroy(StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");

    free(sb->buffer);
    free(sb);
}

char *sb_to_string(StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    char *result = realloc(sb->buffer, sb->length + 1);
    
    if (!result) HANDLE_ERROR("Memory allocation failed");

    sb->buffer = NULL;
    sb->length = 0;
    sb->capacity = 0;
    
    return result;
}

char *sb_to_string_copy(StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    char *result = malloc(sb->length + 1);

    if (!result) HANDLE_ERROR("Memory allocation failed");

    strncpy(result, sb->buffer, sb->length + 1);

    return result;
}

void sb_append_char(StringBuilder *sb, char ch) {
    if (!sb) HANDLE_ERROR("sb is NULL");

    if (sb->length + 2 >= sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity * GROWTH_RATE : 16;

        char *temp_buffer = realloc(sb->buffer, new_capacity);
        if (!temp_buffer) HANDLE_ERROR("Memory allocation failed");
        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    sb->buffer[sb->length++] = ch;
    sb->buffer[sb->length] = '\0';
}

void sb_append_cstr(StringBuilder *sb, const char *str) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    if (!str) HANDLE_ERROR("str is NULL");

    size_t string_length = strlen(str);
    if (string_length == 0) return;

    if (sb->length + string_length + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;
        while (sb->length + string_length + 1 > new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity); 
        if (!temp_buffer) HANDLE_ERROR("Memory allocation failed");

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buffer + sb->length, str, string_length);
    sb->length += string_length;
    sb->buffer[sb->length] = '\0';
}

void sb_append_cstr_len(StringBuilder *sb, const char *str, size_t len) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    if (!str) HANDLE_ERROR("str is NULL");
    if (strlen(str) < len) HANDLE_ERROR("len is out of bounds");

    if (len == 0) return;


    if (sb->length + len + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity ? sb->capacity : 16;
        while (sb->length + len + 1 > new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb->buffer, new_capacity); 
        if (!temp_buffer) HANDLE_ERROR("Memory allocation failed");

        sb->buffer = temp_buffer;
        sb->capacity = new_capacity;
    }

    memcpy(sb->buffer + sb->length, str, len);
    sb->length += len;
    sb->buffer[sb->length] = '\0';
}

void sb_join(StringBuilder *sb_dest, StringBuilder *sb_src) {
    if (!sb_dest) HANDLE_ERROR("sb_dest is NULL");
    if (!sb_src) HANDLE_ERROR("sb_src is NULL");
    if (sb_src->length == 0) return;

    if (sb_dest->length + sb_src->length + 1 >= sb_dest->capacity) {
        size_t new_capacity = sb_dest->capacity ? sb_dest->capacity : 16;

        while (sb_dest->length + sb_src->length + 1 >= new_capacity) {
            new_capacity *= GROWTH_RATE;
        }

        char *temp_buffer = realloc(sb_dest->buffer, new_capacity); 
        if (!temp_buffer) HANDLE_ERROR("Memory allocation failed");

        sb_dest->buffer = temp_buffer;
        sb_dest->capacity = new_capacity;
    }

    memcpy(sb_dest->buffer + sb_dest->length, sb_src->buffer, sb_src->length);
    sb_dest->length += sb_src->length;
    sb_dest->buffer[sb_dest->length] = '\0';
}

void sb_append_format(StringBuilder *sb, const char *format, ...) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    va_list args;

    // ERROR: string format errors can exist here

    va_start(args, format);
    int needed = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (needed < 0) return;

    char *buffer = malloc(needed + 1);

    if (!buffer) HANDLE_ERROR("Memory allocation failed");

    va_start(args, format);
    vsnprintf(buffer, needed + 1, format, args);
    va_end(args);

    sb_append_cstr(sb, buffer);
    free(buffer);
}

void sb_append_cstr_escaped(StringBuilder *sb, const char *str) {
    if (!sb) HANDLE_ERROR("sb is NULL");

    size_t string_length = strlen(str);
    char *buffer = malloc(string_length + 1);
    if (!buffer) HANDLE_ERROR("Memory allocation failed");
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
    if (!sb) HANDLE_ERROR("sb is NULL");

    char *buffer = malloc(len + 1);
    if (!buffer) HANDLE_ERROR("Memory allocation failed");
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
                    break;
                }
            }
            len--;
            continue;
        }
        buffer[pos++] = *str++;
    }
    buffer[pos] = '\0';
    
    sb_append_cstr(sb, buffer);

    free(buffer);
}

void sb_shrink_to_fit(StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    if (sb->length == sb->capacity) return;

    char *temp_buffer = realloc(sb->buffer, sb->length + 1);
    if (!temp_buffer) return;

    sb->buffer = temp_buffer;
}

void sb_clear(StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    free(sb->buffer);
    sb->buffer = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

size_t sb_length(const StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    return sb->length;
}

int sb_is_empty(const StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    return sb->length == 0;
}

const char *sb_peek(const StringBuilder *sb) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    return sb->buffer;
}

void sb_set_char(StringBuilder *sb, size_t index, char ch) {
    if (!sb) HANDLE_ERROR("sb is NULL");

    if (index >= sb->length) HANDLE_ERROR("index is out of bounds");

    sb->buffer[index] = ch;
}

void sb_insert(StringBuilder *sb, size_t pos, const char *str) {
    if (!sb) HANDLE_ERROR("sb is NULL");
    
    if (pos > sb->length) HANDLE_ERROR("pos is out of bounds");

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
    if (!sb) HANDLE_ERROR("sb is NULL"); // ERROR

    if (pos + len > sb->length) HANDLE_ERROR("deleted section is out of bounds");

    memmove(sb->buffer + pos, sb->buffer + pos + len, sb->length - pos - len);

    sb->length -= len;
    memset(sb->buffer + sb->length, 0, len);
    sb->buffer[sb->length] = '\0';
}

int sb_equal(StringBuilder *sb1, StringBuilder *sb2) {
    if (!sb1) HANDLE_ERROR("sb1 is NULL");
    if (!sb2) HANDLE_ERROR("sb2 is NULL");

    if (sb1->length != sb2->length) return 0;

    return strcmp(sb1->buffer, sb2->buffer) == 0;
}
