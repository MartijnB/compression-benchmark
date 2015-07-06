#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "bcl-1.2.0/src/rice.c"

static struct timeval time_start;
static struct timeval time_end;

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    //algorithm breaks above 4G
    if (file_size >= 0x100000000)
        file_size = 0x7FFFF000;

    void* inpbuffer = malloc((size_t) file_size);
    void* outbuffer = malloc((size_t) file_size);
    void* decbuffer = malloc((size_t) file_size);

    fread(inpbuffer, 1, (size_t) file_size, f);

    gettimeofday(&time_start, NULL);
    unsigned int compressed_size = Rice_Compress(inpbuffer, outbuffer, (unsigned int) file_size, RICE_FMT_UINT32);
    gettimeofday(&time_end, NULL);

    Rice_Uncompress(outbuffer, decbuffer, compressed_size, (unsigned int) file_size, RICE_FMT_UINT32);

    printf("%lu %i %li\n", strlen(inpbuffer), compressed_size, file_size);

    if (memcmp(inpbuffer, decbuffer, strlen(inpbuffer)) != 0) {
        printf("Error!\n");
    }

    //printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    //printf("Original: %lu\n",file_size);
    //printf("Compressed: %u\n",compressed_size);
}