//bmp.c
//Ethan Geller

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bmp.h>


BMPPIXEL **alloc_pixels(int pixelWidth, int pixelHeight){
    BMPPIXEL **pixels = (BMPPIXEL **) malloc(pixelHeight * sizeof(BMPPIXEL *));
    int r;
    for(r=0; r<pixelHeight; ++r)
        pixels[r] = (BMPPIXEL *)malloc(pixelWidth * sizeof(BMPPIXEL *));
    
    return pixels;
}

uint16_t convert24to16(BMPPIXEL p) {
  uint16_t r = p.r;
  uint16_t g = p.g;
  uint16_t b = p.b;
  return (((r & 0xf8) <<8) | ((g & 0xfc) << 1) | (b>>3));
}
