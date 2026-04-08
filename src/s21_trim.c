
#include "s21_string.h"

int is_trim_char(char c, const char* trim_chars) {
  if (!trim_chars) return 0;
  while (*trim_chars) {
    if (c == *trim_chars) return 1;
    trim_chars++;
  }
  return 0;
}

void* s21_trim(const char* src, const char* trim_chars) {
  if (src == S21_NULL) return S21_NULL;

  const char* default_chars = " \t\n\v\f\r";
  const char* del = (trim_chars == S21_NULL || *trim_chars == '\0')
                        ? default_chars
                        : trim_chars;

  s21_size len = s21_strlen(src);
  s21_size start = 0;
  s21_size end = len;

  while (start < end && is_trim_char(src[start], del)) {
    start++;
  }

  while (end > start && is_trim_char(src[end - 1], del)) {
    end--;
  }

  s21_size new_len = end - start;
  char* res = (char*)malloc(new_len + 1);

  if (res) {
    for (s21_size i = 0; i < new_len; i++) {
      res[i] = src[start + i];
    }
    res[new_len] = '\0';
  }

  return (void*)res;
}

// #include "s21_string.h"

// int is_trim_char(char c, const char* trim_chars) {
//   int flag = 0;
//   while (trim_chars) {
//     if (c != *trim_chars) {
//       flag = 1;
//     }
//     trim_chars++;
//     flag = 0;
//   }
//   return flag;
// }

// void* s21_trim(const char* src, const char* trim_chars) {
//   char* str_res = S21_NULL;

//   if (src == S21_NULL) {
//     str_res = S21_NULL;
//   } else {
//     s21_size curr_index = 0;
//     str_res = malloc(s21_strlen(src) + 1);

//     if (trim_chars == S21_NULL) {  // типо если трим пустой то печатаем
//     просто
//                                    // строку без изменений
//       for (curr_index = 0; src[curr_index] != '\0'; curr_index++) {
//         str_res[curr_index] = src[curr_index];
//       }

//       curr_index++;
//       str_res[curr_index] = '\0';
//     } else {
//       while (src[curr_index] != '\0') {
//         if ((src[curr_index] >= 'a' && src[curr_index] <= 'z') ||
//             (src[curr_index] >= 'A' && src[curr_index] <= 'Z')) {
//           str_res[curr_index] = src[curr_index];
//         } else if {
//         }
//       }
//     }
//   }
// }