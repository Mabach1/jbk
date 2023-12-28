#include "jbk_ui.h"

#define FORMAT_SIZE (512)

#define _START_RED_TEXT_ "\033[31m"
#define _END_RED_TEXT_ "\033[0m"

enum { INPUT, OUTPUT, BLOCK_SIZE, MAX_DIFF, COMPRESS };

#define INPUT_FLAG "--input"
#define OUTPUT_FLAG "--output"
#define MAX_DIFF_FLAG "--max-diff"
#define BLOCK_SIZE_FLAG "--block-size"
#define COMPRESS_FLAG "--COMPRESS_OVER_U8_MAX"

int string_to_int(const char *str, int *number) {
    int res = 0;

    *number = 0;
    int sign = 1;

    size_t index = 0;
    size_t length = strlen(str);

    // not very useful in our case
    // while (index < length && (str[index] == ' ' || str[index] == '\t')) {
    //     ++index;
    // }

    if (index < length && (str[index] == '-' || str[index] == '+')) {
        sign = str[index] == '-' ? -1 : 1;
        ++index;
    }

    while (index < length && str[index] >= '0' && str[index] <= '9') {
        res = 1;
        *number = (*number * 10) + (str[index] - '0');
        ++index;
    }

    *number *= sign;

    return res;
}

void jbk_error(const char *format, ...) {
    char buffer[FORMAT_SIZE] = {0};

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, FORMAT_SIZE, format, args);

    fprintf(stderr, "+ jbk: " _START_RED_TEXT_ "fatal error:" _END_RED_TEXT_ " %s!\n", buffer);

    va_end(args);
}

void jbk_exit(void) {
    fprintf(stderr, "+ Aborting with value of 1!\n");
    exit(EXIT_FAILURE);
}

JBK_Action jbk_choose_action(const char *fst_arg) {
    if (!fst_arg) {
        jbk_error("No arguments provided");
        jbk_exit();
    }

    if (!strcmp(fst_arg, "compress")) {
        return COMPRESSING;
    }

    if (!strcmp(fst_arg, "decompress")) {
        return DECOMPRESSING;
    }

    if (!strcmp(fst_arg, "info")) {
        return INFO;
    }

    jbk_error("Unrecognized action, can't proceed with (de)compression");
    jbk_exit();

    return 0;
}

CompressArgs compress_args_slurp(int argc, const char **argv) {
    if (argc != 10 && argc != 12) {
        jbk_error("Didn't proved correct amount of arguments");
        jbk_exit();
    }

    CompressArgs res = {.input = NULL, .output = NULL, .max_diff = 0, .block_size = 0, .compress_flag = false};

    bool assigned[] = {false, false, false, false, false};

    for (int i = 2; i < argc - 1; ++i) {
        if (strcmp(argv[i], INPUT_FLAG) == 0 && !assigned[INPUT]) {
            res.input = (char *)malloc(strlen(argv[i + 1]) + 1);
            strcpy(res.input, argv[++i]);
            assigned[INPUT] = true;
        } else if (strcmp(argv[i], OUTPUT_FLAG) == 0 && !assigned[OUTPUT]) {
            res.output = (char *)malloc(1 + strlen(argv[i + 1]) + 1);
            strcpy(res.output, argv[++i]);
            assigned[OUTPUT] = true;
        } else if (strcmp(argv[i], BLOCK_SIZE_FLAG) == 0 && !assigned[BLOCK_SIZE]) {
            if (string_to_int(argv[++i], &res.block_size)) {
                assigned[BLOCK_SIZE] = true;
            }
        } else if (strcmp(argv[i], MAX_DIFF_FLAG) == 0 && !assigned[MAX_DIFF]) {
            if (string_to_int(argv[++i], &res.max_diff)) {
                assigned[MAX_DIFF] = true;
            }
        } else if (strcmp(argv[i], COMPRESS_FLAG) == 0 && !assigned[COMPRESS]) {
            if (strcmp(argv[++i], "true") == 0) {
                res.compress_flag = true;
            }
            assigned[COMPRESS] = true;
        } else {
            if (res.input) free(res.input);
            if (res.output) free(res.output);

            jbk_error("Unknown flag '%s'", argv[i]);
            jbk_exit();
        }
    }

    if (!(assigned[INPUT] && assigned[OUTPUT] && assigned[BLOCK_SIZE] && assigned[MAX_DIFF])) {
        if (res.input) free(res.input);

        if (res.output) free(res.output);

        if (!assigned[INPUT]) jbk_error("Did not provide input path");

        if (!assigned[OUTPUT]) jbk_error("Did not provide output path");

        if (!assigned[BLOCK_SIZE]) jbk_error("Incorrect value or did not provide block size");

        if (!assigned[MAX_DIFF]) jbk_error("Incorrect value or did not provide max difference");

        if (res.compress_flag && !assigned[COMPRESS]) jbk_error("Did not provide appropriate compress flag");

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
        jbk_error("Didn't proved correct amount of arguments");
        jbk_exit();
    }

    DecompressArgs res = {.input = NULL, .output = NULL};

    bool assigned[] = {false, false};

    for (int i = 2; i < argc - 1; ++i) {
        if (strcmp(argv[i], INPUT_FLAG) == 0 && !assigned[INPUT]) {
            res.input = (char *)malloc(strlen(argv[i + 1]) + 1);
            strcpy(res.input, argv[++i]);
            assigned[INPUT] = true;
        } else if (strcmp(argv[i], OUTPUT_FLAG) == 0 && !assigned[OUTPUT]) {
            res.output = (char *)malloc(strlen(argv[i + 1]) + 1);
            strcpy(res.output, argv[++i]);
            assigned[OUTPUT] = true;
        } else {
            if (res.input) free(res.input);
            if (res.output) free(res.output);

            jbk_error("Unknown flag '%s'", argv[i]);
            jbk_exit();
        }
    }

    if (!(assigned[INPUT] && assigned[OUTPUT])) {
        if (res.input) {
            free(res.input);
        }
        if (res.output) {
            free(res.output);
        }

        if (!assigned[INPUT]) jbk_error("Didn't provide input path");

        if (!assigned[OUTPUT]) jbk_error("Didn't provide output path");

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
    fprintf(stdout, "  + compressing over u8 max - %s\n\n", args->compress_flag ? "true" : "false");
}

void jbk_decompress_show_info(DecompressArgs *args) {
    fprintf(stdout, "JBK - decompression into TGA file\n");
    fprintf(stdout, "---------------------------------\n");
    fprintf(stdout, "+ Decompression summary:\n");
    fprintf(stdout, "  + input - %s\n", args->input);
    fprintf(stdout, "  + output - %s\n\n", args->output);
}

void jbk_info(void) {
    fprintf(stdout, "----------------------------------\n");
    fprintf(stdout, "JBK - de/compression of TGA files!\n");
    fprintf(stdout, "----------------------------------\n");
    fprintf(stdout, "+ Author: Matej Baliga\n");
    fprintf(stdout, "+ Brief\n");
    fprintf(stdout, "  + A simple command-line interface application designed for de/compressing TGA files from/into JBK format.\n");
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
    fprintf(stdout, "  - ./jbk decompress --input [path to JBK file] --output [path to TGA file]\n\n");
}

void jbk_show_info(JBK_Action action, CompressArgs *ca, DecompressArgs *da) {
    switch (action) {
        case COMPRESSING:
            jbk_compress_show_info(ca);
            return;
        case DECOMPRESSING:
            jbk_decompress_show_info(da);
            return;
        case INFO:
            jbk_info();
            return;
    }
}