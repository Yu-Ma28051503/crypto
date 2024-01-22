/*
 * DSAを実装する
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX 100

typedef struct {
    int p;  
    int q;
    int g;
    int x;  // 署名鍵
    int y;  // 検証鍵
} DSA;

typedef struct {
    int r;
    int s;
} DSA_SIGN;

/* 乱数を生成する関数 */
int gen_rand(int min, int max)
{
    int rnd;

    rnd = rand() % (max - min + 1) + min;

    return rnd;
}

/* 素数かどうかを判定する関数 */
int is_prime(int num)
{
    int i;

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

/* 素数の乱数を生成する */
int gen_prime_rand(int min, int max)
{
    int rnd;

    while (1) {
        rnd = gen_rand(min, max);
        if (is_prime(rnd) == 1) {
            break;
        }
    }

    return rnd;
}

/* 署名鍵を生成する */
/* y := g^x mod p */
int gen_sign_key(DSA *dsa)
{
    /* pを生成 */
    dsa->p = gen_prime_rand(100, 1000);

    /* qを生成 */
    dsa->q = gen_prime_rand(100, 1000);

    /* gを生成 */
    dsa->g = gen_prime_rand(100, 1000);

    /* xを生成 */
    dsa->x = gen_rand(100, 1000);

    /* yを生成 */
    dsa->y = (int)pow(dsa->g, dsa->x) % dsa->p;

    return 0;
}

/* 署名を生成する */
/* s = (H(m)+xr)/k mod q */
/* r = (g^k mod p) mod q */
int gen_dsa_sign(DSA *dsa, DSA_SIGN *dsa_sign, int *m, int *k)
{
    /* kを生成 */
    *k = gen_rand(2, dsa->p-1);

    /* rを生成 */
    dsa_sign->r = (int)pow(dsa->g, *k) % dsa->p % dsa->q;

    /* sを生成 */
    dsa_sign->s = ((*m) + (dsa->x * dsa_sign->r)) / (*k) % dsa->q;

    return 0;
}

/* 署名を検証する */
/* w := s^{-1} mod q */
/* u1 := H(m)w mod q */
/* u2 := rw mod q */
/* v := (g^u1y^u2 mod p) mod q */
/* v = r ならば、署名は正しい */
int verify_dsa_sign(DSA *dsa, DSA_SIGN *dsa_sign, int *m)
{
    int w, u1, u2, v;

    /* wの生成 */
    w = (int)pow(dsa_sign->s, -1) % dsa->q;

    /* u1の生成 */
    u1 = (*m) * w % dsa->q;

    /* u2の生成 */
    u2 = (dsa_sign->r) * w % dsa->q;

    /* vの生成 */
    v = ((int)pow(dsa->g, u1) * (int)pow(dsa->y, u2)) % dsa->p % dsa->q;

    if(v == dsa_sign->r) return 1;

    return 0;
}


int main(void)
{
    // char str[MAX];  // 署名する文字列
    int m;  // 文字列を数値に変換したもの
    int M;  // M := H(m)
    int k;  // 署名生成のための乱数
    DSA dsa;
    DSA_SIGN dsa_sign;

    srand((unsigned int)time(NULL));

    /* 署名鍵を生成 */
    gen_sign_key(&dsa);

    /* 署名する文字列を入力 */
    // printf("署名する文字列を入力してください\n");
    // scanf("%s", str);

    m = 1234;

    /* 文字列を数値に変換 */
    // m = 0;
    // for (int i = 0; i < strlen(str); i++) {
    //     m += (int)str[i];
    // }

    /* 文字列をハッシュ値に変換 */
    M = m % dsa.q;
    printf("M = %d\n", M);

    /* 署名を生成 */
    gen_dsa_sign(&dsa, &dsa_sign, &M, &k);

    /* 署名を検証 */
    if(verify_dsa_sign(&dsa, &dsa_sign ,&M) == 1) printf("署名は正しいです\n");
    else printf("署名は正しくありません\n");

    return 0;
}