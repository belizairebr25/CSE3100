CC = gcc

CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wshadow \
         -Wformat=2 -g3 -O0

TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
