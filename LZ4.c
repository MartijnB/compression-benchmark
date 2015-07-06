#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "lz4-r130/lib/lz4.c"

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

    if (file_size >= 0x100000000)
        file_size = ((unsigned int) LZ4_MAX_INPUT_SIZE) * 2;

    void* inpbuffer = malloc((size_t) file_size);
    void* outbuffer = malloc((size_t) file_size);

    fread(inpbuffer, 1, (size_t) file_size, f);

    gettimeofday(&time_start, NULL);
    int compressed_size = LZ4_compress_default(inpbuffer, outbuffer, (int) (file_size / 2), (int) (file_size / 2));
    int compressed_size2 = LZ4_compress_default(inpbuffer + (file_size / 2), outbuffer + compressed_size, (int) (file_size / 2), (int) (file_size / 2));
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %u\n",compressed_size + compressed_size2);
}