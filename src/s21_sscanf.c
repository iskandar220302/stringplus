//
// Created by Iskander Zakirov on 26.01.2026.
//

#include <stdint.h>

#include "s21_string.h"

static void skip_spaces(const char** str);
static void parse_format(const char** format, FormatSpec* fs);
static int parse_integer(const char** str, void* out, FormatSpec* fs, int base,
                         int is_signed);
static int parse_string(const char** str, void* out, FormatSpec* fs);
static int parse_char(const char** str, void* out, FormatSpec* fs);
static int handle_specifier(const char** str, FormatSpec* fs, va_list args,
                            int read);
static int parse_scanset(const char** str, void* out, FormatSpec* fs);
static int parse_float(const char** str, void* out, FormatSpec* fs);

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int assigned = 0;
  int read = 0;

  while (*format) {
    if (*format == '%') {
      format++;

      FormatSpec fs;
      parse_format(&format, &fs);

      const char* before = str;
      int success = handle_specifier(&str, &fs, args, read);

      if (!success) break;

      read += (str - before);

      if (!fs.suppress && fs.spec != '%' && fs.spec != 'n') {
        assigned++;
      } else if (!fs.suppress && fs.spec != '%') {
        assigned++;
      }

      format++;
    } else if (isspace((unsigned char)*format)) {
      skip_spaces(&str);
      format++;
    } else {
      if (*format != *str) break;

      format++;
      str++;
      read++;
    }
  }

  va_end(args);
  return assigned;
}

static void skip_spaces(const char** str) {
  while (**str && isspace((unsigned char)**str)) (*str)++;
}

static void parse_format(const char** format, FormatSpec* fs) {
  fs->suppress = 0;
  fs->width = 0;
  fs->length = LEN_NONE;
  fs->spec = 0;

  if (**format == '*') {
    fs->suppress = 1;
    (*format)++;
  }

  while (isdigit((unsigned char)**format)) {
    fs->width = fs->width * 10 + (**format - '0');
    (*format)++;
  }

  if (**format == 'h') {
    (*format)++;
    if (**format == 'h') {
      fs->length = LEN_HH;
      (*format)++;
    } else {
      fs->length = LEN_H;
    }
  } else if (**format == 'l') {
    (*format)++;
    if (**format == 'l') {
      fs->length = LEN_LL;
      (*format)++;
    } else {
      fs->length = LEN_L;
    }
  }
  if (**format == '[') {
    fs->spec = '[';
    (*format)++;
    fs->spec_ptr = *format;

    while (**format && **format != ']') (*format)++;
  } else {
    fs->spec = **format;
  }
}

static int parse_integer(const char** str, void* out, FormatSpec* fs, int base,
                         int is_signed) {
  skip_spaces(str);

  char buffer[256];
  int i = 0;
  const char* s = *str;

  while (*s && !isspace((unsigned char)*s) &&
         (fs->width == 0 || i < fs->width) && i < 255) {
    buffer[i++] = *s++;
  }

  buffer[i] = '\0';

  char* end = NULL;

  if (is_signed) {
    long long v = strtoll(buffer, &end, base);
    if (end == buffer) return 0;

    *str += (end - buffer);

    if (!fs->suppress) {
      switch (fs->length) {
        case LEN_HH:
          *(signed char*)out = (signed char)v;
          break;
        case LEN_H:
          *(short*)out = (short)v;
          break;
        case LEN_L:
          *(long*)out = (long)v;
          break;
        case LEN_LL:
          *(long long*)out = v;
          break;
        default:
          *(int*)out = (int)v;
          break;
      }
    }
  } else {
    unsigned long long v = strtoull(buffer, &end, base);
    if (end == buffer) return 0;

    *str += (end - buffer);

    if (!fs->suppress) {
      switch (fs->length) {
        case LEN_HH:
          *(unsigned char*)out = (unsigned char)v;
          break;
        case LEN_H:
          *(unsigned short*)out = (unsigned short)v;
          break;
        case LEN_L:
          *(unsigned long*)out = (unsigned long)v;
          break;
        case LEN_LL:
          *(unsigned long long*)out = v;
          break;
        default:
          *(unsigned int*)out = (unsigned int)v;
          break;
      }
    }
  }

  return 1;
}

