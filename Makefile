CC = gcc
CFLAGS = -Wall -Iincludes -fprofile-arcs -ftest-coverage
LIBS = -lsqlite3 -lcunit
TARGET = bin/music_salon

SRCS = src/main.c src/auth.c src/db.c src/cd.c src/sale.c src/report.c src/utils.c
OBJS = $(SRCS:src/%.c=build/%.o)

# Тесты
TEST_DIR = tests
TEST_SRCS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_BINS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_DIR)/%)

.PHONY: all clean test coverage help

all: $(TARGET)

# Основное приложение
$(TARGET): $(OBJS)
	mkdir -p bin
	$(CC) -o $@ $^ $(LIBS) -fprofile-arcs -ftest-coverage

# Компиляция исходников
build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка каждого теста отдельно
$(TEST_DIR)/test_%: $(TEST_DIR)/test_%.c $(filter-out build/main.o, $(OBJS))
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Запуск всех тестов
test: $(TEST_BINS)
	@echo "\n=== Running all tests ===\n"
	@for t in $(TEST_BINS); do \
		echo "\n-------- $$t --------"; \
		./$$t; \
	done

# Проверка покрытия кода (локально)
coverage: test
	@echo "\n=== Generating coverage report ===\n"
	gcov -pb src/*.c
	lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch
	lcov --remove coverage.info 'tests/*' '/usr/*' '*/includes/*' --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	@echo "\nCoverage report generated in coverage/index.html"

# Очистка
clean:
	rm -rf build build_tests bin coverage $(TEST_BINS) *.gcov *.gcda *.gcno coverage.info

# Справка
help:
	@echo "Available targets:"
	@echo "  make        - Build the main application"
	@echo "  make test   - Build and run all unit tests"
	@echo "  make coverage - Generate test coverage report (gcov + lcov)"
	@echo "  make clean  - Remove all build artifacts"
	@echo "  make help   - Show this help"
