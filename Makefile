CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address
SRC_DIR = ./src
BIN_DIR = ./bin
EXECUTABLE = $(BIN_DIR)/jbk
SOURCES = $(wildcard $(SRC_DIR)/*.c)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXECUTABLE)
