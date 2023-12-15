CC := gcc
CFLAGS := -g -O3 -Wall -Wextra -Wpedantic -Werror -fsanitize=address
SRC_DIR := src
BUILD_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.DEFAULT_GOAL := all

$(BUILD_DIR)/jbk: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm $(OBJS) -f
	@rm $(BUILD_DIR)/jbk -f

.PHONY: all
all: $(BUILD_DIR)/jbk

.PHONY: force-rebuild
force-rebuild: clean all

