#include <check.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../s21_string.h"

static int sign_int(int value) { return (value > 0) - (value < 0); }

static void assert_ptr_offset_eq(const void* base, const void* p1,
                                 const void* p2) {
  if (p1 == S21_NULL || p2 == S21_NULL) {
    ck_assert_ptr_eq(p1, p2);
  } else {
    ck_assert_int_eq((const char*)p1 - (const char*)base,
                     (const char*)p2 - (const char*)base);
  }
}

static void check_strtok_pair(const char* input, const char* delim) {
  char buf1[256];
  char buf2[256];
  strncpy(buf1, input, sizeof(buf1));
  strncpy(buf2, input, sizeof(buf2));
  buf1[sizeof(buf1) - 1] = '\0';
  buf2[sizeof(buf2) - 1] = '\0';

  char* tok1 = s21_strtok(buf1, delim);
  char* tok2 = strtok(buf2, delim);
  while (tok1 != S21_NULL || tok2 != S21_NULL) {
    ck_assert_int_eq(tok1 == S21_NULL, tok2 == S21_NULL);
    if (tok1 != S21_NULL) {
      ck_assert_str_eq(tok1, tok2);
    }
    tok1 = s21_strtok(S21_NULL, delim);
    tok2 = strtok(S21_NULL, delim);
  }
}

static void check_sprintf_one(const char* fmt, double value) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, fmt, value);
  int std_ret = sprintf(std_buf, fmt, value);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}

static void check_sprintf_two(const char* fmt, double value, int width,
                              int precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, fmt, width, precision, value);
  int std_ret = sprintf(std_buf, fmt, width, precision, value);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}

START_TEST(test_memchr_basic) {
  unsigned char data[] = {0, 1, 2, 3, 4, 5, 'a', 0, 'b'};
  void* s21_res = s21_memchr(data, 3, sizeof(data));
  void* std_res = memchr(data, 3, sizeof(data));
  assert_ptr_offset_eq(data, s21_res, std_res);
}
END_TEST

START_TEST(test_memchr_zero_length) {
  unsigned char data[] = {'a', 'b', 'c'};
  void* s21_res = s21_memchr(data, 'a', 0);
  void* std_res = memchr(data, 'a', 0);
  assert_ptr_offset_eq(data, s21_res, std_res);
}
END_TEST

START_TEST(test_memchr_not_found) {
  unsigned char data[] = {1, 2, 3, 4, 5};
  void* s21_res = s21_memchr(data, 9, sizeof(data));
  void* std_res = memchr(data, 9, sizeof(data));
  assert_ptr_offset_eq(data, s21_res, std_res);
}
END_TEST

START_TEST(test_memchr_find_zero) {
  unsigned char data[] = {'a', 0, 'b'};
  void* s21_res = s21_memchr(data, 0, sizeof(data));
  void* std_res = memchr(data, 0, sizeof(data));
  assert_ptr_offset_eq(data, s21_res, std_res);
}
END_TEST

START_TEST(test_memchr_last_byte) {
  unsigned char data[] = {1, 2, 3, 4, 5};
  void* s21_res = s21_memchr(data, 5, sizeof(data));
  void* std_res = memchr(data, 5, sizeof(data));
  assert_ptr_offset_eq(data, s21_res, std_res);
}
END_TEST

START_TEST(test_memcmp_sign) {
  unsigned char a[] = {1, 2, 3, 4};
  unsigned char b[] = {1, 2, 3, 5};
  ck_assert_int_eq(sign_int(s21_memcmp(a, b, sizeof(a))),
                   sign_int(memcmp(a, b, sizeof(a))));
  ck_assert_int_eq(sign_int(s21_memcmp(a, b, 0)), sign_int(memcmp(a, b, 0)));
}
END_TEST

START_TEST(test_memcmp_unsigned_bytes) {
  unsigned char a[] = {0x80};
  unsigned char b[] = {0x7F};
  ck_assert_int_eq(sign_int(s21_memcmp(a, b, sizeof(a))),
                   sign_int(memcmp(a, b, sizeof(a))));
}
END_TEST

START_TEST(test_memcmp_partial_equal) {
  unsigned char a[] = {1, 2, 3, 4};
  unsigned char b[] = {1, 2, 3, 5};
  ck_assert_int_eq(sign_int(s21_memcmp(a, b, 3)), sign_int(memcmp(a, b, 3)));
}
END_TEST

START_TEST(test_memcpy_basic) {
  char src[] = "abcdef";
  char s21_dest[16] = {0};
  char std_dest[16] = {0};
  s21_memcpy(s21_dest, src, 6);
  memcpy(std_dest, src, 6);
  ck_assert_int_eq(memcmp(s21_dest, std_dest, sizeof(std_dest)), 0);
}
END_TEST

