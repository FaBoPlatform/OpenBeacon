# OpenBeacon

Line Simple Beaconのソースと、バイナリを公開しています。
バイナリ版は、コマンドラインで任意のhwidを割り振る事が可能です。

## Command

line_beacon.hexに、独自のhwidを設定するコマンドは以下の通り。

```shell
// hwid "0107cbfb8a"の場合（最初が0x01の場合は１行目は実行しなくてもOK）
nrfjprog -f NRF51 --memwr 0x10001080 --val 0x01
nrfjprog -f NRF51 --memwr 0x10001084 --val 0x07cbfb8a

// メッセージなしの場合は実行しなくてもOK
// メッセージ長（1~13）
nrfjprog -f NRF51 --memwr 0x10001088 --val 13
// メッセージ
nrfjprog -f NRF51 --memwr 0x1000108c --val 0x01020304
nrfjprog -f NRF51 --memwr 0x10001090 --val 0x05060708
nrfjprog -f NRF51 --memwr 0x10001094 --val 0x090a0b0c
nrfjprog -f NRF51 --memwr 0x10001098 --val 0x0d

// 長さによってデータ部は０パディングしてください。
nrfjprog -f NRF51 --memwr 0x10001088 --val 3
nrfjprog -f NRF51 --memwr 0x1000108c --val 0x00010203
```
