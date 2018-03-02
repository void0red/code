#include <stdio.h>
#include "pnglib.h"
#include "chrlib.h"
#include "base64.h"
int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s target_picture (target_words)\n", argv[0]);
        exit(-1);
    }else if(argc == 3){
        pic_data *out = (pic_data *)malloc(sizeof(pic_data));
        detect_png(argv[1], out);
        char *prv_words = base64_encode(argv[2], (int)strlen(argv[2]));
        int max_data_length = ((out->width - 2) * (out->height - 2) * 3 - 1) / 8;
        int channels = out->channels;
        if(out->bit_depth != 8){
            fprintf(stderr, "only support 8-bit depth picture\n");
            png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
            exit(-1);
        }else if(out->channels == 4){
            fprintf(stderr, "only support 3 channels pictures\n");
            png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
            exit(-1);
        }else if(out->width < 3 || out->height < 3 || max_data_length < strlen(prv_words)){
            fprintf(stderr, "too small pictures");
            png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
            exit(-1);
        }
//        printf("width=%d height=%d\n", out->width, out->height);
        char *words = txt2bin(prv_words);
        int words_len = (int)strlen(words);
        int row_lengths = (out->width - 2) * 3;
        for(int i = 0; i < words_len; i++){
            int x = i / row_lengths + 1;
            int y = i % row_lengths + 3;
            if(words[i] == '0'){
                int data = out->rgba[x][y];
                if(data % 2){
                    int temp = data + 1;
                    if(temp >= 0 && temp <= 255){
                        out->rgba[x][y] = (unsigned char)temp;
                    } else{
                        out->rgba[x][y] -= 1;
                    }
                }
            } else{
                int data = out->rgba[x][y];
                if(!(data % 2)){
                    int temp = data + 1;
                    if(temp >= 0 && temp <= 255 && temp != 'e'){
                        out->rgba[x][y] = (unsigned char)temp;
                    } else{
                        out->rgba[x][y] -= 1;
                    }
                }
            }
//            printf("pic[%d][%d]=%d\n", x, y, out->rgba[x][y]);
        }
        out->rgba[0][0] = 'r';
        out->rgba[0][out->width*3-2] = 'g';
        out->rgba[out->height-1][out->width*3-1] = 'b';
        out->rgba[(words_len+1)/row_lengths + 1][(words_len+1)%row_lengths + 2] = 'e';
        write_png_file("out.png", out);
        png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
        free(words);
        return 0;
    }else if(argc == 2){
        pic_data *out = (pic_data *)malloc(sizeof(pic_data));
        detect_png(argv[1], out);
        int max_data_length = ((out->width - 2) * (out->height - 2) * 3 - 1) / 8;
        char *words = (char *)malloc(max_data_length * sizeof(char) + 1);
        int check_r = out->rgba[0][0];
        int check_g = out->rgba[0][out->width*3-2];
        int check_b = out->rgba[out->height-1][out->width*3-1];
        if(check_r != 'r' || check_g != 'g' || check_b != 'b'){
            fprintf(stderr, "no correct picture\n");
            png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
            exit(-1);
        }
        int cnt = 0;
        int pos = 0;
        for(int i = 1; i < out->height - 1; i++){
            if(pos)break;
            for(int j = 3; j < (out->width - 1) * 3; j++){
//                printf("rgba[%d][%d] = %d, cnt = %d\n", i, j, out->rgba[i][j], cnt);
                if(out->rgba[i][j] == 'e'){
                    pos = 1;
                    break;
                } else if(out->rgba[i][j] % 2){
                    words[cnt++] = '1';
                } else{
                    words[cnt++] = '0';
                }
            }
        }
        png_destroy_read_struct(&out->png_ptr, &out->info_ptr, 0);
        words[cnt] = '\0';
//        puts(words);
        char *prv_ret = bin2txt(words);
        char *ret = base64_decode(prv_ret, (int)strlen(prv_ret));
        free(prv_ret);
        puts(ret);
        free(ret);
        return 0;
    }
}