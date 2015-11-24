
#include	"iextreme.h"
#include	"system/Framework.h"
#include	<iostream>
#include	<fstream>
#include	<string>
#include	"GlobalFunction.h"
#include	"textLoader.h"
#include	"Random.h"
#include	"Collision.h"
#include	"UI.h"
#include	"EventManager.h"
#include	"ItemManager.h"
#include	"CoinManager.h"
#include	"sceneResult.h"
#include	"CharacterManager.h"
#include	"Sound.h"
#include	"Camera.h"
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
		InitPos[0] = Vector3( -10.0f, 10.0f, 15.0f );
		InitPos[1] = Vector3( 10.0f, 10.0f, 15.0f );
		InitPos[2] = Vector3( -10.0f, 10.0f, -15.0f );
		InitPos[3] = Vector3( 10.0f, 10.0f, -15.0f );
	}

	//	�f�X�g���N�^
	GameManager::~GameManager( void )
	{

	}

	//	������
	bool	GameManager::Initialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			charatype[i] = 0;		
			coinNum[i] = 0;
		}
		playerNum = 0;
		stageType = 1;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;

		//	�Q�[���f�[�^�e�L�X�g��ǂݍ���
		//LoadTextData();
		timer = 0;

		return	true;
	}

	//�@�i���C������n�߂邽�߂́j�f�o�b�O�p
	bool	GameManager::InitializeDebug(void)
	{
		charatype[0] = 1;

		for (int i = 1; i < PLAYER_MAX; i++)
		{
			charatype[i] = i;
			coinNum[i] = 0;
		}
		playerNum = 1;	//�@���삷��l��
		stageType = 0;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;
		//	�Q�[���f�[�^�e�L�X�g��ǂݍ���
		LoadTextData();
		timer = timelimit;

		return	true;
	}

	//	���
	void	GameManager::Release( void )
	{

	}

	//	���g���C�p������
	void	GameManager::RetryInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			coinNum[i] = 0;
		}

		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;

		//	�Q�[���f�[�^�e�L�X�g��ǂݍ���
		LoadTextData();
		timer = timelimit;
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	GameManager::Update( void )
	{
		//	�^�C�}�[�X�V
		if (timeStop > 0) timeStop--;
		else timer--;


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
			for ( int i = 0; i < PLAYER_MAX; i++ )	characterManager->SetMode( i, MODE_STATE::WAIT );
		}

		//	�ǂ񂯂u�[�X�g�ݒ�
		if ( timer <= 30 * SECOND )		donketsuBoostState = true;
		else	donketsuBoostState = false;

		//	�j���[�X�ݒ�
		if ( timer == 1 * MINUTE )		newsflag = true;

		//	�C�x���g�ݒ�
		if (timer == 42 * SECOND)	{
			ui->SetAlertFlag(true);
			sound->PlaySE(SE::EVENT_SE);
		}
		if ( timer == 40 * SECOND ) eventManager->SetEvent( EVENT_MODE::SLOPE_CAMERA/*Random::GetInt( 0, EVENT_MODE::NONE - 1 )*/ );

		if ( timer != 0 )
		{
			//	5�b���ƂɃA�C�e�����R���̊m���łŃ����_���ɔz�u
			if ( timer % ( 5* SECOND ) == 0 )
			{
				if (itemflg){
					if (Random::PercentageRandom(0.7f))
					{
						itemManager->Append(Vector3(Random::GetFloat(-20.0f, 20.0f), 50.0f, Random::GetFloat(-20.0f, 15.0f)), ITEM_TYPE::ATTACK_UP);
					}
				}
			}

			//	�Q�b���ƂɃR�C�����S���̊m���Ń����_���ɔz�u
			if ( timer % ( 1 * SECOND ) == 0 )
			{
				if ( Random::PercentageRandom( 0.6f ) )
				{
					if ( coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + m_CoinManager->GetFreeCoinNum() < 201)
					m_CoinManager->Set( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 12.0f ) ), Vector3( 0.0f, -1.0f, 0.0f ), 1.0f );
				}
			}
		}
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
		for (int i = 1; i < PLAYER_MAX; i++)
		{
			if (coinNum[i] < work)
			{
				work = coinNum[i];
				Min = i;
			}
		}

		worst = Min;
	}

	//	�e�L�X�g�ǂݍ���
	void	GameManager::LoadTextData( void )
	{
		//	�������ۑ�����o�b�t�@,�ϐ���p��
		char	buffer[256];
		int		minute = 0;		//	��
		int		second = 0;		//	�b

		//	�t�@�C�����J��
		std::ifstream	ifs( "GameData.txt" );

		//	�ŏ��̓R�����g�Ȃ̂œǂݔ�΂�, �l�͓K��
		ifs.getline( buffer, 50 );

		//	�ǂݍ��񂾒l��ϐ��֑��
		ifs >> minute;

		//	�R�����g��ǂݔ�΂�
		ifs >> buffer;
		ifs.getline( buffer, 50 );

		//	�ǂݍ��񂾒l��ϐ��֑��
		ifs >> second;

		//	���v�l���^�C�����~�b�g�ϐ��֑��
		timelimit = minute * MINUTE + second * SECOND;
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

	//��ʈꎞ��~�c��b���擾
	int		GameManager::GetTimeStop( void )
	{
		int out = this->timeStop;
		return out;
	}

	//�R�C�������擾
	int	GameManager::GetCoinMax(void)const
	{
		return coinmax;
	}

	//�A�C�e���L���擾
	bool	GameManager::GetItemFlg(void)const
	{
		return itemflg;
	}
	//�@���ʍX�V
	int		GameManager::GetRank( int player )
	{
		int num_coin[PLAYER_MAX], temp_coin[PLAYER_MAX];
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			num_coin[i] = temp_coin[i] = GetCoinNum(i);
		}

		for (int i = 0, temp; i < PLAYER_MAX - 1; i++)
		{
			for (int j = PLAYER_MAX - 1; j > i; j--)
			{
				if (temp_coin[j - 1] > temp_coin[j])
				{
					temp = temp_coin[j];
					temp_coin[j] = temp_coin[j - 1];
					temp_coin[j - 1] = temp;
				}
			}
		}

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				if (num_coin[player] == temp_coin[j])
				{
					return PLAYER_MAX - j;
				}
			}
		}

		return	0;
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

	// �J�����̐U���n�ݒ�
	void	GameManager::SetShakeCamera( float wide, int timer )
	{
		if ( mainView ) mainView->ShakeSet( wide, timer );
	}

	//��ʈꎞ��~���Ԑݒ�
	void	GameManager::SetTimeStop( int time )
	{
		this->timeStop = time;
	}

	//�^�C�����~�b�g���ݒ�
	void	GameManager::SetTime(int minute,int second)
	{
		timer=minute*MINUTE + second*SECOND;
	}

	//�R�C�������ݒ�
	void	GameManager::SetCoinMax(int coinmax)
	{
		this->coinmax = coinmax;
	}

	//�A�C�e���L���ݒ�
	void	GameManager::SetItemFlg(bool itemflg)
	{
		this->itemflg = itemflg;
	}