
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"UI.h"
#include	"sceneResult.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	GameManager�N���X
//
//*******************************************************************************

//-------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------
	
	//	static
	int		GameManager::charatype[4] = { 0, 0, 0, 0 };
	int		GameManager::coinNum[4] = { 0, 0, 0, 0 };
	int		GameManager::playerNum = 0;
	int		GameManager::stageType = 0;
	int		GameManager::waitTimer = 0;
	bool	GameManager::changeSceneflag = false;
	int		GameManager::timer = 0;
	int		GameManager::mode = 0;

//-------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------

	//	�R���X�g���N�^
	GameManager::GameManager( void )
	{
	
	}

	//	�f�X�g���N�^
	GameManager::~GameManager( void )
	{

	}

	//	������
	bool	GameManager::Initialize( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			charatype[i] = 0;
			coinNum[i] = 0;
		}
		playerNum = 0;
		stageType = 0;
		changeSceneflag = false;
		timer = TIMELIMIT;
		waitTimer = 2 * SECOND;
		mode = 0;
		return	true;
	}

	//	���
	void	GameManager::Release( void )
	{

	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	GameManager::Update( void )
	{
		switch ( mode )
		{
		case MAINGAME:
			MainGameUpdate();
			break;

		case TIMEUP:
			TimeUpUpdate();
			break;
		}
	}

	//	�`��
	void	GameManager::Render( void )
	{
		switch ( mode )
		{
		case MAINGAME:
			MainGameInfoRender();
			break;

		case TIMEUP:
			TimeUpRender();
			break;
		}
	}

	//	���C���Q�[�����`��
	void	GameManager::MainGameInfoRender( void )
	{
		m_UI->Render();

		if ( !debug )	return;
		//	�f�o�b�O�p
		char	str[256];
		int		maxCoin = 0;
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}
	}

	//	�^�C���A�b�v�`��
	void	GameManager::TimeUpRender( void )
	{
		m_UI->Render();

		if ( !debug )	return;
		//	�f�o�b�O�p
		char	str[256];
		int		maxCoin = 0;
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}

		DrawString( "TimeUp!!", 600, 350, Vector3( 1.0f, 1.0f, 0.0f ) );
	}

//-------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------

	//	���C���Q�[���X�V
	void	GameManager::MainGameUpdate( void )
	{
		timer--;
		if ( timer <= 0 )
		{
			mode = TIMEUP;
		}
		m_UI->SetTimer( timer );
		m_UI->Update();
	}

	//	�^�C���A�b�v�X�V
	void	GameManager::TimeUpUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )
		{
			MainFrame->ChangeScene( new sceneResult() );
			return;
		}
	}

	//	�R�C�����Z
	void	GameManager::AddCoin( int playerNum )
	{
		coinNum[playerNum]++;
	}

	//	�R�C�����Z
	void	GameManager::SubCoin( int playerNum )
	{
		coinNum[playerNum]--;
	}

//-------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------

	//	�V�[���؂�ւ��t���O�擾
	bool	GameManager::GetChangeSceneFlag( void )
	{
		return	changeSceneflag;
	}

	//	�v���C���[�l���擾
	int		GameManager::GetPlayerNum( void )
	{
		return	playerNum;
	}

	//	�v���C���[�^�C�v�擾
	int		GameManager::GetCharacterType( int num )
	{
		return	charatype[num];
	}

	//	�X�e�[�W�^�C�v�擾
	int		GameManager::GetStageType( void )
	{
		return	stageType;
	}

	//	�R�C�����擾
	int		GameManager::GetCoinNum( int num )
	{
		return	coinNum[num];
	}

//-------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------

	//	�v���C���[�l��
	void	GameManager::SetPlayerNum( int num )
	{
		playerNum = num;
	}

	//	�v���C���[�^�C�v
	void	GameManager::SetCharacterType( int num, int type )
	{
		charatype[num] = type;
	}

	//	�X�e�[�W�^�C�v
	void	GameManager::SetStageType( int type )
	{
		stageType = type;
	}

	//	�R�C�����
	void	GameManager::SetCoinNum( int num, int param )
	{
		coinNum[num] = param;
	}