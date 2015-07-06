#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "minilzo-2.09/minilzo.c"

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
        file_size = 0xFFFFFFFF;

    void* inpbuffer = malloc((size_t) file_size);
    void* outbuffer = malloc((size_t) file_size);

    fread(inpbuffer, 1, (size_t) file_size, f);

    lzo_uint compressed_size = file_size;

    void* wrkmem = malloc(((LZO1X_1_MEM_COMPRESS) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t));

    gettimeofday(&time_start, NULL);
    lzo1x_1_compress(inpbuffer, (unsigned int) file_size, outbuffer, &compressed_size, wrkmem);
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %lu\n",compressed_size);
}