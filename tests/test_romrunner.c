// loads a rom and runs it forever.

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba.h>


static struct GBA_Core gba;
static uint8_t* rom_data = NULL;
static size_t rom_size = 0;


static bool romloader(const char* path)
{
    FILE* f = fopen(path, "rb");
    
    if (!f)
    {
        goto fail;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (!file_size)
    {
        goto fail;
    }

    rom_data = (uint8_t*)malloc(file_size);

    if (!rom_data)
    {
        goto fail;
    }

    if (!fread(rom_data, file_size, 1, f))
    {
        goto fail;
    }

    rom_size = (size_t)file_size;

    return true;

fail:
    if (f)  { fclose(f); }
    return false;
}

static void cleanup()
{
    if (rom_data)   { free(rom_data); }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return -1;
    }

    if (!romloader(argv[1]))
    {
        goto fail;
    }

    if (!GBA_init(&gba))
    {
        goto fail;
    }

    if (!GBA_loadrom(&gba, rom_data, rom_size))
    {
        goto fail;
    }

    for (;;)
    {
        GBA_run_frame(&gba);
    }

    cleanup();

    return 0;

fail:
    cleanup();
    return -1;
}
