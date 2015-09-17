
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Image.h"
#include	"GameManager.h"
#include	"Player.h"
#include	"sceneTitle.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResult�N���X
//
//********************************************************************************

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneResult::sceneResult( void )
	{
	
	}

	//	�f�X�g���N�^
	sceneResult::~sceneResult( void )
	{
		SafeDelete( view );
	}

	//	������
	bool	sceneResult::Initialize( void )
	{
		//	�J�����ݒ�
		view = new iexView();

		for ( int i = 0; i < 4; i++ )
		{
			coinNum[i] = 0;
			resultInfo[i].p_Coin = GameManager::GetCoinNum( i );
			resultInfo[i].p_num = i;
		}

		//	�\�[�g
		BubbleSort();

		//	���X�g�{�[�i�X�ݒ�
		SetLastBonus();

		//	�ϐ�������
		step = 0;
		playerNum = 0;
		wait = 0;

		return	true;
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{

	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		view->Activate();
		view->Clear();

		//	�f�o�b�O�����`��
		DrawString( "[sceneResult]", 50, 50 );
		DrawString( "���؁[���Ń^�C�g����", 300, 400, 0xFFFFFF00 );
		DrawString( GameInfo::LastBonus[lastBonus], 300, 150 );

		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "%d�� : player%d�@%d��", i + 1, resultInfo[i].p_num + 1, resultInfo[i].p_Coin );
			DrawString( str, 550, 250 + i * 30, 0xFFFFFFFF );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	�o�u���\�[�g
	void	sceneResult::BubbleSort( void )
	{
		RESULT_INFO		temp;

		for ( int i = 0; i < 4; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s =3; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( resultInfo[s].p_Coin >	resultInfo[s - 1].p_Coin )
				{
					//	�ꎞ�I�ɑޔ�
					temp = resultInfo[s - 1];

					//	����
					resultInfo[s - 1] = resultInfo[s];

					//	�ޔ����Ă����߂�
					resultInfo[s] = temp;
				}
			}
		}
	}

	//	���X�g�{�[�i�X�ݒ�
	void	sceneResult::SetLastBonus( void )
	{
		std::uniform_real_distribution<float> bonusrand( 0.0f, 1.0f );
		if ( bonusrand( ran ) > 0.3f )
		{
			//	���X�g�{�[�i�X��ݒ�
			lastBonus = GameManager::GetLastBonus();
		}
		else
		{
			//	�Ⴄ���ʂ��o��܂Ń��[�v
			while ( lastBonus == GameManager::GetLastBonus() )
			{
				lastBonus = rand() % 5;
			}
		}
	}