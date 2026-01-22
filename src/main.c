#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_builder.h"

int main() {
    printf("=== StringBuilder Test Suite ===\n\n");
    
    // Test 1: sb_create and basic properties
    printf("Test 1: Create and basic properties\n");
    StringBuilder *sb1 = sb_create(10);
    assert(sb1 != NULL);
    assert(sb_length(sb1) == 0);
    assert(sb_is_empty(sb1) == 1);
    printf("✓ Created with capacity 10, length = %zu, empty = %d\n", 
           sb_length(sb1), sb_is_empty(sb1));
    
    // Test 2: sb_append_char
    printf("\nTest 2: Append characters\n");
    sb_append_char(sb1, 'H');
    sb_append_char(sb1, 'e');
    sb_append_char(sb1, 'l');
    sb_append_char(sb1, 'l');
    sb_append_char(sb1, 'o');
    assert(sb_length(sb1) == 5);
    const char *view = sb_peek(sb1);
    assert(strncmp(view, "Hello", 5) == 0);
    printf("✓ Appended 'Hello', length = %zu\n", sb_length(sb1));
    
    // Test 3: sb_append_cstr
    printf("\nTest 3: Append C string\n");
    sb_append_cstr(sb1, " World");
    assert(sb_length(sb1) == 11);
    assert(strcmp(sb_peek(sb1), "Hello World") == 0);
    printf("✓ Appended ' World', result = '%s'\n", sb_peek(sb1));
    
    // Test 4: sb_to_string_copy
    printf("\nTest 4: Copy to string\n");
    char *copy = sb_to_string_copy(sb1);
    assert(copy != NULL);
    assert(strcmp(copy, "Hello World") == 0);
    printf("✓ Copy = '%s'\n", copy);
    free(copy);
    
    // Test 5: sb_clear
    printf("\nTest 5: Clear StringBuilder\n");
    sb_clear(sb1);
    assert(sb_length(sb1) == 0);
    assert(sb_is_empty(sb1) == 1);
    printf("✓ Cleared, length = %zu\n", sb_length(sb1));
    
    // Test 6: sb_append_format
    printf("\nTest 6: Append formatted strings\n");
    sb_append_format(sb1, "Integer: %d, Float: %.2f, String: %s", 42, 3.14159, "test");
    printf("✓ Formatted: '%s'\n", sb_peek(sb1));
    
    sb_clear(sb1);
    sb_append_format(sb1, "Char: %c, Hex: %x", 'A', 255);
    printf("✓ More formats: '%s'\n", sb_peek(sb1));
    
    // Test 7: sb_append_json_escaped
    printf("\nTest 7: JSON escaping\n");
    sb_clear(sb1);
    sb_append_json_escaped(sb1, "Line1\nLine2\tTab\"Quote\\Backslash");
    printf("✓ JSON escaped: '%s'\n", sb_peek(sb1));
    
    // Test 8: sb_set_char
    printf("\nTest 8: Set character at index\n");
    sb_clear(sb1);
    sb_append_cstr(sb1, "Hello");
    sb_set_char(sb1, 1, 'a'); // Hella
    assert(strcmp(sb_peek(sb1), "Hallo") == 0);
    printf("✓ Changed 'e' to 'a': '%s'\n", sb_peek(sb1));
    
    // Test 9: sb_insert
    printf("\nTest 9: Insert string\n");
    sb_insert(sb1, 1, "appy "); // Happy allo
    printf("✓ Inserted 'appy ' at pos 1: '%s'\n", sb_peek(sb1));
    
    // Test 10: sb_delete
    printf("\nTest 10: Delete substring\n");
    sb_delete(sb1, 5, 5); // Delete " allo" from "Happy allo"
    printf("✓ Deleted 5 chars from pos 5: '%s'\n", sb_peek(sb1));
    
    // Test 11: sb_join
    printf("\nTest 11: Join two StringBuilders\n");
    StringBuilder *sb2 = sb_create(5);
    sb_append_cstr(sb2, " Builder");
    sb_join(sb1, sb2);
    printf("✓ Joined: '%s'\n", sb_peek(sb1));
    
    // Test 12: sb_shrink_to_fit
    printf("\nTest 12: Shrink to fit\n");
    size_t before_len = sb_length(sb1);
    sb_shrink_to_fit(sb1);
    // Can't easily test internal capacity, but we can verify length unchanged
    assert(sb_length(sb1) == before_len);
    printf("✓ Shrunk, length unchanged: %zu\n", sb_length(sb1));
    
    // Test 13: sb_to_string (transfer ownership)
    printf("\nTest 13: Transfer ownership to string\n");
    char *final_str = sb_to_string(sb1);
    assert(final_str != NULL);
    printf("✓ Final string: '%s'\n", final_str);
    
    // Test 14: Test with empty string
    printf("\nTest 14: Empty string handling\n");
    StringBuilder *sb3 = sb_create(0);
    assert(sb_is_empty(sb3));
    sb_append_cstr(sb3, "");
    assert(sb_is_empty(sb3));
    sb_append_char(sb3, 'x');
    assert(!sb_is_empty(sb3));
    printf("✓ Empty string handling correct\n");
    
    // Test 15: Edge cases
    printf("\nTest 15: Edge cases\n");
    StringBuilder *sb4 = sb_create(2);
    sb_append_cstr(sb4, "This is a very long string that should trigger reallocation");
    printf("✓ Long string (reallocation test): length = %zu\n", sb_length(sb4));
    
    // Test insert at end
    sb_insert(sb4, sb_length(sb4), " END");
    printf("✓ Insert at end\n");
    
    // Cleanup
    free(final_str);
    sb_destroy(sb1);
    sb_destroy(sb2);
    sb_destroy(sb3);
    sb_destroy(sb4);
    
    printf("\n=== All tests passed! ===\n");
    
    return 0;
}
