CC = gcc
CFLAGS = -Wall -Iincludes -fprofile-arcs -ftest-coverage
LIBS = -lsqlite3 -lcunit
TARGET = bin/music_salon

SRCS = src/main.c src/auth.c src/db.c src/cd.c src/sale.c src/report.c src/utils.c
OBJS = $(SRCS:src/%.c=build/%.o)

# Тесты
TEST_DIR = tests
TEST_BIN = $(TEST_DIR)/run_all_tests
TEST_SRCS = $(TEST_DIR)/test_auth.c $(TEST_DIR)/test_cd.c $(TEST_DIR)/test_sale.c $(TEST_DIR)/test_report.c $(TEST_DIR)/test_utils.c
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=build_tests/%.o)

all: $(TARGET)

# Основное приложение
$(TARGET): $(OBJS)
	mkdir -p bin
	$(CC) -o $@ $^ $(LIBS) -fprofile-arcs -ftest-coverage

build/%.o: src/%.c includes/%.h
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/utils.o: src/utils.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка тестов
$(TEST_BIN): $(TEST_OBJS) $(filter-out build/main.o, $(OBJS))
	mkdir -p $(TEST_DIR)
	$(CC) -o $@ $^ $(LIBS)

build_tests/%.o: $(TEST_DIR)/%.c includes/%.h
	mkdir -p build_tests
	$(CC) $(CFLAGS) -c $< -o $@

# Запуск тестов
test: $(TEST_BIN)
	./$(TEST_BIN)

# Покрытие кода
coverage: test
	gcov -pb src/*.c
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

clean:
	rm -rf build build_tests bin coverage *.gcov *.gcda *.gcno coverage.info

.PHONY: all clean test coverage
