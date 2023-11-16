#! /usr/bin/bash

gcc -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o bin/main ./src/*.c && ./bin/main compress --max-diff 2 --input images/simple.tga --output out.jbk --block-size 4
