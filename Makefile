CC = gcc
CFLAGS = -Wall -Iincludes
LIBS = -lsqlite3
TARGET = bin/music_salon

SRCS = src/main.c src/auth.c src/db.c src/cd.c src/sale.c src/report.c src/utils.c
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

# Создаём папки перед сборкой
$(TARGET): $(OBJS)
	mkdir -p bin
	$(CC) -o $@ $^ $(LIBS)

# Общее правило для .c файлов, у которых есть .h
build/%.o: src/%.c includes/%.h
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Отдельное правило для main.c (нет .h)
build/main.o: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Отдельное правило для utils.c (нет .h)
build/utils.o: src/utils.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

.PHONY: all clean
