#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

int transfer(int);

int transfer(int sock)
{
    // NOTE: 受信時、送信時の動作の詳細設定: 今回は特別なフラグを設定しないので`0`とする
    int RECT_FLAG = 0, SEND_FLAG = 0;
    char send_buf[BUF_SIZE], recv_buf;
    int send_size, recv_size;

    while (1)
    {
        // NOTE: サーバーに送る文字列を取得
        printf("Please Input Message\n");
        scanf("%s", send_buf);

        // NOTE: 文字列を送信
        /**
         *  @param sock 接続済みのソケット
         *  @param send_buf 送信するデータへのポインタ
         *  @param len 送信するデータのサイズ(バイト数)
         *  @param strlen(send_buf) + 1 送信時の動作の詳細設定
         *  @return 実際に接続先に送信したデータのバイト数
         */
        send_size = send(sock, send_buf, strlen(send_buf) + 1, SEND_FLAG);
        if (send_size == -1)
        {
            printf("recv error\n");
            break;
        }
        if (recv_size == 0)
        {
            // NOTE: 受信サイズが0の場合は、相手が接続を閉じていると判断
            printf("connection ended\n");
            break;
        }

        // NOTE: 応答が0の場合はデータ送信終了
        if (recv_buf == 0)
        {
            printf("Finish connection\n");
            break;
        }
    }
    return 0;
}

int main(void)
{
    int sock, DEFAULT_PROTOCOL = 0;
    struct sockaddr_in addr;

    // NOTE: ソケットを作成
    /**
     *  @param domain プロトコルファミリー(アドレスファミリー)を指定
     *  @param type　ソケットのタイプを指定
     *  @param protocol　使用するプロトコルを指定
     */
    sock = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);

    // NOTE: 構造体を全て0にセット
    memset(&addr, 0, sizeof(struct sockaddr_in));

    /** サーバーのIPアドレスとボート番号の情報を設定 */
    // NOTE: アドレスファミリーを指定
    addr.sin_family = AF_INET;
    // NOTE: 使用するポート番号を指定
    addr.sin_port = htons((unsigned short)SERVER_PORT);
    // NOTE: 使用するIPアドレスを指定
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // NOTE: サーバーに接続要求を送信
    printf("Start connect...\n");
    /**
     *  @param sock 接続を行うソケット
     *  @param (struct sockaddr *)&addr　接続先(アドレスやポート番号など)の情報　ソケットに割り当てるアドレスやポート番号の情報 sockaddr_in構造体のインスタンスで、IPアドレスとポート番号を含む
     *  @param sizeof(struct sockaddr_in)　addrのサイズ(バイト数)
     */
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("connect error\n");
        close(sock);
        return -1;
    }
    printf("Finish connect! \n");

    // NOTE: 接続済みのソケットでデータのやり取りを行う
    transfer(sock);

    // NOTE: ソケットの通信をクローズ
    close(sock);
    return 0;
}