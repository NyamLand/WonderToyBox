
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
		Random::Initialize();
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			charatype[i] = 0;		
			coinNum[i] = 0;
			startLife[i] = 5;
		}
		playerNum = 0;
		stageType = 1;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = Random::GetInt( 0, 3 );
		timeStop = 0;
		round = 0;
		timer = 0;
		maxlife = LIFE_MAX_NUM::LIFE_5;
		coinCollision = false;
		eventmode = 0;
		absCoinEventFlag = false;

		FOR( 0, Round::END )
		{
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				totalCoin[value][i] = 0;
			}
		}

		return	true;
	}

	//�@�i���C������n�߂邽�߂́j�f�o�b�O�p
	bool	GameManager::InitializeDebug( void )
	{
		//�@�v���C���[
		//charatype[0] = 0;	//�@�|����
		//charatype[0] = 1;	//�@�v�����Z�X
		charatype[0] = 2;	//�@����
		//charatype[0] = 3;	//�@�C��
		characterManager->Initialize(0, charatype[0], gameManager->InitPos[0], true);

		//�@�X�e�[�W
		//stageType = 0;	//�@��
		stageType = 1;	//�@�X

		//�@����
		//timelimit = 32 * SECOND;

		//�@�R�C���ő�
		maxlife = LIFE_MAX_NUM::LIFE_5;

		//�@CPU������
		for (int i = 1; i < PLAYER_MAX; i++)
		{
			characterManager->Initialize(i, charatype[i], gameManager->InitPos[i], false);
			charatype[i] = i;
		}
		
		//�@�����낢��
		for (int i = 0; i < PLAYER_MAX; i++)	coinNum[i] = 0;
		playerNum = 1;	//�@���삷��l��
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % PLAYER_MAX;
		timeStop = 0;
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
		lastBonus = Random::GetInt( 0, 3 );
		timeStop = 0;
		eventmode = 0;
		absCoinEventFlag = false;

		//	�Q�[���f�[�^�e�L�X�g��ǂݍ���
		//LoadTextData();
		timer = timelimit;
	}

	//	�L�����I��p������

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	GameManager::Update( void )
	{
		//	�^�C�}�[�X�V
		if (timeStop > 0)
		{
			timeStop--;
			return;
		}
		else timer--;

		//-------------------------------------------------------
		//	���Ԑ؂�
		//-------------------------------------------------------
		if ( timer <= 0 )
		{
			timer = 0;
			mode = GAME_MODE::TIMEUP;
			for ( int i = 0; i < PLAYER_MAX; i++ )	characterManager->SetMode( i, MODE_STATE::WAIT );
		}

		//-------------------------------------------------------
		//	�N���C�}�b�N�X��
		//-------------------------------------------------------
		if ( timer == 30 * SECOND ) SetMode( GAME_MODE::CLIMAX );

		//-------------------------------------------------------
		//	�j���[�X�ݒ�
		//-------------------------------------------------------
		if ( timer == 1 * MINUTE )		newsflag = true;

		//-------------------------------------------------------
		//�@�C�x���g
		//-------------------------------------------------------
		int alert_type = 0;
		int alert_sound = 0;

		/*
			�����_���ŃC�x���g����A
			�C�x���g�̊e��p�����[�^�ݒ��A
			eventManager�ŃC�x���g�𔭐�������
		*/

		//�@���o�J�n
		if (timer % EVENT_INTERVAL == EVENT_TIMING)
		{
			if (timer % (3 * EVENT_INTERVAL) == EVENT_TIMING)	absCoinEventFlag = true;

			eventmode = (absCoinEventFlag) ?
				Random::GetInt(EVENT_MODE::COIN_FALL, EVENT_MODE::COIN_DUBBLE) :
				Random::GetInt(0, EVENT_MODE::MAX - 1);

			if (eventmode <= EVENT_MODE::JAM_SLIP)
			{
				alert_type = ALERT_TYPE_INFO::JAM;
				alert_sound = SE::EVENT_SE;
			}
			else if (EVENT_MODE::COIN_FALL <= eventmode  && eventmode <= EVENT_MODE::COIN_DUBBLE)
			{
				alert_type = ALERT_TYPE_INFO::COIN;
				alert_sound = SE::EVENT_SE;		//�@���@�R�C���C�x���g�p�ɕύX
			}
			else
			{
				alert_type = ALERT_TYPE_INFO::MISSION;
				alert_sound = SE::EVENT_SE;		//�@���@�~�b�V�����C�x���g�p�ɕύX
			}

			//�@�摜�p�ɐݒ�
			switch (eventmode)
			{
			case EVENT_MODE::COIN_FALL:			ui->SetEventInfoMode(EVENT_TEX_INFO::C_FALL);	break;
			case EVENT_MODE::COIN_WAVE:			ui->SetEventInfoMode(EVENT_TEX_INFO::C_WAVE);	break;
			case EVENT_MODE::COIN_DUBBLE:		ui->SetEventInfoMode(EVENT_TEX_INFO::C_DUBBLE);	break;
			case EVENT_MODE::JAM_SLOPE_CAMERA:	ui->SetEventInfoMode(EVENT_TEX_INFO::J_CAMERA);	break;
			case EVENT_MODE::JAM_SLIP:			ui->SetEventInfoMode(EVENT_TEX_INFO::J_SLIP);	break;
			case EVENT_MODE::JAM_COIN_GETAWAY:	ui->SetEventInfoMode(EVENT_TEX_INFO::J_GETAWAY); break;
			default:	break;
			}

			ui->SetAlertInfo(true, alert_type);
			sound->PlaySE(alert_sound);
		}
		
		//�@�C�x���g����
		if (timer % EVENT_INTERVAL == EVENT_TIMING - 2*SECOND)
		{
			//ui->SetAlertInfo(false, alert_type);
			eventManager->SetEvent(eventmode);

			absCoinEventFlag = false;
		}
		

		//-------------------------------------------------------
		//		�A�C�e���E�R�C���΂�܂�
		//-------------------------------------------------------
		if ( timer != 0 )
		{
			//	5�b���ƂɃA�C�e�����R���̊m���łŃ����_���ɔz�u
			if ( timer % ( 5* SECOND ) == 0 )
			{
				if (itemflg)
				{
					if ( Random::PercentageRandom( 0.7f ) )
					{
						itemManager->Append(Vector3(Random::GetFloat(-20.0f, 20.0f), 50.0f, Random::GetFloat(-20.0f, 15.0f)), Random::GetInt( 0, ITEM_TYPE::MAX - 1 ) );
					}
				}
			}

			//	�Q�b���ƂɃR�C�����S���̊m���Ń����_���ɔz�u
			if ( timer % ( 1 * SECOND ) == 0 )
			{
				if ( Random::PercentageRandom( 0.6f ) )
				{
					if (coinNum[0] + coinNum[1] + coinNum[2] + coinNum[3] + coinManager->GetFreeCoinNum() < 300)
					{
						if ( Random::PercentageRandom( 0.5f ) )
						{
							coinManager->Append( Vector3( Random::GetFloat( -20.0f, 20.0f ), 50.0f, Random::GetFloat( -20.0f, 12.0f ) ), Vector3( 0.0f, -1.0f, 0.0f ), 1.0f, Coin::COIN_BAG_5 );
						}
					}
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
		if ( coinNum[playerNum] > 0 )
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
	int		GameManager::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	�v���C���[�^�C�v�擾
	int		GameManager::GetCharacterType( int num )const
	{
		int		out = charatype[num];
		return	out;
	}

	//	�X�e�[�W�^�C�v�擾
	int		GameManager::GetStageType( void )const
	{
		int		out = stageType;
		return	out;
	}
	
	//	�R�C�����擾
	int		GameManager::GetCoinNum( int num )const
	{
		int		out = coinNum[num];
		return out;
	}

	//�@�ǂ񂯂����ǂ������擾
	bool	GameManager::GetDonketsuBoostState( void )const
	{
		bool	out = donketsuBoostState;
		return	out;
	}

	//	���X�g�{�[�i�X�擾
	int		GameManager::GetLastBonus( void )const
	{
		int		out = lastBonus;
		return	out;
	}

	//	�^�C�}�[�擾
	int		GameManager::GetTimer( void )const
	{
		int		out = timer;
		return	out;
	}

	//	�j���[�X�t���O�擾
	bool		GameManager::GetNewsFlag( void )const
	{
		bool	out = newsflag;
		return	out;
	}

	//	�ŉ��ʎ擾
	int		GameManager::GetWorst( void )const
	{
		int		out = worst;
		return	worst;
	}

	//	���[�h�擾
	int		GameManager::GetMode( void )const
	{
		int		out = mode;
		return	out;
	}

	//��ʈꎞ��~�c��b���擾
	int		GameManager::GetTimeStop( void )const
	{
		int out = this->timeStop;
		return out;
	}

	//�R�C�������擾
	int		GameManager::GetMaxLife( void )const
	{
		return maxlife;
	}

	//�A�C�e���L���擾
	bool		GameManager::GetItemFlg(void)const
	{
		return itemflg;
	}
	
	//�@���ʍX�V
	int		GameManager::GetRank( int player )const
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

	//	�v���C���[�����̗͎擾
	int		GameManager::GetStartLife( int player )const
	{
		return	startLife[player];
	}

	//	�����E���h�擾
	int		GameManager::GetRound( void )const
	{
		return	round;
	}

	//	�g�[�^���R�C���擾
	int		GameManager::GetTotalCoin( int round, int player )const
	{
		return	totalCoin[round][player];
	}

	//	�R�C���Ƃ̓����蔻���ON,OFF
	bool		GameManager::GetCoinCollision( void )const
	{
		return	coinCollision;
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

	// �^�C�����~�b�g���ݒ�
	void	GameManager::SetTime( int minute,int second )
	{
		timer = minute * MINUTE + second * SECOND;
		timelimit = timer;
	}

	//�R�C�������ݒ�
	void	GameManager::SetMaxLife( int life )
	{
		this->maxlife = life;
		FOR( 0, PLAYER_MAX )	startLife[value] = maxlife + 3;
	}

	//�A�C�e���L���ݒ�
	void	GameManager::SetItemFlg( bool itemflg )
	{
		this->itemflg = itemflg;
	}

	//	�����̗͐ݒ�
	void	GameManager::SetStartLife( int player, int life )
	{
		startLife[player] = life;
	}

	//	���E���h�ݒ�
	void	GameManager::SetRound( int round )
	{
		this->round = round;
	}

	//	�g�[�^���R�C���ݒ�
	void	GameManager::SetTotalCoin( int round, int player, int total )
	{
		totalCoin[round][player] = total;
	}

	//	�R�C�������蔻��̏�ԏK��
	void	GameManager::SetCoinCollision( bool state )
	{
		coinCollision = state;
	}