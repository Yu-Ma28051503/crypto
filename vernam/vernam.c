/*
 * Vernam暗号を実装するプログラム
 * 注意点: 入力は全て小文字アルファベットで行うこと
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000

/* vernam暗号で暗号化する関数 */
void vernam(char *plain, char key, char *cipher)
{
    int i, len;

    len = strlen(plain);
    for (i = 0; i < len; i++) {
        cipher[i] = (plain[i] - 'a' + key - 'a') % 26 + 'a';
    }
    cipher[len] = '\0';

    // 表示
    printf("暗号文：%s\n", cipher);
}

/* vernam暗号で復号する関数 */
void de_vernam(char *cipher, char key, char *plain)
{
    int i, len;

    len = strlen(cipher);
    for (i = 0; i < len; i++) {
        plain[i] = (cipher[i] - key + 26) % 26 + 'a';
    }
    plain[len] = '\0';

    // 表示
    printf("復号文：%s\n", plain);
}

int main(void)
{
    char plain[MAX], key, cipher[MAX];
    int len;

    // 暗号化する平文を入力
    printf("小文字アルファベットのみで入力してください\n");
    printf("平文を入力してください：");
    fgets(plain, MAX, stdin);
    len = strlen(plain) - 1;
    plain[len] = '\0';

    // 時間をシード値として鍵を生成
    srand((unsigned)time(NULL));
    key = rand() % 26 + 'a';

    // 平文と鍵を表示
    printf("平文：%s\n", plain);
    printf("鍵：%c\n", key);

    // 暗号化
    vernam(plain, key, cipher);
    // 復号
    de_vernam(cipher, key, plain);

    return 0;
}
