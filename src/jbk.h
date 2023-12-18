/**
 * @file        jbk.h
 * @author      Matej Baliga, FEI VSB-TUO, BAL0312 (matej.baliga.stv@vsb.cz)
 * @brief       De/compression algorithm and handling of jbk files
 * @date        2024-1-2
 *
 * @copyright   Copyright (c) 2024
 *
 */
#ifndef JBK_JBK
#define JBK_JBK

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "jbk_ui.h"
#include "tga.h"

typedef struct _JBK_Pixel {
    Pixel pixel;
    uint8_t len;
} JBKPixel;

typedef struct _JBK_Header {
    uint16_t block_size;
    uint32_t len;
} JBKHeader;

typedef struct _JBK_File {
    TGAHeader tga_header;
    JBKHeader jbk_header;
    JBKPixel *image;
} JBKFile;

/**
 * @brief   Returns array of compressed pixels
 *
 * @param   tga_file
 * @param   block_size
 * @param   max_compress_difference
 * @param   alen pointer to variable that will hold the number of compressed pixels
 * @param   compress_flag whether or not we'll be compressing over maximum value of uin8_t (default value: false)
 * @return  JBKPixel*
 */
JBKPixel *jbk_compress_tga(const TGAFile *tga_file, const CompressArgs *args, uint32_t *alen);

/**
 * @brief   decompress jbk file into a tga file
 *
 * @param   jbk_file
 * @return  TGAFile
 */
TGAFile jbk_decompress_to_tga(const JBKFile *jbk_file);

/**
 * @brief   save jbk file
 *
 * @param   filename for jbk file
 * @param   image compressed pixels
 * @param   tga_file for information to save to
 * @param   block_size
 * @param   len number of compressed pixels
 */
void jbk_save_file(const char *filename, JBKPixel *image, const TGAFile *tga_file, const uint16_t block_size, const uint32_t len);

/**
 * @brief   loads jbk file into struct
 *
 * @param   filename
 * @return  JBKFile
 */
JBKFile jbk_open_file(const char *filename);

/**
 * @brief   close jbk file, needs to be called when being done working with jbk file
 *
 * @param   file
 */
void jbk_close_file(JBKFile *file);

#endif /* JBK_JBK */
