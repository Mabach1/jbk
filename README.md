# JBK - de/compression of TGA files
This is a repository for JBK, my semester project for Introduction To Programming course at VSB-TUO.

## JBK
JBK is simple CLI application for de/compressing TGA files from/into jbk format. 

## Build
```
$ make
```
## Usage
### Compression
```
$ ./bin/jbk compress --max-diff 2 --input example.tga --output out.jbk --block-size 4
```
Here in this example we're compressing file `example.tga` into a compressed file `out.jbk.` We're compressing with maximum pixel difference of `2` and we're checking pixel values on scale of block `4x4`.
### Decompression
```
$ ./bin/jbk decompress --input out.jbk --output decompressed.tga
```
In this example we're decompressing file `out.jbk`, back into a TGA file `decompressed.tga`

## Resources
* Introduction To Programming Scripts: https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/tga.html?highlight=tga#tga
* TGA File Format Specification: https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
* Creating TGA Image files (Paul Bourke, 1996): http://www.paulbourke.net/dataformats/tga/
* Data padding:
  * Structure Member Alignment, Padding, and Data Packing: https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/
  * GCC Structure Packing Pragmas: https://gcc.gnu.org/onlinedocs/gcc-4.9.3/gcc/Structure-Packing-Pragmas.html#Structure-Packing-Pragmas