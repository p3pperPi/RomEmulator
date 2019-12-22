# ROM EMULATR
---
## 概要
Arduino向けのEEPROM(AT24C08N)をエミュレートするプロジェクト

[ModifiedSample/\*](ModifiedSample/) :サンプルプログラムをとりあえずコンパイルできるようにしたもの

[RomEmulator/\*](RomEmulator/) : EEPROMのエミュレータプログラム

[RomWriter/\*](RomWriter/) : EEPROM一括書き込み用プログラム

[RomTester/\*](RomTester/) : ↑のテスト用プログラム

## [RomWriter/\*](RomWriter/)
Arduino内のEEPROMに、一括でデータを記入するプログラム。RomEmulatorを記入する前にこのプログラムを書き込み、EEPROMの初期値を設定する。

記入するデータは、[RomWriter/data.h](RomWriter/data.h)。内部フォーマットはcsvなので、拡張子を変更すればExcel等での編集が可能。

最後の数値データの後に、カンマなどが入っているとプログラムがエラーを起こす。

- 正常なファイル

```cpp:RomWriter/data.h
0,1,2, ...(省略)... ,13,14,15
```

- エラーを起こす例

```cpp:RomWriter/data.h
0,1,2, ...(省略)... ,13,14,15,
```

プロジェクトに同梱されている[data.h](RomWriter/data.h)はダミーデータ。アドレスの下4ビットをそのまま値として入れている。
Aruduino Uno向けなので、データ数は1024個。

データの個数は過不足があっても良いが、必ず0番目のアドレスから書き込まれて不足分は更新されない。過剰分は無視される。

## [RomEmulator/\*](RomEmulator/)
EEPROM(AT24C08N)をエミュレートするプログラム。

EEPROMとして振る舞うデータは、Arduino内EEPROMを参照する。
24C08Nは1KiBのEEPROMなので、EEPROMも1KiB以上ある前提。

なお、Arduino UnoのEEPROM容量は1KiB。AT24C08NとArduinoのEEPROMのアドレス対応は以下の通り。

|AT24C08Nのページ(I2Cアドレスで指定) | AT24C08Nのメモリアドレス | ArduinoのEEPROMのアドレス |
|:-:|--:|--:|
| 0 (0b1010(A2)00(R/W))| 0(0x00)～255(0xFF) |   0(0x000)～ 255(0x0FF) |
| 1 (0b1010(A2)01(R/W))| 0(0x00)～255(0xFF) | 256(0x100)～ 511(0x1FF) |
| 2 (0b1010(A2)10(R/W))| 0(0x00)～255(0xFF) | 512(0x200)～ 767(0x2FF) |
| 3 (0b1010(A2)11(R/W))| 0(0x00)～255(0xFF) | 768(0x300)～1023(0x3FF) |

SDA,SCLはそれぞれA4,A5ピン。動作時はUSBシリアルで各種デバッグ可能。ボーレート38400。

SoftIICライブラリの関係上、シリアルの送受信に遅延や欠損が発生する場合がある。
特に、PCからのデータ送信は欠損しやすい。

シリアル通信でのデバッグとして実装しているのは、以下の3つ。

- EEPROM全体の読み込み

	シリアルモニタで「R」を送信することで実行。

	Arduino内蔵EEPROMの値をすべて読み込む。

- 1バイトの読み込み

	シリアルモニタで「r」を送信することで実行。

	特定のアドレスのArduino内蔵EEPROMの値を読み込む。

	メモリアドレスはシリアルモニタから指定。「r」入力後に、読み込みたいアドレスを十進数で入力する。

	決定は「e」、キャンセルは「c」。

- 1バイトの書き込み

	シリアルモニタで「w」を送信することで実行。

	特定のアドレスのArduino内蔵EEPROMの値を変更する。

	メモリアドレスと値はシリアルモニタから指定。
	「w」入力後に、変更するアドレス、書き込む値をそれぞれ十進数で入力する。

	決定は「e」、キャンセルは「c」。

[RomEmulator/RomEmulator.ino](RomEmulator/RomEmulator.ino) 26行目の``bool rom_Protected = false;``を``true``に変更すると、I2Cからの値変更を無効にする。


## 参照ライブラリ
[SoftIIC](https://github.com/cirthix/SoftIIC)

## リンク
<!-- - []() -->
- [SoftIIC](https://github.com/cirthix/SoftIIC)
- [AT24C08AN (@DigiKey)](https://www.digikey.jp/product-detail/ja/microchip-technology/AT24C08AN-10SU-2.7/AT24C08AN-10SU2.7-ND/660356)
