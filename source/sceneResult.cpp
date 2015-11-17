
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"CharacterManager.h"
#include	"sceneTitle.h"
#include	"sceneMenu.h"
#include	"sceneMain.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResult�N���X
//
//********************************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

	namespace
	{
		namespace MOVE_MODE
		{
			enum
			{
				RESULT,
				SELECT,
			};
		}

		namespace MENU
		{
			enum
			{
				RESTART,						//	�Đ�
				MOVE_MENU,					//	�ΐ�ݒ��	
				MOVE_TITLE,					//	�^�C�g����
				END,								//	�I�[
			};
		}
	}

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
		Release();
	}

	//	������
	bool	sceneResult::Initialize( void )
	{
		//	����������
		Random::Initialize();

		//	�J�����ݒ�
		CameraInitialize();

		//	�摜�ǂݍ���
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		menuHead.obj = new iex2DObj( "DATA/UI/menu-head.png" );
		originNumber = new iex2DObj( "DATA/UI/number.png" );
		menuText = new iex2DObj( "DATA/Result/result-cho.png" );

		//	���f���ǂݍ���
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );			//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Y2009/Y2009.IEM" ) );					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" ) );				//	���X
		org[CHARACTER_TYPE::TIGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );					//	�g��

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );			//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.02f );			//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale( 0.04f );				//	���X
		org[CHARACTER_TYPE::TIGER]->SetScale( 0.02f );				//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );		//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );			//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );			//	�V�[�t
		org[CHARACTER_TYPE::TIGER]->SetAngle( D3DX_PI );			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );				//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );				//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );	 				//	�V�[�t
		org[CHARACTER_TYPE::TIGER]->SetMotion( 0 );					//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->Update();					//	�|����
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	�P
		org[CHARACTER_TYPE::THIEF]->Update();						//	���X
		org[CHARACTER_TYPE::TIGER]->Update();						//	�g��

		//	���f��������
		ModelInitialize();

		//	�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );
		menuHead.angle = D3DXToRadian( 7.0f );
		
		//	���l�\���̏�����
		FOR( 0, PLAYER_MAX )
		{
			number[value].hundred = 0;
			number[value].ten = 0;
			number[value].one = 0;
		}

		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;

		//	���X�g�{�[�i�X�ݒ�
		SetLastBonus();

		//	���ʗp���\���̏�����
		ResultInfoInitialize();

		//	���l�\���̏�����
		NumberImageInfoInitialize();

		//	�\�[�g������
		Sort();

		//	�����L���O�ݒ�
		SetRank();
		
		//	���ʉ摜�\���̏�����
		RankImageInitialize();

		//	���[���b�g��񏉊���
		{
			rouletteInfo.step = 0;
			rouletteInfo.timer = 0;
		}

		//	���j���[��񏉊���
		{
			menuInfo.select = MENU::RESTART;
			menuInfo.screenH = 0;
			menuInfo.alpha = 0.5f;
			menuInfo.t = 0.0f;
		}

		//	���j���[�摜�\���̏�����
		{
			//	���j���[�̍��ڐ��܂킷
			for ( int i = 0; i < 3; i++ )
			{
				x = static_cast<int>( iexSystem::ScreenWidth / 2 );
				y = static_cast<int>( iexSystem::ScreenHeight * 0.28f );
				w = static_cast<int>( iexSystem::ScreenWidth * 0.55f );
				h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
				ImageInitialize( menuImage[i], x, y + y * i, w, h, 0, 128 * i, 512, 128 );
				menuImage[i].obj = menuText;
				menuImage[i].renderflag = false;
			}
		}

		//	�����N�\���p�\���̏�����
		{
			viewRankInOrder.timer = 0;
			viewRankInOrder.step = 3;
		}
		return	true;
	}

	//	���
	void	sceneResult::Release( void )
	{
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		Random::Release();
	}

