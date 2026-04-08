#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>

#include "s21_string.h"

#define SET_FLAG(f, bit) ((f) |= (bit))
#define HAS_FLAG(f, bit) (((f) & (bit)) != 0)
#define CLR_FLAG(f, bit) ((f) &= ~(bit))
#define START_FMT '%'
#define EPS 1e-15

enum {
  F_MINUS = 1u << 0,  // 0001
  F_PLUS = 1u << 1,   // 0010
  F_SPACE = 1u << 2,  // 0100
  F_ZERO = 1u << 3,   // 1000
  F_HASH = 1u << 4,   // 10000
};

typedef enum { RIGHT_ALIGNMENT, LEFT_ALIGNMENT } ALLIGMENT;

typedef enum { SPACE_PAD = ' ', ZERO_PAD = '0' } PAD;

typedef struct {
  unsigned int flags;  // битовая маска флагов

  ALLIGMENT alligment;
  PAD pad;

  int width;
  int has_precision;
  int precision;

  char length;  // 0, 'h', 'l' , 'L'
  char spec;    // 'c','d','f','s','u','%' ...
} fmt_t;

typedef struct {
  char* ptr_str;
  int count_chars;
} out_t;

static int is_flag(char c) {
  return (c == '-' || c == '+' || c == ' ' || c == '0' || c == '#');
}

static int is_length(char c) { return (c == 'h' || c == 'l' || c == 'L'); }

static int is_spec(char c) {
  return (c == 'c' || c == 'd' || c == 'i' || c == 'f' || c == 's' ||
          c == 'u' || c == '%' || c == 'o' || c == 'x' || c == 'X' ||
          c == 'p' || c == 'e' || c == 'E' || c == 'g' || c == 'G');
}

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

static int is_precision_start(char c) { return c == '.'; }

static int is_additional_int(char c) { return c == '*'; }

static int out_puts(out_t* out, const char* str, int n) {
  if (n <= 0) return 0;
  s21_memcpy(&out->ptr_str[out->count_chars], str, (s21_size)n);
  out->count_chars += n;
  return n;
}

static int out_putc(out_t* out, const char c, int n) {
  if (n <= 0) return 0;
  int byte_n = 0;
  for (; byte_n < n; byte_n++) out->ptr_str[out->count_chars++] = c;

  return byte_n;
}

static int max_len(int a, int b) { return a > b ? a : b; }
static int min_len(int a, int b) { return a < b ? a : b; }

static unsigned long long round_scaled_value(long double scaled) {
  long double intpart;
  long double frac = modfl(scaled, &intpart);
  unsigned long long intpart_num = (unsigned long long)intpart;
  if (frac > 0.5L)
    intpart_num++;
  else if (fabsl(frac - 0.5L) <= EPS && (intpart_num & 1ULL))
    intpart_num++;
  return intpart_num;
}

static int s21_number_to_str(char* buff, uintmax_t number, const int base,
                             const char spec) {
  int len = 0;
  if (number == 0) {
    *buff = '0';
    return 1;
  }
  if (number / base != 0)
    len = s21_number_to_str(buff, number / base, base, spec);
  if (base > 10) {
    uintmax_t value = number % base;
    if (value < 10)
      buff[len] = value + '0';
    else
      buff[len] =
          spec == 'x' || spec == 'p' ? 'a' + (value - 10) : 'A' + (value - 10);
  } else
    buff[len] = number % base + '0';

  return len + 1;
}

