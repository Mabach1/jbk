/**
 * @file        jbk_for_tga.h
 * @author      Matej Baliga, FEI VSB-TUO; BAL0312 (matej.baliga.stv@vsb.cz)
 * @brief       Argument parser for JBK (de)compression of TGA files
 * @date        2023-11-15
 *
 * @copyright   Copyright (c) 2023
 *
 */
#ifndef __JBK_ARGUMENT_PARSER__
#define __JBK_ARGUMENT_PARSER__

#include <stdio.h>      /* fprintf() (for displaying potential errors) */
#include <stdlib.h>     /* malloc(), free */
#include <string.h>     /* strcmp(), strcpy */
#include <assert.h>     /* assert() */
#include <stdbool.h>    /* true, false */

typedef enum { UNRECOGNIZED_ACTION, COMPRESS, DECOMPRESS } JBK_Action;

/**
 * @brief   returns wheter we are gonna be compressing/decompressing
 *
 * @param   fst_arg first argument from argv
 * @return  JBK_Action
 */
JBK_Action jbk_choose_action(const char *fst_arg);

typedef struct _CompressingArgs {
    char *input;
    char *output;
    int block_size;
    int max_diff;
} CompressArgs;

/**
 * @brief   returns a struct of arguments needed for `commpression`
 * 
 * @param   argc
 * @param   argv 
 * @return  CompressArgs 
 */
CompressArgs compress_args_slurp(int argc, const char **argv);
/**
 * @brief   freeing of compression argumnts
 * @param   args 
 */
void compress_args_free(CompressArgs *args);

typedef struct _DecompressArgs {
    char *input;
    char *output;
} DecompressArgs;

/**
 * @brief   returns a struct of arguments needed for `decompression`
 * 
 * @param   argc 
 * @param   argv 
 * @return  DecompressArgs 
 */
DecompressArgs decompress_args_slurp(int argc, const char **argv);
/**
 * @brief   freeing of decompression arguments
 * @param   args 
 */
void decompress_args_free(DecompressArgs *args);

#endif /* __JBK_ARGUMENT_PARSER__ */
