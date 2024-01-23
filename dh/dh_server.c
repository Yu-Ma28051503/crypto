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

    // ソケットアドレスの設定
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // ソケットにソケットアドレスを割り当てる
    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "bind\n");
        return -1;
    }

    // 接続の受付け
    if(listen(sockfd, SOMAXCONN) == -1) {
        fprintf(stderr, "listen\n");
        close(sockfd);
        return -1;
    }

    // 乱数の初期化
    srand((unsigned int)time(NULL));
    b = rand() % ((p-1) - 1 + 1) + 1;

    printf("waiting connection...\n");

    // クライアントからの接続を受け付ける
    if((ret = accept(sockfd, (struct sockaddr *)&client_addr, &len)) == -1) {
        fprintf(stderr, "accept");
        close(sockfd);
        return -1;
    }
    printf("accepted\n");

    // ここからDH鍵交換の処理を記述する
    printf("b = %d\n", b);
    B = pow_mod_int(g, b, p);
    printf("B = %d\n", B);

    // クライアントから公開鍵を受信する
    if(recv(ret, &A, sizeof(A), 0) <= 0) {
        fprintf(stderr, "recv");
        close(ret);
        close(sockfd);
        return -1;
    }

    // クライアントに公開鍵を送信する
    if(send(ret, &B, sizeof(B), 0) <= 0) {
        fprintf(stderr, "send");
        close(ret);
        close(sockfd);
        return -1;
    }

    // 共通鍵を生成する
    s = pow_mod_int(A, b, p);
    printf("s = %d\n", s);

    // ソケットのクローズ
    close(ret);
    close(sockfd);

    return 0;
}