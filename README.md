# JBK compression for TGA files
This is a repository for JBK.

### JBK
JBK is simple CLI application for compressing and decompressing of TGA files

### Usage
#### Compression
```
$ ./bin/jbk compress --max-diff 2 --input example.tga --output out.jbk --block-size 4
```
#### Decompression
```
$ ./bin/jbk decompress --input out.jbk --output decompressed.tga
```

### Sources
* UPR scrips: https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/tga.html?highlight=tga#tga
* File format specification of TGA : https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
* Creating TGA Image files (Pual Bourke, 1996): http://www.paulbourke.net/dataformats/tga/
* Data padding: https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/ 
