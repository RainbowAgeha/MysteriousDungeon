//�Q�[���̏�����
#include "GameInit.h"

//�Q�[���S�̂̏�����
int game_init(){
	//�E�B���h�E�̐ݒ�
	//�E�B���h�E���[�h�̐ݒ�
	ChangeWindowMode(true);
	//�E�B���h�E�̃��T�C�Y
	//Check:���s���ɉ�ʂ̑傫�����ύX�\��
	SetWindowSizeChangeEnableFlag(true);
	//�E�B���h�E�^�C�g��
	SetMainWindowText("�Q�[���e�X�g");

	//�w�i�F�̐ݒ�
	SetBackgroundColor(0, 0, 0);
	//DX���C�u��������������
	//check:-1(��O����)�������ꍇ�́A�Z�b�g�A�b�v�Ɏ��s����
	//�p�\�R���{�̂̋@��̊m�F�����Ă��炤(�����f�o�C�X���Ȃ����B)
	if (DxLib_Init() == -1)return -1;

	//�`������Ԍ��ɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//TODO:�Q�[���̏����������������ōs��
	//PlayerInit();
	return 0;       //����������
}