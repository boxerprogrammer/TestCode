各フォルダ、ファイルの説明
・NinjaSprit.sln : 忍者ゲームプロジェクトファイル
┣NinjaSpritフォルダ
	・Geometry.h,cpp：幾何学に関するもの(ベクトルなど)
	・main.cpp : エントリポイントがある
	・icon.ico : アプリ用アイコン
	┣Gameフォルダ：ゲーム本編に関するプログラム
		・Character：キャラクタ(ゲーム中動くもの)基底クラス
		・Camera：カメラ(スクロール制御)基底クラス
		・Stage：ステージデータ制御クラス
		・Effect：エフェクト発生クラス
		┣Collisionフォルダ：当たり判定に関するプログラム
		┣Enemyフォルダ：敵に関するプログラム
		┣Playerフォルダ：自機操作に関するプログラム
	┣Inputフォルダ：入力周りに関するプログラム
	┣Sceneフォルダ：シーン遷移周りに関するプログラム
	┣Systemフォルダ：ApplicationやDebuggerなどゲーム本編を支えるプログラムがある
	┣Resourceフォルダ：プログラムではなく、画像やサウンドがある
