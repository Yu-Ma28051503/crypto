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
int pow_mod_int(int a, int b, int n)
{
    int ret = 1;

    for(int i = 0; i < b; i++) {
        ret = ret * a % n;
    }

    return ret;
}

int main(void)
{
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
        fprintf(stderr, "socket\n");
        return -1;
    }

    // サーバに接続する
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(50000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // TCPコネクションの確立
    printf("connecting...\n");
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "connect\n");
        close(sockfd);
        return -1;
    }
    printf("connected\n");

    // ここからDH鍵交換の処理を記述する
    // 乱数の初期化
    srand((unsigned int)time(NULL));
    a = rand() % ((p-1) - 1 + 1) + 1;
    printf("a = %d\n", a);
    A = pow_mod_int(g, a, p);
    printf("A = %d\n", A);

    // サーバに公開鍵を送信する
    if(send(sockfd, &A, sizeof(A), 0) <= 0) {
        fprintf(stderr, "send");
        close(sockfd);
        return -1;
    }

    // サーバから公開鍵を受信する
    if(recv(sockfd, &B, sizeof(B), 0) <= 0) {
        fprintf(stderr, "recv");
        close(sockfd);
        return -1;
    }

    // 共通鍵を生成する
    s = pow_mod_int(B, a, p);
    printf("s = %d\n", s);

    // ソケットのクローズ
    close(sockfd);

    return 0;
}