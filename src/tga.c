#include "tga.h"

TGA_Header tga_load_header(FILE *file_ptr) {
    TGA_Header header = {0};
    assert(fread(&header, sizeof(header), 1, file_ptr) == 1);
    return header;
}

Pixel *tga_load_image(const TGA_Header *header, FILE *file_ptr) {
    Pixel *pixels = (Pixel *)malloc(sizeof(Pixel) * header->width * header->height);
    assert(fread(pixels, sizeof(Pixel) * header->width * header->height, 1, file_ptr) == 1);
    return pixels;
}

bool is_tga_file(const char *filename) {
    const char *extension = ".tga";

    const int extension_len = strlen(extension);
    const int filename_len = strlen(filename);

    for (int i = filename_len - 1, j = extension_len - 1; i >= filename_len - extension_len; --i) {
        if (extension[j--] != filename[i]) {
            return false;
        }
    }

    return true;
}

TGA_File tga_open_file(const char *filename) {
    if (!is_tga_file(filename)) {
        jbk_error("File %s is not a TGA file", filename);
        jbk_exit();
    }

    FILE *file_ptr = fopen(filename, "rb");

    if (!file_ptr) {
        jbk_error("Couldn't open %s for compression", filename);
        jbk_exit();
    }

    TGA_File file = {0};

    file.header = tga_load_header(file_ptr);

    // check if its and uncompressed 24-bit or 32-bit TGA image
    if (file.header.image_type != 2 && file.header.image_type != 10) {
        jbk_error("Unsupported TGA format");
        jbk_exit();
    }

    file.image = tga_load_image(&file.header, file_ptr);

    fclose(file_ptr);

    return file;
}

void tga_save_file(const char *filename, const TGA_File *file) {
    FILE *file_ptr = fopen(filename, "wb");

    if (!file_ptr) {
        jbk_error("Couldn't save file %s", filename);
        jbk_exit();
    }

    fwrite(&file->header, sizeof(TGA_Header), 1, file_ptr);
    fwrite(file->image, sizeof(Pixel) * file->header.width * file->header.height, 1, file_ptr);

    fclose(file_ptr);
}

void tga_close_file(TGA_File *file) { 
    if (file->image) {
        free(file->image); 
    }
}
