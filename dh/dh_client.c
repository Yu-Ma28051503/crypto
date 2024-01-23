/*
 * Diffie-Hellman key exchange client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// 整数の累乗を計算する
int pow_int(int a, int b)
{
    int ret = 1;

    for(int i = 0; i < b; i++) {
        ret = ret * a;
    }

    return ret;
}

// DH鍵共有から得た値で共通鍵を生成する
int gen_secret_key(int a, int b, int p)
{
    int i = 0;
    int ret = 1;

    for(i = 0; i < b; i++) {
        ret = (ret * a) % p;
    }

    return ret;
}

int main(void)
{
    int ret = 0;
    int sockfd = 0;
    struct sockaddr_in addr;  // ソケットアドレス

    // DH鍵交換のための変数
    int p = (1 << 10) - 1;  // 公開される大きめの素数
    int g = 1234;  // 公開される底
    int a = 0;  // クライアントの秘密鍵
    int A = 0;  // クライアントの公開鍵
    int B = 0;  // サーバの公開鍵
    int s = 0;  // 共通鍵

    // ソケットの作成
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket");
        return -1;
    }

    // サーバに接続する
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(50000);
    addr.sin_addr.s_addr = inet_addr("1270.0.0.1");

    // TCPコネクションの確立
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "connect");
        return -1;
    }
    printf("connected");

    // ここからDH鍵交換の処理を記述する
    // 乱数の初期化
    srand((unsigned int)time(NULL));
    a = rand() % ((p-1) - 1 + 1) + 1;
    printf("a = %d\n", a);
    A = pow_int(g, a) % p;
    printf("A = %d\n", A);

    // サーバに公開鍵を送信する
    if(send(sockfd, &A, sizeof(A), 0) == -1) {
        fprintf(stderr, "send");
        return -1;
    }

    // サーバから公開鍵を受信する
    if(recv(sockfd, &B, sizeof(B), 0) == -1) {
        fprintf(stderr, "recv");
        return -1;
    }

    // 共通鍵を生成する
    s = gen_secret_key(B, a, p);
    printf("s = %d\n", s);

    // ソケットのクローズ
    close(sockfd);

    return 0;
}