//bmp.h

#include <stdint.h>

typedef struct  {
    unsigned char magic [2];
} bmpfile_magic;

typedef struct {
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
}  BMPFILEHEADER;

typedef struct {
    uint32_t header_sz;
    int32_t width;
    int32_t height;
    uint16_t nplanes;
    uint16_t bitspp;
    uint32_t compress_type;
    uint32_t bmp_bytesz;
    int32_t hres;
    int32_t vres;
    uint32_t ncolors;
    uint32_t nimpcolors;
} BITMAPINFOHEADER;

typedef struct{ // little endian byte order
    uint8_t b;
    uint8_t g;
    uint8_t r;
} BMPPIXEL;

typedef struct {
    bmpfile_magic magic;
    BMPFILEHEADER header;
    BITMAPINFOHEADER info;
    BMPPIXEL pixel;
} BMP;

BMPPIXEL **alloc_pixels(int pixelWidth, int pixelHeight);
uint16_t convert24to16(BMPPIXEL p);
