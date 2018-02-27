//
// Created by void0red on 18-2-25.
//

#include "pnglib.h"
int detect_png(char *file_path, pic_data *out);
int write_png_file(char *file_name , pic_data *graph);
int detect_png(char *file_path, pic_data *out){
    FILE *pic_fp = fopen(file_path, "rb");
    if(pic_fp == NULL) return -1;
    char buf[PNG_BYTES_TO_CHECK];
    png_structp png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    setjmp(png_jmpbuf(png_ptr));
    fread(buf, 1, PNG_BYTES_TO_CHECK, pic_fp);
    int temp = png_sig_cmp((void*)buf, 0, PNG_BYTES_TO_CHECK);
    if (temp!=0) return -1;
    rewind(pic_fp);
    png_init_io(png_ptr, pic_fp);
    out->png_ptr = png_ptr;
    out->info_ptr = info_ptr;
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
    out->channels = png_get_channels(png_ptr, info_ptr);
    out->bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    out->rgba = png_get_rows(png_ptr, info_ptr);
    out->width = png_get_image_width(png_ptr, info_ptr);
    out->height = png_get_image_height(png_ptr, info_ptr);
    return 0;
}
int write_png_file(char *file_name , pic_data *graph){
    FILE *fp = fopen(file_name, "wb");
    if (!fp) {
        fprintf(stderr, "[write_png_file] File %s could not be opened for writing", file_name);
        return -1;
    }
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr){
        fprintf(stderr, "[write_png_file] png_create_write_struct failed");
        return -1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "[write_png_file] png_create_info_struct failed");
        return -1;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "[write_png_file] Error during init_io");
        return -1;
    }
    png_init_io(png_ptr, fp);
    if (setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "[write_png_file] Error during writing header");
        return -1;
    }
    png_byte color_type;
    if(graph->channels == 4) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else color_type = PNG_COLOR_TYPE_RGB;
    png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
                 graph->bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    if (setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "[write_png_file] Error during writing bytes");
        return -1;
    }
    png_write_image(png_ptr, graph->rgba);
    if(setjmp(png_jmpbuf(png_ptr))){
        fprintf(stderr, "[write_png_file] Error during end of write");
        return -1;
    }
    png_write_end(png_ptr, NULL);
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}