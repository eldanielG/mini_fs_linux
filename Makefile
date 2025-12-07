CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/filesystem.c
OBJ = $(SRC:.c=.o)
TARGET = mini_fs

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean