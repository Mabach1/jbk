#ifndef __JBK_USER_INTERFACE__
#define __JBK_USER_INTERFACE__

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tga.h"

typedef enum { COMPRESS, DECOMPRESS, INFO } JBK_Action;

typedef struct _CompressingArgs {
    char *input;
    char *output;
    int block_size;
    int max_diff;
} CompressArgs;

typedef struct _DecompressArgs {
    char *input;
    char *output;
} DecompressArgs;

CompressArgs compress_args_slurp(int argc, const char **argv);
void compress_args_free(CompressArgs *args);

DecompressArgs decompress_args_slurp(int argc, const char **argv);
void decompress_args_free(DecompressArgs *args);

JBK_Action jbk_choose_action(const char *fst_arg);
void jbk_show_info(JBK_Action action, CompressArgs *ca, DecompressArgs *da);

#endif /* __JBK_USER_INTERFACE__ */
