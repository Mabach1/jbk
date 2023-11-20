#include "jbk_ui.h"

typedef enum _JBK_Error {
    UNRECOGNIZED_ACTION,
    NO_INPUT,
    INPUT_EXISTS,
    NO_OUTPUT,
    NO_MAX_DIFF,
    WRONG_MAX_DIFF,
    NO_BLOCK_SIZE,
    WRONG_BLOCK_SIZE,
    ARGC,
} JBK_Error;

const char *jbk_get_error(JBK_Error err) {
    switch (err) {
        case UNRECOGNIZED_ACTION:
            return "Unrecognized action, can't proceed with (de)compression";
        case NO_INPUT:
            return "Didn't provide input path";
        case INPUT_EXISTS:
            return "Input doesn't exists";
        case NO_OUTPUT:
            return "Didn't provdide output path";
        case NO_MAX_DIFF:
            return "Didn't provide max difference";
        case WRONG_MAX_DIFF:
            return "Invalid value of max difference";
        case WRONG_BLOCK_SIZE:
            return "Invalid value of block size";
        case NO_BLOCK_SIZE:
            return "Didn't provide block size";
        case ARGC:
            return "Didn't proved correct amount of arguments";
    }
    return NULL;
}

void jbk_error(JBK_Error err) {
    fprintf(stderr, "+ JBK Error: %s!\n+ Aborting with value of 1!\n", jbk_get_error(err));
    exit(EXIT_FAILURE);
}

JBK_Action jbk_choose_action(const char *fst_arg) {
    if (!strcmp(fst_arg, "compress")) {
        return COMPRESS;
    }

    if (!strcmp(fst_arg, "decompress")) {
        return DECOMPRESS;
    }

    jbk_error(UNRECOGNIZED_ACTION);
    return 0;
}

CompressArgs compress_args_slurp(int argc, const char **argv) {
    if (argc != 10) {
        jbk_error(ARGC);
    }

    CompressArgs res = {.input = NULL, .output = NULL, .max_diff = 0, .block_size = 0};

    bool input_assigned = false;
    bool output_assigned = false;
    bool block_size_assigned = false;
    bool max_diff_assigned = false;

    for (int i = 2; i < argc - 1; ++i) {
        if (strcmp(argv[i], "--input") == 0 && !input_assigned) {
            res.input = (char *)malloc(strlen(argv[i + 1]) + 1);
            strcpy(res.input, argv[i + 1]);
            input_assigned = true;
        }

        if (strcmp(argv[i], "--output") == 0 && !output_assigned) {
            res.output = (char *)malloc(1 + strlen(argv[i + 1]) + 1);
            strcpy(res.output, argv[i + 1]);
            output_assigned = true;
        }

        if (strcmp(argv[i], "--block-size") == 0 && !block_size_assigned) {
            res.block_size = atoi(argv[i + 1]);
            block_size_assigned = true;
        }

        if (strcmp(argv[i], "--max-diff") == 0 && !max_diff_assigned) {
            res.max_diff = atoi(argv[i + 1]);
            max_diff_assigned = true;
        }
    }

    if (!(input_assigned && output_assigned && block_size_assigned && max_diff_assigned)) {
        if (res.input) {
            free(res.input);
        }
        if (res.output) {
            free(res.output);
        }

        if (!input_assigned) {
            fprintf(stderr, "+ JBK Error: %s!\n", jbk_get_error(NO_INPUT));
        }

        if (!output_assigned) {
            fprintf(stderr, "+ JBK Error: %s!\n", jbk_get_error(NO_OUTPUT));
        }

        if (!block_size_assigned) {
            fprintf(stderr, "+ JBK Error: %s!\n", jbk_get_error(NO_BLOCK_SIZE));
        }

        if (!max_diff_assigned) {
            fprintf(stderr, "+ JBK Error: %s!\n", jbk_get_error(NO_MAX_DIFF));
        }

        fprintf(stdout, "+ Aborting with value of 1!\n");

        exit(EXIT_FAILURE);
    }

    return res;
}

void jbk_compress_check_args(CompressArgs *args, TGA_File *tga) {
    if ((tga->header.width * tga->header.height) % args->block_size != 0 || args->block_size < 0) {
        jbk_error(WRONG_BLOCK_SIZE);
    }

    if (args->max_diff < 0) {
        jbk_error(WRONG_MAX_DIFF);
    }
}

void compress_args_free(CompressArgs *args) {
    free(args->input);
    free(args->output);
}

DecompressArgs decompress_args_slurp(int argc, const char **argv) {
    if (argc != 6) {
        assert(0 && "Did not provide correct amount of arguments");
    }

    DecompressArgs res = {.input = NULL, .output = NULL};

    bool input_assigned = false;
    bool output_assigned = false;

    for (int i = 2; i < argc - 1; ++i) {
        if (strcmp(argv[i], "--input") == 0 && !input_assigned) {
            res.input = (char *)malloc(1 + strlen(argv[i + 1]));
            strcpy(res.input, argv[i + 1]);
            input_assigned = true;
        }

        if (strcmp(argv[i], "--output") == 0 && !output_assigned) {
            res.output = (char *)malloc(1 + strlen(argv[i + 1]));
            strcpy(res.output, argv[i + 1]);
            output_assigned = true;
        }
    }

    if (!(input_assigned && output_assigned)) {
        if (res.input) {
            free(res.input);
        }
        if (res.output) {
            free(res.output);
        }
        assert(0 && "Didn't provide all arguments!\n");
    }

    return res;
}

void decompress_args_free(DecompressArgs *args) {
    free(args->input);
    free(args->output);
}

void jbk_compress_show_info(CompressArgs *args) {
    fprintf(stdout, "JBK compression of TGA file\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "+ Compression summary:\n");
    fprintf(stdout, "  + input - %s\n", args->input);
    fprintf(stdout, "  + output - %s\n", args->output);
    fprintf(stdout, "  + block size - %d\n", args->block_size);
    fprintf(stdout, "  + max pixel diff - %d\n", args->max_diff);
}

void jbk_decompress_show_info(DecompressArgs *args) {
    fprintf(stdout, "JBK - decompression into TGA file\n");
    fprintf(stdout, "---------------------------------\n");
    fprintf(stdout, "+ Decompression summary:\n");
    fprintf(stdout, "  + input - %s\n", args->input);
    fprintf(stdout, "  + output - %s\n", args->output);
}
