#! /usr/bin/bash

set -x

if gcc -O2 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -o bin/jbk ./src/*.c; then
	#./bin/main decompress --input examples/compressed/test.jbk --output examples/images/test.tga
	#./bin/main compress --input examples/images/carmack.tga --output examples/compressed/test.jbk --max-diff 50 --block-size 4
else
	echo "Compilation failed."
fi

