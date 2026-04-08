#include <stdlib.h>

#include "s21_string.h"

void* s21_to_lower(const char* str) {
  if (str == S21_NULL) return S21_NULL;

  s21_size len = s21_strlen(str);
  char* new_str = (char*)malloc(len + 1);
  if (!new_str) return S21_NULL;

  for (s21_size i = 0; i < len; i++) {
    char c = str[i];
    new_str[i] = (c >= 'A' && c <= 'Z') ? c + 32 : c;
  }
  new_str[len] = '\0';

  return new_str;
}