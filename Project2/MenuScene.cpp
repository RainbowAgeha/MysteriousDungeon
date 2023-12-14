#include "GameMain.h"
#include "GameSceneMain.h"
#include "MenuScene.h"

//メニュー項目のシーン番号の配列
#define MENU_MAX 3
SCENE_NO menu[ MENU_MAX ] = { SCENE_GAME1, SCENE_GAME2, SCENE_GAME3};
char *menuList[3] = {"ゲームスタート","オプション","終了"};
//選択されたゲームを表すメニュー番号の初期化（menuの添え字）
static int selectedGame = 0;

int startfont;

//文字の点滅用タイマー
int FontTime;


int Key[256]; // キーが押されているフレーム数を格納する

			  // キーの入力状態を更新する
int gpUpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}


//シーン開始前の初期化を行う
BOOL initMenuScene(void){

	SetFontSize(32);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	selectedGame = 0;

	FontTime = 0;

	return TRUE;
}

//フレーム処理
void moveMenuScene(){

	if (gpUpdateKey() == 0) {

		//７メニュー項目の選択
		//７(1) ①新たに↑が押されたら、
		if (Key[KEY_INPUT_UP] == 1) {
			//７(1) ②１つ上のメニュー項目が選択されたとする。
			//      　ただし、それより上のメニュー項目がないときは、最下段のメニュー項目が選択されたとする
			if (--selectedGame < 0) {
				selectedGame = MENU_MAX - 1;
			}
		}


		//７(2) ①新たに↓が押されたら、
		if (Key[KEY_INPUT_DOWN] == 1) {
			//７(2) ②１つ下のメニュー項目が選択されたとする。。
			//      　ただし、それより下のメニュー項目がないときは、最上段のメニュー項目が選択されたとする
			if (++selectedGame >= MENU_MAX) {
				selectedGame = 0;
			}
		}

		//７(3) 新たにボタン１が押されたら選択されているシーンへ
		if (Key[KEY_INPUT_Z] == 1) {
			changeScene(menu[selectedGame]);
		}


		//強制的にシーンに飛ばす
		//changeScene(menu[1]);
	}


}

//レンダリング処理
void renderMenuScene(void){

	DrawString(30, 50, "メニュー画面", GetColor(255, 255, 255));
	DrawString(30, 400, "キー↑↓で選択。ボタン１で決定", GetColor(255, 255, 255));

	//--------------------------------------------
	////文字の点滅演出 パターンA　パタパタと点滅させる。
	//FontTime++;
	////文字を表示する時間を設定
	//if (FontTime < 60) {
	//	DrawString(30, 350, "ボタン１でスタート", GetColor(255, 255, 255));
	//}

	////消す時間は少し短くする
	//if (FontTime > 90) {
	//	FontTime = 0;
	//}
	//--------------------------------------------


	//--------------------------------------------
	//文字の点滅演出 パターンB　文字のフェードイン・アウト
	FontTime++;
	//文字を表示する時間を設定
	if (FontTime < 60) {
		//だんだんと消す
		// 描画モードをアルファブレンドモードにして透明度を減少させる
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (FontTime*4));
		DrawString(30, 350, "ボタン１でスタート", GetColor(255, 255, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}

	//だんだんと表示する
	if (FontTime > 60) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FontTime-60) * 4);
		DrawString(30, 350, "ボタン１でスタート", GetColor(255, 255, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	if (FontTime > 120) {
		FontTime = 0;
	}
	//--------------------------------------------

	//６(2) メニュー項目の表示
	int x = 220, y = 140, gapY = 80;	//（x,y)：表示開始座標　gapY：行の高さ
	for( int i = 0; i < MENU_MAX; i++, y += gapY ){
		
		//６(2) ①選択された項目の表示
		if( i == selectedGame ){
			DrawString(x, y, menuList[i], GetColor(255, 0, 0));
		//６(2) ②選択されていない項目の表示
		}else{
			DrawString(x, y, menuList[i], GetColor(255, 255, 255));
		}
		
	}
}

//シーン終了時の後処理
void releaseMenuScene(void){


}