START_TEST(test_memcpy_zero_length) {
  char src[] = "abcdef";
  char s21_dest[8] = "xxxxxxx";
  char std_dest[8] = "xxxxxxx";
  s21_memcpy(s21_dest, src, 0);
  memcpy(std_dest, src, 0);
  ck_assert_int_eq(memcmp(s21_dest, std_dest, sizeof(std_dest)), 0);
}
END_TEST

START_TEST(test_memset_basic) {
  char s21_buf[12];
  char std_buf[12];
  s21_memset(s21_buf, 'x', sizeof(s21_buf));
  memset(std_buf, 'x', sizeof(std_buf));
  ck_assert_int_eq(memcmp(s21_buf, std_buf, sizeof(std_buf)), 0);
}
END_TEST

START_TEST(test_memset_zero_length) {
  char s21_buf[8] = "xxxxxxx";
  char std_buf[8] = "xxxxxxx";
  s21_memset(s21_buf, 'y', 0);
  ck_assert_int_eq(memcmp(s21_buf, std_buf, sizeof(std_buf)), 0);
}
END_TEST

START_TEST(test_strncat_basic) {
  char s21_buf[32] = "Hello";
  char std_buf[32] = "Hello";
  s21_strncat(s21_buf, "World", 3);
  strncat(std_buf, "World", 3);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_longer_than_src) {
  char s21_buf[32] = "Hi";
  char std_buf[32] = "Hi";
  s21_strncat(s21_buf, "There", 10);
  strncat(std_buf, "There", 10);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_zero) {
  char s21_buf[32] = "Hi";
  char std_buf[32] = "Hi";
  s21_strncat(s21_buf, "There", 0);
  strncat(std_buf, "There", 0);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_empty_src) {
  char s21_buf[32] = "Hi";
  char std_buf[32] = "Hi";
  s21_strncat(s21_buf, "", 5);
  strncat(std_buf, "", 5);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_empty_dest) {
  char s21_buf[32] = "";
  char std_buf[32] = "";
  s21_strncat(s21_buf, "abc", 2);
  strncat(std_buf, "abc", 2);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strchr_basic) {
  const char* str = "abca";
  char* s21_res = s21_strchr(str, 'c');
  char* std_res = strchr(str, 'c');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strchr_null) {
  const char* str = "abca";
  char* s21_res = s21_strchr(str, '\0');
  char* std_res = strchr(str, '\0');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strchr_first_char) {
  const char* str = "abc";
  char* s21_res = s21_strchr(str, 'a');
  char* std_res = strchr(str, 'a');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strchr_not_found) {
  const char* str = "abc";
  char* s21_res = s21_strchr(str, 'z');
  char* std_res = strchr(str, 'z');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strncmp_sign) {
  ck_assert_int_eq(sign_int(s21_strncmp("abc", "abd", 3)),
                   sign_int(strncmp("abc", "abd", 3)));
  ck_assert_int_eq(sign_int(s21_strncmp("abc", "abc", 2)),
                   sign_int(strncmp("abc", "abc", 2)));
  ck_assert_int_eq(sign_int(s21_strncmp("abc", "abc", 0)),
                   sign_int(strncmp("abc", "abc", 0)));
}
END_TEST

START_TEST(test_strncmp_longer) {
  ck_assert_int_eq(sign_int(s21_strncmp("abc", "abcd", 5)),
                   sign_int(strncmp("abc", "abcd", 5)));
}
END_TEST

START_TEST(test_strncmp_prefix_equal) {
  ck_assert_int_eq(sign_int(s21_strncmp("abx", "aby", 2)),
                   sign_int(strncmp("abx", "aby", 2)));
}
END_TEST

START_TEST(test_strncpy_basic) {
  char s21_buf[10];
  char std_buf[10];
  memset(s21_buf, 'x', sizeof(s21_buf));
  memset(std_buf, 'x', sizeof(std_buf));
  s21_strncpy(s21_buf, "abc", 6);
  strncpy(std_buf, "abc", 6);
  ck_assert_int_eq(memcmp(s21_buf, std_buf, sizeof(std_buf)), 0);
}
END_TEST

START_TEST(test_strncpy_zero) {
  char s21_buf[6] = "xxxxx";
  char std_buf[6] = "xxxxx";
  s21_strncpy(s21_buf, "abc", 0);
  ck_assert_int_eq(memcmp(s21_buf, std_buf, sizeof(std_buf)), 0);
}
END_TEST

START_TEST(test_strcspn_basic) {
  ck_assert_uint_eq(s21_strcspn("hello, world", ", "),
                    strcspn("hello, world", ", "));
  ck_assert_uint_eq(s21_strcspn("abcdef", "xyz"), strcspn("abcdef", "xyz"));
}
END_TEST

START_TEST(test_strcspn_empty_reject) {
  ck_assert_uint_eq(s21_strcspn("hello", ""), strcspn("hello", ""));
}
END_TEST

START_TEST(test_strcspn_first_reject) {
  ck_assert_uint_eq(s21_strcspn("hello", "h"), strcspn("hello", "h"));
}
END_TEST

