/*
* 一. RSA PEM文件格式
* 1. PEM私钥格式文件
*   -----BEGIN RSA PRIVATE KEY-----
*   -----END RSA PRIVATE KEY-----
* 2. PEM公钥格式文件
*   -----BEGIN PUBLIC KEY-----
*   -----END PUBLIC KEY-----
* 3. PEM RSAPublicKey公钥格式文件
*   -----BEGIN RSA PUBLIC KEY-----
*   -----END RSA PUBLIC KEY-----
*
* 二. OpenSSL密钥相关命令
* 1. 生成密钥
* 	openssl genrsa -out key.pem 1024
* 		-out 指定生成文件，此文件包含公钥和私钥两部分，所以即可以加密，也可以解密
* 		1024 生成密钥的长度
* 2. 提取PEM格式公钥
* 	openssl rsa -in key.pem -pubout -out pubkey.pem
* 		-in 指定输入的密钥文件
* 		-out 指定提取生成公钥的文件(PEM公钥格式)
* 3. 提取PEM RSAPublicKey格式公钥
* 	openssl rsa -in key.pem -RSAPublicKey_out -out pubkey.pem
* 		-in 指定输入的密钥文件
* 		-out 指定提取生成公钥的文件(PEM RSAPublicKey格式)
* 4. 公钥加密文件
* 	openssl rsautl -encrypt -in input.file -inkey pubkey.pem -pubin -out output.file
* 		-in 指定被加密的文件
* 		-inkey 指定加密公钥文件
* 		-pubin 表面是用纯公钥文件加密
* 		-out 指定加密后的文件
* 5. 私钥解密文件
* 	openssl rsautl -decrypt -in input.file -inkey key.pem -out output.file
* 		-in 指定需要解密的文件
* 		-inkey 指定私钥文件
* 		-out 指定解密后的文件
*
* 三. RSA相关API
* 1. 基本数据结构
*   struct {
*       BIGNUM *n;              // public modulus
*       BIGNUM *e;              // public exponent
*       BIGNUM *d;              // private exponent
*       BIGNUM *p;              // secret prime factor
*       BIGNUM *q;              // secret prime factor
*       BIGNUM *dmp1;           // d mod (p-1)
*       BIGNUM *dmq1;           // d mod (q-1)
*       BIGNUM *iqmp;           // q^-1 mod p
*       // ...
*   } RSA;
*
* 2. BN大数系列函数
* 	//新生成一个BIGNUM结构
* 	BIGNUM *BN_new(void);
* 	//释放一个BIGNUM结构，释放完后a=NULL;
* 	void BN_free(BIGNUM *a);
* 	//初始化所有项均为0，一般为BN_ init(&c)
* 	void BN_init(BIGNUM *);
* 	//将a中所有项均赋值为0，但是内存并没有释放
* 	void BN_clear(BIGNUM *a);
* 	//相当与将BN_free和BN_clear综合，要不就赋值0，要不就释放空间。
* 	void BN_clear_free(BIGNUM *a);
* 	//设置大数a为整数w
* 	int BN_set_word(BIGNUM *a, unsigned long w);
* 	//假如大数a能表示为long型，那么返回一个long型数
* 	unsigned long BN_get_word(BIGNUM *a);
* 	//产生一个加密用的强bits的伪随机数
* 	//若top=-1,最高位为0,top=0，最高位为1,top=1,最高位和次高位为1,bottom为真,随机数为偶数
* 	int BN_rand(BIGNUM *rnd, int bits, int top, int bottom);
* 	//将 a 转化为字符串存入to，to的空间必须大于BN_num_bytes(a)
* 	int BN_bn2bin(const BIGNUM *a, unsigned char *to);
* 	//将s中的len位的正整数转化为大数
* 	BIGNUM *BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret); 
* 	//将大数转化为16进制字符串
* 	char *BN_bn2hex(const BIGNUM *a);
* 	//将大数转化为10进制字符串
* 	char *BN_bn2dec(const BIGNUM *a);
* 	//将16进制字符串转成大数
* 	int BN_hex2bn(BIGNUM **a, const char *str);
* 	//将10进制字符串传成大数
* 	int BN_dec2bn(BIGNUM **a, const char *str);
*
* 3. RSA系列函数
*     //初始化一个RSA结构
*     RSA * RSA_new(void);
*     //释放一个RSA结构
*     void RSA_free(RSA *rsa);
*     //RSA私钥产生函数
*     //产生一个模为num位的密钥对，e为公开的加密指数，一般为65537（0x10001）
*     RSA *RSA_generate_key(int num, unsigned long e,void (*callback)(int,int,void *), void *cb_arg);
*     //判断位数函数, 返回RSA模的位数
*     int RSA_size(const RSA *rsa);
*     //测试p、q是否为素数
*     int RSA_check_key(RSA *rsa);
*
* 4. PEM系列函数
*     //从文件中加载RSAPublicKey格式公钥证书
*     RSA *PEM_read_RSAPublicKey(FILE *fp, RSA **x, pem_password_cb *cb, void *u);
*     //从BIO重加载RSAPublicKey格式公钥证书
*     RSA *PEM_read_bio_RSAPublicKey(BIO *bp, RSA **x, pem_password_cb *cb, void *u);
*     //输出RSAPublicKey公钥证书到文件
*     int PEM_write_RSAPublicKey(FILE *fp, RSA *x);
*     //输出RSAPublicKey公钥证书到BIO
*     int PEM_write_bio_RSAPublicKey(BIO *bp, RSA *x);
*
* 5. RSA加密API
*     int RSA_public_encrypt(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
*     参数说明：
*         flen: 要加密信息长度
*         from: 要加密信息
*         to: 加密后的信息
*         padding: 采取的加密方案, 分为: RSA_PKCS1_PADDING, RSA_PKCS1_OAEP_PADDING, RSA_SSLV23_PADDING, RSA_NO_PADDING
*
* 6. RSA解密API
*     int RSA_private_decrypt(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
*         参数说明：
*             flen: 要解密的信息长度
*             from: 要解密的信息
*             to: 解密后的信息
*             padding: 采取的解密方案
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#define _rsa_hard_key   0
#define _rsa_file_key   1

#define OPENSSL_DEBUG_LOG
#ifdef OPENSSL_DEBUG_LOG
#define OPENSSL_LOGS(format, a...) printf("%s(%d): " format, __FUNCTION__, __LINE__, ##a)
#else
#define OPENSSL_LOGS(format, a...)
#endif

#define PLAIN_PUB_KEY "plainPub.key"
#define PLAIN_PRIV_KEY "plainPriv.key"

static const char *message = "Hello world";

static uint8_t key_mod_2048[256] = {
    0xaa, 0x50, 0x52, 0x6f, 0x6c, 0xfb, 0x5a, 0x6a,
    0xf1, 0x81, 0x79, 0x21, 0x65, 0x3b, 0x48, 0x78,
    0x88, 0xea, 0x93, 0x0a, 0xe5, 0x8d, 0xc5, 0xcc,
    0x7d, 0xd0, 0xe2, 0x9d, 0x5c, 0xc9, 0x8a, 0xd8,

    0x24, 0xb5, 0x9c, 0xc9, 0xda, 0xa0, 0x3d, 0x40,
    0xcb, 0x70, 0xe2, 0x33, 0xe0, 0x20, 0xf5, 0xc0,
    0xcf, 0x7f, 0x96, 0xe0, 0xca, 0x90, 0x28, 0x92,
    0x2e, 0x53, 0xda, 0x52, 0xaa, 0x9a, 0x6d, 0x66,

    0x5a, 0x43, 0x4e, 0x1c, 0xb6, 0x8c, 0x96, 0x58,
    0x95, 0xda, 0xc4, 0x6a, 0x8b, 0x29, 0x16, 0x29,
    0xb6, 0x65, 0x18, 0xc5, 0x6b, 0xbc, 0x53, 0x68,
    0xb9, 0x0d, 0x0a, 0xf9, 0x83, 0x35, 0x8d, 0x01,

    0x77, 0x4f, 0x7e, 0xc3, 0x75, 0xd8, 0xb7, 0x57,
    0x0c, 0xbd, 0xe5, 0x00, 0xab, 0x83, 0xc2, 0xf7,
    0xc8, 0x9c, 0x5d, 0x0b, 0x50, 0xf0, 0xda, 0xfc,
    0x76, 0x27, 0x39, 0xb7, 0x50, 0x20, 0x6a, 0x73,

    0x16, 0x51, 0xb9, 0x69, 0xfb, 0xbf, 0xad, 0x9f,
    0xf7, 0x46, 0x5d, 0x23, 0x0d, 0x47, 0xe9, 0x5a,
    0x47, 0xf2, 0x5b, 0xc1, 0xb1, 0x3b, 0x15, 0x8c,
    0x09, 0x10, 0xaf, 0x95, 0x85, 0xb1, 0x66, 0xc5,

    0x49, 0x58, 0x89, 0x7c, 0x88, 0xbe, 0x97, 0xaf,
    0xbe, 0xf3, 0xa2, 0x48, 0x10, 0xfa, 0x17, 0xd4,
    0x87, 0x98, 0x02, 0xb6, 0xc1, 0xea, 0xa3, 0xaa,
    0xe8, 0xd2, 0xeb, 0x97, 0x8e, 0xba, 0x92, 0xf4,

    0xae, 0x10, 0xb5, 0x7a, 0xdc, 0x71, 0x89, 0xf8,
    0x62, 0x00, 0x17, 0x47, 0x65, 0xd0, 0xa3, 0x8a,
    0x6d, 0x6f, 0x5e, 0x90, 0x95, 0x2a, 0x9a, 0x38,
    0x5a, 0xca, 0xf9, 0xbe, 0x18, 0xc1, 0xd0, 0x09,

    0x01, 0x79, 0xce, 0x2b, 0xb6, 0xeb, 0xcc, 0xc5,
    0xa7, 0x97, 0x6b, 0x40, 0x8b, 0x46, 0x28, 0xab,
    0xd3, 0xa7, 0xde, 0x5f, 0x1a, 0x2e, 0x7e, 0x38,
    0xa8, 0x2d, 0x41, 0xe5, 0x02, 0xf6, 0xaf, 0x11
};
static uint8_t key_pub_exp_2048[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
static uint8_t key_priv_exp_2048[256] = {
    0x97, 0x83, 0x09, 0x32, 0xf3, 0x22, 0xd0, 0x97,
    0x1e, 0xed, 0xed, 0x92, 0x95, 0xfa, 0x1d, 0x48,
    0x8d, 0x95, 0x10, 0xe6, 0xba, 0xbc, 0x5a, 0xe0,
    0xc6, 0x74, 0xd3, 0x84, 0xe6, 0x8c, 0xad, 0x26,

    0x07, 0xd2, 0x40, 0xff, 0x19, 0xad, 0x3f, 0x90,
    0xaa, 0xeb, 0x6b, 0x70, 0x76, 0xcd, 0x1b, 0x93,
    0x41, 0x84, 0xe1, 0xf6, 0x6c, 0x7d, 0xbe, 0x79,
    0xfd, 0x18, 0xbf, 0xba, 0x96, 0xe6, 0xf6, 0xb7,

    0xfd, 0x48, 0xae, 0xee, 0xc0, 0x3f, 0x65, 0x68,
    0xbd, 0x37, 0x77, 0x50, 0xc9, 0x27, 0xed, 0xfd,
    0x81, 0xb3, 0x82, 0x4b, 0xee, 0xc2, 0x04, 0xf9,
    0xbe, 0xd6, 0x29, 0x93, 0x3a, 0xf4, 0xa2, 0xd2,

    0x51, 0x0f, 0xb9, 0x57, 0xad, 0x38, 0xf4, 0x31,
    0x4e, 0xb6, 0x88, 0x49, 0x06, 0x4c, 0x1b, 0x60,
    0x4f, 0x69, 0x3b, 0x24, 0xdc, 0x06, 0xc8, 0xe9,
    0x9b, 0x3a, 0xde, 0x0f, 0x84, 0x0a, 0xb2, 0x66,

    0xe7, 0x27, 0xcc, 0xad, 0xdb, 0x82, 0xb9, 0x18,
    0xd0, 0xcc, 0x88, 0x6c, 0xb1, 0xc2, 0x33, 0x36,
    0x56, 0x0c, 0xec, 0x22, 0x72, 0x61, 0x05, 0x85,
    0x1d, 0xc1, 0xc3, 0xae, 0x29, 0x34, 0x79, 0xdc,

    0x92, 0xe4, 0x8b, 0x55, 0x14, 0x34, 0xa3, 0xba,
    0xd5, 0x67, 0x01, 0x44, 0x2f, 0x3a, 0xc5, 0x5c,
    0x41, 0x88, 0x8a, 0x8b, 0x4b, 0xc7, 0x9c, 0x9b,
    0x86, 0xd5, 0xc2, 0x51, 0x78, 0xc3, 0x19, 0xc9,

    0x34, 0xe3, 0x7e, 0x88, 0x9e, 0x65, 0x10, 0x40,
    0xc2, 0x4f, 0x1d, 0xcc, 0x94, 0x26, 0x2d, 0x1d,
    0x21, 0xb5, 0xf8, 0xdf, 0xde, 0x49, 0x5d, 0xbd,
    0xfb, 0x87, 0x2d, 0x6b, 0x4f, 0x29, 0x55, 0x85,

    0xa0, 0x37, 0x12, 0x23, 0x08, 0x3b, 0x87, 0x1f,
    0x34, 0xd5, 0xf3, 0x82, 0x76, 0xf6, 0xb4, 0x7e,
    0x60, 0xdc, 0x75, 0x08, 0x9a, 0x43, 0xd7, 0xed,
    0x49, 0x2f, 0xc7, 0x2d, 0x1a, 0x20, 0x1c, 0x01
};

static void _openssl_do_digest_sha1(uint8_t *message, uint32_t message_len,
        uint8_t *digest, uint32_t *digest_len)
{
    int     ret;
    SHA_CTX     sha1c;

    memset(&sha1c, 0, sizeof(SHA_CTX));
    ret = SHA1_Init(&sha1c);
    assert(1 == ret);
    ret = SHA1_Update(&sha1c, message, message_len);
    assert(1 == ret);
    ret = SHA1_Final(digest, &sha1c);
    assert(1 == ret);
    *digest_len = 20;
}

static int _openssl_rsa_verify(uint8_t *digest, uint32_t digest_length,
        uint8_t *sig, uint32_t size, RSA *rsa_ctx)
{
    int     ret;
    int     type = NID_sha1;

    ret = RSA_verify(type, digest, digest_length, sig, size, rsa_ctx);
    if (ret != 1) {
        OPENSSL_LOGS("Verify Failed\n");
    } else {
        printf("Openssl verify OK\n");
    }

    return 0;
}

static int _openssl_rsa_signature(uint8_t *digest, uint32_t digest_length,
        uint8_t *sig, uint32_t *size, RSA *rsa_ctx)
{
    int     ret;
    int     type = NID_sha1;

    ret = RSA_sign(type, digest, digest_length, sig, size, rsa_ctx);
    if (ret != 1) {
        OPENSSL_LOGS("Openssl sign Failed!\n");
    } else {
        printf("Openssl sign OK\n");
    }

    return 0;
}

static void _openssl_generate_random_key()
{
    RSA *rsa;
    unsigned char *n_b;
    unsigned char *e_b;
    int n_size;
    int b_size;
    FILE *pub_key_fp;
    FILE *priv_key_fp;

    /* 产生RSA密钥 */
    rsa = RSA_generate_key(2048, 65537, NULL, NULL);

    /* 提取私钥 */
    priv_key_fp = fopen(PLAIN_PRIV_KEY, "w+");
    assert(priv_key_fp);
    PEM_write_RSAPrivateKey(priv_key_fp, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(priv_key_fp);

    /* 提取公钥 */
    n_b = (unsigned char*)malloc(RSA_size(rsa) * sizeof(unsigned char));
    assert(n_b);
    e_b = (unsigned char*)malloc(RSA_size(rsa) * sizeof(unsigned char));
    assert(e_b);

    n_size = BN_bn2bin(rsa->n, n_b);
    b_size = BN_bn2bin(rsa->e, e_b);

    RSA *pubrsa = RSA_new();
    pubrsa->n = BN_bin2bn(n_b, n_size, NULL);
    pubrsa->e = BN_bin2bn(e_b, b_size, NULL);

    pub_key_fp = fopen(PLAIN_PUB_KEY, "w+");
    assert(pub_key_fp);
    PEM_write_RSAPublicKey(pub_key_fp, pubrsa);
    fclose(pub_key_fp);

    RSA_free(rsa);
    RSA_free(pubrsa);

    return;
}

