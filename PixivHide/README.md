项目地址：https://github.com/void0red/code/tree/master/PixivHide

> 软件开发类
>
> 语言：C
>
> 第三方库：[libpng](https://github.com/glennrp/libpng)

软件功能：利用像素各通道值的偏移实现加密文本

函数:

1. `base64.c`

   ```c
   char *base64_encode(const char * data, int data_len);//base64加密
   char *base64_decode(const char * data, int data_len);//base64解密
   ```

2. `pnglib.c`

   ```C
   int detect_png(char *file_path, pic_data *out);//解码png文件
   int write_png_file(char *file_name , pic_data *graph);//重写png文件
   ```

3. 没有了

//TODO

1. need to support other pictures formats
2. need to support higher channels
3. need to add data corruption detection
4. need to fix some memory corruption problems

