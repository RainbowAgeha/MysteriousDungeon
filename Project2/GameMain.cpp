//���C���֐�
#include "GameMain.h"
#include "GameSceneMain.h"
#include "GameInit.h"

//�O���[�o���ϐ�
int game_status = GAMETITLE;

int Input, EdgeInput;      // ���͏��(Input(�������ςȂ�)�AEdgeInput(1��̂݁j 
int FrameStartTime;        // �U�O�e�o�r�Œ�p�A���ԕۑ��p�ϐ�

//WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow){


	/*
	// �E�C���h�E���[�h�ŋN�����邩�m�F����
	if (MessageBox(NULL, "�E�C���h�E���[�h�ŋN�����܂����H", "��ʃ��[�h�m�F", MB_YESNO) == IDYES)
	{
		// �E�C���h�E���[�h�ŋN��
		ChangeWindowMode(TRUE);
	}
	*/

	//���O��\�����Ȃ�
	SetOutApplicationLogValidFlag(FALSE);

	//�Q�[���̏�����(�N�����̏���)
	game_init();
	//���y�Đ��̏�����
	InitSoundMem();	//Dxlib

	// �Q�[�����[�v�O������
	InitGame();

	//���C�����[�v
	while (1){

		// ��ʂɕ`����Ă�����̂����S������
		ClearDrawScreen();

		// �P/�U�O�b���܂ő҂�
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}

		// ���݂̃J�E���g�l��ۑ�
		FrameStartTime = GetNowCount();

		// ���͏�Ԃ��X�V
		{
			int i;

			// �p�b�h�P�ƃL�[�{�[�h������͂𓾂�
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// �G�b�W����������͂��Z�b�g
			EdgeInput = i & ~Input;

			// ���͏�Ԃ̕ۑ�
			Input = i;
		}


		//�Q�[���̍X�V����
		FrameMove();
		//�Q�[���̕`�揈��
		RenderScene();


        //��ʂ̕\��������Ԏ�O���甽�f����
		ScreenFlip();

		//windows�V�X�e�����痈�������������(��O������)
		if (ProcessMessage() == -1)break;

		//ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//Dxlib�̊J��
	DxLib_End();

	// �Q�[���I�����̏�����
	GameRelease();

	//�Q�[�����I������
	return 0;
}