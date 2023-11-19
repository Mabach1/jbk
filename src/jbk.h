/**
 * @file        jbk.h
 * @author      your name (you@domain.com)
 * @brief
 * @date        2023-11-16
 *
 * @copyright   Copyright (c) 2023
 *
 */
#ifndef __JBK_H__
#define __JBK_H__

#include "tga.h"

#include <stdio.h>  
#include <stdlib.h> 
#include <assert.h>
#include <stdint.h>

typedef struct _JBK_Pixel {
    Pixel pixel;
    uint8_t len;
} JBK_Pixel;

typedef struct _JBK_Header {
    uint16_t block_size;
    uint32_t size;
} JBK_Header;

typedef struct _JBK_File {
    TGA_Header tga_header;
    JBK_Header jbk_header;
    JBK_Pixel *image;
} JBK_File;

JBK_Pixel *jbk_compress_tga(TGA_File *tga_file, uint16_t block_size, uint8_t max_compress_difference, uint32_t *alen);
TGA_File jbk_decompress_to_tga(JBK_File *jbk_file);

void jbk_save_file(const char *filename, JBK_Pixel *image, TGA_File *tga_file, uint16_t block_size, uint32_t len);
JBK_File jbk_load_file(const char *filename);

#endif /* JBK_H */