static int parse_string(const char** str, void* out, FormatSpec* fs) {
  skip_spaces(str);

  if (**str == '\0') return 0;

  char* dest = (char*)out;
  int i = 0;

  while (**str && !isspace((unsigned char)**str) &&
         (fs->width == 0 || i < fs->width)) {
    if (!fs->suppress) dest[i] = **str;

    (*str)++;
    i++;
  }

  if (!fs->suppress) dest[i] = '\0';

  return (i > 0);
}

static int parse_char(const char** str, void* out, FormatSpec* fs) {
  int count = fs->width ? fs->width : 1;

  if (**str == '\0') return 0;

  char* dest = (char*)out;

  for (int i = 0; i < count; i++) {
    if (**str == '\0') return 0;

    if (!fs->suppress) dest[i] = **str;

    (*str)++;
  }

  return 1;
}

static int handle_specifier(const char** str, FormatSpec* fs, va_list args,
                            int read) {
  void* p = NULL;

  if (!fs->suppress && fs->spec != '%') p = va_arg(args, void*);

  switch (fs->spec) {
    case 'd':
      return parse_integer(str, p, fs, 10, 1);

    case 'i':
      return parse_integer(str, p, fs, 0, 1);

    case 'u':
      return parse_integer(str, p, fs, 10, 0);

    case 'o':
      return parse_integer(str, p, fs, 8, 0);

    case 'x':
    case 'X':
      return parse_integer(str, p, fs, 16, 0);

    case 'p': {
      skip_spaces(str);

      unsigned long long value = 0;
      FormatSpec tmp = *fs;
      tmp.length = LEN_LL;

      int ok = parse_integer(str, &value, &tmp, 16, 0);
      if (!ok) return 0;

      if (!fs->suppress) *(void**)p = (void*)(uintptr_t)value;

      return 1;
    }

    case 'n':
      if (!fs->suppress) *(int*)p = read;
      return 1;

    case 's':
      return parse_string(str, p, fs);

    case 'c':
      return parse_char(str, p, fs);

    case '%':
      if (**str != '%') return 0;
      (*str)++;
      return 1;

    case '[':
      return parse_scanset(str, p, fs);
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      return parse_float(str, p, fs);

    default:
      return 0;
  }
}

static int parse_scanset(const char** str, void* out, FormatSpec* fs) {
  int table[256] = {0};
  int invert = 0;
  const char* f = fs->spec_ptr;  // объясню ниже
  int i = 0;

  if (*f == '^') {
    invert = 1;
    f++;
  }

  while (*f && *f != ']') {
    if (*(f + 1) == '-' && *(f + 2) && *(f + 2) != ']') {
      for (char c = *f; c <= *(f + 2); c++) table[(unsigned char)c] = 1;
      f += 3;
    } else {
      table[(unsigned char)*f++] = 1;
    }
  }

  if (*f == ']') f++;

  char* dest = (char*)out;

  while (**str && (fs->width == 0 || i < fs->width)) {
    unsigned char c = **str;
    int match = table[c];

    if (invert) match = !match;

    if (!match) break;

    if (!fs->suppress) dest[i] = c;

    (*str)++;
    i++;
  }

  if (!fs->suppress) dest[i] = '\0';

  return (i > 0);
}

static int parse_float(const char** str, void* out, FormatSpec* fs) {
  skip_spaces(str);

  char buffer[256];
  int i = 0;
  const char* s = *str;

  while (*s && !isspace((unsigned char)*s) &&
         (fs->width == 0 || i < fs->width) && i < 255) {
    buffer[i++] = *s++;
  }

  buffer[i] = '\0';

  char* end = NULL;
  double value = strtod(buffer, &end);

  if (end == buffer) return 0;

  *str += (end - buffer);

  if (!fs->suppress) {
    if (fs->length == LEN_L || fs->length == LEN_LL)
      *(double*)out = value;
    else
      *(float*)out = (float)value;
  }

  return 1;
}