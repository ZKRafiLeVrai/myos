#include "../include/string.h"
int strlen(const char* str) {
    int i = 0; while (str[i]) i++; return i;
}
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}
void strcpy(char* dest, const char* src) {
    int i = 0; while ((dest[i] = src[i]) != '\0') i++;
}