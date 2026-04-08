#include "s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size n) {
  const unsigned char* block1 = str1;
  const unsigned char* block2 = str2;
  for (s21_size n_byte = 0; n_byte < n; n_byte++) {
    if (block1[n_byte] != block2[n_byte])
      return (int)(block1[n_byte] - block2[n_byte]);
  }
  return 0;
}

void* s21_memchr(const void* str, int c, s21_size n) {
  unsigned char found_byte = (unsigned char)c;
  const unsigned char* haystack = (const unsigned char*)str;

  for (s21_size n_byte = 0; n_byte < n; n_byte++)
    if (haystack[n_byte] == found_byte) return (void*)(haystack + n_byte);

  return S21_NULL;
}

void* s21_memcpy(void* dest, const void* src, s21_size n) {
  unsigned char* to = (unsigned char*)dest;
  const unsigned char* from = (const unsigned char*)src;

  for (s21_size n_byte = 0; n_byte < n; n_byte++) to[n_byte] = from[n_byte];

  return dest;
}

void* s21_memset(void* str, int c, s21_size n) {
  unsigned char* buff = (unsigned char*)str;
  unsigned char value = (unsigned char)c;

  for (s21_size n_byte = 0; n_byte < n; n_byte++) buff[n_byte] = value;

  return str;
}