
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
	
	//	static
	int		GameManager::charatype[4] = { 0, 0, 0, 0 };
	int		GameManager::coinNum[4] = { 0, 0, 0, 0 };
	int		GameManager::playerNum = 0;
	int		GameManager::stageType = 0;
	int		GameManager::waitTimer = 0;
	bool	GameManager::changeSceneflag = false; 
	bool	GameManager::donketsuBoostState = false;
	int		GameManager::timer = 0;
	int		GameManager::mode = 0;
	int		GameManager::worst = 0;
	int		GameManager::lastBonus = 0;
	bool	GameManager::newsflag = false;
	NewsBar	GameManager::newsbar;
	Image*		GameManager::countDown;
	int				GameManager::count = 0;

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
		waitTimer = 0;
		mode = 0;
		donketsuBoostState = false;
		lastBonus = rand() % 4;

		//	�j���[�X�o�[������
		{
			newsflag = false;
			newsbar.left = 1280;
			newsbar.top = 0;
			newsbar.right = 1280;
			newsbar.bottom = 50;
			newsbar.text = GameInfo::NewsText[lastBonus];
			newsbar.alpha = 0.5f;
			newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
			newsbar.step = 0;
			newsbar.textleft = 1500;
		}

		//	�J�E���g�_�E��������
		{
			countDown = new Image( "DATA/bfUI.png" );
			countDown->Initialize( 640, 360, 350, 350, 0, 0, 256, 256 );
			count = 0;
		}
		return	true;
	}

	//	���
	void	GameManager::Release( void )
	{
		SafeDelete( countDown );
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	GameManager::Update( void )
	{
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			GameStartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			MainGameUpdate();
			break;

		//�@�ǂ񂯂p���o
		case GAME_MODE::DONKETSU_DIRECTION:
			//�@�����Ńr��������i�Ȍ�ύX�Ȃ��j
			DonketsuDirectionUpdate(); 
			break;
		
		case GAME_MODE::TIMEUP:
			TimeUpUpdate();
			break;
		}
	}

	//	�`��
	void	GameManager::Render( void )
	{
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			GameStartRender();
			break;

		case GAME_MODE::MAINGAME:
			MainGameInfoRender();
			break;

		//�@�ǂ񂯂p���o
		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuDirectionRender();
			break;
		
		case GAME_MODE::TIMEUP:
			TimeUpRender();
			break;
		}
	}

	//------------------------------------------------------------------
	//	�Q�[�����쒆
	//------------------------------------------------------------------
	//	�X�V
	void	GameManager::MainGameUpdate( void )
	{
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
			mode = GAME_MODE::TIMEUP;
			
			//	�摜�ǂݍ��݈ʒu�E�T�C�Y�ݒ�
			countDown->SetSize( 600, 370 );
			countDown->SetSearchPos( 0, 512 );
			countDown->SetSearchSize( 1024, 512 );
		}

		//	�^�C�}�[�X�V
		m_UI->SetTimer( timer );
		m_UI->Update();

		//	�ǂ񂯂u�[�X�g�ݒ�
		if ( m_UI->GetTimer() <= 30 * SECOND )		donketsuBoostState = true;
		else	donketsuBoostState = false;
		m_UI->SetDonketsuBoostState( donketsuBoostState );

		//	�j���[�X�ݒ�
		{
			if ( timer == 1 * MINUTE )	newsflag = true;
			if ( newsflag )	SetLastBonusNews();
		}
	}

	//	���C���Q�[�����`��
	void	GameManager::MainGameInfoRender( void )
	{
		m_UI->Render();
		NewsRender();

		if ( !debug )	return;
		//	�f�o�b�O�p
		char	str[256];
		int		maxCoin = 0;
		for ( int i = 0; i < 4; i++ )
		{
			sprintf_s( str, "p%d_coin = %d", i + 1, coinNum[i] );
			DrawString( str, 20, 150 + i * 30 );
		}

		if ( donketsuBoostState )
		{
			DrawString( "�ǂ񂯂u�[�X�g�Ȃ�", 600, 250 );
		}
	}

	//	�j���[�X�`��
	void	GameManager::NewsRender( void )
	{
		if ( newsflag )
		{
			iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
			IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + 10, 500, 200, 0xFFFFFFFF );
		}
	}

	//------------------------------------------------------------------
	//	�Q�[���X�^�[�g���o
	//------------------------------------------------------------------
	//	�X�V
	void	GameManager::GameStartUpdate( void )
	{
		//	�^�C�}�[�X�V
		waitTimer++;

		//	��b���Ƃɉ摜�i�߂�
		if ( waitTimer % SECOND == 0 )
		{
			//	�J�E���g�_�E��
			count++;

			//	�ǂݍ��݈ʒu�E�T�C�Y�ݒ�
			switch ( count )
			{
			case 1:
				countDown->SetSearchPos( 256, 0 );
				break;

			case 2:
				countDown->SetSearchPos( 0, 256 );
				break;

			case 3:
				countDown->SetSearchPos( 512, 0 );
				countDown->SetSearchSize( 512, 512 );
				countDown->SetSize( 750, 750 );
				countDown->SetWave();
				break;

			case 4:
				for ( int i = 0; i < 4; i++ )
				{
					m_Player->SetMode( i, PlayerData::MOVE );
					waitTimer = 2 * SECOND;
				}
				mode = GAME_MODE::MAINGAME;
				break;
			}

		}

		countDown->Update();
	}

	//	�`��
	void	GameManager::GameStartRender( void )
	{
		countDown->NormalRender();
		countDown->Render();
	}

	//------------------------------------------------------------------
	//	�ǂ񂯂��o
	//------------------------------------------------------------------
	//�@�X�V
	void	GameManager::DonketsuDirectionUpdate( void )
	{
		static int wait( 5 * SECOND );
		wait--;

		if ( wait <= 0 )
		{
			wait = 30;
			mode = GAME_MODE::MAINGAME;
		}
	}

	//�@�`��
	void	GameManager::DonketsuDirectionRender( void )
	{
		char	str[256];
		DrawString( "�ǂ񂯂��o", 200, 50 );
		wsprintf( str, "�r���� p%d", worst + 1 );
		DrawString( str, 200, 70 );
	}
	
	//------------------------------------------------------------------
	//	�^�C���A�b�v���o
	//------------------------------------------------------------------
	//�@�X�V
	void	GameManager::TimeUpUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )
		{
			MainFrame->ChangeScene( new sceneResult() );
			return;
		}
	}

	//	�`��
	void	GameManager::TimeUpRender( void )
	{
		m_UI->Render();
		countDown->NormalRender();

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

	//	���X�g�{�[�i�X�j���[�X
	void	GameManager::SetLastBonusNews( void )
	{
		switch ( newsbar.step )
		{
		case 0:
			//	�o�[�o��
			newsbar.left -= 30;
			if ( newsbar.left <= 0 )
			{
				newsbar.left = 0;
				newsbar.step++;
			}
			break;

		case 1:
			//	�e�L�X�g�o��
			newsbar.textleft--;
			if ( newsbar.textleft <= -320 )
			{
				newsbar.textleft = 1500;
				newsbar.step++;
			}
			break;

		case 2:
			//	�o�[�ޔ�
			newsbar.right -= 30;
			if ( newsbar.right <= 0 )
			{
				newsbar.right = 0;
				newsbar.step++;
			}
			break;

		case 3:
			//	������
			newsflag = false;
			newsbar.left = 1280;
			newsbar.right = 1280;
			newsbar.textleft = 1500;
			newsbar.step = 0;
			break;
		}
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

	//�@�ǂ񂯂����ǂ������擾
	bool	GameManager::GetDonketsuBoostState( void )
	{
		return	donketsuBoostState;
	}

	//�@�r�����擾
	int		GameManager::GetWorst( void )
	{
		return	worst;
	}

	//	���X�g�{�[�i�X�擾
	int		GameManager::GetLastBonus( void )
	{
		return	lastBonus;
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

