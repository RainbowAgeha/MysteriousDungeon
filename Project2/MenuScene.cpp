#include "GameMain.h"
#include "GameSceneMain.h"
#include "MenuScene.h"

//���j���[���ڂ̃V�[���ԍ��̔z��
#define MENU_MAX 3
SCENE_NO menu[ MENU_MAX ] = { SCENE_GAME1, SCENE_GAME2, SCENE_GAME3};
char *menuList[3] = {"�Q�[���X�^�[�g","�I�v�V����","�I��"};
//�I�����ꂽ�Q�[����\�����j���[�ԍ��̏������imenu�̓Y�����j
static int selectedGame = 0;

int startfont;

//�����̓_�ŗp�^�C�}�[
int FontTime;


int Key[256]; // �L�[��������Ă���t���[�������i�[����

			  // �L�[�̓��͏�Ԃ��X�V����
int gpUpdateKey() {
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}


//�V�[���J�n�O�̏��������s��
BOOL initMenuScene(void){

	SetFontSize(32);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	selectedGame = 0;

	FontTime = 0;

	return TRUE;
}

//�t���[������
void moveMenuScene(){

	if (gpUpdateKey() == 0) {

		//�V���j���[���ڂ̑I��
		//�V(1) �@�V���Ɂ��������ꂽ��A
		if (Key[KEY_INPUT_UP] == 1) {
			//�V(1) �A�P��̃��j���[���ڂ��I�����ꂽ�Ƃ���B
			//      �@�������A�������̃��j���[���ڂ��Ȃ��Ƃ��́A�ŉ��i�̃��j���[���ڂ��I�����ꂽ�Ƃ���
			if (--selectedGame < 0) {
				selectedGame = MENU_MAX - 1;
			}
		}


		//�V(2) �@�V���Ɂ��������ꂽ��A
		if (Key[KEY_INPUT_DOWN] == 1) {
			//�V(2) �A�P���̃��j���[���ڂ��I�����ꂽ�Ƃ���B�B
			//      �@�������A�����艺�̃��j���[���ڂ��Ȃ��Ƃ��́A�ŏ�i�̃��j���[���ڂ��I�����ꂽ�Ƃ���
			if (++selectedGame >= MENU_MAX) {
				selectedGame = 0;
			}
		}

		//�V(3) �V���Ƀ{�^���P�������ꂽ��I������Ă���V�[����
		if (Key[KEY_INPUT_Z] == 1) {
			changeScene(menu[selectedGame]);
		}


		//�����I�ɃV�[���ɔ�΂�
		//changeScene(menu[1]);
	}


}

//�����_�����O����
void renderMenuScene(void){

	DrawString(30, 50, "���j���[���", GetColor(255, 255, 255));
	DrawString(30, 400, "�L�[�����őI���B�{�^���P�Ō���", GetColor(255, 255, 255));

	//--------------------------------------------
	////�����̓_�ŉ��o �p�^�[��A�@�p�^�p�^�Ɠ_�ł�����B
	//FontTime++;
	////������\�����鎞�Ԃ�ݒ�
	//if (FontTime < 60) {
	//	DrawString(30, 350, "�{�^���P�ŃX�^�[�g", GetColor(255, 255, 255));
	//}

	////�������Ԃ͏����Z������
	//if (FontTime > 90) {
	//	FontTime = 0;
	//}
	//--------------------------------------------


	//--------------------------------------------
	//�����̓_�ŉ��o �p�^�[��B�@�����̃t�F�[�h�C���E�A�E�g
	FontTime++;
	//������\�����鎞�Ԃ�ݒ�
	if (FontTime < 60) {
		//���񂾂�Ə���
		// �`�惂�[�h���A���t�@�u�����h���[�h�ɂ��ē����x������������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (FontTime*4));
		DrawString(30, 350, "�{�^���P�ŃX�^�[�g", GetColor(255, 255, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}

	//���񂾂�ƕ\������
	if (FontTime > 60) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FontTime-60) * 4);
		DrawString(30, 350, "�{�^���P�ŃX�^�[�g", GetColor(255, 255, 255));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
	if (FontTime > 120) {
		FontTime = 0;
	}
	//--------------------------------------------

	//�U(2) ���j���[���ڂ̕\��
	int x = 220, y = 140, gapY = 80;	//�ix,y)�F�\���J�n���W�@gapY�F�s�̍���
	for( int i = 0; i < MENU_MAX; i++, y += gapY ){
		
		//�U(2) �@�I�����ꂽ���ڂ̕\��
		if( i == selectedGame ){
			DrawString(x, y, menuList[i], GetColor(255, 0, 0));
		//�U(2) �A�I������Ă��Ȃ����ڂ̕\��
		}else{
			DrawString(x, y, menuList[i], GetColor(255, 255, 255));
		}
		
	}
}

//�V�[���I�����̌㏈��
void releaseMenuScene(void){


}