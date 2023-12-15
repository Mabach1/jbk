#include "jbk.h"

#define BLOCK_POS(i, j, k, l, w) l + (w * k) + j + (i * w)

int pixel_diff(Pixel p1, Pixel p2) { return (abs(p1.blue - p2.blue) + (abs(p1.green - p2.green)) + (abs(p1.red - p2.red))); }

void compress_block(uint16_t block_starting_height, uint16_t block_starting_width, uint16_t width, JBK_Pixel *res, Pixel *image_buf, uint32_t *index, const CompressArgs *args) {
    bool origin_checked = false;
    int index_copy = *index;

    for (uint16_t i = 0; i < args->block_size; ++i) {
        for (uint16_t j = 0; j < args->block_size; ++j) {
            size_t pos = BLOCK_POS(block_starting_height, block_starting_width, i, j, width);

            if (!origin_checked && pos == (size_t)BLOCK_POS(block_starting_height, block_starting_width, 0, 0, width)) {
                origin_checked = true;
                continue;
            }

            // handling of integer overflow
            if (UINT8_MAX == res[index_copy].len) {
                index_copy += 1;

                if (args->compress_flag) {
                    res[index_copy].pixel = res[index_copy - 1].pixel;
                } else {
                    res[index_copy].pixel = image_buf[pos];
                }

                res[index_copy].len = 1;

                continue;
            }

            if (pixel_diff(res[index_copy].pixel, image_buf[pos]) <= args->max_diff) {
                res[index_copy].len += 1;
            } else {
                index_copy += 1;
                res[index_copy].pixel = image_buf[pos];
                res[index_copy].len = 1;
            }
        }
    }

    *index = index_copy;
}

JBK_Pixel *jbk_compress_tga(const TGA_File *tga_file, const CompressArgs *args, uint32_t *alen) {
    if (args->block_size <= 1 || (tga_file->header.width % args->block_size != 0) || (tga_file->header.height % args->block_size != 0)) {
        jbk_error("Invalid value (%d) of block size!", args->block_size);
        jbk_exit();
    }

    if (args->max_diff < 0) {
        jbk_error("Invalid value (%d) of max difference", args->max_diff);
        jbk_exit();
    }

    Pixel *buf = tga_file->image;

    uint16_t height = tga_file->header.height;
    uint16_t width = tga_file->header.width;

    JBK_Pixel *res = (JBK_Pixel *)malloc(sizeof(JBK_Pixel) * height * width);
    assert(res && "Allocation of compressed pixels failed!\n");

    uint32_t index = 0;

    for (uint16_t i = 0; i < height; i += args->block_size) {
        for (uint16_t j = 0; j < width; j += args->block_size) {
            // first pixel in the block
            res[index].pixel = buf[BLOCK_POS(i, j, 0, 0, width)];
            res[index].len = 1;

            compress_block(i, j, width, res, buf, &index, args);

            index += 1;
        }
    }

    *alen = index;

    res = realloc(res, sizeof(JBK_Pixel) * index);
    assert(res && "Reallocation of compressed pixels failed!\n");

    return res;
}

bool is_jbk_file(const char *filename) {
    const char *extension = ".jbk";

    for (int i = strlen(filename) - 1, j = strlen(extension) - 1; i >= (int)strlen(filename) - 4; --i) {
        if (extension[j--] != filename[i]) {
            return false;
        }
    }

    return true;
}

void jbk_save_file(const char *filename, JBK_Pixel *image, const TGA_File *tga_file, const uint16_t block_size, const uint32_t len) {
    if (!is_jbk_file(filename)) {
        jbk_error("File [%s] is not a jbk file", filename);
        jbk_exit();
    }

    FILE *file_ptr = fopen(filename, "wb");

    if (!file_ptr) {
        jbk_error("Couldn't open compressed file [%s]", filename);
        jbk_exit();
    }

    assert(fwrite(&tga_file->header, sizeof(TGA_Header), 1, file_ptr) == 1);
    assert(fwrite(&block_size, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fwrite(&len, sizeof(uint32_t), 1, file_ptr) == 1);

    assert(fwrite(image, sizeof(JBK_Pixel) * len, 1, file_ptr) == 1);

    fclose(file_ptr);
    free(image);
}

JBK_File jbk_open_file(const char *filename) {
    FILE *file_ptr = fopen(filename, "rb");

    if (!file_ptr) {
        jbk_error("Couldn't open compressed file [%s] for decompression", filename);
        jbk_exit();
    }

    JBK_File res = {0};

    assert(fread(&res.tga_header, sizeof(TGA_Header), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header.len, sizeof(uint32_t), 1, file_ptr) == 1);

    res.image = (JBK_Pixel *)malloc(sizeof(JBK_Pixel) * res.jbk_header.len);
    assert(fread(res.image, sizeof(JBK_Pixel), res.jbk_header.len, file_ptr) == res.jbk_header.len);

    fclose(file_ptr);

    return res;
}

void jbk_close_file(JBK_File *file) {
    if (file->image) {
        free(file->image);
    }
}

TGA_File jbk_decompress_to_tga(const JBK_File *jbk_file) {
    TGA_File res = {0};

    res.header = jbk_file->tga_header;
    res.image = (Pixel *)malloc(sizeof(Pixel) * res.header.width * res.header.width);
    assert(res.image && "Couldn't allocate memory for storing pixels!\n");

    uint32_t width = res.header.width;
    uint32_t height = res.header.height;
    uint16_t block_size = jbk_file->jbk_header.block_size;

    size_t index = 0;

    for (uint32_t i = 0; i < height; i += block_size) {
        for (uint32_t j = 0; j < width; j += block_size) {
            for (uint32_t k = 0; k < block_size; ++k) {
                for (uint32_t l = 0; l < block_size; ++l) {
                    if (jbk_file->image[index].len <= 0) {
                        ++index;
                    }
                    res.image[BLOCK_POS(i, j, k, l, width)] = jbk_file->image[index].pixel;
                    jbk_file->image[index].len -= 1;
                }
            }
        }
    }

    return res;
}