static void _openssl_rsa_enroll_key_hard_code(RSA *rsa_ctx, uint32_t keybits)
{
    unsigned char *n;
    unsigned char *e;
    unsigned char *d;
    unsigned int  keybytes;

    n = key_mod_2048;
    e = key_pub_exp_2048;
    d = key_priv_exp_2048;

    keybytes = (keybits >> 3);

    rsa_ctx->n = BN_bin2bn(n, keybytes, rsa_ctx->n);
    rsa_ctx->e = BN_bin2bn(e, keybytes, rsa_ctx->e);
    rsa_ctx->d = BN_bin2bn(d, keybytes, rsa_ctx->d);
}

static void _openssl_rsa_enroll_priv_key_from_file(RSA **rsa_ctx, uint32_t keybits)
{
    FILE *fp;

    // open the RSA private key PEM file 
    fp = fopen(PLAIN_PRIV_KEY, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open %s for RSA priv params\n", PLAIN_PRIV_KEY);
        assert(0);
    }
    if ((*rsa_ctx = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL)) == NULL) {
        fprintf(stderr, "Unable to read private key parameters\n");
        assert(0);
    }
    fclose(fp);

    // print 
    printf("Content of Private key PEM file\n");
    RSA_print_fp(stdout, *rsa_ctx, 0);
    printf("\n");    
}

