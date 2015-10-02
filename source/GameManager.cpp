
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"UI.h"
#include	"sceneResult.h"
#include	"Image.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	GameManager�N���X
//
//*******************************************************************************

//-------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------
	
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
		timer = TIMELIMIT;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % 4;
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
		//	�^�C�}�[�X�V
		timer--;

		//	�c�莞�ԂR�O�b�łǂ񂯂��o��
		if ( timer == 30 * SECOND )
		{
			DecideWorst();
			mode = GAME_MODE::DONKETSU_DIRECTION;
		}

		//	���Ԑ؂�
		if ( timer <= 0 )
		{
			timer = 0;
			mode = GAME_MODE::TIMEUP;
			for ( int i = 0; i < PLAYER_NUM; i++ )	m_Player->SetMode( i, MODE_STATE::WAIT );
		}

		//	�ǂ񂯂u�[�X�g�ݒ�
		if ( timer <= 30 * SECOND )		donketsuBoostState = true;
		else	donketsuBoostState = false;

		//	�j���[�X�ݒ�
		if ( timer == 1 * MINUTE )		newsflag = true;
	}

	//	�`��
	void	GameManager::Render( void )
	{
		//	���`��
	}

//-------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------

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

	//�@�r�����N��������
	void	GameManager::DecideWorst( void )
	{
		//�@�v���C���[���m�̃R�C�������r����
		//�@�R�C�������ŏ��̃v���C���[�̔ԍ���Ԃ��i�͂��j

		int work(coinNum[0]);
		int Min(0);
		for (int i = 1; i < 4; i++)
		{
			if (coinNum[i] < work)
			{
				work = coinNum[i];
				Min = i;
			}
		}

		worst = Min;
	}

//-------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------

	//	�v���C���[�l���擾
	int		GameManager::GetPlayerNum( void )
	{
		return	playerNum;
	}

	//	�v���C���[�^�C�v�擾
	int		GameManager::GetCharacterType( int num )
	{
		int		out = charatype[num];
		return	out;
	}

	//	�X�e�[�W�^�C�v�擾
	int		GameManager::GetStageType( void )
	{
		int		out = stageType;
		return	out;
	}

	//	�R�C�����擾
	int		GameManager::GetCoinNum( int num )
	{
		int		out = coinNum[num];
		return out;
	}

	//�@�ǂ񂯂����ǂ������擾
	bool	GameManager::GetDonketsuBoostState( void )
	{
		bool	out = donketsuBoostState;
		return	out;
	}

	//	���X�g�{�[�i�X�擾
	int		GameManager::GetLastBonus( void )
	{
		int		out = lastBonus;
		return	out;
	}

	//	�^�C�}�[�擾
	int		GameManager::GetTimer( void )
	{
		int		out = timer;
		return	out;
	}

	//	�j���[�X�t���O�擾
	bool	GameManager::GetNewsFlag( void )
	{
		bool	out = newsflag;
		return	out;
	}

	//	�ŉ��ʎ擾
	int		GameManager::GetWorst( void )
	{
		int		out = worst;
		return	worst;
	}

	//	���[�h�擾
	int		GameManager::GetMode( void )
	{
		int		out = mode;
		return	out;
	}

	//	���̎擾
	GameManager*	GameManager::GetInstance( void )
	{
		static	GameManager	out;
		return	&out;
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

	//	���[�h�ݒ�
	void	GameManager::SetMode( const int& modeNum )
	{
		mode = modeNum;
	}

	//	�j���[�X�t���O�ݒ�
	void	GameManager::SetNewsFlag( const bool& flag )
	{
		newsflag = flag;
	}
