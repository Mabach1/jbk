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
            return "Didn't provide output path";
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

JBK_Action jbk_choose_action(const char *fst_arg) {
    if (!strcmp(fst_arg, "compress")) {
        return COMPRESS;
    }

    if (!strcmp(fst_arg, "decompress")) {
        return DECOMPRESS;
    }

    if (!strcmp(fst_arg, "-info")) {
        return INFO;
    }

    fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n+ Aborting with value of 1!\n", jbk_get_error(UNRECOGNIZED_ACTION));
    exit(EXIT_FAILURE);

    return 0;
}

CompressArgs compress_args_slurp(int argc, const char **argv) {
    if (argc != 10) {
        fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n+ Aborting with value of 1!\n", jbk_get_error(ARGC));
        exit(EXIT_FAILURE);
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
            fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n", jbk_get_error(NO_INPUT));
        }

        if (!output_assigned) {
            fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n", jbk_get_error(NO_OUTPUT));
        }

        if (!block_size_assigned) {
            fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n", jbk_get_error(NO_BLOCK_SIZE));
        }

        if (!max_diff_assigned) {
            fprintf(stderr, "\033[31m+ JBK Error:\033[0m %s!\n", jbk_get_error(NO_MAX_DIFF));
        }

        fprintf(stdout, "+ Aborting with value of 1!\n");

        exit(EXIT_FAILURE);
    }

    return res;
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

void jbk_info(void) {
    fprintf(stdout, "JBK de/compression for TGA files!\n");
    fprintf(stdout, "---------------------------------\n");
    fprintf(stdout, "+ Author: Matej Baliga\n");
    fprintf(stdout, "+ Brief\n");
    fprintf(stdout, "  + Simple CLI application for compressing and decompressing of TGA files into format JBK.\n");
    fprintf(stdout, "  + JBK is a custom format that works similary to JPEG.\n");
    fprintf(stdout, "+ Usage\n");
    fprintf(stdout, "  + Compression\n");
    fprintf(stdout,
            "  - ./jbk compress --input [path to TGA file] --output [path to JBK file] --max-diff [max difference of neighboring pixels] "
            "--block-size [block size]\n");
    fprintf(stdout, "  + Decompression\n");
    fprintf(stdout, "  - ./jbk decompress --input [path to JBK file] --output [path to TGA file]\n");
}

void jbk_show_info(JBK_Action action, CompressArgs *ca, DecompressArgs *da) {
    switch (action) {
        case COMPRESS:
            jbk_compress_show_info(ca);
            return;
        case DECOMPRESS:
            jbk_decompress_show_info(da);
            return;
        case INFO:
            jbk_info();
            return;
    }
}