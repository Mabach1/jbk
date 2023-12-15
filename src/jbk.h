/**
 * @file        jbk.h
 * @author      Matej Baliga, FEI VSB-TUO, BAL0312 (matej.baliga.stv@vsb.cz)
 * @brief       De/compression algorithm and handling of jbk files
 * @date        2024-1-2
 *
 * @copyright   Copyright (c) 2024
 *
 */
#ifndef __JBK_H__
#define __JBK_H__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tga.h"
#include "jbk_ui.h"

typedef struct _JBK_Pixel {
    Pixel pixel;
    uint8_t len;
} JBK_Pixel;

typedef struct _JBK_Header {
    uint16_t block_size;
    uint32_t len;
} JBK_Header;

typedef struct _JBK_File {
    TGA_Header tga_header;
    JBK_Header jbk_header;
    JBK_Pixel *image;
} JBK_File;

/**
 * @brief   Returns array of compressed pixels
 *
 * @param   tga_file
 * @param   block_size
 * @param   max_compress_difference
 * @param   alen pointer to variable that will hold the number of compressed pixels
 * @param   compress_flag whether or not we'll be compressing over maximum value of uin8_t (default value: false)
 * @return  JBK_Pixel*
 */
JBK_Pixel *jbk_compress_tga(TGA_File *tga_file, int block_size, int max_compress_difference, uint32_t *alen, bool compress_flag);

/**
 * @brief   decompress jbk file into a tga file
 *
 * @param   jbk_file
 * @return  TGA_File
 */
TGA_File jbk_decompress_to_tga(const JBK_File *jbk_file);

/**
 * @brief   save jbk file
 *
 * @param   filename for jbk file
 * @param   image compressed pixels
 * @param   tga_file for information to save to
 * @param   block_size
 * @param   len number of compressed pixels
 */
void jbk_save_file(const char *filename, JBK_Pixel *image, const TGA_File *tga_file, const uint16_t block_size, const uint32_t len);

/**
 * @brief   loads jbk file into struct
 *
 * @param   filename
 * @return  JBK_File
 */
JBK_File jbk_open_file(const char *filename);

/**
 * @brief   close jbk file, needs to be called when being done working with jbk file
 *
 * @param   file
 */
void jbk_close_file(JBK_File *file);

#endif /* JBK_H */
