#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"
#include "MyUtil.h"

#pragma warning(disable:4244)	//�^�ϊ��x���𖳎�

/**
* ���@��@�ɂ����H����
* 
* 1.���H�S�̂��\������2�����z����A������5�ȏ�̊�Ő������܂��B
* 2.���H�̊O����ʘH�Ƃ��A����ȊO��ǂƂ��܂��B
* 3.x, y�Ƃ��Ɋ�ƂȂ���W(�C��)�����@��J�n���W�Ƃ��܂��B
* 4.�w����W��ʘH�Ƃ��܂��B
* 5.���Ɍ@��i�߂����(1�Z���悪�ʘH����2�Z���悪�ǂ̕���)�������_���Ō��肵�A2�Z����܂ŒʘH�Ƃ��܂��B
* 6.�@��i�߂��Ȃ��Ȃ�܂ŌJ��Ԃ��܂��B
* 7.�@��i�߂����ʎl���̂ǂ��ɂ��i�߂Ȃ��Ȃ����ꍇ�A���łɒʘH�ƂȂ������W(x, y�Ƃ��Ɋ)��
*   �����_���Ɏ擾���A���@�菈�����ċA�I�ɌĂяo���܂��B
* 8.�Ō�ɊO����ǂɖ߂��܂��B
*/

/**
* �_�̂΂��@�ɂ����H����
* 
* ���H�S�̂��\������2�����z����A������5�ȏ�̊�Ő������܂��B
* ���H�̊O����ǂƂ��A����ȊO��ʘH�Ƃ��܂��B
* x, y�Ƃ��ɋ����ƂȂ���W��ǐL�΂��J�n���W(���)�Ƃ��ă��X�g�A�b�v���܂��B
* �ǐL�΂��J�n���W���烉���_���ō��W�����o���A�ʘH�̏ꍇ�̂ݕǐL�΂��������s���܂��B
  ���ׂĂ̌����W���ǂɂȂ�܂ŌJ��Ԃ��܂��B
* �w����W��ǂƂ��܂��B
* ���Ɍ@��i�߂����(�ׂ̃Z�����ʘH�̕�������2�Z���悪���݊g�����̕ǂł͂Ȃ�����)�������_���Ō��肵���܂��B
* �g���������2�Z���悪�ǂ̏ꍇ(�����̕ǂɐڑ����ꂽ�ꍇ)�A�ǂ̊g�����I�����܂��B
* �ʘH�̏ꍇ�A���̃Z�����瑱���Ċg�����܂��B(5. �̏������ċA�I�ɌĂяo���B)
* �l�������ׂČ��݊g�����̕ǂ̏ꍇ�A�g���ł�����W��������܂ŁA���݊g�����̕ǂ��o�b�N���āA
  �ǂ̊g�����ĊJ���܂��B
* ���ׂĂ̌����W����(�g����)�ɂȂ�Ί����ł��B
*/

extern int Input, EdgeInput;      // ���͏��(Input(�������ςȂ�)�AEdgeInput(1��̂݁j 

#define SCREEN_WIDTH     (640)    // ��ʂ̉���
#define SCREEN_HEIGHT    (480)    // ��ʂ̏c��
#define CHIP_SIZE        (32)     // ��̃`�b�v�̃T�C�Y
#define MAP_WIDTH        (31)     // �}�b�v�̉���
#define MAP_HEIGHT       (31)     // �}�b�v�̏c��
#define MAP_SCREEN_WIDTH (20)     // ��ʏ㌩����}�b�v�̉���
#define MAP_SCREEN_HEIGHT (15)     // ��ʏ㌩����}�b�v�̉���

#define G                (0.3F)   // �L�����Ɋ|����d�͉����x
#define JUMP_POWER       (9.0F)   // �L�����̃W�����v��
#define SPEED            (32.0F)   // �L�����̈ړ��X�s�[�h
#define CHAR_SIZE        (30)     // �v���C���[�̃T�C�Y

