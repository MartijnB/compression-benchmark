#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define LZF_STATE_ARG 1

#define main main__
#include "liblzf-3.6/lzf_c.c"

#include "liblzf-3.6/lzf_d.c"
#undef main

static struct timeval time_start;
static struct timeval time_end;

int main(int argc, char *argv[]) {
    char* inpbuffer = malloc((size_t) 1024);
    char* outbuffer = malloc((size_t) 1024);
    char* decbuffer = malloc((size_t) 1024);

    strcpy(inpbuffer, "Heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeello World!");

    LZF_STATE htab;

    unsigned int compressed_size = 0;
    for (int i = 0; i < strlen(inpbuffer); i += 128) {
        unsigned int comp_length = (unsigned int) ((i + 128 < strlen(inpbuffer)) ? 128 : strlen(inpbuffer) - i);

        compressed_size += lzf_compress(inpbuffer + i, comp_length, outbuffer + compressed_size, 1024 - compressed_size, htab);
    }

    //unsigned int compressed_size = lzf_compress(inpbuffer, strlen(inpbuffer), outbuffer, 1024);
    unsigned int decompress_size = lzf_decompress(outbuffer, compressed_size, decbuffer, 1024);

    printf("%lu %i %i\n", strlen(inpbuffer), compressed_size, decompress_size);

    //printf("%s", decbuffer);

    if (strncmp(inpbuffer, decbuffer, strlen(inpbuffer)) != 0) {
        printf("Error!\n");
    }

    return 0;

/*
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size >= 0x100000000)
        file_size = 0xFFFFFFFF;

    void* inpbuffer = malloc((size_t) file_size);
    void* outbuffer = malloc((size_t) file_size);

    fread(inpbuffer, 1, (size_t) file_size, f);

    gettimeofday(&time_start, NULL);
    unsigned int compressed_size = lzf_compress(inpbuffer, (unsigned int) file_size, outbuffer, (unsigned int) file_size);
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %u\n",compressed_size);
    */
}