/*
 * Diffie-Hellman key exchange server
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

#define DEFAULT_PORT 50000

// 整数の累乗を計算する
int pow_int(int a, int b)
{
    int i = 0;
    int ret = 1;

    for(i = 0; i < b; i++) {
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
    int port = DEFAULT_PORT;  // ポート番号
    struct sockaddr_in addr;  // ソケットアドレス
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);

    // DH鍵交換のための変数
    int p = (1 << 10) - 1;  // 公開される大きめの素数
    int g = 1234;  // 公開される底
    int b = 0;  // サーバの秘密鍵
    int B = 0;  // サーバの公開鍵
    int A = 0;  // クライアントの公開鍵
    int s = 0;  // 共通鍵

    // ソケットの作成
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "socket");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));

    // ソケットアドレスの設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // ソケットにソケットアドレスを割り当てる
    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "bind");
        return -1;
    }

    // 接続の受付け
    if(listen(sockfd, SOMAXCONN) == -1) {
        fprintf(stderr, "listen");
        return -1;
    }

    // クライアントからの接続を受け付ける
    printf("waiting connection...");
    if((ret = accept(sockfd, (struct sockaddr *)&client_addr, &len)) == -1) {
        fprintf(stderr, "accept");
        return -1;
    }
    printf("accepted");

    // ここからDH鍵交換の処理を記述する
    // 乱数の初期化
    srand((unsigned int)time(NULL));
    b = rand() % ((p-1) - 1 + 1) + 1;
    printf("b = %d\n", b);
    B = pow_int(g, b) % p;
    printf("B = %d\n", B);

    // クライアントから公開鍵を受信する
    if(recv(ret, &A, sizeof(A), 0) == -1) {
        fprintf(stderr, "recv");
        return -1;
    }

    // クライアントに公開鍵を送信する
    if(send(ret, &B, sizeof(B), 0) == -1) {
        fprintf(stderr, "send");
        return -1;
    }

    // 共通鍵を生成する
    s = gen_secret_key(A, b, p);
    printf("s = %d\n", s);

    // ソケットのクローズ
    close(ret);
    close(sockfd);

    return 0;
}