static void _openssl_rsa_enroll_pub_key_from_file(RSA **rsa_ctx, uint32_t keybits)
{
    FILE *fp;

    // open the RSA public key PEM file
    fp = fopen(PLAIN_PUB_KEY, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open %s for RSA pub params\n", PLAIN_PUB_KEY);
        assert(0);
    }
    if ((*rsa_ctx = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL)) == NULL) {
        fprintf(stderr, "Unable to read public key parameters\n");
        assert(0);
    }                                                         
    fclose(fp);

    printf("Content of Public key PEM file\n");
    RSA_print_fp(stdout, *rsa_ctx, 0);
    printf("\n");    
}

/*
 * the melthod of get key
 *  1. get hard-code key
 *  2. read from file
 *  3. generate random key
 *
 * */
static void _openssl_rsa_signature_verify(uint32_t type)
{
    int         ret;
    uint8_t     ctext[256];
    uint32_t    size = 256;
    uint8_t     digest[20];
    uint32_t    digest_length = 20;
    RSA         *rsa_priv_ctx;
    RSA         *rsa_pub_ctx;

    rsa_priv_ctx = RSA_new();
    rsa_pub_ctx = RSA_new();

    switch (type) {
        case _rsa_hard_key:
            _openssl_rsa_enroll_key_hard_code(rsa_priv_ctx, 2048);
            _openssl_rsa_enroll_key_hard_code(rsa_pub_ctx, 2048);
            break;
        case _rsa_file_key:
            _openssl_generate_random_key();
            _openssl_rsa_enroll_priv_key_from_file(&rsa_priv_ctx, 2048);
            _openssl_rsa_enroll_pub_key_from_file(&rsa_pub_ctx, 2048);
            break;
        default:
            assert(0);
    }

    _openssl_do_digest_sha1((uint8_t*)message, strlen((const char*)message),
            digest, &digest_length);

    ret = _openssl_rsa_signature(digest, digest_length, ctext, &size, rsa_priv_ctx);
    if (0 == ret) {
        printf("OpenSSL Signature OK\n");
    } else {
        OPENSSL_LOGS("OpenSSL Signature Fail\n");
        assert(0);
    }

    ret = _openssl_rsa_verify(digest, digest_length, ctext, size, rsa_pub_ctx);
    if (0 == ret) {
        printf("OpenSSL verify OK\n");
    } else {
        OPENSSL_LOGS("OpenSSL verify Fail\n");
        assert(0);
    }

    RSA_free(rsa_priv_ctx);
    RSA_free(rsa_pub_ctx);
}

int main(void)
{
    _openssl_rsa_signature_verify(_rsa_hard_key);
    _openssl_rsa_signature_verify(_rsa_file_key);
    return 0;
}
