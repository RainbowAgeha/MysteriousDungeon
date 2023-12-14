#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"
#include "MyUtil.h"

#pragma warning(disable:4244)	//型変換警告を無視

/**
* 穴掘り法による迷路生成
* 
* 1.迷路全体を構成する2次元配列を、幅高さ5以上の奇数で生成します。
* 2.迷路の外周を通路とし、それ以外を壁とします。
* 3.x, yともに奇数となる座標(任意)を穴掘り開始座標とします。
* 4.指定座標を通路とします。
* 5.次に掘り進める方向(1セル先が通路かつ2セル先が壁の方向)をランダムで決定し、2セル先まで通路とします。
* 6.掘り進められなくなるまで繰り返します。
* 7.掘り進めた結果四方のどこにも進めなくなった場合、すでに通路となった座標(x, yともに奇数)を
*   ランダムに取得し、穴掘り処理を再帰的に呼び出します。
* 8.最後に外周を壁に戻します。
*/

/**
* 棒のばし法による迷路生成
* 
* 迷路全体を構成する2次元配列を、幅高さ5以上の奇数で生成します。
* 迷路の外周を壁とし、それ以外を通路とします。
* x, yともに偶数となる座標を壁伸ばし開始座標(候補)としてリストアップします。
* 壁伸ばし開始座標からランダムで座標を取り出し、通路の場合のみ壁伸ばし処理を行います。
  すべての候補座標が壁になるまで繰り返します。
* 指定座標を壁とします。
* 次に掘り進める方向(隣のセルが通路の方向かつ2セル先が現在拡張中の壁ではない方向)をランダムで決定しします。
* 拡張する方向2セル先が壁の場合(既存の壁に接続された場合)、壁の拡張を終了します。
* 通路の場合、そのセルから続けて拡張します。(5. の処理を再帰的に呼び出す。)
* 四方がすべて現在拡張中の壁の場合、拡張できる座標が見つかるまで、現在拡張中の壁をバックして、
  壁の拡張を再開します。
* すべての候補座標が壁(拡張済)になれば完了です。
*/

extern int Input, EdgeInput;      // 入力状態(Input(押しっぱなし)、EdgeInput(1回のみ） 

#define SCREEN_WIDTH     (640)    // 画面の横幅
#define SCREEN_HEIGHT    (480)    // 画面の縦幅
#define CHIP_SIZE        (32)     // 一つのチップのサイズ
#define MAP_WIDTH        (31)     // マップの横幅
#define MAP_HEIGHT       (31)     // マップの縦幅
#define MAP_SCREEN_WIDTH (20)     // 画面上見えるマップの横幅
#define MAP_SCREEN_HEIGHT (15)     // 画面上見えるマップの横幅

#define G                (0.3F)   // キャラに掛かる重力加速度
#define JUMP_POWER       (9.0F)   // キャラのジャンプ力
#define SPEED            (32.0F)   // キャラの移動スピード
#define CHAR_SIZE        (30)     // プレイヤーのサイズ

//マップ種類定数
enum MAPPATTERN {
	map_pattern_None = 0,
	map_pattern_Wall,

	map_pattern_End		//番兵
};

//現在使用中のマップデータ
char NowMapData[MAP_HEIGHT][MAP_WIDTH];

// マップデータ
char MapData[MAP_HEIGHT][MAP_WIDTH] =
{
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,
	1,0,0,0,1, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,1, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,1, 0,0,1,1,1, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,1, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,

	1,0,0,0,1, 0,0,0,0,0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,1,0,0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,1,0,0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,1,0,0, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,1,1,1,1, 1,0,0,0,1, 1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,	//10

	1,0,0,1,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,

	1,0,0,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,	//20

	1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,

	1,0,1,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,

	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,
};

char MapData2[MAP_HEIGHT][MAP_WIDTH] =
{
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,1,

	1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,0, 0,0,1,1,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 0,0,1,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0, 1,0,0,0,1,	//10

	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,

	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,	//20

	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,

	1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,0, 0,0,0,0,1, 1,0,0,0,1,
	1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
	1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,1,
	1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,	//30
};

//敵の仕様Tips
//敵は左右から迫ってくる。

//キャラクターの行動パターン
struct CHARACTER {
	int Hp;
	int WorldMapX;
	int WorldMapY;
	int AIPattern;
	int DeadFlag;		//0:生きている　1:使用していない
	int EnemyAICount;
};

