#include "jbk_for_tga.h"

void compress(CompressArgs *args) {
    TGA_File tga = tga_open_file(args->input);

    uint32_t num_pixel = 0;
    JBK_Pixel *compress_image = jbk_compress_tga(&tga, args->block_size, args->max_diff, &num_pixel);

    jbk_save_file(args->output, compress_image, &tga, args->block_size, num_pixel);

    tga_close_file(&tga);
}

void decompress(DecompressArgs *args) {
    JBK_File jbk = jbk_load_file(args->input);
    TGA_File tga = jbk_decompress_to_tga(&jbk);

    tga_save_file(args->output, &tga);

    tga_close_file(&tga);
}

int main(int argc, const char **argv) {
    JBK_Action action = jbk_choose_action(argv[1]);

    if (COMPRESS == action) {
        CompressArgs args = compress_args_slurp(argc, argv);
        compress(&args);
        jbk_show_info(action, &args, NULL);
        compress_args_free(&args);
    }

    if (DECOMPRESS == action) {
        DecompressArgs args = decompress_args_slurp(argc, argv);
        decompress(&args);
        jbk_show_info(action, NULL, &args);
        decompress_args_free(&args);
    }

    return 0;
}
