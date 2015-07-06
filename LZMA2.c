#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/time.h>

#include "xz-5.2.1/src/liblzma/api/lzma.h"

static bool
init_encoder(lzma_stream *strm, uint32_t preset)
{
    // Initialize the encoder using a preset. Set the integrity to check
    // to CRC64, which is the default in the xz command line tool. If
    // the .xz file needs to be decompressed with XZ Embedded, use
    // LZMA_CHECK_CRC32 instead.
    lzma_ret ret = lzma_easy_encoder(strm, preset, LZMA_CHECK_CRC64);

    // Return successfully if the initialization went fine.
    if (ret == LZMA_OK)
        return true;

    // Something went wrong. The possible errors are documented in
    // lzma/container.h (src/liblzma/api/lzma/container.h in the source
    // package or e.g. /usr/include/lzma/container.h depending on the
    // install prefix).
    const char *msg;
    switch (ret) {
        case LZMA_MEM_ERROR:
            msg = "Memory allocation failed";
            break;

        case LZMA_OPTIONS_ERROR:
            msg = "Specified preset is not supported";
            break;

        case LZMA_UNSUPPORTED_CHECK:
            msg = "Specified integrity check is not supported";
            break;

        default:
            // This is most likely LZMA_PROG_ERROR indicating a bug in
            // this program or in liblzma. It is inconvenient to have a
            // separate error message for errors that should be impossible
            // to occur, but knowing the error code is important for
            // debugging. That's why it is good to print the error code
            // at least when there is no good error message to show.
            msg = "Unknown error, possibly a bug";
            break;
    }

    fprintf(stderr, "Error initializing the encoder: %s (error code %u)\n",
            msg, ret);
    return false;
}

static struct timeval time_start;
static struct timeval time_end;

int main(int argc, char *argv[]) {
    lzma_stream strm = LZMA_STREAM_INIT;

    init_encoder(&strm, 6);

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

    strm.next_in = inpbuffer;
    strm.avail_in = file_size;
    strm.next_out = outbuffer;
    strm.avail_out = file_size;

    gettimeofday(&time_start, NULL);
    lzma_code(&strm, LZMA_FINISH);
    gettimeofday(&time_end, NULL);

    printf("Duration: %lu\n", ((time_end.tv_sec * 1000) + (time_end.tv_usec / 1000)) - ((time_start.tv_sec * 1000) + (time_start.tv_usec / 1000)));
    printf("Original: %lu\n",file_size);
    printf("Compressed: %lu\n",file_size - strm.avail_out);
}


