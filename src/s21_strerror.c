#include "s21_string.h"

#ifdef __linux__  // Linux
static const char* const s21_errlist[] = {
    "Success",                                            // 0
    "Operation not permitted",                            // 1
    "No such file or directory",                          // 2
    "No such process",                                    // 3
    "Interrupted system call",                            // 4
    "Input/output error",                                 // 5
    "No such device or address",                          // 6
    "Argument list too long",                             // 7
    "Exec format error",                                  // 8
    "Bad file descriptor",                                // 9
    "No child processes",                                 // 10
    "Resource temporarily unavailable",                   // 11
    "Cannot allocate memory",                             // 12
    "Permission denied",                                  // 13
    "Bad address",                                        // 14
    "Block device required",                              // 15
    "Device or resource busy",                            // 16
    "File exists",                                        // 17
    "Invalid cross-device link",                          // 18
    "No such device",                                     // 19
    "Not a directory",                                    // 20
    "Is a directory",                                     // 21
    "Invalid argument",                                   // 22
    "Too many open files in system",                      // 23
    "Too many open files",                                // 24
    "Inappropriate ioctl for device",                     // 25
    "Text file busy",                                     // 26
    "File too large",                                     // 27
    "No space left on device",                            // 28
    "Illegal seek",                                       // 29
    "Read-only file system",                              // 30
    "Too many links",                                     // 31
    "Broken pipe",                                        // 32
    "Numerical argument out of domain",                   // 33
    "Numerical result out of range",                      // 34
    "Resource deadlock avoided",                          // 35
    "File name too long",                                 // 36
    "No locks available",                                 // 37
    "Function not implemented",                           // 38
    "Directory not empty",                                // 39
    "Too many levels of symbolic links",                  // 40
    "Unknown error 41",                                   // 41
    "No message of desired type",                         // 42
    "Identifier removed",                                 // 43
    "Channel number out of range",                        // 44
    "Level 2 not synchronized",                           // 45
    "Level 3 halted",                                     // 46
    "Level 3 reset",                                      // 47
    "Link number out of range",                           // 48
    "Protocol driver not attached",                       // 49
    "No CSI structure available",                         // 50
    "Level 2 halted",                                     // 51
    "Invalid exchange",                                   // 52
    "Invalid request descriptor",                         // 53
    "Exchange full",                                      // 54
    "No anode",                                           // 55
    "Invalid request code",                               // 56
    "Invalid slot",                                       // 57
    "Unknown error 58",                                   // 58
    "Bad font file format",                               // 59
    "Device not a stream",                                // 60
    "No data available",                                  // 61
    "Timer expired",                                      // 62
    "Out of streams resources",                           // 63
    "Machine is not on the network",                      // 64
    "Package not installed",                              // 65
    "Object is remote",                                   // 66
    "Link has been severed",                              // 67
    "Advertise error",                                    // 68
    "Srmount error",                                      // 69
    "Communication error on send",                        // 70
    "Protocol error",                                     // 71
    "Multihop attempted",                                 // 72
    "RFS specific error",                                 // 73
    "Bad message",                                        // 74
    "Value too large for defined data type",              // 75
    "Name not unique on network",                         // 76
    "File descriptor in bad state",                       // 77
    "Remote address changed",                             // 78
    "Can not access a needed shared library",             // 79
    "Accessing a corrupted shared library",               // 80
    ".lib section in a.out corrupted",                    // 81
    "Attempting to link in too many shared libraries",    // 82
    "Cannot exec a shared library directly",              // 83
    "Invalid or incomplete multibyte or wide character",  // 84
    "Interrupted system call should be restarted",        // 85
    "Streams pipe error",                                 // 86
    "Too many users",                                     // 87
    "Socket operation on non-socket",                     // 88
    "Destination address required",                       // 89
    "Message too long",                                   // 90
    "Protocol wrong type for socket",                     // 91
    "Protocol not available",                             // 92
    "Protocol not supported",                             // 93
    "Socket type not supported",                          // 94
    "Operation not supported",                            // 95
    "Protocol family not supported",                      // 96
    "Address family not supported by protocol",           // 97
    "Address already in use",                             // 98
    "Cannot assign requested address",                    // 99
    "Network is down",                                    // 100
    "Network is unreachable",                             // 101
    "Network dropped connection on reset",                // 102
    "Software caused connection abort",                   // 103
    "Connection reset by peer",                           // 104
    "No buffer space available",                          // 105
    "Transport endpoint is already connected",            // 106
    "Transport endpoint is not connected",                // 107
    "Cannot send after transport endpoint shutdown",      // 108
    "Too many references: cannot splice",                 // 109
    "Connection timed out",                               // 110
    "Connection refused",                                 // 111
    "Host is down",                                       // 112
    "No route to host",                                   // 113
    "Operation already in progress",                      // 114
    "Operation now in progress",                          // 115
    "Stale file handle",                                  // 116
    "Structure needs cleaning",                           // 117
    "Not a XENIX named type file",                        // 118
    "No XENIX semaphores available",                      // 119
    "Is a named type file",                               // 120
    "Remote I/O error",                                   // 121
    "Disk quota exceeded",                                // 122
    "No medium found",                                    // 123
    "Wrong medium type",                                  // 124
    "Operation canceled",                                 // 125
    "Required key not available",                         // 126
    "Key has expired",                                    // 127
    "Key has been revoked",                               // 128
    "Key was rejected by service",                        // 129
    "Owner died",                                         // 130
    "State not recoverable",                              // 131
    "Operation not possible due to RF-kill",              // 132
    "Memory page has hardware error"};                    // 133

