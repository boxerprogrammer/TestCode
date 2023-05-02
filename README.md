# TestCode
テストコードorコードの断片です。ほとんどがDxLibを使用しています。
なお、 **動作させるには環境変数にDXLIB_DIRを作って、そこにDxLibの本体を入れておかないと動作しないので注意してください。** 
また、作った時の年代でVS2019だったりVS2022だったりするので適度にコンバートしてください。
## atan2test
授業用のatan2のテストコードです
## CSTest
ComputeShaderのテストコードです(@koguchit氏のComputeShaderサンプル(https://github.com/kgussan/simpleGpgpu )をDirectX12仕様にしようとしています)
今のところ、スレッドIDを書き込むだけのサンプルです。
**ComputeShaderに興味がない人はここは読まなくて大丈夫です(非DxLib)**
## edgedataloader
ドット絵ソフトEdgeのアニメーションデータコンバート用テストコード
**未完成です**
## GetClipboardTexts
職場のイベントで使用した抽選機用のソースコードです。
DxLibに対してクリップボードから、コピペをするときの参考にはなるかもしれません。
## gradiusV
所謂、某社のグラディ〇ス5における自機のぐにゃぐにゃレーザーを実装。
**見た目だけなので当たり判定が必要ですが未実装です。**
## HitCircleTest
1年生の数学の授業用のソースコードです。円と円の当たり判定を行います。
**なお、コメントを書いておりますがこのコメント通りにコードを追加しないと当たり判定を行いません(教育用なので)**
そんなに難しくないので大丈夫でしょう。
## HitCircleTest2
HitCircleTestの実装済みのやつです
## HitFanTest
扇形と円の当たり判定です。
サーチライト判定とかに使えるんじゃないでしょうか。

## multiscroll
多重スクロールのテストコード(授業用)
![image](https://user-images.githubusercontent.com/8528629/235661640-5397380e-b284-4473-9cbd-82953117bfb2.png)
なお多重スクロールと言いつつ、手前に流れる川に結構な工夫がしてあります(シェーダコード)
また、実行中にBを押すと画面手前に血の雨が降ります(笑)

## NetWork
C++側でログをTCP/IPで送信し、C#側で受信して表示するサンプル
これは完全に趣味と言うか実験ですね。CEDEC2020のニンテンドーの講演
https://cedec.cesa.or.jp/2020/session/detail/s5e7df23196221.html
に触発されて作りました。
## NjTest
IREMの「最後の忍道」的なゲームをC++＆DxLibで作ったサンプルです。
ゲームまるまる入っていますが **著作権の関係上、素材は抜いてます。** 遊びたい場合はReadMeに必要な素材が書いてるので
itch.ioあたりからダウンロードして、適切なフォルダに配置して遊びましょう。

## nVidiaTutorialTest
nVidiaのDXRチュートリアル(HelloTriangleからレイトレーシングをやる)を参考に、モデル表示まで実装中

## onefinger
OneFingerDeathPunch的なものを作ろうとして放置中のプロジェクト。現在タイトル画面のタイトルをラスタスクロールでぐにゃぐにゃ歪ませるテストだけ行っている。
なお、現状はアセットに「GothicVania Church」を使用しており(https://ansimuz.itch.io/gothicvania-church-pack )からダウンロードして、アセットを適切な所においてほしい。
**もちろん未完成**

## PadState
DxLibを使用したパッド状態の出力サンプル。パッド差して何かを押すとそのコードが見れます。
## RankViewer
職場のイベントで使用したランキング可視化のソースコードです(データがないと意味がないので見ても無駄です)
## RectTest
矩形の当たり判定のテストです。矩形の中に侵入できないような作りになっています。
ゲームを作る際の当たり判定や、地形との当たり判定に使えるかもしれません。
## SlashEffectTest
斬撃エフェクト(兼背景歪ませ)のためのテストコード。
ZとXで斬撃の角度調整。カーソルキーで斬撃を移動できます。スト５のナッシュの技に触発されて作りました。
![image](https://user-images.githubusercontent.com/8528629/235660434-af53633f-9493-4eb4-a2e6-28257ef66c86.png)
## SmoothDamp
GemeProgrammingGems4の1章10項のSmoothDampを自分なりに解釈して、イージングに利用してみました。



