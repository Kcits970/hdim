#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dump.h"
#include "options.h"

int main(int argc, char **argv)
{
    int dump_mode = 0, char_mode = 0, version_mode = 0;
    char *output_file = NULL;

    if (argc < 2)
        goto bad_usage;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
            dump_mode = 1;
        else if (strcmp(argv[i], "-c") == 0)
            char_mode = 1;
        else if (strcmp(argv[i], "-V") == 0)
            version_mode = 1;
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
            output_file = argv[++i];
        else
            goto bad_usage;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "./hdim: ");
        perror(argv[1]);
        return 1;
    }

    if (dump_mode)
        handle_option_d(f);
    if (char_mode)
        handle_option_c(f);
    if (version_mode)
        handle_option_v();
    if (output_file)
        handle_option_o(f, output_file);

    fclose(f);
    return 0;

bad_usage:
    fprintf(stderr, "Usage: ./hdim <file> [-d] [-c] [-V] [-o output_file]\n");
    return 1;
}
