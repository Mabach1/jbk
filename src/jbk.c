#include "jbk.h"

#define BLOCK_POS(i, j, k, l, w) l + (w * k) + j + (i * w)

int pixel_diff(Pixel p1, Pixel p2) { return (abs(p1.blue - p2.blue) + (abs(p1.green - p2.green)) + (abs(p1.red - p2.red))); }

void compress_block(uint16_t block_starting_height, uint16_t block_starting_width, uint16_t width, JBKPixel *res, Pixel *image_buf, uint32_t *index, const CompressArgs *args) {
    bool origin_checked = false;
    uint32_t index_copy = *index;

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

void block_size_recommended(int height, int width) {
    fprintf(stderr, "+ Recommended sizes: ");

    for (int i = 1; i < 120; ++i) {
        if ((height % i == 0) && (width % i == 0)) {
            fprintf(stderr, "%d ", i);
        }
    }

    fprintf(stderr, "\n");
}

JBKPixel *jbk_compress_tga(const TGAFile *tga_file, const CompressArgs *args, uint32_t *alen) {
    if (args->block_size <= 1 || (tga_file->header.width % args->block_size != 0) || (tga_file->header.height % args->block_size != 0)) {
        jbk_error("Invalid value (%d) of block size!", args->block_size);
        block_size_recommended(tga_file->header.height, tga_file->header.width);
        jbk_exit();
    }

    if (args->max_diff <= 0) {
        jbk_error("Invalid value (%d) of max difference", args->max_diff);
        jbk_exit();
    }

    Pixel *buf = tga_file->image;

    uint16_t height = tga_file->header.height;
    uint16_t width = tga_file->header.width;

    JBKPixel *res = (JBKPixel *)malloc(sizeof(JBKPixel) * height * width);
    assert(res && "Allocation of compressed pixels failed!\n");

    uint32_t index = 0;

    for (uint16_t i = 0; i < height; i += args->block_size) {
        for (uint16_t j = 0; j < width; j += args->block_size) {
            // first pixel in the block
            res[index].pixel = buf[BLOCK_POS(i, j, 0, 0, width)];
            res[index].len = 1;

            compress_block(i, j, width, res, buf, &index, args);

            // at the end of block we automaticaly start a new sequence of pixels
            index += 1;
        }
    }

    *alen = index;

    res = realloc(res, sizeof(JBKPixel) * index);
    assert(res && "Reallocation of compressed pixels failed!\n");

    return res;
}

bool is_jbk_file(const char *filename) {
    const char *extension = ".jbk";

    const int extension_len = strlen(extension);
    const int filename_len = strlen(filename);

    for (int i = filename_len - 1, j = extension_len - 1; i >= filename_len - extension_len; --i) {
        if (extension[j--] != filename[i]) {
            return false;
        }
    }

    return true;
}

void jbk_save_file(const char *filename, JBKPixel *image, const TGAFile *tga_file, const uint16_t block_size, const uint32_t len) {
    if (!is_jbk_file(filename)) {
        jbk_error("File %s is not a jbk file", filename);
        jbk_exit();
    }

    FILE *file_ptr = fopen(filename, "wb");

    if (!file_ptr) {
        jbk_error("Couldn't open compressed file %s", filename);
        jbk_exit();
    }

    assert(fwrite(&tga_file->header, sizeof(TGAHeader), 1, file_ptr) == 1);
    assert(fwrite(&block_size, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fwrite(&len, sizeof(uint32_t), 1, file_ptr) == 1);

    assert(fwrite(image, sizeof(JBKPixel) * len, 1, file_ptr) == 1);

    fclose(file_ptr);
    free(image);
}

JBKFile jbk_open_file(const char *filename) {
    FILE *file_ptr = fopen(filename, "rb");

    if (!file_ptr) {
        jbk_error("Couldn't open compressed file %s for decompression", filename);
        jbk_exit();
    }

    JBKFile res = {0};

    assert(fread(&res.tga_header, sizeof(TGAHeader), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header.len, sizeof(uint32_t), 1, file_ptr) == 1);

    res.image = (JBKPixel *)malloc(sizeof(JBKPixel) * res.jbk_header.len);
    assert(fread(res.image, sizeof(JBKPixel), res.jbk_header.len, file_ptr) == res.jbk_header.len);

    fclose(file_ptr);

    return res;
}

void jbk_close_file(JBKFile *file) {
    if (file->image) {
        free(file->image);
    }
}

TGAFile jbk_decompress_to_tga(const JBKFile *jbk_file) {
    TGAFile res = {0};

    res.header = jbk_file->tga_header;
    res.image = (Pixel *)malloc(sizeof(Pixel) * res.header.width * res.header.width);
    assert(res.image && "Couldn't allocate memory for storing pixels!\n");

    uint16_t width = res.header.width;
    uint16_t height = res.header.height;
    uint16_t block_size = jbk_file->jbk_header.block_size;

    size_t index = 0;

    for (uint16_t i = 0; i < height; i += block_size) {
        for (uint16_t j = 0; j < width; j += block_size) {
            for (uint16_t k = 0; k < block_size; ++k) {
                for (uint16_t l = 0; l < block_size; ++l) {
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
