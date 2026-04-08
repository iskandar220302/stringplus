#include "s21_string.h"

s21_size s21_strlen(const char* str) {
  const char* p_str = str;
  while (*p_str++);
  return (s21_size)((p_str - 1) - str);
}

char* s21_strchr(const char* str, int c) {
  for (;;) {
    if ((unsigned char)(*str) == (unsigned char)c)
      return (char*)str;
    else if (*str == '\0')
      break;
    ++str;
  }
  return S21_NULL;
}

char* s21_strncpy(char* dest, const char* src, s21_size n) {
  s21_size n_byte = 0;
  for (; n_byte < n && src[n_byte] != '\0'; n_byte++)
    dest[n_byte] = src[n_byte];
  for (; n_byte < n; n_byte++) dest[n_byte] = '\0';

  return dest;
}

char* s21_strncat(char* dest, const char* src, s21_size n) {
  char* start_copy = s21_strchr(dest, '\0');
  s21_size n_byte = 0;

  for (; n_byte < n && src[n_byte] != '\0'; n_byte++) {
    start_copy[n_byte] = src[n_byte];
  }
  start_copy[n_byte] = '\0';

  return dest;
}

int s21_strncmp(const char* str1, const char* str2, s21_size n) {
  s21_size char_i = 0;
  for (; char_i < n && str1[char_i] == str2[char_i] && str1[char_i] != '\0';
       char_i++);
  if (char_i == n) return 0;
  return (int)((unsigned char)(str1[char_i]) - (unsigned char)(str2[char_i]));
}

char* s21_strpbrk(const char* str1, const char* str2) {
  while (*str1) {
    if (s21_strchr(str2, (unsigned char)*str1)) return (char*)str1;
    str1++;
  }
  return S21_NULL;
}

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size len = 0;
  for (; str1[len]; len++)
    if (s21_strchr(str2, (unsigned char)str1[len])) return len;

  return len;
}

char* s21_strrchr(const char* str, int c) {
  char* end_str = s21_strchr(str, '\0');
  unsigned char found = (unsigned char)c;
  while (end_str >= str) {
    if ((unsigned char)(*end_str) == found) return end_str;
    if (end_str == str) break;
    end_str--;
  }

  return S21_NULL;
}

char* s21_strstr(const char* haystack, const char* needle) {
  s21_size len_needle = s21_strlen(needle);
  if (!len_needle) return (char*)haystack;
  char* substr = S21_NULL;
  while (*haystack && (substr = s21_strchr(haystack, *needle))) {
    if (s21_strncmp(substr, needle, len_needle) == 0) return substr;
    haystack = substr + 1;
  }
  return S21_NULL;
}

char* s21_strtok(char* str, const char* delim) {
  static char* sub_str = S21_NULL;

  sub_str = str ? str : sub_str;
  if (!sub_str || !(*sub_str)) return S21_NULL;

  while (*sub_str && s21_strchr(delim, *sub_str)) sub_str++;

  char* delimiter = s21_strpbrk(sub_str, delim);

  str = sub_str;

  if (delimiter) {
    *delimiter = '\0';
    sub_str = delimiter + 1;
  } else
    sub_str = S21_NULL;

  return *str ? str : S21_NULL;
}