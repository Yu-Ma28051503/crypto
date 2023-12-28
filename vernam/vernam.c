/*
 * Vernam暗号を実装するプログラム
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000
#define CHAR_MAX 128

/* char*をビットで出力する関数 */
void printb(char *str)
{
    int len;

    len = strlen(str);
    for (int i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (str[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

/* vernam暗号で暗号化する関数 */
void vernam(char *plain, char *key, char *cipher)
{
    int len;

    len = strlen(plain);
    for (int i = 0; i < len; i++) {
        cipher[i] = plain[i] ^ key[i];
    }
    cipher[len] = '\0';

    // 表示
    printf("暗号文: %s\n", cipher);
    printf("暗号文(bit): ");
    printb(cipher);
}

/* vernam暗号で復号する関数 */
void de_vernam(char *cipher, char *key, char *plain)
{
    int len;

    len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        plain[i] = cipher[i] ^ key[i];
    }
    plain[len] = '\0';

    // 表示
    printf("復号文: %s\n", plain);
}

int main(void)
{
    char plain[MAX];
    char key[MAX];
    char cipher[MAX];
    int len;

    // 暗号化する平文を入力
    printf("平文を入力してください：");
    fgets(plain, MAX, stdin);
    len = strlen(plain) - 1;
    plain[len] = '\0';

    // 時間をシード値として鍵を生成
    srand((unsigned)time(NULL));
    for(int i = 0; i < len; i++) {
        key[i] = rand() % CHAR_MAX;
    }
    key[len] = '\0';

    // 平文と鍵を表示
    printf("平文：%s\n", plain);
    printf("鍵:");
    printb(key);

    // 暗号化
    vernam(plain, key, cipher);
    // 復号
    de_vernam(cipher, key, plain);

    return 0;
}
