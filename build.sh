#! /usr/bin/bash

set -x

clear

if gcc -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o bin/main ./src/*.c; then
	#./bin/main compress a a a a a a a a 
	./bin/main decompress --input examples/compressed/test.jbk --output examples/images/test.tga
	#./bin/main compress --input examples/images/carmack.tga --output examples/compressed/test.jbk --max-diff 70 --block-size 64
else
	echo "Compilation failed."
fi

