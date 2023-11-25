#!/bin/bash

set -xe

clear

if [ $# -gt 0 ]; then
    gcc -O3 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o ./bin/jbk -D $1 src/*.c
else
    gcc -O3 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o ./bin/jbk src/*.c
fi

