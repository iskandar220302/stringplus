CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
DEBUG_FLAGS = -g -O0
GCOV_FLAGS = --coverage
SANITIZE = -fsanitize=address
SANITIZE_FLAGS = -Wno-deprecated-declarations

AR = ar
ARFLAGS = rcs
RM = rm -f

LIB = s21_string.a

SRC = $(wildcard s21_*.c)
OBJ = $(SRC:.c=.o)

TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_BIN = s21_tests

UNAME_S := $(shell uname)

# pkg-config для check (macOS/Linux)
PKG_CHECK_CFLAGS := $(shell pkg-config --cflags check 2>/dev/null || echo "")
PKG_CHECK_LIBS := $(shell pkg-config --libs check 2>/dev/null || echo "-lcheck -lm -lpthread")

ifeq ($(UNAME_S),Linux)
    CHECK_LIBS_FALLBACK = -lcheck -lm -lpthread -lsubunit
else
    CHECK_LIBS_FALLBACK = -lcheck -lm -lpthread
endif

.PHONY: all clean test gcov_report valgrind rebuild format test_debug memory_leaks test_asan docker-test

# ================= BUILD =================
all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# ================= TEST =================
test: $(LIB)
	@if [ -z "$(TEST_SRC)" ]; then \
		echo "No tests found in $(TEST_DIR)"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(PKG_CHECK_CFLAGS) $(TEST_SRC) $(LIB) $(PKG_CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)

# ================= DEBUG =================
test_debug:
	$(CC) $(DEBUG_FLAGS) $(PKG_CHECK_CFLAGS) $(TEST_SRC) $(SRC) $(PKG_CHECK_LIBS) -o $(TEST_BIN)
	gdb ./$(TEST_BIN)

# ================= COVERAGE =================
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(PKG_CHECK_CFLAGS) $(SRC) $(TEST_SRC) $(PKG_CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)
	mkdir -p gcov_report

ifeq ($(UNAME_S),Linux)
	lcov -t "s21_string" -o gcov_report/coverage.info -c -d .
	genhtml -o gcov_report gcov_report/coverage.info
else
	gcovr -r . --html --html-details -o gcov_report/index.html
endif
	@echo "Coverage report: gcov_report/index.html"

# ================= MEMORY CHECK =================
memory_leaks: $(TEST_BIN)
	@echo "=== Memory Leaks Check (macOS leaks) ==="
	leaks --atExit -- ./${TEST_BIN}

memory_debug: test_debug
	@echo "=== Memory Leaks with stack trace ==="
	MallocStackLogging=1 leaks --atExit -- ./${TEST_BIN}

valgrind: test
ifeq ($(UNAME_S),Linux)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TEST_BIN)
else
	$(MAKE) memory_leaks
endif

# ================= ADDRESS SANITIZER =================
test_asan:
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) $(SANITIZE) $(PKG_CHECK_CFLAGS) $(TEST_SRC) $(SRC) $(PKG_CHECK_LIBS) -o $(TEST_BIN)
	ASAN_OPTIONS=abort_on_error=1:detect_leaks=1 ./$(TEST_BIN)

test_asan_leaks: test_asan
	leaks --atExit -- ./${TEST_BIN}

# ================= CLEAN =================
clean:
	$(RM) $(OBJ) $(LIB) $(TEST_BIN)
	$(RM) *.gcno *.gcda *.gcov gmon.out
	rm -rf gcov_report

# ================= REBUILD =================
rebuild: clean all

# ================= STYLE =================
format:
	clang-format -style=Google -i *.h $(SRC) $(TEST_SRC)

# ================= DOCKER =================
docker-test:
	docker build -t s21_string_tests .
	docker run --rm s21_string_tests make test

# ================= CPPCHECK =================
cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 *.c *.h

# ================= CLANG-TIDY =================
clang-tidy:
	clang-tidy $(SRC) $(TEST_SRC) -- -std=c11 -Wall

# ================= COMPILE COMMANDS =================
compile_commands.json: clean
	$(CC) -c $(CFLAGS) $(SRC) --target=$(shell $(CC) -dumpmachine) -MJ compile_commands.json.tmp
	sed '1s/^/[\n/' compile_commands.json.tmp | sed '$$s/,$/]/' > compile_commands.json
	$(RM) compile_commands.json.tmp
