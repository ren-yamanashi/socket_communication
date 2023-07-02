#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

// NOTE: strcmp: 二つの文字列を比較。一致する場合は0を、一致しない場合は0以外の値を返す
// NOTE: inet_addr: IPアドレスを表した文字列を数値に変換する
// NOTE: htons: PC上で扱うデータをネットワーク上で扱うデータに変換。
//       sin_port にはネットワークバイトオーダーのデータを設定する必要がある。PC上でのデータの扱い方（ホストバイトオーダー）とネットワーク上でのデータの扱い方が異なるので、ネットワーク上でのデータに変換してから sin_port に設定

int transfer(int);

int transfer(int sock)
{

    // NOTE: 受信時、送信時の動作の詳細設定: 今回は特別なフラグを設定しないので`0`とする
    int RECT_FLAG = 0, SEND_FLAG = 0;
    int recv_size, send_size;
    char recv_buf[BUF_SIZE], send_buf;
    while (1)
    {
        // NOTE: クライアントからのデータ(文字列)を受信
        /**
         *  @param sock 接続済みのソケット
         *  @param recv_buf 受信データを格納するバッファのアドレス
         *  @param BUF_SIZE bufのサイズ(バイト数)
         *  @param RECT_FLAG 受信時の動作の詳細設定
         *  @return 実際に接続先から受信したデータのバイト数
         */
        recv_size = recv(sock, recv_buf, BUF_SIZE, RECT_FLAG);
        if (recv_size == -1)
        {
            printf("recv error\n");
            break;
        }
        if (recv_size == 0)
        {
            // NOTE: 受信サイズが0の場合は、相手の接続が閉じていると判断
            printf("connection ended\n");
            break;
        }

        // NOTE: 正常な場合は、受信した文字列を表示
        printf("%s\n", recv_buf);

        // NOTE: 文字列がfinishならクライアントとの接続終了
        if (strcmp(recv_buf, "finish") == 0)
        {
            // NOTE: 接続終了を表す0を送信
            send_buf = 0;
            /**
             *  @param sock 接続済みのソケット
             *  @param &send_buf 送信するデータへのポインタ
             *  @param 1 送信するデータのサイズ(バイト数)
             *  @param SEND_FLAG 送信時の動作の詳細設定
             *  @return 実際に接続先に送信したデータのバイト数
             */
            send_size = send(sock, &send_buf, 1, SEND_FLAG);
            if (send_size == -1)
            {
                printf("send error\n");
                break;
            }
        }
        else
        {
            // NOTE: クライアントとの接続を継続
            send_buf = 1;
            /**
             *  @param sock 接続済みのソケット
             *  @param &send_buf 送信するデータへのポインタ
             *  @param 1 送信するデータのサイズ(バイト数)
             *  @param SEND_FLAG 送信時の動作の詳細設定
             *  @return 実際に接続先に送信したデータのバイト数
             */
            send_size = send(sock, &send_buf, 1, SEND_FLAG);
            if (send_size == -1)
            {
                printf("send error\n");
                break;
            }
        }
    }
    return 0;
}

int main(void)
{
    int w_addr, c_sock, DEFAULT_PROTOCOL = 0;
    struct sockaddr_in a_addr;

    // NOTE: ソケットを作成
    /**
     *  @param AF_INET プロトコルファミリー(アドレスファミリー)を指定: AF_INETはIPv4を使用することを指定。(IPv6はAF_INT6)
     *  @param SOCK_STREAM　ソケットのタイプを指定: 順序性と信頼性があり、双方向の接続されたバイトストリーム（TCPのような）を提供することを指定。データグラム（UDPのような接続なしのメッセージ）ソケットを使用する場合は、SOCK_DGRAMを指定する。
     *  @param DEFAULT_PROTOCOL　使用するプロトコルを指定
     */
    w_addr = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (w_addr == -1)
    {
        printf("socket error\n");
        return -1;
    }

    // NOTE: 構造体を全て0にセット
    memset(&a_addr, 0, sizeof(struct sockaddr_in));

    /** サーバーのIPアドレスとボート番号の情報を設定 */
    // NOTE: アドレスファミリーを指定
    a_addr.sin_family = AF_INET;
    // NOTE: 使用するポート番号を指定
    a_addr.sin_port = htons((unsigned short)SERVER_PORT);
    // NOTE: 使用するIPアドレスを指定
    a_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // NOTE: ソケットを特定のネットワークアドレス（IPアドレスとポート番号の組）に紐付ける
    /**
     *  @param w_addr ソケット
     *  @param `(const struct sockaddr *)&a_addr`　ソケットに割り当てるアドレスやポート番号の情報 sockaddr_in構造体のインスタンスで、IPアドレスとポート番号を含む
     *  @param `sizeof(a_addr)` addrのサイズ(バイト数)
     *  @return 成功したかのフラグ 成功時に0、エラー時に-1を返す
     */
    if (bind(w_addr, (const struct sockaddr *)&a_addr, sizeof(a_addr)) == -1)
    {
        printf("bind error\n");
        close(w_addr);
        return -1;
    }

    // NOTE: ソケットを接続待ちに設定
    /**
     *  @param sockfd 接続を待つソケット
     *  @param backlog 接続要求を保持する数
     *  @return 成功したかのフラグ 成功時に0、エラー時に-1を返す
     */
    if (listen(w_addr, 3) == -1)
    {
        printf("listen error\n");
        close(w_addr);
        return -1;
    }

    while (1)
    {
        // NOTE: 接続を受け付ける
        printf("Waiting connect...\n");
        /**
         *  @param w_addr 接続待ちの状態になっているソケット
         *  @param NULL　接続先の情報へのポインタ
         *  @param NULL　addrのサイズ(バイト数)へのポインタ
         *  @return 接続が確立されたソケット
         */
        c_sock = accept(w_addr, NULL, NULL);
        if (c_sock == -1)
        {
            printf("accept error\n");
            close(w_addr);
            return -1;
        }
        printf("Connected!!\n");

        // NOTE: 接続済みのソケットでデータのやり取り
        transfer(c_sock);

        // NOTE: ソケット通信をクローズ
        close(c_sock);

        // NOTE: 次の接続要求を受け付ける
    }

    // NOTE: 接続待ちのソケットをクローズ
    close(w_addr);

    return 0;
}