//�}�b�v��ޒ萔
enum MAPPATTERN {
	map_pattern_None = 0,
	map_pattern_Wall,

	map_pattern_End		//�ԕ�
};

//���ݎg�p���̃}�b�v�f�[�^
char NowMapData[MAP_HEIGHT][MAP_WIDTH];

// �}�b�v�f�[�^
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

//�G�̎d�lTips
//�G�͍��E���甗���Ă���B

//�L�����N�^�[�̍s���p�^�[��
struct CHARACTER {
	int Hp;
	int WorldMapX;
	int WorldMapY;
	int AIPattern;
	int DeadFlag;		//0:�����Ă���@1:�g�p���Ă��Ȃ�
	int EnemyAICount;
};

//�v���C���[�錾
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

//�����t���O
#define ISALIVE 0
#define ISDEAD 1

//==�ϐ���`==============================================================
float PlX, PlY;            // �v���C���[�̍��W(���S���W)

float FeX, FeY;				//�d���̈ʒu
int FeShotCoolTime;			//�e�̔��ˊ��o

int MapGapX, MapGapY;
int MapTipXGap, MapTipYGap;	//�}�b�v�`�b�v�̉��ړ���(TipSize�𒴂�����0��)
int MapTipXScroll, MapTipYScroll;	//�}�b�v�`�b�v�̏����ʒu����̉��ړ���

//�d�����v�J�v�J�������邽�߂̃J�E���g�ϐ�
int SinTime;

//�w�i�ϐ�
int BackImage;
int BackImage_SizeX, BackImage_SizeY;
float BackImage_PosX, BackImage_PosY;

//�X�N���[�����̓�����G�l�~�[�ɓ`����ϐ�
float scrollMoveX, scrollMoveY;

//���݂̃X�e�[�W��
int StageNum;

//���������Ă��邩�t���O
int GetKeyFlag;

//���ԃ|�C���g�t���O
int GetCheckPointFlag;

enum GAMEMODE {
	GAMEMODE_START = 0,	//�Q�[���J�n
	GAMEMODE_PLAYER_TURN,		//�v���C���[�̑���܂�
	GAMEMODE_ENEMY_TURN,
	GAMEMODE_GAMECLEAR,
	GAMEMODE_GAMEOVER
};
int GameMode;

enum LAYER {
	LAYER_PLAYER,
	LAYER_ENEMY,
};


//==�ϐ���`==============================================================

//����������
void StatusInit() {
	// �v���C���[�̍��W(���S���W)�@�v���C���[�͉�ʐ^�񒆂ɌŒ�
	PlX = PlY = 16 + 8 * CHIP_SIZE;  

	FeX = FeY = 0;				//�d���̈ʒu
	FeShotCoolTime = 0;			//�e�̔��ˊ��o
	MapGapX = 0;
	MapTipXGap = 0;
	MapTipXScroll = 0;

	FeShotCoolTime = 0;

	//�v���C���[�X�e�[�^�X�̏�����
	player.Hp = 10;
	player.WorldMapX = 8;
	player.WorldMapY = 8;
	player.DeadFlag = 0;

	//�G�X�e�[�^�X�̏�����
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i].Hp = 10;
		enemy[i].WorldMapX = 3;
		enemy[i].WorldMapY = 3;
		enemy[i].DeadFlag = ISDEAD;
		enemy[i].AIPattern = ENEMY_MOVE_RIGHT;
	}

	//�X�e�[�W��
	StageNum = 1;
	GetKeyFlag = 0;
	GameMode = GAMEMODE_START;

}

// �}�b�v�`�b�v�̒l���擾����֐�
int GetChipParam(int X, int Y)
{
	// �}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
	if (X >= MAP_WIDTH || Y >= MAP_HEIGHT || X < 0 || Y < 0) return 0;

	// �w��̍��W�ɊY������}�b�v�̏���Ԃ�
	return NowMapData[Y][X];
}