//----------------------------------------------------------------------------
//	�e���񏉊���
//----------------------------------------------------------------------------

	//	�J����������
	void	sceneResult::CameraInitialize( void )
	{
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		mainView->Set( Vector3( 0.0f, 5.2f, -70.0f ), Vector3( 0.0f, 5.2f, 0.0f ) );
		mainView->Activate();
	}

	//	���f��������
	void	sceneResult::ModelInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	���f�����ݒ�
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->Update();
		}
	}

	//	���ʗp�\���̊֘A������
	void	sceneResult::ResultInfoInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�Q�[���I�����̃f�[�^���i�[( �����Ń{�[�i�X���ݒ肵�Ă��� )
			originInfo[i].coin = gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	�����L���O�v�Z�p�ɑ��v�f�[�^���i�[( �{�[�i�X���l����������A���̃R�C�������Ƀ{�[�i�X�𑫂��A�����N�̓\�[�g�ɂ����邽�ߓK���ɑ�� )
			sortInfo[i].coin = originInfo[i].coin;
			sortInfo[i].rank = i;
		}
	}

	//	���l�\���̏�����
	void	sceneResult::NumberImageInfoInitialize( void )
	{	
		Vector3	out;

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�N���C�A���g���W�ɕϊ�
			WorldToClient( obj[i]->GetPos(), out, matView * matProjection );

			//	���S���W�E�����X�P�[���ݒ�
			numberImageInfo[i].pos.x = static_cast<int>( out.x );
			numberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.38f );
			numberImageInfo[i].scale = 100;

			//	�e�ʉ摜�ݒ�
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;

			//	���l�摜�\���̏�����
			SetNumberImageInfo( i, originInfo[i].coin );
		}
	}

	//	���ʉ摜�\���̏�����
	void	sceneResult::RankImageInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			Vector3	out;

			//	�N���C�A���g���W�ɕϊ�
			WorldToClient( obj[i]->GetPos(), out, matView * matProjection );

			//	�\���̏�����
			int 	x = static_cast<int>( out.x );
			int		y = static_cast<int>( iexSystem::ScreenHeight * 0.55f );
			int		w = static_cast<int>( iexSystem::ScreenWidth * 0.12f );
			int		h = static_cast<int>( iexSystem::ScreenHeight * 0.13f );

			int		sx = originInfo[i].rank * 128;
			int		sy = 128;
			int		sw = 128;
			int		sh = 64;
			ImageInitialize( rankImage[i], x, y, w, h, sx, sy, sw, sh );
			rankImage[i].obj = originNumber;
			rankImage[i].renderflag = false;
		}
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{
		//	�e���f���X�V
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Update();
		}

		switch ( mode )
		{
		case MOVE_MODE::RESULT:
			ResultUpdate();
			break;

		case MOVE_MODE::SELECT:
			SelectUpdate();
			break;
		}

		//	�V�[���ړ��Ǘ�
		MoveScene();
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		mainView->Activate();
		mainView->Clear();

		//	�w�i�`��
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	���U���g�V�[���`��
		RenderImage( menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM );

		//	�v���C���[�`��
		FOR( 0, PLAYER_MAX )
		{
			//	�v���C���[�`��
			obj[value]->Render( shader3D, "toon" );
		}

		//	���l�`��
		NumberImageRender();

		//	���ʕ`��
		RankRender();

		//	���j���[�p�X�N���[���`��
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		//	���j���[���ڕ`��
		SelectRender();
	}

	//	���U���g���̍X�V
	void	sceneResult::ResultUpdate( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false;

		switch ( step )
		{
		case 0:		//	���[���b�g
			isEnd = Roulette();

			//	����{�^���ŃX�L�b�v
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				for ( int i = 0; i < PLAYER_MAX; i++ )
				{
					SetNumberImageInfo( i, originInfo[i].coin );
					SetWave( rankImage[i], 1.5f );
					rankImage[i].renderflag = true;
				}
				
				step = 2;
			}

			//	��]���I��������
			if ( isEnd )		step++;
			break;
			
		case 1:		//	���ʕ`��
			isFinViewRankInOrder = ViewRankInOrder();
			isEnd = RankWave();

			//	����{�^���ŃX�L�b�v
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				for ( int i = 0; i < PLAYER_MAX; i++ )
				{
					SetNumberImageInfo( i, originInfo[i].coin );
					SetWave( rankImage[i], 1.5f );
					rankImage[i].renderflag = true;
				}

				step = 2;
			}

			//	�g��I����ɑI����
			if ( isFinViewRankInOrder && isEnd )
			{
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					step = 0;
					mode = MOVE_MODE::SELECT;
				}
			}
			break;

		case 2:	//	���ʕ`��(�X�L�b�v�p)
			for ( int i = 0; i < PLAYER_MAX; i++ )		isEnd = WaveUpdate( rankImage[i] );

			//	�g��I����ɑI����
			if ( isEnd )
			{
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					step = 0;
					mode = MOVE_MODE::SELECT;
				}
			}
			break;
		}
	}

	//	�Z���N�g���̍X�V
	void	sceneResult::SelectUpdate( void )
	{
		bool	isEnd = false;

		switch ( step )
		{
		case 0:
			menuInfo.t += 0.03f;
			if ( menuInfo.t >= 1.0f )
			{
				menuInfo.t = 1.0f;
				
				//	���j���[���ڕ`��
				for ( int i = 0; i < 3; i++ )		menuImage[i].renderflag = true;
			}
			isEnd = Lerp( menuInfo.screenH, 0, static_cast<int>( iexSystem::ScreenHeight ), menuInfo.t );

			if ( isEnd )step++;
			break;

		case 1:
			//	�㉺�őI��
			if ( input[0]->Get( KEY_UP ) == 3 )		menuInfo.select--;
			if ( input[0]->Get( KEY_DOWN ) == 3 )	menuInfo.select++;
			if ( menuInfo.select < 0 )						menuInfo.select = MENU::END - 1;
			if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::RESTART;
			
			//	�ǂݍ��݈ʒu�ύX
			for ( int i = 0; i < 3; i++ )
			{
				if ( i == menuInfo.select )	menuImage[i].sx = 512;
				else									menuImage[i].sx = 0;
			}

			//	����
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				changeScene = true;
			}
			break;
		}
	}

	//�Z���N�g��ʕ`��
	void	sceneResult::SelectRender( void )
	{
		for ( int i = 0; i < 3; i++ )
		{
			RenderImage( menuImage[i], menuImage[i].sx, menuImage[i].sy, menuImage[i].sw, menuImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	���l�摜�\���̕`��
	void	sceneResult::NumberImageRender( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�P�O�O�̈ʕ`��
			int		sx = numberImageInfo[i].hundred.sx;
			int		sy = numberImageInfo[i].hundred.sy;
			int		sw = numberImageInfo[i].hundred.sw;
			int		sh = numberImageInfo[i].hundred.sh;

			if ( numberImageInfo[i].hundredRenderFlag )
				RenderImage( numberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	�P�O�̈ʕ`��
			sx = numberImageInfo[i].ten.sx;
			sy = numberImageInfo[i].ten.sy;
			sw = numberImageInfo[i].ten.sw;
			sh = numberImageInfo[i].ten.sh;
			RenderImage( numberImageInfo[i].ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	�P�̈ʕ`��
			sx = numberImageInfo[i].one.sx;
			sy = numberImageInfo[i].one.sy;
			sw = numberImageInfo[i].one.sw;
			sh = numberImageInfo[i].one.sh;
			RenderImage( numberImageInfo[i].one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
		}
	}

	//	�����N�`��
	void	sceneResult::RankRender( void )
	{
		//	�����N�`��
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			int		sx = rankImage[i].sx;
			int		sy = rankImage[i].sy;
			int		sw = rankImage[i].sw;
			int		sh = rankImage[i].sh;
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::NORMAL );
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::WAVE );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	�\�[�g
	void	sceneResult::Sort( void )
	{
		//	�ޔ�p
		SORT_INFO temp;

		for ( int i = 0; i < PLAYER_MAX; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( sortInfo[s].coin >	sortInfo[s - 1].coin )
				{
					//	�ꎞ�I�ɑޔ�
					temp = sortInfo[s - 1];

					//	����
					sortInfo[s - 1] = sortInfo[s];

					//	�ޔ����Ă����߂�
					sortInfo[s] = temp;
				}
			}
		}
	}

	//	���X�g�{�[�i�X�ݒ�
	void	sceneResult::SetLastBonus( void )
	{
		if ( Random::PercentageRandom( 0.3f ) )
		{
			//	���X�g�{�[�i�X��ݒ�
			lastBonus = gameManager->GetLastBonus();
		}
		else
		{
			//	�Ⴄ���ʂ��o��܂Ń��[�v
			while ( lastBonus == gameManager->GetLastBonus() )
			{
				lastBonus = Random::GetInt( 0, 4 );
			}
		}
	}

	//	���l�\���̂ɒl���Z�b�g����
	void	sceneResult::SetNumberInfo( NUMBER_INFO& number, int coin )
	{
		number.hundred = coin / 100 % 10;
		number.ten = coin / 10 % 10;
		number.one = coin % 10;
	}

	//	�\�[�g���������ʂ����̃f�[�^�ɔ��f
	void	sceneResult::SetRank( void )
	{
		//	�\�[�g�̌��ʂ𔽉f
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				if ( i == sortInfo[n].rank )
				{
					originInfo[i].rank = n;
				}
			}
		}
	}

	//	���l�ɂ��킹�Đ��l�\���̏���ݒ�,�P�O�O�ȏォ�Ŕz�u���ύX
	void	sceneResult::SetNumberImageInfo( const int& player, const int& coin )
	{
		//	�����m�F
		if ( coin >= 100 )		numberImageInfo[player].hundredRenderFlag = true;
		else							numberImageInfo[player].hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( number[player], coin );

		//	�e�ʉ摜�\���̏�����
		int		x, y, w, h, sx, sy, sw, sh;
		if ( numberImageInfo[player].hundredRenderFlag )
		{
			//	�P�O�̈ʐݒ�
			x = numberImageInfo[player].pos.x;
			y = numberImageInfo[player].pos.y;
			w = h = numberImageInfo[player].scale;
			sx = number[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�
			x = numberImageInfo[player].pos.x - static_cast<int>( numberImageInfo[player].ten.w / 1.5f );
			sx = number[player].hundred * 64;
			ImageInitialize( numberImageInfo[player].hundred, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numberImageInfo[player].pos.x + static_cast<int>( numberImageInfo[player].ten.w / 1.5f );
			sx = number[player].one * 64;
			ImageInitialize( numberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	�P�O�̈ʐݒ�
			w = h = numberImageInfo[player].scale;
			x = numberImageInfo[player].pos.x - w / 3;
			y = numberImageInfo[player].pos.y;
			sx = number[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numberImageInfo[player].pos.x + w / 3;
			sx = number[player].one * 64;
			ImageInitialize( numberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
	}

	//	���l���[���b�g( �I�������true�������� )
	bool	sceneResult::Roulette( void )
	{
		//	�S�l���m�肵�Ă���true��������
		if ( rouletteInfo.step > PLAYER_MAX )
		{
			return	true;
		}

		rouletteInfo.timer++;

		for ( int i = rouletteInfo.step; i < PLAYER_MAX; i++ )
		{
			//	�K���Ȑ��l������
			SetNumberImageInfo( i, Random::GetInt( 100, 999 ) );
		}

		//	���񂾂���]���Ԃ�����ƒ���
		if ( rouletteInfo.step == 0 )
		{
			if ( rouletteInfo.timer % 90 == 0 )
			{
				rouletteInfo.step++;
				rouletteInfo.timer = 0;
			}
		}
		else
		{
			if ( rouletteInfo.timer % 50 == 0 )
			{
				rouletteInfo.step++;
				rouletteInfo.timer = 0;
			}
		}

		//	���l����
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( i < rouletteInfo.step )
			{
				SetNumberImageInfo( i, originInfo[i].coin );
			}
		}

		return	false;
	}

	//	���ʂ����Ԃɕ\��
	bool	sceneResult::ViewRankInOrder( void )
	{	
		//	�S�l���m�肵�Ă���true��������
		if ( viewRankInOrder.step == -1 )
		{
			return	true;
		}

		viewRankInOrder.timer++;

		if ( viewRankInOrder.timer % 30 == 0 )
		{
			//	�����N�`��ݒ�
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				if ( originInfo[i].rank == viewRankInOrder.step )
				{
					rankImage[i].renderflag = true;
					SetWave( rankImage[i], 1.5f );
				}
			}

			viewRankInOrder.step--;
			viewRankInOrder.timer = 0;
		}
		

		return	false;
	}

	//	���ʂ̔g��̍X�V�ƏI���`�F�b�N
	bool	sceneResult::RankWave( void )
	{
		bool isEnd[4];

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			isEnd[i] = false;
			isEnd[i] = WaveUpdate( rankImage[i] );
		}

		if ( isEnd[0] && isEnd[1] && isEnd[2] && isEnd[3] )
		{
			return	true;
		}

		return	false;
	}

	//	�V�[���ړ��t���O����������I�𒆂̍��ڂɍ������V�[���Ɉړ�����i ���̊֐��͐�΂ɍX�V�̍Ō�ɌĂ� �j
	void	sceneResult::MoveScene( void )
	{
		if ( !changeScene )	return;

		switch ( menuInfo.select )
		{
		case MENU::RESTART:
			//	�Q�[����񏉊���
			gameManager->RetryInitialize();
			MainFrame->ChangeScene( new sceneMain() );
			return;
			break;

		case MENU::MOVE_MENU:
			MainFrame->ChangeScene( new sceneMenu() );
			return;
			break;

		case MENU::MOVE_TITLE:
			MainFrame->ChangeScene( new sceneTitle() );
			return;
			break;
		}
	}