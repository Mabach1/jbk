#ifndef __JBK_USER_INTERFACE__
#define __JBK_USER_INTERFACE__

#include "tga.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <assert.h>     
#include <stdbool.h>    

typedef enum { COMPRESS, DECOMPRESS } JBK_Action;

JBK_Action jbk_choose_action(const char *fst_arg);

typedef struct _CompressingArgs {
    char *input;
    char *output;
    int block_size;
    int max_diff;
} CompressArgs;

CompressArgs compress_args_slurp(int argc, const char **argv);
void compress_args_free(CompressArgs *args);
void jbk_compress_show_info(CompressArgs *args);
void jbk_compress_check_args(CompressArgs *args, TGA_File *tga);

typedef struct _DecompressArgs {
    char *input;
    char *output;
} DecompressArgs;

DecompressArgs decompress_args_slurp(int argc, const char **argv);
void decompress_args_free(DecompressArgs *args);
void jbk_decompress_show_info(DecompressArgs *args);

#endif /* __JBK_USER_INTERFACE__ */
