
#ifndef MD5_FORENCRPTY_H
#define MD5_FORENCRPTY_H
typedef unsigned int md5_int;
struct MD5_struct {
    md5_int A;
    md5_int B;
    md5_int C;
    md5_int D;
    md5_int lenbuf;
    char buffer[128];
};
void md5_init(struct MD5_struct *ctx,char * buffer);
void md5_process(struct MD5_struct * ctx);
char * md5_fini(struct MD5_struct *ctx);
void md5_buffer_full(struct MD5_struct * ctx);
char *md5(char *in);
#endif