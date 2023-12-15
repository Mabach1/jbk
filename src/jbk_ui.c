#include "jbk_ui.h"

#define FORMAT_SIZE (4096)

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
    NO_COMPRESS_FLAG
} JBK_ErrorMsg;

void jbk_error(const char *format, ...) {
    char buffer[FORMAT_SIZE] = {0}; 

    va_list args; 
    va_start(args, format);
    vsnprintf(buffer, FORMAT_SIZE, format, args);

    fprintf(stderr, "\033[31m+ jbk error:\033[0m");
    fprintf(stderr, "%s!\n", buffer);

    va_end(args);
}

void jbk_exit(void) {
    fprintf(stderr, "+ Aborting with value of 1!\n");
    exit(EXIT_FAILURE);
}

const char *jbk_get_error(JBK_ErrorMsg err) {
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
        case NO_COMPRESS_FLAG:
            return "Didn't provide appropriate compress flag";
        case ARGC:
            return "Didn't proved correct amount of arguments";
    }
    return NULL;
}

JBK_Action jbk_choose_action(const char *fst_arg) {
    if (!fst_arg) {
        jbk_error("No arguments provided!");
        jbk_exit();
    }

    if (!strcmp(fst_arg, "compress")) {
        return COMPRESS;
    }

    if (!strcmp(fst_arg, "decompress")) {
        return DECOMPRESS;
    }

    if (!strcmp(fst_arg, "-info")) {
        return INFO;
    }


    jbk_error(jbk_get_error(UNRECOGNIZED_ACTION));
    jbk_exit();

    return 0;
}

CompressArgs compress_args_slurp(int argc, const char **argv) {
    if (argc != 10 && argc != 12) {
        jbk_error(jbk_get_error(ARGC));
        jbk_exit();
    }

    CompressArgs res = {.input = NULL, .output = NULL, .max_diff = 0, .block_size = 0, .compress_flag = false};

    bool input_assigned = false;
    bool output_assigned = false;
    bool block_size_assigned = false;
    bool max_diff_assigned = false;
    bool compress_flag_assigned = false;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--COMPRESS_OVER_U8_MAX") == 0 && !compress_flag_assigned) {
            if (strcmp(argv[i + 1], "true") == 0) {
                res.compress_flag = true;
            }
            compress_flag_assigned = true;        
        }

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
            jbk_error(jbk_get_error(NO_INPUT));
        }

        if (!output_assigned) {
            jbk_error(jbk_get_error(NO_OUTPUT));
        }

        if (!block_size_assigned) {
            jbk_error(jbk_get_error(NO_BLOCK_SIZE));
        }

        if (!max_diff_assigned) {
            jbk_error(jbk_get_error(NO_MAX_DIFF));
        }

        if (!compress_flag_assigned) {
            jbk_error(jbk_get_error(NO_COMPRESS_FLAG));
        }

        jbk_exit();
    }

    return res;
}

void compress_args_free(CompressArgs *args) {
    free(args->input);
    free(args->output);
}

DecompressArgs decompress_args_slurp(int argc, const char **argv) {
    if (argc != 6) {
        jbk_error(jbk_get_error(ARGC));
        jbk_exit();
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

        if (!input_assigned) {
            jbk_error(jbk_get_error(NO_INPUT));
        }

        if (!output_assigned) {
            jbk_error(jbk_get_error(NO_OUTPUT));
        }

        jbk_exit();
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
    fprintf(stdout, "  + compressing over u8 max - %s\n", args->compress_flag ? "true" : "false");
}

void jbk_decompress_show_info(DecompressArgs *args) {
    fprintf(stdout, "JBK - decompression into TGA file\n");
    fprintf(stdout, "---------------------------------\n");
    fprintf(stdout, "+ Decompression summary:\n");
    fprintf(stdout, "  + input - %s\n", args->input);
    fprintf(stdout, "  + output - %s\n", args->output);
}

void jbk_info(void) {
    fprintf(stdout, "----------------------------------\n");
    fprintf(stdout, "JBK - de/compression of TGA files!\n");
    fprintf(stdout, "----------------------------------\n");
    fprintf(stdout, "+ Author: Matej Baliga\n");
    fprintf(stdout, "+ Brief\n");
    fprintf(stdout, "  + Simple CLI application for compressing and decompressing of TGA files into format JBK.\n");
    fprintf(stdout, "  + JBK is a custom format that works similary to JPEG.\n");
    fprintf(stdout, "+ Usage\n");
    fprintf(stdout, "  + Compression\n");
    fprintf(stdout,
            "  - ./jbk compress\n"
            "\t--input [path to TGA file]\n" 
            "\t--output [path to JBK file]\n" 
            "\t--max-diff [max difference of neighboring pixels]\n"
            "\t--block-size [block size]\n"
            "\t--COMPRESS_OVER_U8_MAX true (optional)\n");
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