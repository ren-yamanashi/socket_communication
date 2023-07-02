# 概要

サーバーはソケットに PC 自身の IP アドレスを設定し、クライアントは`connect` で接続要求を送る先のアドレスを PC 自身のアドレスに設定している。

つまり、クライアントが`connect` を実行すると、同じ PC のサーバーのソケットに接続しに行くようになる。

なのでこのプログラムでは、クライアント、サーバーどちらのIPアドレスも`127.0.0.1`を設定しています
これはループバックアドレスと呼ばれ、プログラムを実行するPC自身のアドレスを示すアドレスです。

<br />

# プログラムの実行

## コンパイル

```bin
make compileServer
make compileClient
```

server.exe と client.exe が生成されます。
これらを実行することでプログラムを起動することができます

<br />
<br />

## サーバーファイルの実行

ターミナルを立ち上げ、以下のコマンドを実行します。

```bin
make runServer
```

`server.exe`では、listen が実行され、ソケットは接続待ち状態になります。さらに accept が実行されて接続要求が来るまで待機している状態になります。

<br />
<br />

## クライアントファイルの実行

先ほどとは別のターミナルウィンドウから以下のコマンドを実行します。

```bin
make runClient
```

<br />
<br />

`client.exe`は`server.exe`が作成したソケットに対して、`connect`を行います。
`server.exe`のソケットは既に接続待ち状態なので、`client.exe`は`connect`に成功して、データのやり取りに移行します。

```bin
Start connect...
Finish connect!
```

<br />
<br />

接続確立後は、client.exe に文字列を入力すれば、それを`client.exe`が`server.exe`に送信します。
それを受信した`server.exe`が受信した文字列をターミナルに表示します。
`client.exe` に文字列を入力してからエンターキーを押すと、`server.exe` 側でその文字列が表示されることが確認できると思います。

```bin
Start connect...
Finish connect!
Please Input Message
```
