/**
 * @file        jbk_ui.h
 * @author      Matej Baliga, FEI VSB-TUO, BAL0312 (matej.baliga.stv@vsb.cz)
 * @brief       Basic user interface for working with JBK de/compressor
 * @date        2024-1-2
 *
 * @copyright   Copyright (c) 2024
 *
 */
#ifndef JBK_JBK_UI
#define JBK_JBK_UI

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief   displaying error messages
 *
 * @param   format
 * @param   ...
 */
void jbk_error(const char *format, ...);

/**
 * @brief   exiting with error message
 *
 */
void jbk_exit(void);

typedef enum JBKAction { COMPRESSING, DECOMPRESSING, INFO } JBKAction;

typedef struct _CompressingArgs {
    char *input;
    char *output;
    int block_size;
    int max_diff;
    bool compress_flag;
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
 * @return  JBKAction
 */
JBKAction jbk_choose_action(const char *fst_arg);

/**
 * @brief   will show info to corresponding arguments
 *
 * If we compressing, we will pass compression arguments into the function and the other argument will be NULL
 *
 * @param   action
 * @param   compression_args (can be optional, => NULL)
 * @param   decompression_args (can be optional, => NULL)
 */
void jbk_show_info(JBKAction action, CompressArgs *compression_args, DecompressArgs *decompression_args);

#endif /* JBK_JBK_UI */