// �ړ��悪�ړ��ł��邩�ǂ����`�F�b�N����
int MapMoveCheck(int _x, int _y,int layer) {

	//�ړ��悪�}�b�v��Ȃɂ�������
	if (GetChipParam(_x, _y) == 0) {

		//�v���C���[����̏ꍇ�͓G�Ɣ���s��
		switch (layer) {
		case LAYER_PLAYER:

			//�����Ă��鐔�����J��Ԃ�
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (enemy[i].DeadFlag == ISALIVE) {
					if (_x == enemy[i].WorldMapX && _y == enemy[i].WorldMapY) {
						//���̈ړ���͏d�Ȃ����̂ŁA�ړ��ł��Ȃ��ƕԂ�
						return -1;
					}
				}
			}

			//�I�u�W�F�N�g���������Ă�������Ȃ������̂ŁA�ړ��ł��鎖��Ԃ�
			return 0;

			break;

		case LAYER_ENEMY:

			//�v���C���[�Ƃ̓����蔻����s��
			if (_x == player.WorldMapX && _y == player.WorldMapY) {
				//���̈ړ���͏d�Ȃ����̂ŁA�ړ��ł��Ȃ��ƕԂ�
				return -1;
			}

			//�d�Ȃ��Ă��Ȃ��̂ňړ��ł���ƕԂ�
			return 0;

			break;

		}

	}

	//�}�b�v��0�ł͂Ȃ������̂ňړ��ł��Ȃ��ƕԂ�
	return -1;
}



