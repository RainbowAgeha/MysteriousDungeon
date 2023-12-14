#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"		//マクロの使用
#include "Game2Scene.h"

#pragma warning(disable : 4996)

//マップエディターツールを作る
//・マップデータ　配列 int型
//・ファイルの入出力

//マップの最大値
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

//マップデータ
int MapData[MAP_WIDTH][MAP_HEIGHT];

//画像の宣言
int img_pallet[5];

//クラス内関数を作成（他から呼ばない）
void MapSave(char *_FileName);
void MapLoad(char *_FileName);



// シーン開始前の初期化を行う
BOOL initGame2Scene(void){
	//画像の読み込み
	LoadDivGraph("res/pallet.jpg", 64, 5, 1, 64, 64, img_pallet);

	//マップデータの初期化
	//TODO：forを使うパターンと、ZeroMemoryを使うパターン
	//1.forを使うやりかた。
	for (int i = 0; i < MAP_WIDTH; i++){
		for (int j = 0; j < MAP_HEIGHT; j++){
			MapData[i][j] = 0;
		}
	}
	//2.ZeroMemoryを使う方法
	//ZeroMemory(MapData, (int)Max_MapX*Max_MapY);

	//ファイルを書きこむ(データがない場合)
	//MapSave("res/mapdata.dat");
	//ファイルを読み込む
	MapLoad("res/mapdata.dat");

	return TRUE;
}
//	フレーム処理
void moveGame2Scene(){
	int mouseX;
	int mouseY;

	//クリックした場所を取得する
	GetMousePoint(&mouseX, &mouseY);
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
		int mapX = mouseX / 64;
		int mapY = mouseY / 64;
		//クリックした場所が配列外を超えていないか
		if ((mapX < 0 || mapX < MAP_WIDTH) && (mapY < 0 || mapY < MAP_HEIGHT)){
			MapData[mapX][mapY] = (MapData[mapX][mapY] + 1) % 4;
		}
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0){
		MapSave("res/mapdata.dat");
	}

}
//	レンダリング処理
void renderGame2Scene(void){
	//マップの表示
	for (int i = 0; i < MAP_WIDTH; i++){
		for (int j = 0; j < MAP_HEIGHT; j++){
			//マスの管理に対して、表示画像を変更
			switch (MapData[i][j])
			{
			case 0:		//白
				DrawGraph(i * 64, j * 64, img_pallet[0],true);
				break;
			case 1:		//赤
				DrawGraph(i * 64, j * 64, img_pallet[1], true);
				break;
			case 2:		//青
				DrawGraph(i * 64, j * 64, img_pallet[2], true);
				break;
			case 3:		//緑
				DrawGraph(i * 64, j * 64, img_pallet[3], true);
				break;
			default:
				break;
			}
		}
	}


	//DrawString(30, 50, "マップエディター", GetColor(255, 255, 255));

}

//	シーン終了時の後処理
void releaseGame2Scene(void){
	
}

// 当り判定コールバック 　　　ここでは要素を削除しないこと！！
void  Game2SceneCollideCallback( int nSrc, int nTarget, int nCollideID ){


}

void MapSave(char *_FileName){
	//ファイルを開く（書き込み形式）
	FILE *fp = fopen(_FileName, "w");

	//ファイルにMaoDataを書き込む
	fwrite(MapData, sizeof(int), MAP_WIDTH*MAP_HEIGHT, fp);

	//ファイルを閉じる
	fclose(fp);
}

void MapLoad(char *_FileName){
	//ファイルを開く（書き込み形式）
	FILE *fp = fopen(_FileName, "r");

	//ファイルにMaoDataを書き込む
	fread(MapData, sizeof(int), MAP_WIDTH*MAP_HEIGHT, fp);

	//ファイルを閉じる
	fclose(fp);

}
