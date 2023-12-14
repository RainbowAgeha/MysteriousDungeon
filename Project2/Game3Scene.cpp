#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game3Scene.h"



// シーン開始前の初期化を行う
BOOL initGame3Scene(void){


	return TRUE;
}

//	フレーム処理
void moveGame3Scene(){


}

//	レンダリング処理
void renderGame3Scene(void){

	DrawString(30, 50, "ゲーム画面３です", GetColor(255, 255, 255));

}

//	シーン終了時の後処理
void releaseGame3Scene(void){


}

// 当り判定コールバック 　　　ここでは要素を削除しないこと！！
void  Game3SceneCollideCallback( int nSrc, int nTarget, int nCollideID ){



}
