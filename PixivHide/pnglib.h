//
// Created by void0red on 18-2-25.
//

#ifndef PNG_PNGLIB_H
#define PNG_PNGLIB_H
#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PNG_BYTES_TO_CHECK 4
typedef struct _pic_data{
    int width, height, channels, bit_depth;
    png_bytepp rgba;
    png_structp png_ptr;
    png_infop info_ptr;
}pic_data;
#endif //PNG_PNGLIB_H
int detect_png(char *file_path, pic_data *out);
int write_png_file(char *file_name , pic_data *graph);