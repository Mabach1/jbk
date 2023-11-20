#include "tga.h"

TGA_Header tga_load_header(FILE *file_ptr) {
    TGA_Header header = {0};
    assert(fread(&header, sizeof(header), 1, file_ptr) == 1);
    return header;
}

Pixel *tga_load_image(TGA_Header *header, FILE *file_ptr) {
    Pixel *pixels = (Pixel *)malloc(sizeof(Pixel) * header->width * header->height);
    assert(fread(pixels, sizeof(Pixel) * header->width * header->height, 1, file_ptr) == 1);
    return pixels;
}

TGA_File tga_open_file(const char *filename) {
    FILE *file_ptr = fopen(filename, "rb");
    assert(file_ptr && "Coudn't open file");

    TGA_File file = {0};

    file.header = tga_load_header(file_ptr);
    file.image = tga_load_image(&file.header, file_ptr);

    fclose(file_ptr);

    return file;
}

void tga_save_file(const char *filename, TGA_File *file) {
    FILE *file_ptr = fopen(filename, "wb");
    assert(file_ptr && "Coudn't open TGA file to save it!\n");

    fwrite(&file->header, sizeof(TGA_Header), 1, file_ptr);
    fwrite(file->image, sizeof(Pixel) * file->header.width * file->header.height, 1, file_ptr);

    fclose(file_ptr);
}

void tga_free(TGA_File *file) { free(file->image); }
