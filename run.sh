#! /usr/bin/bash

gcc -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o bin/main ./src/*.c && ./bin/main
