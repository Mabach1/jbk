#ifndef __TGA_H__
#define __TGA_H__

#include <stdio.h>  
#include <stdlib.h> 
#include <assert.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct _TGA_Header {
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t image_type;
    uint16_t color_map_origin;
    uint16_t color_map_length;
    uint8_t color_map_depth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    uint8_t descriptor;
} TGA_Header;
#pragma pack(pop)

typedef struct _Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;

typedef struct _TGA_File {
    TGA_Header header;
    Pixel *image;
} TGA_File;

TGA_File tga_open_file(const char *filename);
void tga_close_file(TGA_File *file);

void tga_save_file(const char *filename, TGA_File *file);

#endif /* TGA_H */