START_TEST(test_strcspn_empty_str1) {
  ck_assert_uint_eq(s21_strcspn("", "abc"), strcspn("", "abc"));
}
END_TEST

START_TEST(test_strerror_basic) {
  ck_assert_str_eq(s21_strerror(0), strerror(0));
  ck_assert_str_eq(s21_strerror(2), strerror(2));
  ck_assert_str_eq(s21_strerror(-1), strerror(-1));
  ck_assert_str_eq(s21_strerror(999), strerror(999));
}
END_TEST

START_TEST(test_strlen_basic) {
  ck_assert_uint_eq(s21_strlen(""), strlen(""));
  ck_assert_uint_eq(s21_strlen("hello"), strlen("hello"));
  ck_assert_uint_eq(s21_strlen("hello world"), strlen("hello world"));
}
END_TEST

START_TEST(test_strlen_embedded_null) {
  const char str[] = "abc\0def";
  ck_assert_uint_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strpbrk_basic) {
  const char* str = "hello";
  char* s21_res = s21_strpbrk(str, "xyzl");
  char* std_res = strpbrk(str, "xyzl");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strpbrk_empty_set) {
  const char* str = "hello";
  char* s21_res = s21_strpbrk(str, "");
  char* std_res = strpbrk(str, "");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strpbrk_first_char) {
  const char* str = "hello";
  char* s21_res = s21_strpbrk(str, "h");
  char* std_res = strpbrk(str, "h");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strpbrk_none) {
  const char* str = "hello";
  char* s21_res = s21_strpbrk(str, "xyz");
  char* std_res = strpbrk(str, "xyz");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strrchr_basic) {
  const char* str = "abca";
  char* s21_res = s21_strrchr(str, 'a');
  char* std_res = strrchr(str, 'a');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strrchr_not_found) {
  const char* str = "abca";
  char* s21_res = s21_strrchr(str, 'z');
  char* std_res = strrchr(str, 'z');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strrchr_null) {
  const char* str = "abca";
  char* s21_res = s21_strrchr(str, '\0');
  char* std_res = strrchr(str, '\0');
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strstr_basic) {
  const char* str = "hello world";
  char* s21_res = s21_strstr(str, "world");
  char* std_res = strstr(str, "world");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strstr_equal) {
  const char* str = "hello";
  char* s21_res = s21_strstr(str, "hello");
  char* std_res = strstr(str, "hello");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strstr_not_found) {
  const char* str = "hello";
  char* s21_res = s21_strstr(str, "world");
  char* std_res = strstr(str, "world");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strstr_longer_needle) {
  const char* str = "hi";
  char* s21_res = s21_strstr(str, "hello");
  char* std_res = strstr(str, "hello");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strstr_empty) {
  const char* str = "hello";
  char* s21_res = s21_strstr(str, "");
  char* std_res = strstr(str, "");
  assert_ptr_offset_eq(str, s21_res, std_res);
}
END_TEST

START_TEST(test_strtok_basic) { check_strtok_pair(" a,b,,c;d ", " ,;"); }
END_TEST

START_TEST(test_strtok_only_delim) { check_strtok_pair(";;;;", ";"); }
END_TEST

START_TEST(test_strtok_empty_delim) { check_strtok_pair("abc", ""); }
END_TEST

START_TEST(test_strtok_no_delim) { check_strtok_pair("abc", ","); }
END_TEST

START_TEST(test_strtok_empty_input) { check_strtok_pair("", ","); }
END_TEST

