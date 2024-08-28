# TestCode
テストコードorコードの断片です。ほとんどがDxLibを使用しています。
なお、 **動作させるには環境変数にDXLIB_DIRを作って、そこにDxLibの本体を入れておかないと動作しないので注意してください。** 
また、作った時の年代でVS2019だったりVS2022だったりするので適度にコンバートしてください。

## 2DFire
2Dにおける比の表現。ピクセルシェーダと、DxLibの暈し表現を併用している。
チョットこの後は背景を歪ませるのもやりたいと思う。

![image](https://github.com/boxerprogrammer/TestCode/assets/8528629/479060f0-dabe-47b4-a626-ba1a953eefb4)

動画↓
https://twitter.com/CTsuchinoko/status/1657570050596020224


## 2DShadow
なかにRaycastShadowとSpriteShadowというソリューションがあり、それぞれ
### RaycastShadow
これは、光と障害物の当たり判定を想定して、どこまで光が届くのかをシミュレートしています。ステルスゲーとかに使えるかなと思います。

![image](https://user-images.githubusercontent.com/8528629/235663808-eefab72e-b1f5-43d6-aab8-31d332b7a2c8.png)
### Sprite2DShadow
こいつは2Dで陰影をつけるテストコードです。なお、２D画像から法線マップを作るツールとしてはLaigterというのを使っています。
https://azagaya.itch.io/laigter

![image](https://user-images.githubusercontent.com/8528629/235664271-1f6940d3-9613-4571-be4e-663ce981452c.png)

実行するとマウスで点光源を動かすことができます。

## 3DOutline
ノイズのテストだの色々と混ざってますが、選択中のオブジェクトにだけ輪郭を付与するテストコードです
なお、輪郭は「背面法」により生成したものです
![image](https://github.com/user-attachments/assets/f4e9c11f-ac3f-41d0-9a50-701210b27189)



## AnimationFrame
特定の枠線をくるくると光らせるものです。選択中の選択肢などに使用するといいと思います
DxLibにおけるDrawMaskのテストです
![image](https://github.com/user-attachments/assets/20bba275-db00-4f4d-99a7-f9eb1a67d7b9)


## atan2test
授業用のatan2のテストコードです。現在マウスがいる位置を、デカいカーソルが指し示します
![image](https://github.com/user-attachments/assets/34484cd4-eef4-4c1b-bdce-e64efef65a9d)


## blurbloom
DxLibにおいてシェーダを用いないで、ブルーム(グレア…光ってる)を表現するサンプルです。

![image](https://user-images.githubusercontent.com/8528629/235820941-86a15d32-90c8-4b63-a7b5-4b8a23778925.png)

## BoxingIK
グローブの位置と向き(手首の向き)で肘の位置を調整するサンプルです。自分の研究用なのでソースコードが見づらいかも。
EキーとRキーでグローブの向きが変わります
![image](https://github.com/user-attachments/assets/c3a4a046-0a51-4d78-bb93-3c84e509549d)


## CaptureMesh
画面のスクショをメッシュ化して割るっていうやつですが、なんか先祖返りしてるのでそのうち修正します。
![image](https://github.com/user-attachments/assets/5aebfc1a-36ef-436b-b1a2-78a2a49c5f54)


## CSTest
ComputeShaderのテストコードです(@koguchit氏のComputeShaderサンプル(https://github.com/kgussan/simpleGpgpu )をDirectX12仕様にしようとしています)
今のところ、スレッドIDを書き込むだけのサンプルです。
**ComputeShaderに興味がない人はここは読まなくて大丈夫です(非DxLib)**

## Dissolve
オブジェクトがモヤモヤっ、と消えるエフェクトです
### 2DDissolve

![image](https://user-images.githubusercontent.com/8528629/235821924-fe73af35-8c0b-4173-9a6e-5734e1f9a485.png)

### 3DDissolve

![image](https://user-images.githubusercontent.com/8528629/235821986-94fc58f9-8c30-449e-b8e7-a3258ad3c7de.png)


## edgedataloader
ドット絵ソフトEdgeのアニメーションデータコンバート用テストコード
**未完成です**

## FScanF
テキストからfscanfでデータを読み取って、それを3Dの向きに反映させるためだけのプログラム

## GetClipboardTexts
職場のイベントで使用した抽選機用のソースコードです。
DxLibに対してクリップボードから、コピペをするときの参考にはなるかもしれません。
## gradiusV
所謂、某社のグラディ〇ス5における自機のぐにゃぐにゃレーザーを実装。
**見た目だけなので当たり判定が必要ですが未実装です。**

![image](https://user-images.githubusercontent.com/8528629/235819785-a254cc47-543e-4ce7-b0fe-cbb2f386fb8b.png)

キーボードのカーソルキーで移動、ZとXでレーザーを回転します

## Hight2Normal
HightはHeightのスペルミスです。高さ情報を法線情報に変換します。左上にプロシージャルな波のモノクロ画像を生成し、それを法線情報に変換して右上に出力しています。
真ん中の下はその法線情報を用いて画像を歪ませたものです。

![image](https://github.com/boxerprogrammer/TestCode/assets/8528629/5a5c119f-3856-4962-90f4-93547527d85f)


## HitCircleTest
1年生の数学の授業用のソースコードです。円と円の当たり判定を行います。
**なお、コメントを書いておりますがこのコメント通りにコードを追加しないと当たり判定を行いません(教育用なので)**
そんなに難しくないので大丈夫でしょう。
## HitCircleTest2
HitCircleTestの実装済みのやつです
## HitFanTest
扇形と円の当たり判定です。
サーチライト判定とかに使えるんじゃないでしょうか。
![image](https://github.com/user-attachments/assets/285afe61-fabd-472f-92a1-88786850d3a6)


## Index2Index
何のために作ったコードか不明 **あとで消す**

## LoadModel
ただモデルをロードするだけじゃなくて、シェーダでPBRっぽくしている
![image](https://github.com/user-attachments/assets/af45c199-0f0a-410a-9e36-2b16281f5c57)


## masktest
DxLibのマスクを使って某ゲームの「ブチッ！」を再現しようとしました。オサレ。

![image](https://user-images.githubusercontent.com/8528629/235958826-94246765-6548-43ba-9417-58a3e34407c3.png)


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

![image](https://user-images.githubusercontent.com/8528629/235820678-c6f31ed4-d45d-462a-a407-5ce9db48dacb.png)


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

##PeterShirley
Raytracing One Weekendやろうとしてるやつ



## RankViewer
職場のイベントで使用したランキング可視化のソースコードです(データがないと意味がないので見ても無駄です)

## RaycastBoxTest
画面上の点から奥行き方向に向かう線と**回転する直方体**の交差判定。DxLibの関数を使わず数学的に判定を行っている。
![image](https://github.com/user-attachments/assets/1c681792-1a88-4c3f-bf11-efdbb0ab6bdf)
https://x.com/CTsuchinoko/status/1828764083027333592

## Raymarching
3DのSDFを用いたレイマーチングのサンプル

![image](https://github.com/boxerprogrammer/TestCode/assets/8528629/8e9d49b6-84d2-4077-a983-0ca2056a7b31)

## ReadLaigter
## RectTest
矩形の当たり判定のテストです。矩形の中に侵入できないような作りになっています。
ゲームを作る際の当たり判定や、地形との当たり判定に使えるかもしれません。
## SlashEffectTest
斬撃エフェクト(兼背景歪ませ)のためのテストコード。
ZとXで斬撃の角度調整。カーソルキーで斬撃を移動できます。スト５のナッシュの技に触発されて作りました。

![image](https://user-images.githubusercontent.com/8528629/235660434-af53633f-9493-4eb4-a2e6-28257ef66c86.png)

## SmoothDamp
GemeProgrammingGems4の1章10項のSmoothDampを自分なりに解釈して、イージングに利用してみました。

## SpriteOutline
なかにSpriteOutlineとSpriteAuraというのが入っています。
### SpriteOutline
これは、普通の画像に対して輪郭線を付けるものです。大したことをしてないように見えますがちょっとテクが必要です。

![image](https://user-images.githubusercontent.com/8528629/235664735-bdcbc0bc-f6ce-4ea7-b85b-b0a41a9979ee.png)

### SpriteAura
SpriteOutlineを応用し、時間経過でアニメーションさせて色んな効果を出すことができます。

![image](https://user-images.githubusercontent.com/8528629/235665293-a09d6c86-9c1b-4b15-b588-6f4aa4541625.png)