//プレイヤー宣言
#define ENEMY_MAX 5
CHARACTER player;
CHARACTER enemy[ENEMY_MAX];

enum EnemyPattern{
	ENEMY_MOVE_LEFT = 0,
	ENEMY_MOVE_RIGHT,
	ENEMY_MOVE_UP,
	ENEMY_MOVE_DOWN,
	ENEMY_ATTACK
};

//生死フラグ
#define ISALIVE 0
#define ISDEAD 1

//==変数定義==============================================================
float PlX, PlY;            // プレイヤーの座標(中心座標)

float FeX, FeY;				//妖精の位置
int FeShotCoolTime;			//弾の発射感覚

int MapGapX, MapGapY;
int MapTipXGap, MapTipYGap;	//マップチップの横移動量(TipSizeを超えたら0に)
int MapTipXScroll, MapTipYScroll;	//マップチップの初期位置からの横移動量

//妖精をプカプカ浮かせるためのカウント変数
int SinTime;

//背景変数
int BackImage;
int BackImage_SizeX, BackImage_SizeY;
float BackImage_PosX, BackImage_PosY;

//スクロール中の動作をエネミーに伝える変数
float scrollMoveX, scrollMoveY;

//現在のステージ数
int StageNum;

//鍵を持っているかフラグ
int GetKeyFlag;

//中間ポイントフラグ
int GetCheckPointFlag;

enum GAMEMODE {
	GAMEMODE_START = 0,	//ゲーム開始
	GAMEMODE_PLAYER_TURN,		//プレイヤーの操作まち
	GAMEMODE_ENEMY_TURN,
	GAMEMODE_GAMECLEAR,
	GAMEMODE_GAMEOVER
};
int GameMode;

enum LAYER {
	LAYER_PLAYER,
	LAYER_ENEMY,
};


//==変数定義==============================================================

//初期化処理
void StatusInit() {
	// プレイヤーの座標(中心座標)　プレイヤーは画面真ん中に固定
	PlX = PlY = 16 + 8 * CHIP_SIZE;  

	FeX = FeY = 0;				//妖精の位置
	FeShotCoolTime = 0;			//弾の発射感覚
	MapGapX = 0;
	MapTipXGap = 0;
	MapTipXScroll = 0;

	FeShotCoolTime = 0;

	//プレイヤーステータスの初期化
	player.Hp = 10;
	player.WorldMapX = 8;
	player.WorldMapY = 8;
	player.DeadFlag = 0;

	//敵ステータスの初期化
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i].Hp = 10;
		enemy[i].WorldMapX = 3;
		enemy[i].WorldMapY = 3;
		enemy[i].DeadFlag = ISDEAD;
		enemy[i].AIPattern = ENEMY_MOVE_RIGHT;
	}

	//ステージ数
	StageNum = 1;
	GetKeyFlag = 0;
	GameMode = GAMEMODE_START;

}

// マップチップの値を取得する関数
int GetChipParam(int X, int Y)
{
	// マップからはみ出ていたら 0 を返す
	if (X >= MAP_WIDTH || Y >= MAP_HEIGHT || X < 0 || Y < 0) return 0;

	// 指定の座標に該当するマップの情報を返す
	return NowMapData[Y][X];
}

// 移動先が移動できるかどうかチェックする
int MapMoveCheck(int _x, int _y,int layer) {

	//移動先がマップ上なにも無いか
	if (GetChipParam(_x, _y) == 0) {

		//プレイヤーからの場合は敵と判定行う
		switch (layer) {
		case LAYER_PLAYER:

			//生きている数だけ繰り返す
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (enemy[i].DeadFlag == ISALIVE) {
					if (_x == enemy[i].WorldMapX && _y == enemy[i].WorldMapY) {
						//その移動先は重なったので、移動できないと返す
						return -1;
					}
				}
			}

			//オブジェクトを検索しても当たらなかったので、移動できる事を返す
			return 0;

			break;

		case LAYER_ENEMY:

			//プレイヤーとの当たり判定を行う
			if (_x == player.WorldMapX && _y == player.WorldMapY) {
				//その移動先は重なったので、移動できないと返す
				return -1;
			}

			//重なっていないので移動できると返す
			return 0;

			break;

		}

	}

	//マップが0ではなかったので移動できないと返す
	return -1;
}



