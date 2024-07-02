/**
 * @file my_string.c
 * @author YOUR NAME HERE
 * @brief Your implementation of the famous string.h library functions!
 *
 * NOTE: NO ARRAY NOTATION IS ALLOWED IN THIS FILE
 *
 * @date 2024-03-xx
 */

#include "my_string.h"

/* Note about UNUSED_PARAM
*
* UNUSED_PARAM is used to avoid compiler warnings and errors regarding unused function
* parameters prior to implementing the function. Once you begin implementing this
* function, you can delete the UNUSED_PARAM lines.
*/

/**
 * @brief Calculate the length of a string
 *
 * @param s a constant C string
 * @return size_t the number of characters in the passed in string
 */
size_t my_strlen(const char *s)
{
    // UNUSED_PARAM(s);

    size_t i = 0;
    while(*((s) + (i)) != '\0') {
        ++i;
    }
    return i;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string to be compared
 * @param s2 Second string to be compared
 * @param n First (at most) n bytes to be compared
 * @return int representing the difference between the strings:
 *          - 0 if the strings are equal
 *          - arbitrary positive number if s1 > s2
 *          - arbitrary negative number if s1 < s2
 */
int my_strncmp(const char *s1, const char *s2, size_t n)
{
    UNUSED_PARAM(s1);
    UNUSED_PARAM(s2);
    UNUSED_PARAM(n);

    while (n--) {
        if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0') {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
    }
    return 0;


    // size_t start = 0;
    // size_t end = 0;

    // size_t i = 0;
    // while(*((s1) + (i)) != '\0' && i < n) {
    //     ++i;
    // }
    // return 0;
}

/**
 * @brief Copy a string
 *
 * @param dest The destination buffer
 * @param src The source to copy from
 * @param n maximum number of bytes to copy
 * @return char* a pointer same as dest
 */
char *my_strncpy(char *dest, const char *src, size_t n)
{
    // UNUSED_PARAM(dest);
    // UNUSED_PARAM(src);

    // UNUSED_PARAM(n);

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        if (*(src + i) != '\0') {
            *(dest + i) = *(src + i);
        } else if (*(src + i) == '\0') {
            *(dest + i) = '\0';
            break;
        }
    }

    for (size_t j = i; j < n; ++j) {
        *(dest + j) = '\0';
    }

    return dest;
}

/**
 * @brief Concatenates two strings and stores the result
 * in the destination string
 *
 * @param dest The destination string
 * @param src The source string
 * @param n The maximum number of bytes (or characters) from src to 
 * concatenate
 * @return char* a pointer same as dest
 */
char *my_strncat(char *dest, const char *src, size_t n)
{
    // UNUSED_PARAM(dest);
    // UNUSED_PARAM(src);
    // UNUSED_PARAM(n);

    size_t destIdx = 0;
    while (*(dest + destIdx) != '\0') {
        ++destIdx;
    }

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        *(dest + destIdx + i) = *(src + i);
    }

    return dest;
}

/**
 * @brief Copies the character c into the first n
 * bytes of memory starting at *str
 *
 * @param str The pointer to the block of memory to fill
 * @param c The character to fill in memory
 * @param n The number of bytes of memory to fill
 * @return char* a pointer same as str
 */
void *my_memset(void *str, int c, size_t n)
{
    // UNUSED_PARAM(str);
    // UNUSED_PARAM(c);
    // UNUSED_PARAM(n);

    unsigned char *ptr = (unsigned char *)str; 
    unsigned char byteValue = (unsigned char)c; 

    for (size_t i = 0; i < n; ++i) {
        *(ptr + i) = byteValue; 
    }

    return str;
}

/**
 * @brief Checks whether the string is a palindrome
 * (i.e., reads the same forwards and backwards)
 * assuming that the case of letters is irrelevant.
 * 
 * @param str The pointer to the string
 * @return 1 if the string is a palindrome,
 * or 0 if the string is not
*/
int is_palindrome_ignore_case(const char *str) 
{
    // UNUSED_PARAM(str);

    size_t length = 0;
    while (*(str + length) != '\0') {
        ++length;
    }

    if (length == 0) {
        return 1;
    }

    size_t start = 0;
    size_t end = length - 1;

    while (start < end) {
        char begChar = *(str + start);
        char endChar = *(str + end);

        if (begChar >= 'A' && begChar <= 'Z') {
            begChar += 'a' - 'A';
        }
        if (endChar >= 'A' && endChar <= 'Z') {
            endChar += 'a' - 'A';
        }

        if (begChar != endChar) {
            return 0;
        }

        ++start;
        --end;
    }

    return 1;
}

/**
 * @brief Apply a Caesar shift to each character
 * of the provided string in place.
 * 
 * @param str The pointer to the string
 * @param shift The amount to shift by
*/
void caesar_shift(char *str, int shift) 
{
    // UNUSED_PARAM(str);
    // UNUSED_PARAM(shift);

    while (*str != '\0') {
        if (*str >= 'A' && *str <= 'Z') {
            *str = 'A' + (*str - 'A' + shift) % 26;
        } else if (*str >= 'a' && *str <= 'z') {
            *str = 'a' + (*str - 'a' + shift) % 26;
        }
        ++str;
    }

    return;
}

/**
 * @brief Mutate the string in-place to
 * remove duplicate characters that appear
 * consecutively in the string.
 * 
 * @param str The pointer to the string
*/
void deduplicate_str(char *str) 
{
    UNUSED_PARAM(str);

    size_t i = 0;
    while (*(str + i) != '\0') {
        if (*(str + i) == *(str + i + 1)) {
            size_t j = i + 1;
            while (*(str + j) != '\0') {
                *(str + j - 1) = *(str + j);
                ++j;
            }
            *(str + j - 1) = '\0';
        } else {
            ++i;
        }
    }

    return;
}

/**
 * @brief Swap the position of
 * two strings in memory.
 * 
 * @param s1 The first string
 * @param s2 The second string
*/
void swap_strings(char **s1, char **s2) 
{
    UNUSED_PARAM(s1);
    UNUSED_PARAM(s2);

    char *temp = *s1;
    *s1 = *s2;
    *s2 = temp;

    return;
}