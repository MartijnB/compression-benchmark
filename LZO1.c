#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "lzo-2.09/include/lzo/lzodefs.h"
#include "lzo-2.09/include/lzo/lzoconf.h"
#include "lzo-2.09/include/lzo/lzo1.h"

// From the FAQ:
// - if you have little memory available for compression use LZO1X-1(11)

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

    void* wrkmem = malloc(LZO1_MEM_COMPRESS);

    gettimeofday(&time_start, NULL);
    lzo1_compress(inpbuffer, (unsigned int) file_size, outbuffer, &compressed_size, wrkmem);
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %lu\n",compressed_size);
}