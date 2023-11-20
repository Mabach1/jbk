#include "jbk.h"

#define BLOCK_POS(i, j, k, l, w) l + (w * k) + j + (i * w)

int pixel_diff(Pixel p1, Pixel p2) {
    return (abs(p1.blue - p2.blue) + (abs(p1.green - p2.green)) + (abs(p1.red - p2.red)));
}

JBK_Pixel *jbk_compress_tga(TGA_File *tga_file, uint16_t block_size, uint8_t max_compress_difference, uint32_t *alen) {
    Pixel *buf = tga_file->image;

    uint32_t height = tga_file->header.height;
    uint32_t width = tga_file->header.width;

    JBK_Pixel *res = (JBK_Pixel *)malloc(sizeof(JBK_Pixel) * height * width);
    assert(res && "Allocation of compressed pixels failed!\n");

    uint32_t index = 0;

    for (uint32_t i = 0; i < height; i += block_size) {
        for (uint32_t j = 0; j < width; j += block_size) {
            res[index].pixel = buf[BLOCK_POS(i, j, 0, 0, width)];
            res[index].len = 1;
            for (uint32_t k = 0; k < block_size; ++k) {
                for (uint32_t l = 0; l < block_size; ++l) {
                    size_t pos = BLOCK_POS(i, j, k, l, width);

                    // this is the first pixel we set few lines earlier
                    if (pos == (size_t)BLOCK_POS(i, j, 0, 0, width)) {
                        continue;
                    }

                    if (255 == res[index].len) {
                        index += 1;
                        res[index].pixel = buf[pos];
                        res[index].len = 1;
                        continue;
                    }

                    if (pixel_diff(res[index].pixel, buf[pos]) <= max_compress_difference) {
                        res[index].len += 1;
                    } else {
                        index += 1;
                        res[index].pixel = buf[pos];
                        res[index].len = 1;
                    }
                }
            }
            index += 1;
        }
    }

    *alen = index;

    res = realloc(res, sizeof(JBK_Pixel) * index);
    assert(res && "Reallocation of compressed pixels failed!\n");

    return res;
}

void jbk_save_file(const char *filename, JBK_Pixel *image, TGA_File *tga_file, uint16_t block_size, uint32_t len) {
    FILE *file_ptr = fopen(filename, "wb");
    assert(file_ptr && "Coudn't open file to save compress tga file into!\n");

    assert(fwrite(&tga_file->header, sizeof(TGA_Header), 1, file_ptr) == 1);
    assert(fwrite(&block_size, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fwrite(&len, sizeof(uint32_t), 1, file_ptr) == 1);

    assert(fwrite(image, sizeof(JBK_Pixel) * len, 1, file_ptr) == 1);

    free(image);
    fclose(file_ptr);
}

JBK_File jbk_load_file(const char *filename) {
    FILE *file_ptr = fopen(filename, "rb");
    assert(file_ptr && "Coudn't open JBK file for decompression!\n");

    JBK_File res = {0};

    assert(fread(&res.tga_header, sizeof(TGA_Header), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header, sizeof(uint16_t), 1, file_ptr) == 1);
    assert(fread(&res.jbk_header.size, sizeof(uint32_t), 1, file_ptr) == 1);

    res.image = (JBK_Pixel *)malloc(sizeof(JBK_Pixel) * res.jbk_header.size);

    assert(fread(res.image, sizeof(JBK_Pixel), res.jbk_header.size, file_ptr) == res.jbk_header.size);

    fclose(file_ptr);

    return res;
}

TGA_File jbk_decompress_to_tga(JBK_File *jbk_file) {
    TGA_File res = {0};

    res.header = jbk_file->tga_header;
    res.image = (Pixel *)malloc(sizeof(Pixel) * res.header.width * res.header.width);
    assert(res.image && "Coudn't allocate memory for storing pixels!\n");

    uint32_t width = res.header.width;
    uint32_t height = res.header.height;
    uint16_t bloco_size = jbk_file->jbk_header.block_size;

    size_t index = 0;

    for (uint32_t i = 0; i < height; i += bloco_size) {
        for (uint32_t j = 0; j < width; j += bloco_size) {
            for (uint32_t k = 0; k < bloco_size; ++k) {
                for (uint32_t l = 0; l < bloco_size; ++l) {
                    if (jbk_file->image[index].len <= 0) {
                        ++index;
                    }
                    res.image[BLOCK_POS(i, j, k, l, width)] = jbk_file->image[index].pixel;
                    jbk_file->image[index].len -= 1;
                }
            }
        }
    }

    free(jbk_file->image);

    return res;
}
