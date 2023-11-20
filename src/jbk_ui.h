/**
 * @file        jbk_ui.h
 * @author      Matej Baliga, FEI VSB-TUO, BAL0312 (matej.baliga.stv@vsb.cz)
 * @brief       Basic user interface for working with JBK de/compressor
 * @date        2024-1-2
 * 
 * @copyright   Copyright (c) 2024
 * 
 */
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

/**
 * @brief   get arguments for `compression`
 * 
 * @param   argc 
 * @param   argv 
 * @return  CompressArgs 
 */
CompressArgs compress_args_slurp(int argc, const char **argv);

/**
 * @brief   free arguments for `compression`, needs to be called after being done using arguments
 * 
 * @param   args 
 */
void compress_args_free(CompressArgs *args);

/**
 * @brief   get arguments for `decompression`
 * 
 * @param   argc 
 * @param   argv 
 * @return  DecompressArgs 
 */
DecompressArgs decompress_args_slurp(int argc, const char **argv);

/**
 * @brief   free arguments for `decompression`, needs to be called after being done using arguments
 * 
 * @param   args 
 */
void decompress_args_free(DecompressArgs *args);

/**
 * @brief   accepts first argument and proceed with de/compression
 * 
 * @param   fst_arg 
 * @return  JBK_Action 
 */
JBK_Action jbk_choose_action(const char *fst_arg);

/**
 * @brief   will show info to corresponding arguments
 * 
 * If we compressing, we will pass compression arguments into the function and the other argument will be NULL
 * 
 * @param   action 
 * @param   compression_args (NULL) 
 * @param   decompression_args (NULL)
 */
void jbk_show_info(JBK_Action action, CompressArgs *compression_args, DecompressArgs *decompression_args);

#endif /* __JBK_USER_INTERFACE__ */