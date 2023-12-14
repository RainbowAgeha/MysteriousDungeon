//ゲームの初期化
#include "GameInit.h"

//ゲーム全体の初期化
int game_init(){
	//ウィンドウの設定
	//ウィンドウモードの設定
	ChangeWindowMode(true);
	//ウィンドウのリサイズ
	//Check:実行中に画面の大きさが変更可能か
	SetWindowSizeChangeEnableFlag(true);
	//ウィンドウタイトル
	SetMainWindowText("ゲームテスト");

	//背景色の設定
	SetBackgroundColor(0, 0, 0);
	//DXライブラリ初期化処理
	//check:-1(例外処理)が来た場合は、セットアップに失敗する
	//パソコン本体の機器の確認をしてもらう(音声デバイスがない等。)
	if (DxLib_Init() == -1)return -1;

	//描画先を一番後ろにする
	SetDrawScreen(DX_SCREEN_BACK);

	//TODO:ゲームの初期化処理もここで行う
	//PlayerInit();
	return 0;       //初期化完了
}