// �V�[���J�n�O�̏��������s��
BOOL initGame1Scene(void) {
	//�e�ϐ��̏�����
	StatusInit();

	//�N�����Ԃɂ�郉���_���V�[�h�𐶐�
	SRand((unsigned int)time(NULL));

	//�摜�f�[�^�̓ǂݍ���
	BackImage = LoadGraph("res/back.jpg", true);
	//�w�i�摜�̉𑜓x���擾
	GetGraphSize(BackImage, &BackImage_SizeX, &BackImage_SizeY);
	//�w�i�摜����������Ɉړ�
	BackImage_PosX = 0;
	BackImage_PosY = (float)-BackImage_SizeY + 480;

	//�X�e�[�W��
	StageNum = 1;
	GetKeyFlag = 0;
	GameMode = GAMEMODE_START;

	GetCheckPointFlag = 0;

	//�}�b�v�f�[�^�̈ړ�
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j< MAP_WIDTH; j++) {
			NowMapData[i][j] = MapData[i][j];
		}
	}

	//�Ƃ肠�����G�l�~�[����
	for (int i = 0; i < 1; i++) {
		if (enemy[i].DeadFlag == ISDEAD) {
			//��ځ[��I
			enemy[i].Hp = 10;
			enemy[i].WorldMapX = 3;
			enemy[i].WorldMapY = 3;
			enemy[i].DeadFlag = ISALIVE;
			enemy[i].AIPattern = ENEMY_MOVE_RIGHT;
		}
	}
	

	return TRUE;
}
//	�t���[������
void moveGame1Scene() {

	switch (GameMode) {
	case GAMEMODE_START:
		//�����K�w�Ƃ��A�_���W��������\������Ƃ�

		GameMode = GAMEMODE_PLAYER_TURN;

		break;
	case GAMEMODE_PLAYER_TURN:

		//�v���C���[�̓��͍s�����s������G�̍s���ɐ؂�ւ���
		
		// ���E�̈ړ�������@��
		if ((EdgeInput & PAD_INPUT_LEFT) != 0) {
			//�ړ���Ƀu���b�N��������Έړ��ł���
			if (MapMoveCheck(player.WorldMapX - 1, player.WorldMapY,LAYER_PLAYER) == 0) {
				//�v���C���[�͓������A�}�b�v�𓮂���
				MapTipXScroll += 1;
				player.WorldMapX -= 1;

			}
			//����������͂��������̂ŁA�G�̃^�[���ɐ؂�ւ�
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		// ���E�̈ړ�������@�E
		if ((EdgeInput & PAD_INPUT_RIGHT) != 0) {
			//�ړ���Ƀu���b�N��������Έړ��ł���
			if (MapMoveCheck(player.WorldMapX + 1, player.WorldMapY, LAYER_PLAYER) == 0) {
				MapTipXScroll -= 1;
				player.WorldMapX += 1;
			}
			//����������͂��������̂ŁA�G�̃^�[���ɐ؂�ւ�
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		//�㉺�̓��͂��s���@��
		if ((EdgeInput & PAD_INPUT_UP) != 0) {
			//�ړ���Ƀu���b�N��������Έړ��ł���
			if (MapMoveCheck(player.WorldMapX, player.WorldMapY -1, LAYER_PLAYER) == 0) {
				player.WorldMapY -= 1;
				MapTipYScroll += 1;
			}
			//����������͂��������̂ŁA�G�̃^�[���ɐ؂�ւ�
			GameMode = GAMEMODE_ENEMY_TURN;
		}

		if ((EdgeInput & PAD_INPUT_DOWN) != 0) {
			//�ړ���Ƀu���b�N��������Έړ��ł���
			if (MapMoveCheck(player.WorldMapX, player.WorldMapY + 1, LAYER_PLAYER) == 0) {
				player.WorldMapY += 1;
				MapTipYScroll -= 1;
		}
			//����������͂��������̂ŁA�G�̃^�[���ɐ؂�ւ�
			GameMode = GAMEMODE_ENEMY_TURN;
		}


		break;
	case GAMEMODE_ENEMY_TURN:

		//�����Ă��鐔�����J��Ԃ�
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemy[i].DeadFlag == ISALIVE) {
				// �G�l�~�[�̈ړ�����
				switch (enemy[i].AIPattern) {
				case ENEMY_MOVE_LEFT:

					//�ړ���Ƀu���b�N��������Έړ��ł���
					if (MapMoveCheck(enemy[i].WorldMapX - 1, enemy[i].WorldMapY, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapX -= 1;
					}
					else {
						//�ǂɂԂ������̂ŁA�ړ�������ς���
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

					// ���E�̈ړ�������@�E
				case ENEMY_MOVE_RIGHT:
					//�ړ���Ƀu���b�N��������Έړ��ł���
					if (MapMoveCheck(enemy[i].WorldMapX + 1, enemy[i].WorldMapY, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapX += 1;
					}
					else {
						//�ǂɂԂ������̂ŁA�ړ�������ς���
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

				case ENEMY_MOVE_UP:
					//�ړ���Ƀu���b�N��������Έړ��ł���
					if (MapMoveCheck(enemy[i].WorldMapX , enemy[i].WorldMapY -1, LAYER_ENEMY) == 0) {
							enemy[i].WorldMapY -= 1;
					}
					else {
						//�ǂɂԂ������̂ŁA�ړ�������ς���
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}
					break;

				case ENEMY_MOVE_DOWN:
					//�ړ���Ƀu���b�N��������Έړ��ł���
					if (MapMoveCheck(enemy[i].WorldMapX, enemy[i].WorldMapY + 1, LAYER_ENEMY) == 0) {
						enemy[i].WorldMapY += 1;
					}
					else {
						//�ǂɂԂ������̂ŁA�ړ�������ς���
						enemy[i].AIPattern = GetRand(ENEMY_MOVE_DOWN);
					}

					break;
				}
			}
		}

		//�S���̓G�̍s�����I������̂ŁA�v���C���[�̃^�[��
		GameMode = GAMEMODE_PLAYER_TURN;

		break;

	case GAMEMODE_GAMECLEAR:
		//�Q�[���N���A���Ƀ{�^�����������玟�̃X�e�[�W��
		if ((Input & PAD_INPUT_1) != 0) {
			//�X�e�[�^�X�����������܂��B
			StatusInit();

			//�摜�f�[�^�̓ǂݍ���(2�X�e�[�W�ڂ̉摜�ɐؑւ���Ȃ炱���ŁB
			BackImage = LoadGraph("res/back.jpg", true);
			//�w�i�摜�̉𑜓x���擾
			GetGraphSize(BackImage, &BackImage_SizeX, &BackImage_SizeY);
			//�w�i�摜����������Ɉړ�
			BackImage_PosX = 0;
			BackImage_PosY = (float)-BackImage_SizeY + 480;

			//�X�e�[�W���̍X�V
			StageNum = 2;
			GetKeyFlag = 0;
			GameMode = GAMEMODE_START;

			//�}�b�v��؂�ւ��܂�
			for (int i = 0; i < MAP_HEIGHT; i++) {
				for (int j = 0; j < MAP_WIDTH; j++) {
					NowMapData[i][j] = MapData2[i][j];
				}
			}
		}
		break;
	}


}
//	�����_�����O����
void renderGame1Scene(void){

	//�w�i�摜�̕\��
	DrawGraph(BackImage_PosX, BackImage_PosY, BackImage, true);

	// �}�b�v�̕`��
	int i, j;
	unsigned int gColor = 0;

	//�}�b�v�̕\���ʒu��ݒ�
	int FieldRenderBaseX = MapTipXScroll * CHIP_SIZE;
	int FieldRenderBaseY = MapTipYScroll * CHIP_SIZE;

	//�}�b�v��\��
	for (i = 0; i < MAP_HEIGHT; i++){
		for (j = 0; j < MAP_WIDTH; j++){

			if (NowMapData[i][j] == map_pattern_None) continue;

			switch(NowMapData[i][j]){
			case map_pattern_Wall:
				//�ʏ�u���b�N
				gColor = GetColor(255, 255, 255);
				break;
			case 2:
				//�S�[���u���b�N
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


	// �v���C���[�͉�ʂ̒��S������Œ�
	DrawBox(PlX - CHAR_SIZE * 0.5F, PlY - CHAR_SIZE * 0.5F,
		PlX + CHAR_SIZE * 0.5F + 1, PlY + CHAR_SIZE * 0.5F,
		GetColor(255, 0, 0), TRUE);

	// �G�l�~�[�̕`��(�v���C���[���ړ�������Ɉړ�����̂ŕ\�����������������B
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].DeadFlag == ISALIVE) {

			DrawBox(enemy[i].WorldMapX * CHIP_SIZE + FieldRenderBaseX,
				enemy[i].WorldMapY * CHIP_SIZE + FieldRenderBaseY,
				enemy[i].WorldMapX * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseX,
				enemy[i].WorldMapY * CHIP_SIZE + CHIP_SIZE + FieldRenderBaseY,
				GetColor(0, 0, 255), TRUE);

		}
	}

	//���������Ă��鎞�͌���\������
	if (GetKeyFlag == 1) {
		// �c�ɂ����Ɠ����悤�ɂ���B
		if (SinTime++ > 360) SinTime = 0;
		DrawBox((int)FeX, (float)FeY, (int)FeX + 10, (float)FeY + 10, GetColor(255, 255, 0), TRUE);
	}

	//�Q�[���N���A�\��
	if (GameMode == GAMEMODE_GAMECLEAR) {
		DrawString(50, 200 ,"GameClear!!!!",GetColor(255, 255, 255));
		DrawString(50, 250, "Button 1 to Next Stage", GetColor(255, 255, 255));
	}

}

//	�V�[���I�����̌㏈��
void releaseGame1Scene(void){

	DeleteGraph(BackImage);
}



