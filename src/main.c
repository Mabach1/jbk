#include "jbk_for_tga.h"

void compress(const CompressArgs *args) {
    TGAFile tga = tga_open_file(args->input);

    uint32_t num_pixel = 0;
    JBKPixel *compressed_image = jbk_compress_tga(&tga, args, &num_pixel);

    jbk_save_file(args->output, compressed_image, &tga, args->block_size, num_pixel);

    tga_close_file(&tga);
}

void decompress(const DecompressArgs *args) {
    JBKFile jbk = jbk_open_file(args->input);
    TGAFile tga = jbk_decompress_to_tga(&jbk);

    tga_save_file(args->output, &tga);

    tga_close_file(&tga);
    jbk_close_file(&jbk);
}

int main(int argc, const char **argv) {
    JBK_Action action = jbk_choose_action(argv[1]);

    if (COMPRESS == action) {
        CompressArgs args = compress_args_slurp(argc, argv);
        compress(&args);
        jbk_show_info(action, &args, NULL);
        compress_args_free(&args);
        return 0;
    }

    if (DECOMPRESS == action) {
        DecompressArgs args = decompress_args_slurp(argc, argv);
        decompress(&args);
        jbk_show_info(action, NULL, &args);
        decompress_args_free(&args);
        return 0;
    }

    jbk_show_info(action, NULL, NULL);

    return 0;
}