static const char* parse_fmt(const char* src, va_list* ap, fmt_t* form) {
  if (!src || !*src) return S21_NULL;
  int additional_int = 0;  // дополнителный аргумент из va_list
  s21_memset(form, 0, sizeof(*form));

  // Разбор флагов
  while (is_flag(*src)) {
    switch (*src) {
      case '+':
        SET_FLAG(form->flags, F_PLUS);
        break;
      case '-': {
        SET_FLAG(form->flags, F_MINUS);
        form->alligment = LEFT_ALIGNMENT;
        break;
      }
      case '0': {
        SET_FLAG(form->flags, F_ZERO);
        form->pad = ZERO_PAD;
        break;
      }
      case ' ':
        SET_FLAG(form->flags, F_SPACE);
        break;
      case '#':
        SET_FLAG(form->flags, F_HASH);
        break;
    }
    src++;
  }
  if (HAS_FLAG(form->flags, F_PLUS)) CLR_FLAG(form->flags, F_SPACE);

  if (!HAS_FLAG(form->flags, F_ZERO)) form->pad = SPACE_PAD;

  if (HAS_FLAG(form->flags, F_MINUS)) {
    CLR_FLAG(form->flags, F_ZERO);
    form->pad = SPACE_PAD;
  }

  while (is_digit(*src) ||
         (additional_int = is_additional_int(*src)) == 1)  // Ширина
  {
    if (additional_int) {
      src++;
      int width = va_arg(*ap, int);
      if (width < 0) {
        width = -width;
        SET_FLAG(form->flags, F_MINUS);
        form->alligment = LEFT_ALIGNMENT;
        CLR_FLAG(form->flags, F_ZERO);
        form->pad = SPACE_PAD;
      }
      form->width = width;
      break;
    } else
      form->width = form->width * 10 + ((*src++) - '0');
  }
  // Точность
  if (is_precision_start(*src)) {
    form->has_precision = 1;
    src++;
    while (is_digit(*src))
      form->precision = form->precision * 10 + ((*src++) - '0');
    additional_int = is_additional_int(*src);
    if (additional_int) {
      int precision = va_arg(*ap, int);
      form->has_precision = precision < 0 ? 0 : 1;
      form->precision = form->has_precision ? precision : 0;
      src++;
    }
  }

  if (is_length(*src)) form->length = *src++;  // Длина

  if (is_spec(*src)) form->spec = *src++;  // Спецификатор

  return src;
}

