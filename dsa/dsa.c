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

int gen_rand(int min, int max)
{
    return rand() % (max - min + 1) + min;
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
        rnd = rand() % (max - min + 1) + min;
        if (is_prime(rnd) == 1) {
            break;
        }
    }

    return rnd;
}

int pow_mod_int(int a, int b, int n)
{
    int res = 1;

    for(int i = 0; i < b; i++) {
        res = res * a % n;
    }

    return res;
}

/* 署名鍵を生成する */
/* y := g^x mod p */
int gen_sign_key(DSA *dsa)
{
    /* pを生成 */
    dsa->p = gen_prime_rand(1000, 10000);

    /* qを生成 */
    dsa->q = gen_prime_rand(1000, 10000);

    /* gを生成 */
    dsa->g = gen_prime_rand(1000, 10000);

    /* xを生成 */
    dsa->x = gen_rand(1000, 10000);

    /* yを生成 */
    dsa->y = pow_mod_int(dsa->g, dsa->x, dsa->p);

    return 0;
}

/* 署名を生成する */
/* s = (H(m)+xr)/k mod q */
/* r = (g^k mod p) mod q */
int gen_dsa_sign(DSA *dsa, DSA_SIGN *dsa_sign, int *m, int *k)
{
    printf("sign\n");
    printf("m = %d\n", *m);

    /* kを生成 */
    *k = gen_rand(2, dsa->p-1);

    /* rを生成 */
    dsa_sign->r = pow_mod_int(dsa->g, *k, dsa->p) % dsa->q;

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
    int u1, u2, v;

    printf("verify\n");
    printf("m = %d\n", *m);

    /* wを生成 */
    int w = pow_mod_int(dsa_sign->s, -1, dsa->q);

    /* u1を生成 */
    u1 = (*m) * w % dsa->q;

    /* u2を生成 */
    u2 = dsa_sign->r * w % dsa->q;

    /* vを生成 */
    v = (pow_mod_int(dsa->g, u1, dsa->p) * pow_mod_int(dsa->y, u2, dsa->p)) % dsa->q;
    printf("v = %d\n", v);

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

    m = 123456;

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

    /* 値を表示 */
    printf("p = %d\n", dsa.p);
    printf("q = %d\n", dsa.q);
    printf("g = %d\n", dsa.g);
    printf("x = %d\n", dsa.x);
    printf("y = %d\n", dsa.y);
    printf("r = %d\n", dsa_sign.r);
    printf("s = %d\n", dsa_sign.s);

    /* 署名を検証 */
    if(verify_dsa_sign(&dsa, &dsa_sign ,&M) == 1) printf("署名は正しいです\n");
    else printf("署名は正しくありません\n");

    return 0;
}