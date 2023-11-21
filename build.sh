#! /usr/bin/bash

set -x

clear

if gcc -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o bin/jbk ./src/*.c; then
	#./bin/jbk decompress --input examples/compressed/test.jbk --output examples/images/test.tga
	#./bin/jbk compress --input examples/images/carmack.tga --output examples/compressed/test.jbk --max-diff 120 --block-size 128
else
	echo "Compilation failed."
fi

