#define DEBUG
#include "jbk_for_tga.h"

void compress(CompressArgs *args) {
    TGA_File tga = tga_open_file(args->input);
#ifdef DEBUG
    tga_header_print(&tga.header);
#endif
    tga_free(&tga);
}

void decompress(DecompressArgs *args) {
    (void)args;
}

int main(int argc, const char **argv) {
    JBK_Action action = jbk_choose_action(argv[1]);

    if (COMPRESS == action) {
        CompressArgs args = compress_args_slurp(argc, argv);

        compress(&args);

        compress_args_free(&args);
    }

    if (DECOMPRESS == action) {
        DecompressArgs args = decompress_args_slurp(argc, argv);

        decompress(&args);

        decompress_args_free(&args);
    }

    return 0;
}