static int handle_spec(fmt_t* form, va_list* ap, out_t* out) {
  if (form->spec == '%') return out_putc(out, '%', 1);

  int item_len = 0, pad_len = 0;

  if (form->spec == 'c') {
    item_len++;
    const char c = (char)va_arg(*ap, int);
    pad_len = max_len(0, form->width - item_len);

    if (form->alligment == LEFT_ALIGNMENT) {
      out_putc(out, c, 1);
      out_putc(out, SPACE_PAD, pad_len);
    }

    if (form->alligment == RIGHT_ALIGNMENT) {
      out_putc(out, SPACE_PAD, pad_len);
      out_putc(out, c, 1);
    }
    return pad_len + 1;
  }

  if (form->spec == 's') {
    const char* str = (const char*)va_arg(*ap, char*);
    if (str) {
      item_len = form->has_precision
                     ? min_len((int)s21_strlen(str), form->precision)
                     : (int)s21_strlen(str);
    }

    pad_len = max_len(0, form->width - item_len);

    if (form->alligment == LEFT_ALIGNMENT) {
      out_puts(out, str, item_len);
      out_putc(out, SPACE_PAD, pad_len);
    }

    if (form->alligment == RIGHT_ALIGNMENT) {
      out_putc(out, SPACE_PAD, pad_len);
      out_puts(out, str, item_len);
    }
    return pad_len + item_len;
  }

  if (form->spec == 'd' || form->spec == 'i') {
    long long number = 0;
    if (!form->length) number = (long long)va_arg(*ap, int);
    if (form->length == 'h') number = (long long)((short)va_arg(*ap, int));
    if (form->length == 'l') number = (long long)va_arg(*ap, long);

    int writed_chars = out->count_chars;
    char str_number[21] = {0};

    const char sign = number < 0 ? '-' : '+';
    int sign_len = 0;
    int precision_pad = 0;

    unsigned long long u_number = number < 0
                                      ? (unsigned long long)(-(number + 1)) + 1
                                      : (unsigned long long)number;
    item_len = s21_number_to_str(str_number, u_number, 10, 0);
    if (form->has_precision) {
      form->pad = SPACE_PAD;
      if (form->precision == 0 && number == 0)
        item_len = 0;
      else
        precision_pad =
            form->precision > item_len ? form->precision - item_len : 0;
    }
    if (sign == '-' || HAS_FLAG(form->flags, F_PLUS) ||
        HAS_FLAG(form->flags, F_SPACE))
      sign_len = 1;

    pad_len = max_len(0, form->width - (sign_len + precision_pad + item_len));

    if (form->alligment == LEFT_ALIGNMENT) {
      if (sign == '-' || HAS_FLAG(form->flags, F_PLUS)) out_putc(out, sign, 1);
      if (HAS_FLAG(form->flags, F_SPACE) && sign == '+')
        out_putc(out, SPACE_PAD, 1);
      out_putc(out, ZERO_PAD, precision_pad);
      out_puts(out, str_number, item_len);
      out_putc(out, form->pad, pad_len);
    }

    if (form->alligment == RIGHT_ALIGNMENT) {
      if (form->pad != ZERO_PAD) {
        out_putc(out, form->pad, pad_len);
        if (sign == '-' || HAS_FLAG(form->flags, F_PLUS))
          out_putc(out, sign, 1);
        if (HAS_FLAG(form->flags, F_SPACE) && sign == '+')
          out_putc(out, SPACE_PAD, 1);
      } else {
        if (sign == '-' || HAS_FLAG(form->flags, F_PLUS))
          out_putc(out, sign, 1);
        if (HAS_FLAG(form->flags, F_SPACE) && sign == '+')
          out_putc(out, SPACE_PAD, 1);
        out_putc(out, form->pad, pad_len);
      }

      out_putc(out, ZERO_PAD, precision_pad);
      out_puts(out, str_number, item_len);
    }

    return out->count_chars - writed_chars;
  }

  if (form->spec == 'u' || form->spec == 'o' || form->spec == 'x' ||
      form->spec == 'X' || form->spec == 'p') {
    uintmax_t number = 0;

    if (form->length == 'h')
      number = (uintmax_t)((unsigned short)va_arg(*ap, unsigned int));
    else if (form->length == 'l')
      number = (uintmax_t)va_arg(*ap, unsigned long);
    else if (form->spec == 'p')
      number = (uintmax_t)(uintptr_t)(void*)va_arg(*ap, void*);
    else
      number = (uintmax_t)va_arg(*ap, unsigned int);

    int base = 0;

    if (form->spec == 'o')
      base = 8;
    else if (form->spec == 'x' || form->spec == 'X' || form->spec == 'p')
      base = 16;
    else
      base = 10;

    int writed_chars = out->count_chars;
    char str_number[64] = {0};
    char prefix[4] = {0};
    int prefix_len = 0;

    int precision_pad = 0;

    item_len = s21_number_to_str(str_number, number, base, form->spec);

    if (form->spec == 'p') {
      if (number) {
        s21_memcpy(prefix, "0x", 2);
        prefix_len = 2;
      } else {
        s21_memcpy(str_number, "0x0", 3);
        item_len = 3;
      }
    }
    if (form->has_precision) {
      form->pad = SPACE_PAD;
      CLR_FLAG(form->flags, F_ZERO);
      if (form->precision == 0 && number == 0)
        item_len = 0;
      else
        precision_pad =
            form->precision > item_len ? form->precision - item_len : 0;
    }

    if (form->spec == 'o' && HAS_FLAG(form->flags, F_HASH)) {
      if (number == 0 && (form->has_precision && form->precision == 0)) {
        str_number[0] = '0';
        item_len = 1;
      }
      if (number != 0 && !precision_pad) prefix[0] = '0';
    }

    if ((form->spec == 'x' || form->spec == 'X') &&
        HAS_FLAG(form->flags, F_HASH)) {
      if (number != 0) s21_strncpy(prefix, form->spec == 'x' ? "0x" : "0X", 2);
    }

    prefix_len = s21_strlen(prefix);
    pad_len = max_len(
        0, form->width - ((int)s21_strlen(prefix) + precision_pad + item_len));

    if (form->alligment == LEFT_ALIGNMENT) {
      out_puts(out, prefix, prefix_len);
      out_putc(out, ZERO_PAD, precision_pad);
      out_puts(out, str_number, item_len);
      out_putc(out, form->pad, pad_len);
    }

    if (form->alligment == RIGHT_ALIGNMENT) {
      if (form->pad == SPACE_PAD) {
        out_putc(out, form->pad, pad_len);
        out_puts(out, prefix, prefix_len);
        out_putc(out, ZERO_PAD, precision_pad);
        out_puts(out, str_number, item_len);
      }
      if (form->pad == ZERO_PAD) {
        out_puts(out, prefix, prefix_len);
        out_putc(out, ZERO_PAD, pad_len);
        out_putc(out, ZERO_PAD, precision_pad);
        out_puts(out, str_number, item_len);
      }
    }

    return out->count_chars - writed_chars;
  }

  if (form->spec == 'f' || form->spec == 'e' || form->spec == 'E' ||
      form->spec == 'g' || form->spec == 'G') {
    long double f_number = 0.;
    if (form->length == 'L')
      f_number = (long double)va_arg(*ap, long double);
    else
      f_number = (long double)va_arg(*ap, double);

    const int neg_sign = signbit(f_number);

    int exp10 = 0;
    char orig = form->spec;
    int upper = (orig == 'E' || orig == 'G');

    if (isnan(f_number) || isinf(f_number)) {
      char str[5] = {0};
      int str_len = 0;

      if (neg_sign)
        s21_memcpy(str, "-", ++str_len);
      else if (HAS_FLAG(form->flags, F_PLUS) && !neg_sign)
        s21_memcpy(str, "+", ++str_len);
      else if (HAS_FLAG(form->flags, F_SPACE) && !neg_sign)
        s21_memcpy(str, " ", ++str_len);

      if (isnan(f_number)) s21_memcpy(&str[str_len], upper ? "NAN" : "nan", 3);

      if (isinf(f_number)) s21_memcpy(&str[str_len], upper ? "INF" : "inf", 3);

      str_len += 3;

      pad_len = max_len(0, form->width - str_len);

      if (form->alligment == LEFT_ALIGNMENT) {
        out_puts(out, str, str_len);
        out_putc(out, SPACE_PAD, pad_len);
      }

      if (form->alligment == RIGHT_ALIGNMENT) {
        out_putc(out, SPACE_PAD, pad_len);
        out_puts(out, str, str_len);
      }

      return pad_len + str_len;
    }

    if (neg_sign) f_number = fabsl(f_number);

    int g_mode = (orig == 'g' || orig == 'G');
    int g_precision = form->has_precision ? form->precision : 6;
    if (g_precision > 19) g_precision = 19;
    if (g_mode && g_precision == 0) g_precision = 1;

    if (!form->has_precision) form->precision = 6;

    exp10 = (f_number == 0.0L) ? 0 : (int)floorl(log10l(f_number));

    if (g_mode) {
      int exp10_round = exp10;
      if (f_number != 0.0L) {
        long double norm = f_number / powl(10, exp10);
        long double scale_sig = 1.0L;
        for (int i = 0; i < g_precision - 1; i++) scale_sig *= 10.0L;
        long double scaled_sig = norm * scale_sig;
        unsigned long long intpart_num_sig = round_scaled_value(scaled_sig);
        long double rounded_norm = (long double)intpart_num_sig / scale_sig;
        if (rounded_norm >= 10.0L) {
          rounded_norm /= 10.0L;
          exp10_round++;
        }
      }

      int use_exp = (exp10_round < -4 || exp10_round >= g_precision);

      if (use_exp) {
        form->spec = (orig == 'G') ? 'E' : 'e';
        form->precision = g_precision - 1;
        exp10 = exp10_round;
      } else {
        form->spec = 'f';
        form->precision = g_precision - (exp10_round + 1);
        if (form->precision < 0) form->precision = 0;
      }
      form->has_precision = 1;
    }

    if (form->precision > 19) form->precision = 19;

    if (form->spec == 'e' || form->spec == 'E') {
      f_number /= powl(10, exp10);
    }

    unsigned long long scale = 1;
    for (int i = 0; i < form->precision; i++) scale *= 10ULL;

    long double scaled = f_number * (long double)scale;

    if (scaled > (long double)ULLONG_MAX) {
      while (form->precision > 0 && scaled > (long double)ULLONG_MAX) {
        form->precision--;
        scale /= 10ULL;
        scaled = f_number * (long double)scale;
      }
    }

    unsigned long long intpart_num = round_scaled_value(scaled);

    unsigned long long int_part = intpart_num / scale;
    unsigned long long frac_part = intpart_num % scale;

    if (form->spec == 'e' || form->spec == 'E') {
      if (int_part == 10) {
        int_part = 1;
        exp10++;
      }
    }

    char str_number[128] = {0};
    item_len = s21_number_to_str(str_number, int_part, 10, 0);

    int precision_pad = 0;

    if (form->precision > 0) {
      str_number[item_len++] = '.';
      char frac_number[64] = {0};
      int frac_len = s21_number_to_str(frac_number, frac_part, 10, 0);
      precision_pad = max_len(0, form->precision - frac_len);
      s21_memset(str_number + item_len, ZERO_PAD, precision_pad);
      s21_memcpy(str_number + item_len + precision_pad, frac_number,
                 min_len(frac_len, form->precision));
      item_len += precision_pad + min_len(frac_len, form->precision);
    }

    if (HAS_FLAG(form->flags, F_HASH) && form->precision == 0)
      str_number[item_len++] = '.';

    if (form->spec == 'e' || form->spec == 'E') {
      char exp_str[20] = {0};
      if (form->spec == 'e')
        s21_strncpy(exp_str, exp10 < 0 ? "e-00" : "e+00", 4);
      if (form->spec == 'E')
        s21_strncpy(exp_str, exp10 < 0 ? "E-00" : "E+00", 4);

      unsigned exp_abs = (unsigned)(exp10 < 0 ? -exp10 : exp10);
      char* ptr_exp = exp_abs < 10 ? exp_str + 3 : exp_str + 2;
      s21_number_to_str(ptr_exp, exp_abs, 10, 0);
      int len_exp = (int)s21_strlen(exp_str);
      s21_memcpy(&str_number[item_len], exp_str, len_exp);
      item_len += len_exp;
    }

    char sign = 0;
    int len_sign = 0;

    if (neg_sign) {
      sign = '-';
      len_sign++;
    } else if (HAS_FLAG(form->flags, F_PLUS) && !neg_sign) {
      sign = '+';
      len_sign++;
    } else if (HAS_FLAG(form->flags, F_SPACE) && !neg_sign) {
      sign = ' ';
      len_sign++;
    }

    int total_len = len_sign + item_len;

    if (g_mode && !HAS_FLAG(form->flags, F_HASH)) {
      int exp_len = 0;
      char* start_str = s21_strchr(str_number, '.');
      if (start_str) {
        int has_trim = 0;
        char exp_temp[20] = {0};
        char* exp_str = &str_number[s21_strcspn(str_number, "eE")];
        char* end_str = exp_str - 1;
        int e_mode = (*exp_str == 'e' || *exp_str == 'E');
        if (e_mode) {
          exp_len = (int)s21_strlen(exp_str);
          s21_memcpy(exp_temp, exp_str, exp_len);
        }
        while ((*end_str == '0' || *end_str == '.') && end_str >= start_str) {
          *end_str-- = '\0';
          has_trim = 1;
        }
        if (e_mode && has_trim) s21_strncat(end_str, exp_temp, exp_len);
        item_len = (int)s21_strlen(str_number);
        total_len = len_sign + item_len;
      }
    }

    pad_len = max_len(0, form->width - total_len);

    if (form->alligment == LEFT_ALIGNMENT) {
      out_putc(out, sign, len_sign);
      out_puts(out, str_number, item_len);
      out_putc(out, form->pad, pad_len);
    }

    if (form->alligment == RIGHT_ALIGNMENT) {
      if (HAS_FLAG(form->flags, F_ZERO)) {
        out_putc(out, sign, len_sign);
        out_putc(out, form->pad, pad_len);
      } else {
        out_putc(out, form->pad, pad_len);
        out_putc(out, sign, len_sign);
      }
      out_puts(out, str_number, item_len);
    }

    return total_len + pad_len;
  }

  return 0;
}

int s21_sprintf(char* str, const char* format, ...) {
  va_list ap;
  fmt_t input_form = {0};
  out_t output_form = {.ptr_str = str};
  va_start(ap, format);
  while (*format) {
    if (*format == START_FMT) {
      format = parse_fmt(++format, &ap, &input_form);
      if (!format) break;
      handle_spec(&input_form, &ap, &output_form);
    } else
      output_form.ptr_str[output_form.count_chars++] = *format++;
  }
  va_end(ap);
  output_form.ptr_str[output_form.count_chars] = '\0';
  return output_form.count_chars;
}
