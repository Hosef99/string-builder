#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

typedef struct {
    char *buffer;
    size_t length;
    size_t capacity;
} StringBuilder;

StringBuilder *sb_create(size_t initial_capacity);
void sb_destroy(StringBuilder *sb);
char *sb_to_string(StringBuilder *sb);  // Returns string (transfers ownership)
char *sb_to_string_copy(StringBuilder *sb);  // Returns copy

void sb_append_char(StringBuilder *sb, char ch);
void sb_append_cstr(StringBuilder *sb, const char *str);
void sb_join(StringBuilder *sb_dest, StringBuilder *sb_src);

void sb_append_format(StringBuilder *sb, const char *format, ...); // do it like printf, please support %d, %f, %s, %c, %x
void sb_append_json_escaped(StringBuilder *sb, const char *str);  // For JSON output

void sb_shrink_to_fit(StringBuilder *sb);
void sb_clear(StringBuilder *sb);  // Reset length to 0

size_t sb_length(const StringBuilder *sb); 
int sb_is_empty(const StringBuilder *sb);
const char *sb_peek(const StringBuilder *sb);  // Read-only view

void sb_set_char(StringBuilder *sb, size_t index, char ch);
void sb_insert(StringBuilder *sb, size_t pos, const char *str);
void sb_delete(StringBuilder *sb, size_t pos, size_t len);

void sb_cmp(StringBuilder *sb, const char *str);

#endif