// シーン開始前の初期化を行う
BOOL initGame1Scene(void) {
	//各変数の初期化
	StatusInit();

	//起動時間によるランダムシードを生成
	SRand((unsigned int)time(NULL));

	//画像データの読み込み
	BackImage = LoadGraph("res/back.jpg", true);
	//背景画像の解像度を取得
	GetGraphSize(BackImage, &BackImage_SizeX, &BackImage_SizeY);
	//背景画像を左下を基準に移動
	BackImage_PosX = 0;
	BackImage_PosY = (float)-BackImage_SizeY + 480;

	//ステージ数
	StageNum = 1;
	GetKeyFlag = 0;
	GameMode = GAMEMODE_START;

	GetCheckPointFlag = 0;

	//マップデータの移動
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j< MAP_WIDTH; j++) {
			NowMapData[i][j] = MapData[i][j];
		}
	}

	//とりあえずエネミー生成
	for (int i = 0; i < 1; i++) {
		if (enemy[i].DeadFlag == ISDEAD) {
			//りぼーん！
			enemy[i].Hp = 10;
			enemy[i].WorldMapX = 3;
			enemy[i].WorldMapY = 3;
			enemy[i].DeadFlag = ISALIVE;
			enemy[i].AIPattern = ENEMY_MOVE_RIGHT;
		}
	}
	

	return TRUE;
}
//	フレーム処理
void moveGame1Scene() {

	switch (GameMode) {
	case GAMEMODE_START:
		//◯◯階層とか、ダンジョン名を表示するとか

		GameMode = GAMEMODE_PLAYER_TURN;

		break;
	case GAMEMODE_PLAYER_TURN:

		//プレイヤーの入力行動を行ったら敵の行動に切り替える
		
		// 左右の移動を見る　左
		if ((EdgeInput & PAD_INPUT_LEFT) != 0) {
			//移動先にブロックが無ければ移動できる
			if (MapMoveCheck(player.WorldMapX - 1, player.WorldMapY,LAYER_PLAYER) == 0) {
				//プレイヤーは動かず、マップを動かす
				MapTipXScroll += 1;
				player.WorldMapX -= 1;

			}
			//何かしら入力があったので、敵のターンに切り替え
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		// 左右の移動を見る　右
		if ((EdgeInput & PAD_INPUT_RIGHT) != 0) {
			//移動先にブロックが無ければ移動できる
			if (MapMoveCheck(player.WorldMapX + 1, player.WorldMapY, LAYER_PLAYER) == 0) {
				MapTipXScroll -= 1;
				player.WorldMapX += 1;
			}
			//何かしら入力があったので、敵のターンに切り替え
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		//上下の入力を行う　上
		if ((EdgeInput & PAD_INPUT_UP) != 0) {
			//移動先にブロックが無ければ移動できる
			if (MapMoveCheck(player.WorldMapX, player.WorldMapY -1, LAYER_PLAYER) == 0) {
				player.WorldMapY -= 1;
				MapTipYScroll += 1;
			}
			//何かしら入力があったので、敵のターンに切り替え
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		if ((EdgeInput & PAD_INPUT_DOWN) != 0) {
			//移動先にブロックが無ければ移動できる
			if (MapMoveCheck(player.WorldMapX, player.WorldMapY + 1, LAYER_PLAYER) == 0) {
				player.WorldMapY += 1;
				MapTipYScroll -= 1;
		}
			//何かしら入力があったので、敵のターンに切り替え
			GameMode = GAMEMODE_ENEMY_TURN;
		}


		break;
	case GAMEMODE_ENEMY_TURN:

		//生きている数だけ繰り返す
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemy[i].DeadFlag == ISALIVE) {
				// エネミーの移動処理
				switch (enemy[i].AIPattern) {
				case ENEMY_MOVE_LEFT:

					//移動先にブロックが無ければ移動できる
					if (MapMoveCheck(enemy[i].WorldMapX - 1, enemy[i].WorldMapY, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapX -= 1;
					}
					else {
						//壁にぶつかったので、移動方向を変える
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

					// 左右の移動を見る　右
				case ENEMY_MOVE_RIGHT:
					//移動先にブロックが無ければ移動できる
					if (MapMoveCheck(enemy[i].WorldMapX + 1, enemy[i].WorldMapY, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapX += 1;
					}
					else {
						//壁にぶつかったので、移動方向を変える
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

				case ENEMY_MOVE_UP:
					//移動先にブロックが無ければ移動できる
					if (MapMoveCheck(enemy[i].WorldMapX , enemy[i].WorldMapY -1, LAYER_ENEMY) == 0) {
							enemy[i].WorldMapY -= 1;
					}
					else {
						//壁にぶつかったので、移動方向を変える
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

				case ENEMY_MOVE_DOWN:
					//移動先にブロックが無ければ移動できる
					if (MapMoveCheck(enemy[i].WorldMapX, enemy[i].WorldMapY + 1, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapY += 1;
					}
					else {
						//壁にぶつかったので、移動方向を変える
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}

					break;
				}
			}
		}

		//全部の敵の行動が終わったので、プレイヤーのターン
		GameMode = GAMEMODE_PLAYER_TURN;

		break;

	case GAMEMODE_GAMECLEAR:
		//ゲームクリア中にボタンを押したら次のステージへ
		if ((Input & PAD_INPUT_1) != 0) {
			//ステータスを初期化します。
			StatusInit();

			//画像データの読み込み(2ステージ目の画像に切替えるならここで。
			BackImage = LoadGraph("res/back.jpg", true);
			//背景画像の解像度を取得
			GetGraphSize(BackImage, &BackImage_SizeX, &BackImage_SizeY);
			//背景画像を左下を基準に移動
			BackImage_PosX = 0;
			BackImage_PosY = (float)-BackImage_SizeY + 480;

			//ステージ数の更新
			StageNum = 2;
			GetKeyFlag = 0;
			GameMode = GAMEMODE_START;

			//マップを切り替えます
			for (int i = 0; i < MAP_HEIGHT; i++) {
				for (int j = 0; j < MAP_WIDTH; j++) {
					NowMapData[i][j] = MapData2[i][j];
				}
			}
		}
		break;
	}


}
//	レンダリング処理
void renderGame1Scene(void){

	//背景画像の表示
	DrawGraph(BackImage_PosX, BackImage_PosY, BackImage, true);

	// マップの描画
	int i, j;
	unsigned int gColor = 0;

	//マップの表示位置を設定
	int FieldRenderBaseX = MapTipXScroll * CHIP_SIZE;
	int FieldRenderBaseY = MapTipYScroll * CHIP_SIZE;

	//マップを表示
	for (i = 0; i < MAP_HEIGHT; i++){
		for (j = 0; j < MAP_WIDTH; j++){

			if (NowMapData[i][j] == map_pattern_None) continue;

			switch(NowMapData[i][j]){
			case map_pattern_Wall:
				//通常ブロック
				gColor = GetColor(255, 255, 255);
				break;
			case 2:
				//ゴールブロック
				gColor = GetColor(255, 0, 0);
				break;
			case 3:
				gColor = GetColor(255, 0, 0);
				break;
			case 4:
				gColor = GetColor(255, 0, 0);
				break;
			}
			DrawBox(j * CHIP_SIZE + FieldRenderBaseX,
				i * CHIP_SIZE + FieldRenderBaseY,
				j * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseX,
				i * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseY,
				gColor, TRUE);
		}
	}


	// プレイヤーは画面の中心あたり固定
	DrawBox(PlX - CHAR_SIZE * 0.5F, PlY - CHAR_SIZE * 0.5F,
		PlX + CHAR_SIZE * 0.5F + 1, PlY + CHAR_SIZE * 0.5F,
		GetColor(255, 0, 0), TRUE);

	// エネミーの描画(プレイヤーが移動した後に移動するので表示がおかしいかも。
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].DeadFlag == ISALIVE) {

			DrawBox(enemy[i].WorldMapX * CHIP_SIZE + FieldRenderBaseX,
				enemy[i].WorldMapY * CHIP_SIZE + FieldRenderBaseY,
				enemy[i].WorldMapX * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseX,
				enemy[i].WorldMapY * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseY,
				GetColor(0, 0, 255), TRUE);

		}
	}

	//鍵を持っている時は鍵を表示する
	if (GetKeyFlag == 1) {
		// 縦にゆらゆらと動くようにする。
		if (SinTime++ > 360) SinTime = 0;
		DrawBox((int)FeX, (float)FeY, (int)FeX + 10, (float)FeY + 10, GetColor(255, 255, 0), TRUE);
	}

	//ゲームクリア表示
	if (GameMode == GAMEMODE_GAMECLEAR) {
		DrawString(50, 200 ,"GameClear!!!!",GetColor(255, 255, 255));
		DrawString(50, 250, "Button 1 to Next Stage", GetColor(255, 255, 255));
	}

}

//	シーン終了時の後処理
void releaseGame1Scene(void){

	DeleteGraph(BackImage);
}