START_TEST(test_sprintf_basic) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "Hello %s", "world");
  int std_ret = sprintf(std_buf, "Hello %s", "world");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_width_flags) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%-10s|", "abc");
  int std_ret = sprintf(std_buf, "|%-10s|", "abc");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%010d|", 23);
  std_ret = sprintf(std_buf, "|%010d|", 23);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_int_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%+6d|%.0d|", 123, 0);
  int std_ret = sprintf(std_buf, "|%+6d|%.0d|", 123, 0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%-+6d|%.0d|", 123, 0);
  std_ret = sprintf(std_buf, "|%-+6d|%.0d|", 123, 0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_space_flag) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|% d|", 7);
  int std_ret = sprintf(std_buf, "|% d|", 7);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_width_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%8.4u|", 12U);
  int std_ret = sprintf(std_buf, "|%8.4u|", 12U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%-13.4u|", 12U);
  std_ret = sprintf(std_buf, "|%-13.4u|", 12U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_bases) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  unsigned int value = 48879U;
  int s21_ret =
      s21_sprintf(s21_buf, "|%u|%o|%x|%X|", value, value, value, value);
  int std_ret = sprintf(std_buf, "|%u|%o|%x|%X|", value, value, value, value);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_base_basic) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%u|%o|%x|%X|", 42U, 42U, 42U, 42U);
  int std_ret = sprintf(std_buf, "|%u|%o|%x|%X|", 42U, 42U, 42U, 42U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_width_align) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%5u|%-5u|", 42U, 42U);
  int std_ret = sprintf(std_buf, "|%5u|%-5u|", 42U, 42U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%5x|%-5x|", 0x2AU, 0x2AU);
  std_ret = sprintf(std_buf, "|%5x|%-5x|", 0x2AU, 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_zero_pad) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%05u|", 42U);
  int std_ret = sprintf(std_buf, "|%05u|", 42U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%08x|", 0x2AU);
  std_ret = sprintf(std_buf, "|%08x|", 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_precision_over_zero) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  const char* fmt_u = "|%08.3u|";
  const char* fmt_x = "|%08.3x|";
  int s21_ret = s21_sprintf(s21_buf, fmt_u, 42U);
  int std_ret = sprintf(std_buf, fmt_u, 42U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, fmt_x, 0x2AU);
  std_ret = sprintf(std_buf, fmt_x, 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_zero_precision_zero) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%.0u|", 0U);
  int std_ret = sprintf(std_buf, "|%.0u|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%5.0u|", 0U);
  std_ret = sprintf(std_buf, "|%5.0u|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%.0x|", 0U);
  std_ret = sprintf(std_buf, "|%.0x|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%5.0x|", 0U);
  std_ret = sprintf(std_buf, "|%5.0x|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_hex_hash) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#x|%#X|", 0x2AU, 0x2AU);
  int std_ret = sprintf(std_buf, "|%#x|%#X|", 0x2AU, 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%#x|", 0U);
  std_ret = sprintf(std_buf, "|%#x|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_hex_hash_zero_pad) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#08x|", 0x2AU);
  int std_ret = sprintf(std_buf, "|%#08x|", 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%#08X|", 0x2AU);
  std_ret = sprintf(std_buf, "|%#08X|", 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_octal_hash) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#o|%#.0o|%#o|", 0U, 0U, 9U);
  int std_ret = sprintf(std_buf, "|%#o|%#.0o|%#o|", 0U, 0U, 9U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_octal_hash_width) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#5o|%#05o|", 9U, 9U);
  int std_ret = sprintf(std_buf, "|%#5o|%#05o|", 9U, 9U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_star_width_precision_hex) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%*x|", 6, 0x2A);
  int std_ret = sprintf(std_buf, "|%*x|", 6, 0x2A);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%.*x|", 4, 0x2A);
  std_ret = sprintf(std_buf, "|%.*x|", 4, 0x2A);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%*.*x|", 6, 4, 0x2A);
  std_ret = sprintf(std_buf, "|%*.*x|", 6, 4, 0x2A);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%*x|", -6, 0x2A);
  std_ret = sprintf(std_buf, "|%*x|", -6, 0x2A);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_o_hash_zero_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#.0o|", 0U);
  int std_ret = sprintf(std_buf, "|%#.0o|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_star_negative_width) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int width = -8;
  int s21_ret = s21_sprintf(s21_buf, "|%*d|", width, 42);
  int std_ret = sprintf(std_buf, "|%*d|", width, 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_length) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  short hs = (short)-123;
  long ld = (long)123456;
  unsigned short hu = (unsigned short)65000;
  unsigned long lu = (unsigned long)4000000000UL;
  int s21_ret = s21_sprintf(s21_buf, "|%hd|%ld|%hu|%lu|", hs, ld, hu, lu);
  int std_ret = sprintf(std_buf, "|%hd|%ld|%hu|%lu|", hs, ld, hu, lu);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_char_and_percent) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "x=%3c %%", 'A');
  int std_ret = sprintf(std_buf, "x=%3c %%", 'A');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%8.3f|%+.2f|", -12.3456, 1.25);
  int std_ret = sprintf(std_buf, "|%8.3f|%+.2f|", -12.3456, 1.25);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%8.3f|%+.2f|", -12.3456, -INFINITY);
  std_ret = sprintf(std_buf, "|%8.3f|%+.2f|", -12.3456, -INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%8.3f|%+.2f|", -12.3456, -NAN);
  std_ret = sprintf(std_buf, "|%8.3f|%+.2f|", -12.3456, -NAN);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%8.3f|%+.2f|", -12.3456, INFINITY);
  std_ret = sprintf(std_buf, "|%8.3f|%+.2f|", -12.3456, INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  s21_ret = s21_sprintf(s21_buf, "|%8.3f|% .2f|", -12.3456, INFINITY);
  std_ret = sprintf(std_buf, "|%8.3f|% .2f|", -12.3456, INFINITY);
}
END_TEST

START_TEST(test_sprintf_scientific) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%e|", 12.3456);
  int std_ret = sprintf(std_buf, "|%e|", 12.3456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%E|", 12.3456);
  std_ret = sprintf(std_buf, "|%E|", 12.3456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%12.4e|", 123.456);
  std_ret = sprintf(std_buf, "|%12.4e|", 123.456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%-12.4E|", 123.456);
  std_ret = sprintf(std_buf, "|%-12.4E|", 123.456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%015.2e|", 12.0);
  std_ret = sprintf(std_buf, "|%015.2e|", 12.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%+e|", 12.0);
  std_ret = sprintf(std_buf, "|%+e|", 12.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|% e|", 12.0);
  std_ret = sprintf(std_buf, "|% e|", 12.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#.0e|", 12.0);
  std_ret = sprintf(std_buf, "|%#.0e|", 12.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  int width = 14;
  int precision = 3;
  s21_ret = s21_sprintf(s21_buf, "|%*.*E|", width, precision, 9876.543);
  std_ret = sprintf(std_buf, "|%*.*E|", width, precision, 9876.543);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.2e|", -123.4);
  std_ret = sprintf(std_buf, "|%.2e|", -123.4);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.3e|", 0.0001234);
  std_ret = sprintf(std_buf, "|%.3e|", 0.0001234);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_general) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%g|", 12.3456);
  int std_ret = sprintf(std_buf, "|%g|", 12.3456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%G|", 12.3456);
  std_ret = sprintf(std_buf, "|%G|", 12.3456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.4g|", 12345.67);
  std_ret = sprintf(std_buf, "|%.4g|", 12345.67);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#g|", 12.0);
  std_ret = sprintf(std_buf, "|%#g|", 12.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%12.5G|", 0.000123456);
  std_ret = sprintf(std_buf, "|%12.5G|", 0.000123456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_general_cases) {
  check_sprintf_one("%.6g", 12345.0);
  check_sprintf_one("%.4g", 12345.0);
  check_sprintf_one("%.6g", 0.00012345);
  check_sprintf_one("%.5g", 0.00012345);
  check_sprintf_one("%.4g", 0.00012345);
  check_sprintf_one("%.3g", 0.00012345);
  check_sprintf_one("%.6g", 0.000012345);

  check_sprintf_one("%.6g", 12.3400);
  check_sprintf_one("%.6g", 12.3000);
  check_sprintf_one("%.6g", 12.0);
  check_sprintf_one("%.6g", 1.23000e5);
  check_sprintf_one("%.6g", 1.20000e-5);
  check_sprintf_one("%.6g", 1000.0);
  check_sprintf_one("%.6g", 1000.10);

  check_sprintf_one("%#.6g", 12.3400);
  check_sprintf_one("%#.6g", 12.0);
  check_sprintf_one("%#.6g", 1.2e5);

  check_sprintf_one("%.0g", 12.34);
  check_sprintf_one("%.0g", 0.001234);
  check_sprintf_one("%.0G", 999.9);

  check_sprintf_one("%.0g", 2.5);
  check_sprintf_one("%.0f", 2.5);
  check_sprintf_one("%.0e", 2.5);
  check_sprintf_one("%.1g", 9.95);
  check_sprintf_one("%.2g", 9.95);
  check_sprintf_one("%.3g", 9.995);

  check_sprintf_one("%+g", 12.34);
  check_sprintf_one("% g", 12.34);
  check_sprintf_one("%+g", -12.34);
  check_sprintf_one("% g", -12.34);

  check_sprintf_one("%10g", 12.34);
  check_sprintf_one("%-10g", 12.34);
  check_sprintf_one("%010g", 12.34);
  check_sprintf_one("%+010g", 12.34);
  check_sprintf_one("% 010g", 12.34);
  check_sprintf_one("%10.4g", 12345.0);
  check_sprintf_one("%010.4g", 12345.0);

  check_sprintf_one("%12e", 12.34);
  check_sprintf_one("%-12e", 12.34);
  check_sprintf_one("%012e", 12.34);
  check_sprintf_one("%+012e", 12.34);

  check_sprintf_two("%*.*g", 12.34, 10, 6);
  check_sprintf_two("%*.*g", 12.34, -10, 6);
  check_sprintf_two("%*.*g", 12.34, 10, 0);
  check_sprintf_two("%*.*e", 12.34, 12, 4);
  check_sprintf_two("%*.*G", 0.000012345, 14, 6);

  check_sprintf_one("%.6g", 99999.9);
  check_sprintf_one("%.5g", 99999.9);
  check_sprintf_one("%.4g", 99999.9);

  check_sprintf_one("%.6g", 0.0000999999);
  check_sprintf_one("%.5g", 0.0000999999);
  check_sprintf_one("%.4g", 0.0000999999);

  check_sprintf_one("%g", -0.0);
  check_sprintf_one("%+g", -0.0);
  check_sprintf_one("% e", -0.0);
  check_sprintf_one("%f", -0.0);

  check_sprintf_one("%g", NAN);
  check_sprintf_one("%+g", NAN);
  check_sprintf_one("% g", NAN);

  check_sprintf_one("%g", INFINITY);
  check_sprintf_one("%+g", INFINITY);
  check_sprintf_one("% g", INFINITY);
  check_sprintf_one("%10g", INFINITY);
  check_sprintf_one("%010g", INFINITY);

  check_sprintf_one("%g", -INFINITY);
  check_sprintf_one("%+g", -INFINITY);
  check_sprintf_one("% g", -INFINITY);
  check_sprintf_one("%10g", -INFINITY);
  check_sprintf_one("%010g", -INFINITY);

  check_sprintf_one("%.6g", DBL_MAX);
  check_sprintf_one("%.6g", DBL_MIN);
  check_sprintf_one("%.6g", 1e308);
  check_sprintf_one("%.6g", 1e-308);
}
END_TEST

START_TEST(test_sprintf_pointer) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  void* ptr = (void*)0x2a;

  int s21_ret = s21_sprintf(s21_buf, "|%p|", ptr);
  int std_ret = 0;
  ck_assert_str_eq(s21_buf, "|0x2a|");
  ck_assert_int_eq(s21_ret, (int)strlen("|0x2a|"));

  s21_ret = s21_sprintf(s21_buf, "|%10p|", ptr);
  ck_assert_str_eq(s21_buf, "|      0x2a|");
  ck_assert_int_eq(s21_ret, (int)strlen("|      0x2a|"));

  s21_ret = s21_sprintf(s21_buf, "|%-10p|", ptr);
  ck_assert_str_eq(s21_buf, "|0x2a      |");
  ck_assert_int_eq(s21_ret, (int)strlen("|0x2a      |"));

  s21_ret = s21_sprintf(s21_buf, "|%.5p|", ptr);
  ck_assert_str_eq(s21_buf, "|0x0002a|");
  ck_assert_int_eq(s21_ret, (int)strlen("|0x0002a|"));

  s21_ret = s21_sprintf(s21_buf, "|%010p|", ptr);
  ck_assert_str_eq(s21_buf, "|0x0000002a|");
  ck_assert_int_eq(s21_ret, (int)strlen("|0x0000002a|"));

  char dummy = 'x';
  void* ptr_local = &dummy;
  s21_ret = s21_sprintf(s21_buf, "|%p|", ptr_local);
  std_ret = sprintf(std_buf, "|%p|", ptr_local);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  int width = 12;
  s21_ret = s21_sprintf(s21_buf, "|%*p|", width, ptr);
  std_ret = sprintf(std_buf, "|%*p|", width, ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  width = -12;
  s21_ret = s21_sprintf(s21_buf, "|%*p|", width, ptr);
  std_ret = sprintf(std_buf, "|%*p|", width, ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  void* null_ptr = S21_NULL;
  s21_ret = s21_sprintf(s21_buf, "|%p|", null_ptr);
  std_ret = sprintf(std_buf, "|%p|", null_ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%10p|", null_ptr);
  std_ret = sprintf(std_buf, "|%10p|", null_ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%-10p|", null_ptr);
  std_ret = sprintf(std_buf, "|%-10p|", null_ptr);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_string_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%.3s|", "abcdef");
  int std_ret = sprintf(std_buf, "|%.3s|", "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_star_width) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int width = 6;
  int s21_ret = s21_sprintf(s21_buf, "|%*d|", width, 123);
  int std_ret = sprintf(std_buf, "|%*d|", width, 123);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_star_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int precision = 2;
  int s21_ret = s21_sprintf(s21_buf, "|%.*f|", precision, 12.3456);
  int std_ret = sprintf(std_buf, "|%.*f|", precision, 12.3456);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_star_width_precision) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int width = 8;
  int precision = 3;
  int s21_ret = s21_sprintf(s21_buf, "|%*.*s|", width, precision, "abcdef");
  int std_ret = sprintf(std_buf, "|%*.*s|", width, precision, "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
  width = -8;
  precision = -3;
  s21_ret = s21_sprintf(s21_buf, "|%*.*f|", width, precision, 12.01);
  std_ret = sprintf(std_buf, "|%*.*f|", width, precision, 12.01);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_edge_cases) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%10.5d|", 42);
  int std_ret = sprintf(std_buf, "|%10.5d|", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%+010d|", 42);
  std_ret = sprintf(std_buf, "|%+010d|", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|% 010d|", 42);
  std_ret = sprintf(std_buf, "|% 010d|", 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%+10d|", -42);
  std_ret = sprintf(std_buf, "|%+10d|", -42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%10d|", INT_MIN);
  std_ret = sprintf(std_buf, "|%10d|", INT_MIN);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.15d|", INT_MAX);
  std_ret = sprintf(std_buf, "|%.15d|", INT_MAX);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%u|", UINT_MAX);
  std_ret = sprintf(std_buf, "|%u|", UINT_MAX);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#8x|", 0x2AU);
  std_ret = sprintf(std_buf, "|%#8x|", 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#.0x|", 0U);
  std_ret = sprintf(std_buf, "|%#.0x|", 0U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#.5o|", 9U);
  std_ret = sprintf(std_buf, "|%#.5o|", 9U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%10.3s|", "abcdef");
  std_ret = sprintf(std_buf, "|%10.3s|", "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%-10.3s|", "abcdef");
  std_ret = sprintf(std_buf, "|%-10.3s|", "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.0s|", "abcdef");
  std_ret = sprintf(std_buf, "|%.0s|", "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.*s|", -3, "abcdef");
  std_ret = sprintf(std_buf, "|%.*s|", -3, "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%-3c|", 'A');
  std_ret = sprintf(std_buf, "|%-3c|", 'A');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%5c|", '%');
  std_ret = sprintf(std_buf, "|%5c|", '%');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%Lf|", (long double)1.0L / 3.0L);
  std_ret = sprintf(std_buf, "|%Lf|", (long double)1.0L / 3.0L);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_flags) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%+010.2f|", 3.14);
  int std_ret = sprintf(std_buf, "|%+010.2f|", 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|% 010.2f|", 3.14);
  std_ret = sprintf(std_buf, "|% 010.2f|", 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%-10.2f|", 3.14);
  std_ret = sprintf(std_buf, "|%-10.2f|", 3.14);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#8.0f|", 3.0);
  std_ret = sprintf(std_buf, "|%#8.0f|", 3.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#10.0e|", 3.0);
  std_ret = sprintf(std_buf, "|%#10.0e|", 3.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#12.3G|", 12345.0);
  std_ret = sprintf(std_buf, "|%#12.3G|", 12345.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_additional_flags) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%010d|", -42);
  int std_ret = sprintf(std_buf, "|%010d|", -42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%5.0d|", 0);
  std_ret = sprintf(std_buf, "|%5.0d|", 0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.*d|", -3, 7);
  std_ret = sprintf(std_buf, "|%.*d|", -3, 7);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%*i|", -6, 123);
  std_ret = sprintf(std_buf, "|%*i|", -6, 123);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%0*d|", 6, 123);
  std_ret = sprintf(std_buf, "|%0*d|", 6, 123);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%.*d|", 5, 42);
  std_ret = sprintf(std_buf, "|%.*d|", 5, 42);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#8.4x|", 0x2AU);
  std_ret = sprintf(std_buf, "|%#8.4x|", 0x2AU);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%#6.4o|", 9U);
  std_ret = sprintf(std_buf, "|%#6.4o|", 9U);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%3s|", "abcdef");
  std_ret = sprintf(std_buf, "|%3s|", "abcdef");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%*c|", -4, 'A');
  std_ret = sprintf(std_buf, "|%*c|", -4, 'A');
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%e|", 1e100);
  std_ret = sprintf(std_buf, "|%e|", 1e100);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_nan_inf_upper) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%E|", NAN);
  int std_ret = sprintf(std_buf, "|%E|", NAN);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%G|", NAN);
  std_ret = sprintf(std_buf, "|%G|", NAN);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%+E|", INFINITY);
  std_ret = sprintf(std_buf, "|%+E|", INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|% E|", INFINITY);
  std_ret = sprintf(std_buf, "|% E|", INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%010E|", INFINITY);
  std_ret = sprintf(std_buf, "|%010E|", INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);

  s21_ret = s21_sprintf(s21_buf, "|%10G|", -INFINITY);
  std_ret = sprintf(std_buf, "|%10G|", -INFINITY);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_hash_float) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%#.0f|", 3.0);
  int std_ret = sprintf(std_buf, "|%#.0f|", 3.0);
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_percent_literal) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "|%%|");
  int std_ret = sprintf(std_buf, "|%%|");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_percent_multiple) {
  char s21_buf[256] = {0};
  char std_buf[256] = {0};
  int s21_ret = s21_sprintf(s21_buf, "%% %% %%%%");
  int std_ret = sprintf(std_buf, "%% %% %%%%");
  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

Suite* s21_string_suite(void) {
  Suite* suite = suite_create("s21_string");

  TCase* tc_mem = tcase_create("mem");
  tcase_add_test(tc_mem, test_memchr_basic);
  tcase_add_test(tc_mem, test_memchr_zero_length);
  tcase_add_test(tc_mem, test_memchr_not_found);
  tcase_add_test(tc_mem, test_memchr_find_zero);
  tcase_add_test(tc_mem, test_memchr_last_byte);
  tcase_add_test(tc_mem, test_memcmp_sign);
  tcase_add_test(tc_mem, test_memcmp_unsigned_bytes);
  tcase_add_test(tc_mem, test_memcmp_partial_equal);
  tcase_add_test(tc_mem, test_memcpy_basic);
  tcase_add_test(tc_mem, test_memcpy_zero_length);
  tcase_add_test(tc_mem, test_memset_basic);
  tcase_add_test(tc_mem, test_memset_zero_length);
  suite_add_tcase(suite, tc_mem);

  TCase* tc_str = tcase_create("str");
  tcase_add_test(tc_str, test_strncat_basic);
  tcase_add_test(tc_str, test_strncat_longer_than_src);
  tcase_add_test(tc_str, test_strncat_zero);
  tcase_add_test(tc_str, test_strncat_empty_src);
  tcase_add_test(tc_str, test_strncat_empty_dest);
  tcase_add_test(tc_str, test_strchr_basic);
  tcase_add_test(tc_str, test_strchr_null);
  tcase_add_test(tc_str, test_strchr_first_char);
  tcase_add_test(tc_str, test_strchr_not_found);
  tcase_add_test(tc_str, test_strncmp_sign);
  tcase_add_test(tc_str, test_strncmp_longer);
  tcase_add_test(tc_str, test_strncmp_prefix_equal);
  tcase_add_test(tc_str, test_strncpy_basic);
  tcase_add_test(tc_str, test_strncpy_zero);
  tcase_add_test(tc_str, test_strcspn_basic);
  tcase_add_test(tc_str, test_strcspn_empty_reject);
  tcase_add_test(tc_str, test_strcspn_first_reject);
  tcase_add_test(tc_str, test_strcspn_empty_str1);
  tcase_add_test(tc_str, test_strerror_basic);
  tcase_add_test(tc_str, test_strlen_basic);
  tcase_add_test(tc_str, test_strlen_embedded_null);
  tcase_add_test(tc_str, test_strpbrk_basic);
  tcase_add_test(tc_str, test_strpbrk_empty_set);
  tcase_add_test(tc_str, test_strpbrk_first_char);
  tcase_add_test(tc_str, test_strpbrk_none);
  tcase_add_test(tc_str, test_strrchr_basic);
  tcase_add_test(tc_str, test_strrchr_not_found);
  tcase_add_test(tc_str, test_strrchr_null);
  tcase_add_test(tc_str, test_strstr_basic);
  tcase_add_test(tc_str, test_strstr_equal);
  tcase_add_test(tc_str, test_strstr_not_found);
  tcase_add_test(tc_str, test_strstr_longer_needle);
  tcase_add_test(tc_str, test_strstr_empty);
  tcase_add_test(tc_str, test_strtok_basic);
  tcase_add_test(tc_str, test_strtok_only_delim);
  tcase_add_test(tc_str, test_strtok_empty_delim);
  tcase_add_test(tc_str, test_strtok_no_delim);
  tcase_add_test(tc_str, test_strtok_empty_input);
  suite_add_tcase(suite, tc_str);

  TCase* tc_sprintf = tcase_create("sprintf");
  tcase_add_test(tc_sprintf, test_sprintf_basic);
  tcase_add_test(tc_sprintf, test_sprintf_width_flags);
  tcase_add_test(tc_sprintf, test_sprintf_int_precision);
  tcase_add_test(tc_sprintf, test_sprintf_space_flag);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_width_precision);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_bases);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_base_basic);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_width_align);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_zero_pad);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_precision_over_zero);
  tcase_add_test(tc_sprintf, test_sprintf_unsigned_zero_precision_zero);
  tcase_add_test(tc_sprintf, test_sprintf_hex_hash);
  tcase_add_test(tc_sprintf, test_sprintf_hex_hash_zero_pad);
  tcase_add_test(tc_sprintf, test_sprintf_octal_hash);
  tcase_add_test(tc_sprintf, test_sprintf_octal_hash_width);
  tcase_add_test(tc_sprintf, test_sprintf_o_hash_zero_precision);
  tcase_add_test(tc_sprintf, test_sprintf_length);
  tcase_add_test(tc_sprintf, test_sprintf_char_and_percent);
  tcase_add_test(tc_sprintf, test_sprintf_float);
  tcase_add_test(tc_sprintf, test_sprintf_scientific);
  tcase_add_test(tc_sprintf, test_sprintf_general);
  tcase_add_test(tc_sprintf, test_sprintf_general_cases);
  tcase_add_test(tc_sprintf, test_sprintf_pointer);
  tcase_add_test(tc_sprintf, test_sprintf_string_precision);
  tcase_add_test(tc_sprintf, test_sprintf_star_width);
  tcase_add_test(tc_sprintf, test_sprintf_star_precision);
  tcase_add_test(tc_sprintf, test_sprintf_star_width_precision);
  tcase_add_test(tc_sprintf, test_sprintf_edge_cases);
  tcase_add_test(tc_sprintf, test_sprintf_float_flags);
  tcase_add_test(tc_sprintf, test_sprintf_additional_flags);
  tcase_add_test(tc_sprintf, test_sprintf_nan_inf_upper);
  tcase_add_test(tc_sprintf, test_sprintf_star_negative_width);
  tcase_add_test(tc_sprintf, test_sprintf_star_width_precision_hex);
  tcase_add_test(tc_sprintf, test_sprintf_hash_float);
  tcase_add_test(tc_sprintf, test_sprintf_percent_literal);
  tcase_add_test(tc_sprintf, test_sprintf_percent_multiple);
  suite_add_tcase(suite, tc_sprintf);

  return suite;
}

int main(void) {
  Suite* suite = s21_string_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
