#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "zlib-1.2.8/adler32.c"
#undef DO1
#undef DO8

#include "zlib-1.2.8/crc32.c"
#include "zlib-1.2.8/trees.c"

#define static_tree_desc_s static_tree_desc_s__

#include "zlib-1.2.8/deflate.c"

static struct timeval time_start;
static struct timeval time_end;

int main(int argc, char *argv[]) {
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    deflateInit(&strm, Z_DEFAULT_COMPRESSION);

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

    strm.avail_in = file_size;
    strm.next_in = inpbuffer;

    strm.avail_out = file_size;
    strm.next_out = outbuffer;

    gettimeofday(&time_start, NULL);
    deflate(&strm, Z_FINISH);
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %lu\n",file_size - strm.avail_out);
}

#undef GZIP
#define internal_state internal_state__

#include "zlib-1.2.8/zutil.c"