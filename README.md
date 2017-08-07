# OpenBeacon

## Firmwareの焼き方

[Firmwareの焼き方](./docs/README.md)

##概要
NordicｎRF51,ｎRF52で、すでにビルドされているhexファイルを書き込む方法を説明します。ここではNodic NRF51を使用している技適認証済みBLEモジュールRaytacMDBT40を例としています。

## 環境構築
### nrfgoStudioをインストール
Windowsを用意。Windowsの場合は、ファームウェア書き込みソフトnRFgo Studioがあり、Windows64ビット版nRFgo Studio-Win64ダウンロードし、typicalに設定しインストールします。
ダウンロード先：
https://www.nordicsemi.com/jpn/node_176/2.4GHz-RF/nRFgo-Starter-Kit

![nrfgoStduioDownload](https://github.com/FaBoPlatform/OpenBeacon/Photo/DownLoadG01.png "nrfgoStudio")

### ソフトデバイスをインストール

nrfgoStudioをインストールしたら、無線やデータを管理する基本ソフトウェアのソフトデバイスを準備する。ソフトデバイスには３種類あり、目的にあったソフトウェアデバイスを使用しなくてはいけない。ビーコンは、ペリフェラル機器なのでs110セントラル機器はS120,ペリフェラルとセントラル機器になるものは、s130をダウンロードして、指定してください。バージョンにもご注意ください。
ソフトデバイスをダウンロード
http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF51822

s110の場合は、S110-SD-v8をダウンロード（2016/4/8現在）

s130の場合は、S130-SDをダウンロード(2016/4/8現在)　

##ファームウェア（FW)を書き込む

### 接続方法
Nordic nRF51882は、ARMマイコンが内蔵してあり、ARMマイコンのデバッグにはJ-linkが必要になる。J-linkにはARMマイコンによってターゲットがあり、Nordic nRF51882(ARM M0)は安価なJ-link lite(OEMによる購入)が必要となります。書き込みのインターフェースにはJ-TAGの機能縮小版のSWDで通信を行います。
書き込むには、（BLEなど）ターゲットにも３Vの電源が必要になります。nRFgo studioを起動してターゲットをJ-linkと接続してnRF5x Programmingを選択すると、J-linkLiteのLEDが点滅しアクセスが始まる。（Windwos10ではnrfgoStudioが落ちやすいです。）

### フラッシュメモリを初期化
RaytacBeacon MDBT40の場合は、はじめからデフォルトのテストファームウェアが入っており、ファームウェアを更新する場合でも、フラッシュメモリ領域に残っているので、 すべて初期化や消去しなくてはいけない。nrfgoStudioで簡単に消去できます。
nrfgoStudioの消去ボタンを押します。

### ソフトデバイスの書き込み
ソフトデバイスをスタック領域に書き込みます。 Softdevieタブを選択して、ファイル解凍したファイルの中にs110_nrf51_8.0.0_softdevice.hexがある。これを選択し書き込みます。

ソフトデバイスが書き込みが成功すると、Region0(Softdevice)ができます。

### ファームウェアを書き込む
バイナリ化されたファームウェアを書き込む。Program Applicationタブを選択して、任意の.hexファイルを選択しWriteボタンを押して書き込む。compreteの表示がでたら、終了。パケットスニファー等で電波が出ているかを確認する。なお、ファームウェアの偽造防止のため、吸出し防止する場合は、チェックをすることで、禁止することができます。
