#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>

static const char *message = "Hello world";
#define SHA1_DIGEST_LENGTH 20

static uint8_t keyMod1[256] = {
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

static uint8_t keyPubExp1[256] = {
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

static uint8_t keyPrivExp1[256] = {
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

static uint8_t key_prime1[128] = {
    0xe1, 0xc3, 0x78, 0x73, 0x31, 0x9c, 0x1b, 0xe9,
    0x04, 0xee, 0xaf, 0xfc, 0x94, 0xe0, 0x5b, 0xcd,
    0x76, 0x85, 0x51, 0x04, 0xfc, 0xa1, 0xf9, 0x80,
    0xf3, 0xb9, 0x10, 0xb2, 0x9e, 0xfc, 0xa5, 0x86,
    0xbb, 0x46, 0xb0, 0xc6, 0x6c, 0x42, 0xe9, 0x41,
    0x0f, 0x43, 0x35, 0xc1, 0x80, 0xa8, 0x8a, 0xf6,
    0x1f, 0x65, 0xbb, 0x18, 0x53, 0xf2, 0x91, 0x24,
    0xca, 0x9c, 0x60, 0xd3, 0x7c, 0x30, 0x82, 0x9b,
    0x41, 0x38, 0x78, 0x82, 0x00, 0xae, 0xc0, 0x63,
    0xde, 0xbb, 0xb3, 0x21, 0x78, 0xaa, 0x11, 0xdf,
    0x39, 0x8c, 0x44, 0xec, 0x3d, 0xca, 0x41, 0xfe,
    0x15, 0x49, 0xb1, 0x06, 0x05, 0x25, 0x73, 0xeb,
    0xc0, 0x40, 0x4a, 0xb2, 0x65, 0x12, 0x5c, 0x70,
    0x1b, 0x4b, 0x6e, 0xdc, 0x09, 0x01, 0xde, 0x62,
    0x90, 0x09, 0xa4, 0x04, 0xc7, 0x04, 0x17, 0xb2,
    0xd0, 0xc8, 0xbb, 0xa5, 0x5a, 0x1a, 0x0b, 0x61
};

static uint8_t key_prime2[128] = {
    0xc1, 0x1f, 0xb3, 0x1c, 0xb5, 0x76, 0xc1, 0xd0,
    0x9d, 0xd1, 0xe7, 0xdc, 0x43, 0x47, 0x73, 0x72,
    0xb1, 0xfd, 0x79, 0xf4, 0x10, 0x65, 0xae, 0xf4,
    0x55, 0x2e, 0xb9, 0xdb, 0x12, 0x9a, 0x92, 0x4a,
    0xbb, 0x6a, 0x11, 0x6f, 0x60, 0x1a, 0xcc, 0xd8,
    0xa4, 0xab, 0x2b, 0x60, 0x67, 0x85, 0xca, 0x7c,
    0x90, 0x7b, 0xec, 0x91, 0xba, 0xc5, 0x6c, 0xf8,
    0xd8, 0xc0, 0x6e, 0x04, 0x79, 0x4d, 0x57, 0xfb,
    0x4a, 0x19, 0xbe, 0x9e, 0x42, 0xbd, 0x06, 0x0f,
    0xc3, 0x81, 0xcb, 0x22, 0x65, 0x73, 0x36, 0x27,
    0xf2, 0x04, 0xb0, 0x2e, 0x69, 0xd4, 0x94, 0xca,
    0x49, 0xd6, 0x91, 0xbf, 0x32, 0x36, 0x75, 0xfd,
    0x7d, 0xf0, 0x7b, 0x69, 0x65, 0x86, 0xed, 0x62,
    0x77, 0x61, 0xd3, 0x2b, 0x32, 0x44, 0x85, 0x59,
    0x25, 0x02, 0x76, 0xec, 0xea, 0x2d, 0x31, 0x38,
    0x83, 0x20, 0x45, 0x2b, 0xd5, 0x4f, 0x71, 0xb1
};

static uint8_t key_exponent1[128] = {
    0xcc, 0x20, 0x69, 0x2f, 0xfe, 0x93, 0x1f, 0xc9,
    0x38, 0xe4, 0x77, 0xca, 0xf0, 0xf1, 0x4b, 0x10,
    0x23, 0x31, 0xb7, 0x67, 0xc2, 0xe4, 0xd1, 0x2b,
    0x7f, 0x32, 0xbd, 0x9e, 0x43, 0x92, 0x3c, 0x37,
    0xed, 0xec, 0xa4, 0xb9, 0x43, 0xf9, 0xdb, 0xdd,
    0x2f, 0x67, 0x9d, 0x9b, 0x30, 0x31, 0x34, 0x34,
    0x05, 0x7f, 0x7e, 0x03, 0x29, 0x24, 0x11, 0x88,
    0x2a, 0x89, 0xea, 0x22, 0x4e, 0x0f, 0xae, 0x87,
    0xa2, 0x18, 0xe5, 0x1c, 0xe6, 0x71, 0x1b, 0x13,
    0x31, 0xb8, 0x94, 0xe6, 0x96, 0xab, 0x6f, 0x2d,
    0x5e, 0x63, 0x2d, 0x8e, 0x4c, 0xf0, 0x2b, 0x6b,
    0x08, 0xe4, 0x70, 0x62, 0xf9, 0x14, 0x3c, 0x13,
    0xd4, 0xe8, 0x4c, 0xa1, 0xb0, 0x31, 0x41, 0xd3,
    0x3b, 0x5a, 0xa1, 0x09, 0xe2, 0x3b, 0x06, 0x50,
    0x27, 0xdc, 0x0f, 0xa9, 0x22, 0x28, 0xd9, 0x24,
    0x9d, 0x12, 0x62, 0x57, 0xc7, 0x77, 0xf5, 0x01
};

static uint8_t key_exponent2[128] = {
    0x8e, 0x24, 0x32, 0xc4, 0x72, 0x9b, 0x51, 0xd3,
    0xa6, 0x15, 0x14, 0xbe, 0x7d, 0x96, 0x5d, 0x56,
    0x91, 0xc2, 0xaa, 0xbc, 0x04, 0x45, 0x93, 0x1b,
    0xaa, 0x86, 0x95, 0xac, 0xb4, 0x68, 0xa8, 0x3b,
    0xf0, 0xed, 0x04, 0xa0, 0x20, 0xf0, 0x18, 0xcd,
    0x6b, 0x85, 0xc0, 0x8b, 0x58, 0x26, 0x94, 0xfe,
    0x51, 0xa9, 0xa4, 0x59, 0x2e, 0x4a, 0x99, 0x06,
    0x1f, 0x22, 0xa2, 0xd7, 0x10, 0x20, 0xe9, 0x1c,
    0xbf, 0x51, 0x6b, 0x87, 0x38, 0x3f, 0xbf, 0xf2,
    0x62, 0x47, 0x69, 0xa7, 0x8b, 0x5b, 0xdd, 0x04,
    0x51, 0x91, 0x39, 0xcb, 0x2f, 0x81, 0x93, 0x71,
    0xc0, 0x1e, 0xf9, 0x6e, 0xec, 0x1a, 0x41, 0x85,
    0xad, 0xe8, 0x0e, 0x2d, 0x68, 0xd8, 0x82, 0x67,
    0xf2, 0xce, 0x41, 0x8a, 0x8d, 0x98, 0xf9, 0x3c,
    0xbf, 0x8c, 0xac, 0xd3, 0x7b, 0x18, 0x80, 0x1d,
    0xc4, 0x05, 0x40, 0x64, 0x1e, 0xc0, 0x36, 0x41
};

static uint8_t key_coefficient[128] = {
    0x13, 0xb8, 0x9e, 0xdc, 0x48, 0xde, 0x7e, 0xb9,
    0x43, 0x37, 0x98, 0x7e, 0x97, 0xb6, 0x5b, 0x4c,
    0x23, 0x8a, 0xed, 0x75, 0xf5, 0x42, 0x37, 0x5a,
    0xe2, 0x5c, 0x04, 0xbc, 0x68, 0x81, 0xd9, 0x75,
    0x19, 0xba, 0x6e, 0x4f, 0x89, 0x63, 0xfe, 0xc2,
    0x26, 0xc5, 0x18, 0xb1, 0xc7, 0x03, 0xc0, 0x15,
    0x5d, 0x36, 0x8c, 0x2b, 0xa2, 0xa0, 0x34, 0x03,
    0x27, 0x25, 0xcd, 0xf5, 0x58, 0xf8, 0xa4, 0x95,
    0x3b, 0x88, 0x58, 0xd2, 0xf3, 0xe2, 0xe6, 0x92,
    0x06, 0x17, 0x82, 0x0d, 0x94, 0x4e, 0x7a, 0x3b,
    0x9e, 0xef, 0xa9, 0xe5, 0x3f, 0xbc, 0x4b, 0xa7,
    0xed, 0x9c, 0x80, 0xc7, 0x0a, 0xe2, 0x7e, 0x53,
    0x6c, 0xea, 0x3f, 0xae, 0xa8, 0x9d, 0x02, 0x44,
    0xf4, 0x5e, 0x9c, 0x35, 0x1d, 0x01, 0x3e, 0x98,
    0xc3, 0xe4, 0x41, 0x5a, 0x0f, 0x40, 0x25, 0x1c,
    0xda, 0x56, 0x4e, 0x70, 0x70, 0x0f, 0x0b, 0xea
};

static void _openssl_random_dump(char *s, uint8_t *buf, uint32_t size)
{
    uint32_t i;

    assert(s);
    assert(buf);

    printf("dump %s\n", s);
    for (i = 0; i < size; i++) {
        if (0 == (i & 0x7) && i)
            printf("\n");

        printf("0x%02x ", *(buf + i));
    }
    printf("\n\n");
}

static void _openssl_block_dump(char *s, uint8_t *buf, uint32_t size)
{
    uint32_t i;
    uint32_t round = size >> 4;

    assert(s);
    assert(buf);

    printf("dump %s\n", s);
    for (i = 0; i < round; i++) {
        printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x  0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                *(buf + (i << 4) + 0),
                *(buf + (i << 4) + 1),
                *(buf + (i << 4) + 2),
                *(buf + (i << 4) + 3),
                *(buf + (i << 4) + 4),
                *(buf + (i << 4) + 5),
                *(buf + (i << 4) + 6),
                *(buf + (i << 4) + 7),
                *(buf + (i << 4) + 8),
                *(buf + (i << 4) + 9),
                *(buf + (i << 4) + 10),
                *(buf + (i << 4) + 11),
                *(buf + (i << 4) + 12),
                *(buf + (i << 4) + 13),
                *(buf + (i << 4) + 14),
                *(buf + (i << 4) + 15));
    }
    printf("\n");
}

static int _openssl_rsa_verify(uint8_t *sig, uint32_t size)
{
    int     ret;
    int     type = NID_sha1;
    unsigned char   dgst[SHA1_DIGEST_LENGTH];
    unsigned int    dgstlen;
    unsigned int    keybits = 2048;
    unsigned int    keybytes;

    SHA_CTX sha1c;
    RSA *rsa = NULL;

    unsigned char *n;
    unsigned char *e;
    unsigned char *d;
    unsigned char *p;
    unsigned char *q;
    unsigned char *dmp1;
    unsigned char *dmq1;
    unsigned char *iqmp;

    n = keyMod1;
    e = keyPubExp1;
    d = keyPrivExp1;
    p = key_prime1;
    q = key_prime2;
    dmp1 = key_exponent1;
    dmq1 = key_exponent2;
    iqmp = key_coefficient;

    ret = SHA1_Init(&sha1c);
    assert(1 == ret);
    ret = SHA1_Update(&sha1c, message, strlen(message));
    assert(1 == ret);
    ret = SHA1_Final(dgst, &sha1c);
    assert(1 == ret);

    dgstlen = SHA1_DIGEST_LENGTH;

    rsa = RSA_new();
    if (rsa == NULL) {
        return -1;
    }

    keybytes = (keybits >> 3);

    rsa->p = BN_bin2bn(p, (keybytes >> 1), rsa->p);
    rsa->q = BN_bin2bn(q, (keybytes >> 1), rsa->q);
    rsa->dmp1 = BN_bin2bn(dmp1, (keybytes >> 1), rsa->dmp1);
    rsa->dmq1 = BN_bin2bn(dmq1, (keybytes >> 1), rsa->dmq1);
    rsa->iqmp = BN_bin2bn(iqmp, (keybytes >> 1), rsa->iqmp);

    rsa->n = BN_bin2bn(n, keybytes, rsa->n);
    rsa->e = BN_bin2bn(e, keybytes, rsa->e);
    rsa->d = BN_bin2bn(d, keybytes, rsa->d);

    ret = RSA_verify(type, dgst, dgstlen, sig, size, rsa);
    if (ret != 1) {
        printf("Verify Failed\n");
    } else {
        printf("Openssl verify OK\n");
    }

    RSA_free(rsa);

    return 0;
}

static int _openssl_rsa_signature(uint8_t *sig, uint32_t *size)
{
    int     ret;
    int     type = NID_sha1;
    unsigned char   digest[SHA1_DIGEST_LENGTH];
    unsigned int    digest_length;
    unsigned int    keybits = 2048;
    unsigned int    keybytes;
    SHA_CTX     sha1c;
    RSA         *rsa = NULL;

    unsigned char *n;
    unsigned char *e;
    unsigned char *d;
    unsigned char *p;
    unsigned char *q;
    unsigned char *dmp1;
    unsigned char *dmq1;
    unsigned char *iqmp;

    n = keyMod1;
    e = keyPubExp1;
    d = keyPrivExp1;
    p = key_prime1;
    q = key_prime2;
    dmp1 = key_exponent1;
    dmq1 = key_exponent2;
    iqmp = key_coefficient;

    ret = SHA1_Init(&sha1c);
    assert(1 == ret);
    ret = SHA1_Update(&sha1c, message, strlen(message));
    assert(1 == ret);
    ret = SHA1_Final(digest, &sha1c);
    assert(1 == ret);

    digest_length = SHA1_DIGEST_LENGTH;

    rsa = RSA_new();
    if (rsa == NULL) {
        return -1;
    }

    keybytes = (keybits >> 3);

    rsa->p = BN_bin2bn(p, (keybytes >> 1), rsa->p);
    rsa->q = BN_bin2bn(q, (keybytes >> 1), rsa->q);
    rsa->dmp1 = BN_bin2bn(dmp1, (keybytes >> 1), rsa->dmp1);
    rsa->dmq1 = BN_bin2bn(dmq1, (keybytes >> 1), rsa->dmq1);
    rsa->iqmp = BN_bin2bn(iqmp, (keybytes >> 1), rsa->iqmp);

    rsa->n = BN_bin2bn(n, keybytes, rsa->n);
    rsa->e = BN_bin2bn(e, keybytes, rsa->e);
    rsa->d = BN_bin2bn(d, keybytes, rsa->d);

    ret = RSA_sign(type, digest, digest_length, sig, size, rsa);
    if (ret != 1) {
        printf("Openssl sign Failed!\n");
    } else {
        printf("Openssl sign OK\n");
    }

    RSA_free(rsa);

    return 0;
}

static void _openssl_rsa_signature_verify()
{
    int         ret;
    uint8_t     ctext[256];
    uint32_t    size;

    ret = _openssl_rsa_signature(ctext, &size);
    if (0 == ret) {
        printf("PKCS #1 v1.5, OpenSSL Signature OK\n");
    } else {
        printf("PKCS #1 v1.5, OpenSSL Signature Fail\n");
    }

    _openssl_block_dump("signature", ctext, size);

    ret = _openssl_rsa_verify(ctext, size);
    if (0 == ret) {
        printf("PKCS #1 v1.5, OpenSSL verify OK\n");
    } else {
        printf("PKCS #1 v1.5, OpenSSL verify Fail\n");
    }
}


#define SetKey \
    key->n = BN_bin2bn(n, 256, key->n); \
    key->e = BN_bin2bn(e, 256, key->e); \
    key->d = BN_bin2bn(d, 256, key->d); \
    key->p = BN_bin2bn(p, 128, key->p); \
    key->q = BN_bin2bn(q, 128, key->q); \
    key->dmp1 = BN_bin2bn(dmp1, 128, key->dmp1); \
    key->dmq1 = BN_bin2bn(dmq1, 128, key->dmq1); \
    key->iqmp = BN_bin2bn(iqmp, 128, key->iqmp);

static void convert_key(RSA *key, uint32_t *key_len)
{
    static unsigned char *n = keyMod1;
    static unsigned char *e = keyPubExp1;
    static unsigned char *d = keyPrivExp1;
    static unsigned char *p = key_prime1;
    static unsigned char *q = key_prime2;
    static unsigned char *dmp1 = key_exponent1;
    static unsigned char *dmq1 = key_exponent2;
    static unsigned char *iqmp = key_coefficient;

    SetKey;
    *key_len = 256;
}

static int _openssl_rsa_encrypt(uint8_t *out, uint32_t *size)
{
    int         err = 0;
    int         num;
    uint32_t    clen = 0;
    RSA         *key;

    assert(out);
    assert(size);

    key = RSA_new();
    convert_key(key, &clen);
    key->flags |= RSA_FLAG_NO_CONSTTIME;

    num = RSA_public_encrypt(strlen(message), (unsigned char*)message,
            out, key, RSA_PKCS1_PADDING);
    if (num != 256) {
        printf("PKCS#1 v1.5 encryption failed!\n");
        err = -1;
    }

    RSA_free(key);

    *size = num;
    return err;
}

static int _openssl_rsa_decrypt(uint8_t *out, uint32_t size, uint8_t *plain, uint32_t *length)
{
    uint32_t    clen = 0;
    RSA         *key;

    assert(out);
    assert(plain);
    assert(length);

    key = RSA_new();
    convert_key(key, &clen);
    key->flags |= RSA_FLAG_NO_CONSTTIME;

    *length = RSA_private_decrypt(
            size, out, plain, key, RSA_PKCS1_PADDING);

    RSA_free(key);

    return 0;
}

static void _openssl_rsa_encrypt_decrypt()
{
    int         ret;
    uint8_t     ctext[256];
    uint32_t    size = 256;
    uint8_t     plain[256] = {0};
    uint32_t    length = 256;

    _openssl_random_dump("message", (uint8_t*)message, (uint32_t)strlen(message));

    ret = _openssl_rsa_encrypt(ctext, &size);
    if (0 == ret) {
        printf("PKCS #1 v1.5, OpenSSL Encrypto OK\n");
    } else {
        printf("PKCS #1 v1.5, OpenSSL Encrypto Fail\n");
    }

    _openssl_block_dump("rsa encrypto text", ctext, size);

    ret = _openssl_rsa_decrypt(ctext, size, plain, &length);
    if (0 == ret) {
        printf("PKCS #1 v1.5, OpenSSL decrypto OK\n");
    } else {
        printf("PKCS #1 v1.5, OpenSSL decrypto fail\n");
    }

    _openssl_random_dump("plain", plain, length);
}

int main(void)
{
    _openssl_rsa_encrypt_decrypt();

    _openssl_rsa_signature_verify();

    return 0;
}
