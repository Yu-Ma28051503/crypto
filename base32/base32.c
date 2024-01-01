/*
 * base32を実装したプログラム
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX 100
#define SHIFT_BIT 8 

char* base32(char *str)
{
    long p = (long)str;  // 入力文字列
    char *q = NULL;  // 出力文字列
    const char *base32_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    int bit = 0x00;

    // 8ビットずつ5文字に変換
    for(int i = 0; i < 5; i++) {
        printf("i: %d\n", i);
        // 8ビットずつ取り出す
        bit = p & 0xff;
        // 5ビット分の文字を取り出す
        strncpy(q++, &base32_table[bit], 1);
        // 5ビット分の文字を取り出したので、次の5ビット分を取り出す
        p = p << 5;
    }

    return q;
}

int main(void)
{
    char *input = NULL;
    char output[MAX];
    int start = 0;  // 変換する文字列の開始位置

    // 入力
    printf("Input string: ");
    fgets(input, MAX, stdin);

    // 変換
    while(*input != '\0') {
        char* tmp = NULL;

        // 8Byte(40bit)を取り出す
        printf("start: %d\n", start);
        strncpy(tmp, input, 8);
        // 5文字に変換
        printf("tmp: %s\n", tmp);
        tmp = base32(tmp);
        // 出力文字列に追加
        strncat(output, tmp, 5);
        
        input += 5;
    }

    // 出力
    printf("Output string: %s\n", output);

    return 0;
}