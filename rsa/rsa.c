/*
 * RSA暗号を実装する
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1000
#define ULONG unsigned long
#define PUB_KEY_SIZE 8

/* RSAのステータスを入れておく */
typedef struct {
    ULONG p;
    ULONG q;
    ULONG n;
    ULONG e;
    ULONG d;
    ULONG phi_n;
} RSA;

/* 乱数を生成する関数 */
ULONG gen_rand(ULONG min, ULONG max)
{
    printf("gen_rand\n");
    ULONG rnd;

    rnd = rand() % (max - min + 1) + min;

    return rnd;
}

/* 素数かどうかを判定する関数 */
int is_prime(ULONG num)
{
    int i;
    printf("is_prime\n");

    if (num < 2) {
        return 0;
    } else if (num == 2) {
        return 1;
    } else if (num % 2 == 0) {
        return 0;
    }

    for (i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}

/* 2048ビットの公開鍵を生成 */
int gen_pub_key(RSA *rsa)
{
    ULONG p, q;
    printf("gen_pub_key\n");

    do {
        p = gen_rand(1 << PUB_KEY_SIZE-1, (1 << PUB_KEY_SIZE) - 1);
    }while (!is_prime(p));

    do {
        q = gen_rand(1 << PUB_KEY_SIZE-1, (1 << PUB_KEY_SIZE) - 1);
    }while (!is_prime(q) && p != q);

    return set_rsa_status(rsa, p, q);
}

/* 拡張ユークリッドの互除法 */
int extgcd(ULONG a, ULONG b, ULONG *x, ULONG *y)
{
    int d = a;
    printf("extgcd\n");
    if (b != 0) {
        d = extgcd(b, a % b, y, x);
        *y -= (a / b) * *x;
    } else {
        *x = 1;
        *y = 0;
    }
    return d;
}

int set_rsa_status(RSA *rsa, ULONG p, ULONG q)
{
    rsa->p = p;
    rsa->q = q;
    rsa->n = p * q;
    rsa->e = 3;
    rsa->phi_n = (p - 1) * (q - 1);

    return 0;
}

int rsa_pub_encrypt(int plain, RSA *rsa)
{
    int cipher;
    printf("rsa_pub_encrypt\n");

    cipher = plain ^ rsa->e % rsa->n;

    return cipher;
}

int rsa_pri_decrypt(int cipher, RSA *rsa)
{
    int plain;
    printf("rsa_pri_decrypt\n");

    plain = cipher ^ rsa->d % rsa->n;

    return plain;
}

int main(void)
{
    RSA rsa;
    // char plain[MAX];
    // char cipher[MAX];
    // char de_plain[MAX];

    // 乱数の初期化
    srand((unsigned int)time(NULL));

    // 公開鍵を生成
    if(gen_pub_key(&rsa) == 0) {
        printf("公開鍵の生成に成功しました\n");
    } else {
        printf("公開鍵の生成に失敗しました\n");
    }

    // 秘密鍵を生成
    extgcd(rsa.e, rsa.phi_n, &rsa.d, &rsa.d);

    // RSAの構造体の値を出力
    printf("p = %lu\n", rsa.p);
    printf("q = %lu\n", rsa.q);
    printf("n = %lu\n", rsa.n);
    printf("e = %lu\n", rsa.e);
    printf("d = %lu\n", rsa.d);
    printf("phi_n = %lu\n", rsa.phi_n);

    // // 平文を入力
    // printf("平文を入力してください：");
    // fgets(plain, MAX, stdin);
    // plain[strlen(plain) - 1] = '\0';

    // // 暗号化
    // for (int i = 0; i < strlen(plain); i++) {
    //     cipher[i] = rsa_pub_encrypt(plain[i], &rsa);
    // }

    // 復号


    return 0;
}