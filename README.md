# JBK - compression of TGA files

## Table of Contents

- [JBK - compression of TGA files](#jbk---compression-of-tga-files)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [JBK](#jbk)
  - [Build](#build)
  - [Usage](#usage)
    - [Compression](#compression)
    - [Decompression](#decompression)
  - [Examples](#examples)
  - [COMPRESS\_OVER\_U8\_MAX](#compress_over_u8_max)
      - [Without the `COMPRESS_OVER_U8_MAX` flag](#without-the-compress_over_u8_max-flag)
      - [With the `COMPRESS_OVER_U8_MAX` flag](#with-the-compress_over_u8_max-flag)
  - [Resources](#resources)
  - [Other resources](#other-resources)

## Introduction

This repository houses JBK, a command-line interface application designed for de/compressing TGA files into/from JBK format. Developed as a semester project for the Introduction to Programming course at VSB-TUO.

## JBK

JBK is a simple CLI application that facilitates the compression and decompression of TGA files using the JBK format.

## Build
To build the JBK application, use the following command:
```
$ make
```
## Usage
### Compression
``` bash
$ ./bin/jbk compress --max-diff 2 --input example.tga --output out.jbk --block-size 4
```
or
``` bash
$ ./bin/jbk compress --max-diff 2 --input example.tga --output out.jbk --block-size 4 --COMPRESS_OVER_U8_MAX true
```
For more information about the `COMPRESS_OVER_U8_MAX` flag read the respective paragraph <br>

Here in this example we're compressing `example.tga` file into a compressed `out.jbk.` file. We're compressing with maximum pixel difference of `2` and checking pixel values on scale of block `4x4`. It is important to bear in mind the block size. Both the height and width must be dividable by the block size otherwise the file will not be compressed! But don't worry, if you don't want to calculate divisibility by your self, the application will do it for you.

### Decompression
``` bash
$ ./bin/jbk decompress --input out.jbk --output decompressed.tga
```
In this example we're decompressing file `out.jbk`, back into a TGA file `decompressed.tga`

## Examples
compression examples

Original image 
![original image](./examples/images/flowers.png)

image compressed with value of maximum difference 12 and block size 16 (approximately 49.2% of the original file size)
![not noticeable compression](./examples/images/flowers_low_compression.png)

image compressed with value of maximum difference 70 and block size 16 (approximately 7.9% of the original file size)
![compressed image](./examples/images/flower-compressed.png)

## COMPRESS_OVER_U8_MAX
There are two different possible implementation of JBK algorithm. You can see the difference your self by building the application with the `COMPRESS_OVER_U8_MAX`. <br>

The difference is in the 'length' calculation. When we're compressing we take a pixel and compare it with the one next to it. If they're similar, we increase the 'length' of the first pixel and compare it to the next one in the sequence. The issue arise when we reach length of 255, since this is the maximum of 8-bit integer which we use to store the length. In the first version of implementation, when we exceed the value of 255 the next pixel is the current one that was being compared, with the `COMPRESS_OVER_U8_MAX` flag the next pixel is the same as the original one. 

#### Without the `COMPRESS_OVER_U8_MAX` flag
![without flag](./examples/images/carmack_compressed_without_flag.png)
#### With the `COMPRESS_OVER_U8_MAX` flag
![with flag](./examples/images/carmack_with_flag.png)

## Resources
* [Introduction To Programming Scripts](https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/tga.html?highlight=tga#tga)
* [TGA File Format Specification](https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf)
* [Creating TGA Image files (Paul Bourke, 1996)](http://www.paulbourke.net/dataformats/tga/)
* Data padding:
  * [Structure Member Alignment, Padding, and Data Packing](https://www.geeksforgeeks.org/structure-member-alignment-padding-and-data-packing/)
  * [GCC Structure Packing Pragmas](https://gcc.gnu.org/onlinedocs/gcc-4.9.3/gcc/Structure-Packing-Pragmas.html#Structure-Packing-Pragmas)

## Other resources 
* [picture used for examples](https://filesamples.com/formats/tga)