# OpenBeacon

## Firmwareの焼き方

[Firmwareの焼き方](./docs/README.md)

## 概要
NordicｎRF51またはｎRF52で、すでにビルドされているhexファイルを書き込む方法を説明します。ここではNodic NRF51を使用している技適認証済みBLEモジュールRaytacMDBT40を例としています。ファームウェアは、ソフトデバイスとアプリケーションの２つで構成されております。

## 環境構築
### nrfgoStudioをインストール
Windowsを用意。Windowsの場合は、GUIによりファームウェア書き込みできるWindowsアプリケーションnRFgo Studioがあり、Windows64ビット版nRFgo Studio-Win64ダウンロードし、typicalに設定しインストールします。さらに、自動的にウィザードが立ち上がり、Commnad-Toolチェーンも一緒にインストールします。
ダウンロード先：
https://www.nordicsemi.com/jpn/node_176/2.4GHz-RF/nRFgo-Starter-Kit

![nrfgoStduioDownload](./Photo/DownLoadG01.jpg  "nrfgoStudio")
nRFgoダウンロード

### ソフトデバイスをインストール

nrfgoStudioをインストールしたら、無線やデータを管理する基本ソフトウェアのソフトデバイスを準備する。ソフトデバイスには３種類あり、目的にあったソフトウェアデバイスを使用しなくてはいけない。ビーコンは、ペリフェラル機器なのでs110。セントラルはS120,ペリフェラルとセントラルになるものは、s130をダウンロードして、指定してください。なお、バージョンにもご注意ください。高機能になるほど容量が大きいのでRAMやROMの空き容量など注意が必要になります。

ソフトデバイスをダウンロード
http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF51822

![Softdevice](./Photo/Softdevice.png  "Softdevice")

・ダウンロードできるソフトデバイス一覧

nRF51であれば、以下のものをダウンロードすれば十分でしょう。
s110の場合は、S110-SD-v8をダウンロード
s130の場合は、S130-SDをダウンロード　

## ファームウェア（FW)を書き込む

### 接続方法
Nordic nRF51882のコアはARMマイコンが内蔵してあり、ARMマイコンのデバッグにはJ-linkが必要になる。J-linkにはARMマイコンによってターゲット対象が異なり、Nordic nRF51882(ARM M0)は安価なJ-link liteのグレード以上(OEMによる購入)が必要となります。（使い勝手は多少悪くなりますがnRF51DKでも書き込みは可能です。）書き込みのインターフェースにはJ-TAGの機能縮小版のSWDで通信を行います。また、書き込むには、（BLEなど）ターゲットにも３Vの電源が必要になります。Fabo 2004 JLinkを用意し、下記の写真のようにします。

![Connectting](./Photo/Connetting.JPG  "Connectting")
接続の様子

### フラッシュメモリを初期化
RaytacBeacon MDBT40の場合は、はじめからデフォルトのテストファームウェアが入っており、ファームウェアを更新する場合でも、フラッシュメモリ領域に残っているので、 すべて初期化や消去しなくてはいけない。nrfgoStudioで簡単に消去できます。

・ターゲットとアクセス
nRFgo studioを起動してターゲットをJ-linkと接続してnRF5x Programmingを選択すると、J-linkLiteのLEDが点滅しアクセスが始まる。（Windwos10ではnrfgoStudioが落ちやすいです。）

・ファームウェア消去
nrfgoStudioの Erase allボタンを押します。

![Erase_LI.jpg](./Photo/Erase_LI.jpg  "Erase_LI")
ファームウェア消去後、空きフラッシュROMが256KBになる。（nRF51の中には、128KBサイズになるものがあるので注意、nRF52は512KB）


### ソフトデバイスの書き込み
ソフトデバイスをスタック領域に書き込みます。 Softdevieタブを選択して、任意のファイル解凍したファイルの中にs130_nrf51_2.0.1_softdevice.hex（今回の場合）がある。これを選択し書き込みます。


![ProgrammingSoftDevice_LI](./Photo/ProgrammingSoftDevice_LI.jpg  "ProgrammingSoftDevice_LI")
任意のソフトデバイスを選択。programボタンを押すことで書き込みが開始されます。

![ProgrammingSoftDeviceAfter_LI](./Photo/ProgrammingSoftDeviceAfter_LI.jpg  "ProgrammingSoftDeviceAfter_LI")
ソフトデバイスが書き込みが成功すると、successfullyが表示され、Region0(Softdevice)ができます。

### ファームウェアを書き込む

バイナリ化されたファームウェアを書き込む。Program Applicationタブを選択して、任意の.hexファイルを選択しWriteボタンを押して書き込む。successfullyの表示がでたら、終了。パケットスニファー等で電波が出ているかを確認する。なお、ファームウェアの偽造防止のため、吸出し防止する場合は、Lock entire chip from readback チェックをすることで、禁止することができます。

![ProgrammingApplication](./Photo/ProgrammingApplication_LI.jpg  "ProgrammingApplication_LI")
任意のアプリケーションを選択。programボタンで書き込みが開始されます。

![ProgrammingApplicationAfter](./Photo/ProgrammingApplicationAfter_LI.jpg  "ProgrammingApplicationAfter_LI")
アプリケーションが書き込みが成功すると、Region１にアプリケーションが書き込まれます。

書き込み後は、電波が出ているかどうか、BLE対応パケットスニファやアプリケーション（iBeaconで使えるiPhoneアプリAkabeaconManagerなど)により確認してください。
