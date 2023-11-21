# JBK compression for TGA files
This is a repository for JBK, my semestrial project for Introduction To Programming course on VSB-TUO.

## JBK
JBK is simple CLI application for compressing and decompressing of TGA files. 

## Usage
### Build
```
$ make
```
### Compression
```
$ ./bin/jbk compress --max-diff 2 --input example.tga --output out.jbk --block-size 4
```
### Decompression
```
$ ./bin/jbk decompress --input out.jbk --output decompressed.tga
```

## Resources
* UPR scrips: https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/tga.html?highlight=tga#tga
* File format specification of TGA : https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
* Creating TGA Image files (Pual Bourke, 1996): http://www.paulbourke.net/dataformats/tga/
* Data padding: 
  * https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/ 
  * https://gcc.gnu.org/onlinedocs/gcc-4.9.3/gcc/Structure-Packing-Pragmas.html#Structure-Packing-Pragmas