#elif defined(__APPLE__) && defined(__MACH__)
static const char* const s21_errlist[] = {
    "Undefined error: 0",                               // 0
    "Operation not permitted",                          // 1
    "No such file or directory",                        // 2
    "No such process",                                  // 3
    "Interrupted system call",                          // 4
    "Input/output error",                               // 5
    "Device not configured",                            // 6
    "Argument list too long",                           // 7
    "Exec format error",                                // 8
    "Bad file descriptor",                              // 9
    "No child processes",                               // 10
    "Resource deadlock avoided",                        // 11
    "Cannot allocate memory",                           // 12
    "Permission denied",                                // 13
    "Bad address",                                      // 14
    "Block device required",                            // 15
    "Resource busy",                                    // 16
    "File exists",                                      // 17
    "Cross-device link",                                // 18
    "Operation not supported by device",                // 19
    "Not a directory",                                  // 20
    "Is a directory",                                   // 21
    "Invalid argument",                                 // 22
    "Too many open files in system",                    // 23
    "Too many open files",                              // 24
    "Inappropriate ioctl for device",                   // 25
    "Text file busy",                                   // 26
    "File too large",                                   // 27
    "No space left on device",                          // 28
    "Illegal seek",                                     // 29
    "Read-only file system",                            // 30
    "Too many links",                                   // 31
    "Broken pipe",                                      // 32
    "Numerical argument out of domain",                 // 33
    "Result too large",                                 // 34
    "Resource temporarily unavailable",                 // 35
    "Operation now in progress",                        // 36
    "Operation already in progress",                    // 37
    "Socket operation on non-socket",                   // 38
    "Destination address required",                     // 39
    "Message too long",                                 // 40
    "Protocol wrong type for socket",                   // 41
    "Protocol not available",                           // 42
    "Protocol not supported",                           // 43
    "Socket type not supported",                        // 44
    "Operation not supported",                          // 45
    "Protocol family not supported",                    // 46
    "Address family not supported by protocol family",  // 47
    "Address already in use",                           // 48
    "Can't assign requested address",                   // 49
    "Network is down",                                  // 50
    "Network is unreachable",                           // 51
    "Network dropped connection on reset",              // 52
    "Software caused connection abort",                 // 53
    "Connection reset by peer",                         // 54
    "No buffer space available",                        // 55
    "Socket is already connected",                      // 56
    "Socket is not connected",                          // 57
    "Can't send after socket shutdown",                 // 58
    "Too many references: can't splice",                // 59
    "Operation timed out",                              // 60
    "Connection refused",                               // 61
    "Too many levels of symbolic links",                // 62
    "File name too long",                               // 63
    "Host is down",                                     // 64
    "No route to host",                                 // 65
    "Directory not empty",                              // 66
    "Too many processes",                               // 67
    "Too many users",                                   // 68
    "Disc quota exceeded",                              // 69
    "Stale NFS file handle",                            // 70
    "Too many levels of remote in path",                // 71
    "RPC struct is bad",                                // 72
    "RPC version wrong",                                // 73
    "RPC prog. not avail",                              // 74
    "Program version wrong",                            // 75
    "Bad procedure for program",                        // 76
    "No locks available",                               // 77
    "Function not implemented",                         // 78
    "Inappropriate file type or format",                // 79
    "Authentication error",                             // 80
    "Need authenticator",                               // 81
    "Device power is off",                              // 82
    "Device error",                                     // 83
    "Value too large to be stored in data type",        // 84
    "Bad executable (or shared library)",               // 85
    "Bad CPU type in executable",                       // 86
    "Shared library version mismatch",                  // 87
    "Malformed Mach-o file",                            // 88
    "Operation canceled",                               // 89
    "Identifier removed",                               // 90
    "No message of desired type",                       // 91
    "Illegal byte sequence",                            // 92
    "Attribute not found",                              // 93
    "Bad message",                                      // 94
    "EMULTIHOP (Reserved)",                             // 95
    "No message available on STREAM",                   // 96
    "ENOLINK (Reserved)",                               // 97
    "No STREAM resources",                              // 98
    "Not a STREAM",                                     // 99
    "Protocol error",                                   // 100
    "STREAM ioctl timeout",                             // 101
    "Operation not supported on socket",                // 102
    "Policy not found",                                 // 103
    "State not recoverable",                            // 104
    "Previous owner died",                              // 105
    "Interface output queue is full",                   // 106
    "Capabilities insufficient",                        // 107
    "Unknown error: 108",                               // 108
    "Unknown error: 109",                               // 109
    "Unknown error: 110",                               // 110
    "Unknown error: 111",                               // 111
    "Unknown error: 112",                               // 112
    "Unknown error: 113",                               // 113
    "Unknown error: 114",                               // 114
    "Unknown error: 115",                               // 115
    "Unknown error: 116",                               // 116
    "Unknown error: 117",                               // 117
    "Unknown error: 118",                               // 118
    "Unknown error: 119",                               // 119
    "Unknown error: 120",                               // 120
    "Unknown error: 121",                               // 121
    "Unknown error: 122",                               // 122
    "Unknown error: 123",                               // 123
    "Unknown error: 124",                               // 124
    "Unknown error: 125",                               // 125
    "Unknown error: 126",                               // 126
    "Unknown error: 127",                               // 127
    "Unknown error: 128",                               // 128
    "Unknown error: 129",                               // 129
    "Unknown error: 130",                               // 130
    "Unknown error: 131",                               // 131
    "Unknown error: 132",                               // 132
    "Unknown error: 133",                               // 133
};
#else
// Если платформа неизвестна
#error "Unsupported platform"
#endif

#define S21_ERR_MAX ((int)(sizeof(s21_errlist) / sizeof(s21_errlist[0]) - 1))

static char* s21_int_to_str(char* buffer, long number) {
  if (number == 0) {
    *buffer++ = '0';
    return buffer;
  }
  if (number < 0) {
    *buffer++ = '-';
    number = -number;
  }
  if (number / 10 != 0) buffer = s21_int_to_str(buffer, number / 10);
  *buffer++ = (number % 10) + '0';
  return buffer;
}
char* s21_strerror(int errnum) {
#ifdef __linux__
  static char unknown_msg[64] = "Unknown error ";
  static const s21_size len_unknown_msg = 14;
#elif defined(__APPLE__) && defined(__MACH__)
  static char unknown_msg[64] = "Unknown error: ";
  static const s21_size len_unknown_msg = 15;
#endif

  if (errnum >= 0 && errnum <= S21_ERR_MAX) {
    return (char*)s21_errlist[errnum];
  } else {
    char* end = s21_int_to_str(unknown_msg + len_unknown_msg, (long)errnum);
    *end = '\0';

    return unknown_msg;
  }
}