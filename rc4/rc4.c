/*
 * RC4の実装
 *
 * build: $ gcc -o rc4 rc4.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define S_BOX_LEN 256
#define KEY_LEN 16 // 128bit key

typedef unsigned char byte;

void Swap(byte *x, byte *y)
{
    byte tmp = *x;
    *x = *y;
    *y = tmp;
}

void Printf_Hex(byte *data, int len)
{
    for (int i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            printf("\n");
        }

        printf(" %02X", data[i]);
    }
}

/* init S box */
void Init_Sbox(byte *s, byte *key, int key_len)
{
    int i = 0, j = 0;

    for (i = 0; i < S_BOX_LEN; i++) {
        s[i] = (byte)i;
    }

    for (i = 0; i < S_BOX_LEN; i++) {
        j = (j + s[i] + key[i % key_len]) & 0xFF;
        Swap(&s[i], &s[j]);
    }

    // debug
    printf("S = [\n");
    Printf_Hex(s, S_BOX_LEN);
    printf("\n]\n");
}

/* Pseudo-Random Generation Algorithm */
byte Prga(byte *s, int *i, int *j)
{
    *i = (*i + 1) & 0xFF;
    *j = (*j + s[*i]) & 0xFF;
    Swap(&s[*i], &s[*j]);

    return s[(s[*i] + s[*j]) & 0xFF];
}

/* generate key */
int Generate_Rc4_Key(byte *key, int key_len)
{
    int ret = 0;
    byte tmp_key[KEY_LEN] = {0xff, 0xfd, 0x26, 0x78,
                             0x61, 0x80, 0x37, 0x45,
                             0xd4, 0x18, 0x53, 0x1c,
                             0x90, 0x11, 0x97, 0xa5};
    
    memcpy(key, tmp_key, key_len);

    return ret;
}

int main(void)
{
    int i  = 0;
    int j  = 0;
    byte k = 0;

    char *plain = "Hello, RC4!";
    int plain_len = 0;
    byte cipher[100];
    byte dec[100];
    
    byte key[KEY_LEN];
    byte S_box[S_BOX_LEN];
    byte S_for_dec[S_BOX_LEN];

    if (Generate_Rc4_Key(key, KEY_LEN) != 0) {
        fprintf(stderr, "Error: generation key\n");

        return 1;
    }

    Init_Sbox(S_box, key, KEY_LEN);
    memcpy(S_for_dec, S_box, S_BOX_LEN);

    plain_len = strlen(plain);
    if (plain_len > 100) {
        fprintf(stderr, "Too long plain text\n");

        return 1;
    }

    printf("plain text:   %s\n", plain);
    // Printf_Hex(plain, plain_len);
    printf("ptext (hex): ");
    for (int n = 0; n < plain_len; n++) {
        printf(" %02X", plain[n]);
    }
    printf("\n");

    /* encryption */
    printf("cipher text: ");
    for (int n = 0; n < plain_len; n++) {
        k = Prga(S_box, &i, &j);
        cipher[n] = (byte)plain[n] ^ k;
        printf(" %02X", cipher[n]);
    }
    printf("\n");

    /* decryption */
    printf("dec text(hex):");
    for (int n = 0, i = 0, j = 0; n < plain_len; n++) {
        k = Prga(S_for_dec, &i, &j);
        dec[n] = cipher[n] ^ k;
        printf(" %02X", dec[n]);
    }
    printf("\n");

    if (strncmp(plain, (char *)dec, plain_len) == 0) {
        printf("valid!\n");
    }
    else {
        printf("invalid!\n");
    }

    return 0;
}
