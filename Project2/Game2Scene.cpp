#include "GameMain.h"
#include "GameSceneMain.h"
#include "Game1Scene.h"		//�}�N���̎g�p
#include "Game2Scene.h"

#pragma warning(disable : 4996)

//�}�b�v�G�f�B�^�[�c�[�������
//�E�}�b�v�f�[�^�@�z�� int�^
//�E�t�@�C���̓��o��

//�}�b�v�̍ő�l
#define MAP_WIDTH 20
#define MAP_HEIGHT 20

//�}�b�v�f�[�^
int MapData[MAP_WIDTH][MAP_HEIGHT];

//�摜�̐錾
int img_pallet[5];

//�N���X���֐����쐬�i������Ă΂Ȃ��j
void MapSave(char *_FileName);
void MapLoad(char *_FileName);



// �V�[���J�n�O�̏��������s��
BOOL initGame2Scene(void){
	//�摜�̓ǂݍ���
	LoadDivGraph("res/pallet.jpg", 64, 5, 1, 64, 64, img_pallet);

	//�}�b�v�f�[�^�̏�����
	//TODO�Ffor���g���p�^�[���ƁAZeroMemory���g���p�^�[��
	//1.for���g����肩���B
	for (int i = 0; i < MAP_WIDTH; i++){
		for (int j = 0; j < MAP_HEIGHT; j++){
			MapData[i][j] = 0;
		}
	}
	//2.ZeroMemory���g�����@
	//ZeroMemory(MapData, (int)Max_MapX*Max_MapY);

	//�t�@�C������������(�f�[�^���Ȃ��ꍇ)
	//MapSave("res/mapdata.dat");
	//�t�@�C����ǂݍ���
	MapLoad("res/mapdata.dat");

	return TRUE;
}
//	�t���[������
void moveGame2Scene(){
	int mouseX;
	int mouseY;

	//�N���b�N�����ꏊ���擾����
	GetMousePoint(&mouseX, &mouseY);
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
		int mapX = mouseX / 64;
		int mapY = mouseY / 64;
		//�N���b�N�����ꏊ���z��O�𒴂��Ă��Ȃ���
		if ((mapX < 0 || mapX < MAP_WIDTH) && (mapY < 0 || mapY < MAP_HEIGHT)){
			MapData[mapX][mapY] = (MapData[mapX][mapY] + 1) % 4;
		}
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0){
		MapSave("res/mapdata.dat");
	}

}
//	�����_�����O����
void renderGame2Scene(void){
	//�}�b�v�̕\��
	for (int i = 0; i < MAP_WIDTH; i++){
		for (int j = 0; j < MAP_HEIGHT; j++){
			//�}�X�̊Ǘ��ɑ΂��āA�\���摜��ύX
			switch (MapData[i][j])
			{
			case 0:		//��
				DrawGraph(i * 64, j * 64, img_pallet[0],true);
				break;
			case 1:		//��
				DrawGraph(i * 64, j * 64, img_pallet[1], true);
				break;
			case 2:		//��
				DrawGraph(i * 64, j * 64, img_pallet[2], true);
				break;
			case 3:		//��
				DrawGraph(i * 64, j * 64, img_pallet[3], true);
				break;
			default:
				break;
			}
		}
	}


	//DrawString(30, 50, "�}�b�v�G�f�B�^�[", GetColor(255, 255, 255));

}

//	�V�[���I�����̌㏈��
void releaseGame2Scene(void){
	
}

// ���蔻��R�[���o�b�N �@�@�@�����ł͗v�f���폜���Ȃ����ƁI�I
void  Game2SceneCollideCallback( int nSrc, int nTarget, int nCollideID ){


}

void MapSave(char *_FileName){
	//�t�@�C�����J���i�������݌`���j
	FILE *fp = fopen(_FileName, "w");

	//�t�@�C����MaoData����������
	fwrite(MapData, sizeof(int), MAP_WIDTH*MAP_HEIGHT, fp);

	//�t�@�C�������
	fclose(fp);
}

void MapLoad(char *_FileName){
	//�t�@�C�����J���i�������݌`���j
	FILE *fp = fopen(_FileName, "r");

	//�t�@�C����MaoData����������
	fread(MapData, sizeof(int), MAP_WIDTH*MAP_HEIGHT, fp);

	//�t�@�C�������
	fclose(fp);

}
