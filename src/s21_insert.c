#include <stdlib.h>

#include "s21_string.h"
/*
void* s21_insert(const char* src, const char* str, s21_size start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;
  }

  int src_len = s21_strlen(src);  // длина первой строки
  int str_len = s21_strlen(str);  // длина второй строки

  if (start_index > (s21_size)src_len) {
    return S21_NULL;
  }

  char* str_res = S21_NULL;

  str_res = malloc(str_len + src_len + 1);  // выделяем память для результата

  int cnt_i_src = 0;
  int cnt_j_str = 0;
  int cnt_for_res = 0;

  while (cnt_i_src < (int)start_index) {
    str_res[cnt_for_res] = src[cnt_i_src];
    cnt_i_src++;
    cnt_for_res++;
  }

  while (cnt_j_str < str_len) {
    str_res[cnt_for_res] = str[cnt_j_str];
    cnt_j_str++;
    cnt_for_res++;
  }

  while (cnt_i_src < src_len) {
    str_res[cnt_for_res] = src[cnt_i_src];
    cnt_i_src++;
    cnt_for_res++;
  }

  str_res[cnt_for_res] = '\0';

  return str_res;
}*/

void* s21_insert(const char* src, const char* str, s21_size start_index) {
  if (!src || !str || start_index > s21_strlen(src)) return S21_NULL;

  s21_size str_len = s21_strlen(src);
  s21_size src_len = s21_strlen(str);
  char* str_res = malloc(str_len + src_len + 1);

  if (!str_res) return S21_NULL;

  s21_size cnt_for_res = 0, cnt_i_src = 0, cnt_j_str = 0;

  while (cnt_i_src < start_index && src[cnt_i_src]) {
    str_res[cnt_for_res++] = src[cnt_i_src++];
  }

  while (cnt_j_str < src_len) {
    str_res[cnt_for_res++] = str[cnt_j_str++];
  }

  while (src[cnt_i_src]) {
    str_res[cnt_for_res++] = src[cnt_i_src++];
  }

  str_res[cnt_for_res] = '\0';
  return str_res;
}
