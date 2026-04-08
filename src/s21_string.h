#ifndef S21_STRING_H_
#define S21_STRING_H_

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long s21_size;

#define S21_NULL ((void*)0)

// ================= MEMORY =================

void* s21_memchr(const void* str, int c, s21_size n);
int s21_memcmp(const void* str1, const void* str2, s21_size n);
void* s21_memcpy(void* dest, const void* src, s21_size n);
void* s21_memset(void* str, int c, s21_size n);

// ================= STRING =================

char* s21_strncat(char* dest, const char* src, s21_size n);
char* s21_strchr(const char* str, int c);
int s21_strncmp(const char* str1, const char* str2, s21_size n);
char* s21_strncpy(char* dest, const char* src, s21_size n);
s21_size s21_strcspn(const char* str1, const char* str2);
char* s21_strerror(int errnum);
s21_size s21_strlen(const char* str);
char* s21_strpbrk(const char* str1, const char* str2);
char* s21_strrchr(const char* str, int c);
char* s21_strstr(const char* haystack, const char* needle);
char* s21_strtok(char* str, const char* delim);

// ================= ADDITIONAL =================

void* s21_insert(const char* src, const char* str, s21_size start_index);
void* s21_trim(const char* src, const char* trim_chars);  //?
void* s21_to_upper(const char* str);
void* s21_to_lower(const char* str);

// ================= sprintf =================

int s21_sprintf(char* str, const char* format, ...);

// ================= sscanf =================

typedef enum { LEN_NONE, LEN_H, LEN_HH, LEN_L, LEN_LL } Length;

typedef struct {
  int suppress;
  int width;
  Length length;
  char spec;
  const char* spec_ptr;
} FormatSpec;

int s21_sscanf(const char* str, const char* format, ...);

#endif  // S21_STRING_H_