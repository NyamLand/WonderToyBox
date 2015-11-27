
#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"sceneTitle.h"
#include	"UI.h"

//****************************************************************************************
//
//	UI�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------

	namespace
	{
		namespace SCREEN_SIZE
		{
			const int WIDTH = iexSystem::ScreenWidth;
			const int HEIGHT = iexSystem::ScreenHeight;
		}

		//    ����
		namespace FACE_INFO
		{
			//�@�u�y�E��E���E�{�v
			enum
			{
				Normal,
				Good,
				Sad,
				Angry
			};
		}

		//	�^�C�g���e�L�X�g���W���


		//	�^�C�g���e�L�X�g�ǂݍ��ݍ��W���
		namespace TITLE_TEXT_SRCPOS_INFO
		{
			enum
			{
				MENU_SRC_POS_X,
				MENU_SRC_POS_Y,
			};
		}

		

		

		//�@�ǂ񂯂��o�p
		namespace DD_TIMING
		{
			const int WAIT_MAX = 5 * SECOND + 30;
			const int DB_LOCK = 5 * SECOND;
			const int FACE_START = 4 * SECOND + 40;
			const int FACE_LOCK = 2 * SECOND + 30;
			const int P_START = 1 * SECOND + 50;
			const int P_LOCK = 20;
		}
	}

	//	����
	UI*	ui = nullptr;

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	UI::UI( void )
	{

	}

	//	�f�X�g���N�^
	UI::~UI( void )
	{
		Release();
	}

	//	������
	bool	UI::Initialize( int scene )
	{
		this->scene = scene;

		//	���[�h�ʏ�����
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleInitialize();
			break;

		case UI_MODE::MAIN:
			MainInitialize();
			break;

		case UI_MODE::RESULT:
			ResultInitialize();
			break;
		}

		return	true;
	}

	//	���
	void	UI::Release( void )
	{
		switch ( scene )
		{
		case UI_MODE::TITLE:
			TitleRelease();
			break;

		case UI_MODE::MAIN:
			MainRelease();
			break;

		case	UI_MODE::RESULT:
			ResultRelease();
			break;
		}
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	UI::Update( const int& mode )
	{
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleUpdate( mode );
			break;

		case UI_MODE::MAIN:
			MainUpdate( mode );
			break;

		case UI_MODE::RESULT:
			ResultUpdate( mode );
			break;
		}
	}

	//	�`��
	void	UI::Render( const int& mode )
	{
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleRender( mode );
			break;

		case UI_MODE::MAIN:
			MainRender( mode );
			break;

		case UI_MODE::RESULT:
			ResultRender( mode );
			break;
		}
		DrawDebug();
	}

	//�@�f�o�b�O�\��
	void	UI::DrawDebug()
	{
		if (debug)
		{
			
		}
	}

//------------------------------------------------------------------------------
//	�e�V�[��������
//------------------------------------------------------------------------------

	//	�^�C�g���p������
	void	UI::TitleInitialize( void )
	{
		//	�摜������
		titleInfo.textImage.obj = new iex2DObj( "DATA/UI/menu/menu-int.png" );

		//	�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 1.01f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.6f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.14f );
		ImageInitialize( titleInfo.textImage, x, y, w, h, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_X, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_Y, 1024, 128 );
		titleInfo.textImage.renderflag = false;
		
		//	�ϐ�������
		titleInfo.mode = 0;
		titleInfo.step = 0;
		titleInfo.param = 0.0f;
		titleInfo.t = 0.0f;
		titleInfo.moveState = false;
		titleInfo.savePos = 0;
	}

	//	���C���p������
	void	UI::MainInitialize( void )
	{
		timer.obj = new iex2DObj("DATA/UI/timer.png");
		coinbar = new iex2DObj("DATA/BG/coin_gage.png");
		gauge.obj = coinbar;
		backgauge.obj = coinbar;
		frame.obj = coinbar;
		face = new iex2DObj("DATA/UI/chara_emotion.png");
		faceImage.obj = face;
		countImage.obj = new iex2DObj("DATA/UI/bfUI.png");
		alertImage.obj = new iex2DObj("DATA/UI/alert.png");
		playerNumber = new iex2DObj( "DATA/UI/cursor.png" );

		//	���ʕϐ������� 
		changeflag = false;
		for (int i = 0; i < PLAYER_MAX; i++)
			charatype[i] = gameManager->GetCharacterType(i);

		//	�eUI��񏉊���
		CoinBarInitialize();
		TimerInitialize();
		StartAndTimeUpInitialize();
		NewsBarInitialize();
		DonketsuDirectionInitialize();
		AlertInitialize();
		PlayerNumberInitialize();
	}

	//	���U���g�p������
	void	UI::ResultInitialize( void )
	{

	}

//------------------------------------------------------------------------------
//	�e�V�[�����
//------------------------------------------------------------------------------

	//	�^�C�g���p���
	void	UI::TitleRelease( void )
	{
		SafeDelete( titleInfo.textImage.obj );
	}

	//	���C���p���
	void	UI::MainRelease( void )
	{
		SafeDelete( timer.obj );
		SafeDelete( coinbar );
		SafeDelete( ddInfo.face.obj );
		SafeDelete( ddInfo.DB.obj );
		SafeDelete( ddInfo.P.obj );
		SafeDelete( face );
		SafeDelete( countImage.obj );
		SafeDelete( alertImage.obj );
		SafeDelete( playerNumber );
	}

	//	���U���g�p���
	void	UI::ResultRelease( void )
	{

	}

//------------------------------------------------------------------------------
//	�e�V�[���X�V
//------------------------------------------------------------------------------

	//	�^�C�g���X�V
	void	UI::TitleUpdate( int mode )
	{
		//	�摜�ݒ�
		SetImageSrcPos( mode );

		bool	isEnd = false;
		switch ( titleInfo.step ) 
		{
		case 0:
			//	�ǂ����������Ă���
			isEnd = FlyingIn();
			
			//	�������I���Ƃ���
			if ( isEnd )
			{
				titleInfo.step++;
			}
			break;

		case 1:
			//	�ӂ�ӂ킳����
			titleInfo.textImage.y = airPlaneInfo.IN_END_POS_Y + static_cast<int>( 10.0f * sinf( titleInfo.t ) );
			titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y  );
			titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * -1.0f ) * cosf( titleInfo.t );
			titleInfo.t += D3DX_PI / 180.0f * 1.0f;

			if ( mode == TITLE_MODE::MOVE_MAIN )
			{
				titleInfo.textImage.t = 0.0f;
				titleInfo.param = 0.0f;
				titleInfo.savePos = titleInfo.textImage.y;
				titleInfo.step++;
			}
			break;

		case 2:
			FlyingOut( titleInfo.savePos );
			break;
		}
	}
	
	//	���C���X�V
	void	UI::MainUpdate( int mode )
	{
		PlayerNumberUpdate();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			NewsBarUpdate();
			CoinBarUpdate();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuDirectionUpdate();
			break;

		case GAME_MODE::CLIMAX:
			TimerUpdate();
			NewsBarUpdate();
			CoinBarUpdate();
			LastProduction();

			//�@�ǂ񂯂̊�́u�{�v�ɁB�i���ԊǗ����Ă�Ƃ��Ł��̏��������������ǂ��ꂩ��ύX���肻���₩��Ƃ肠���������ɏ����Ă鋖���ăj��������j
			state_type[gameManager->GetWorst()] = FACE_INFO::Angry;
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}

		//	�x������
		if (alertInfo.flag)	AlertUpdate();
	}

	//	���U���g�X�V
	void	UI::ResultUpdate( int mode )
	{

	}

//------------------------------------------------------------------------------
//	�e�V�[���`��
//------------------------------------------------------------------------------

	//	�^�C�g���`��
	void	UI::TitleRender( int mode )
	{
		RenderImage( titleInfo.textImage, titleInfo.textImage.sx, titleInfo.textImage.sy, 1024, 128, IMAGE_MODE::ADOPTPARAM );
	}

	//	���C���`��
	void	UI::MainRender( int mode )
	{
		PlayerNumberRender();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			DonketsuDirectionRender();
			break;

		case GAME_MODE::CLIMAX:
			//TimerRender();
			LastProductionRender();
			NewsBarRender();
			CoinBarRender();

			break;

		case GAME_MODE::TIMEUP:
			FinishRender();
			break;
		}

		if (alertInfo.flag)	AlertRender();
	}

	//	���U���g�`��
	void	UI::ResultRender( int mode )
	{

	}

//------------------------------------------------------------------------------
//	�^�C�g�����쏉����
//------------------------------------------------------------------------------

	//	��s�@������
	void	UI::AirPlaneInitialize( void )
	{
		airPlaneInfo.IN_START_POS_X = static_cast<int>(iexSystem::ScreenWidth * 1.5f);
		airPlaneInfo.IN_START_POS_Y = static_cast<int>(iexSystem::ScreenHeight * -0.13f);
		airPlaneInfo.IN_END_POS_X = static_cast<int>(iexSystem::ScreenWidth / 2);
		airPlaneInfo.IN_END_POS_Y = static_cast<int>(iexSystem::ScreenHeight * 0.2f);
		airPlaneInfo.OUT_START_POS_X = static_cast<int>(iexSystem::ScreenWidth / 2);
		airPlaneInfo.OUT_END_POS_X = static_cast<int>(iexSystem::ScreenWidth * -0.55f);
		airPlaneInfo.OUT_END_POS_Y = static_cast<int>(iexSystem::ScreenHeight * 0);
		airPlaneInfo.ROLL_POINT_ADJUST_X = 200;
	}

	//	���ł���
	bool	UI::FlyingIn( void )
	{
		if ( titleInfo.textImage.t >= 1.0f )	return	true;

		//	�p�����[�^���Z
		titleInfo.textImage.t += 0.01f;
		if ( titleInfo.textImage.t >= 1.0f )	titleInfo.textImage.t = 1.0f;

		//	�p�����[�^����
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, titleInfo.textImage.t );

		//	��ړ�
		Lerp( titleInfo.textImage.y, airPlaneInfo.IN_START_POS_Y, airPlaneInfo.IN_END_POS_Y, titleInfo.param );

		//	�p�����[�^����
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.textImage.t );

		//	���ړ�
		Lerp(titleInfo.textImage.x, airPlaneInfo.IN_START_POS_X, airPlaneInfo.IN_END_POS_X, titleInfo.param);

		//	��]
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
		titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * -1.0f ) * sinf( D3DX_PI  / 2 * titleInfo.textImage.t );

		if ( titleInfo.textImage.t >= 1.0f )	return	true;
		return	false;
	}

	//	���ł���
	bool	UI::FlyingOut( int startPos )
	{
		if ( titleInfo.textImage.t >= 1.0f )	return	true;

		//	�p�����[�^���Z
		titleInfo.textImage.t += 0.01f;
		if ( titleInfo.textImage.t >= 1.0f )	titleInfo.textImage.t = 1.0f;

		//	�p�����[�^����
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, titleInfo.textImage.t );

		//	��ړ�
		Lerp( titleInfo.textImage.y, startPos, ( int )airPlaneInfo.OUT_END_POS_Y, titleInfo.param );

		//	�p�����[�^����
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv3, titleInfo.textImage.t );

		//	���ړ�
		Lerp( titleInfo.textImage.x, airPlaneInfo.OUT_START_POS_X, airPlaneInfo.OUT_END_POS_X, titleInfo.param );

		//	��]
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
		titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * - 1.0f ) * sinf( D3DX_PI / 2 * titleInfo.textImage.t );

		if ( titleInfo.textImage.t >= 1.0f )	return	true;
		return	false;
	}

//------------------------------------------------------------------------------
//	���C�����쏉����
//------------------------------------------------------------------------------

	//	�R�C���o�[������
	void	UI::CoinBarInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.93f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.77f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.11f );
		ImageInitialize( frame, x, y, w, h, 0, 32 * 5, 512, 64);
		ImageInitialize( backgauge, frame.x, frame.y, frame.w - (frame.w / 10), frame.h - (frame.h / 2), 0, 32 * 4, 480, 32);
		ImageInitialize( gauge, frame.x, frame.y, 0, backgauge.h, 0, 32, 0, 32);
		w = static_cast<int>( iexSystem::ScreenWidth * 0.025f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.04f );
		ImageInitialize(faceImage, 0, frame.y - (frame.h / 2), w, h, 256, 256, 256, 256);

		for (int i = 0; i < NUM_BAR; i++)
		{
			bar_x[i] = frame.x;
			bar_sx[i] = backgauge.sx;
			state_x[i] = 0;
		}
	}

	//	�^�C�}�[������
	void	UI::TimerInitialize( void )
	{

		//	�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.4f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.05f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.09f );
		ImageInitialize( timer, x, y, w, h, 64, 0, 64, 64 );
		for ( int i = 0; i < 2; i++ )
		{
			minute[i] = 0;
		}
	}

	//	�j���[�X�o�[������
	void	UI::NewsBarInitialize( void )
	{
		//	�j���[�X�o�[������
		newsbar.renderflag = false;
		newsbar.left = iexSystem::ScreenWidth;
		newsbar.top = 0;
		newsbar.right = iexSystem::ScreenWidth;
		newsbar.bottom = static_cast<int>( iexSystem::ScreenHeight * ( 0.07f ) );
		newsbar.text = GameInfo::NewsText[ gameManager->GetLastBonus() ];
		newsbar.alpha = 0.5f;
		newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
		newsbar.step = 0;
		newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * ( 1.17f ) );
	}

	//	�J�E���g�_�E���E�X�^�[�g�E�I�����o
	void	UI::StartAndTimeUpInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.27f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		ImageInitialize( countImage, x, y, w, h, 0, 0, 256, 256 );
		countImage.renderflag = true;
		count = 0;
		waitTimer = 0;
	}

	//	�ǂ񂯂��o������
	void	UI::DonketsuDirectionInitialize( void )
	{
		//�@�\���̏�����
		
		//�@�烋�[���b�g�֘A
		ddInfo.face.obj = new iex2DObj("DATA/UI/chara_emotion.png");
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		ImageInitialize(ddInfo.face, x, y, 0, 0, FACE_INFO::Normal * 256, 0 * 256, 256, 256);
		ddInfo.f = 0;
		ddInfo.roulette = 0;
		ddInfo.face_step = -1;

		//�@DonketsuBoooooooooooost!!!
		ddInfo.DB.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		x = static_cast<int>( iexSystem::ScreenWidth * 0.94f );
		y = static_cast<int>( iexSystem::ScreenHeight * -0.14f );
		ImageInitialize(ddInfo.DB, x, y, 0, 0, 0, 0, 512, 256);
		ddInfo.DB.angle = 0;
		ddInfo.DB_step = -1;

		//�@�HP�֘A
		x = static_cast<int>( iexSystem::ScreenWidth * 0.78f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.76f );
		ddInfo.P.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		ImageInitialize(ddInfo.P, x, y, 0, 0, 0, 256, 128, 128);
		ddInfo.P.renderflag = false;
		ddInfo.P_step = -1;
	}

	//	�x�����o������
	void	UI::AlertInitialize( void )
	{
		alertInfo.flag = false;
		alertInfo.alpha = 0.0f;
		alertInfo.timer = 0;
		alertInfo.param = 0.0f;
		alertInfo.type = 0;

		//	�摜�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.15f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.27f );
		ImageInitialize( alertImage, x, y, w, h, 0, 0, 256, 256 );
		alertImage.renderflag = true;
	}

	//	HurryUp���o������
	void	UI::HurryUpInitialize( void )
	{
		hurryInfo.alpha = 0.0f;
		hurryInfo.flag = false;
		hurryInfo.param = 0.0f;
		hurryInfo.timer = 0;
	}

	//	�v���C���[�ԍ��摜������
	void	UI::PlayerNumberInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			pNumImage[i].obj = playerNumber;
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.04f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.07f );
			ImageInitialize( pNumImage[i], 0, 0, w, h, ( i % 2 ) * 128, ( i / 2 ) * 128, 128, 128 );
		}
	}
	
//------------------------------------------------------------------------------
//	���C������X�V
//------------------------------------------------------------------------------

	//	�^�C�}�[�֘A����
	void	UI::TimerUpdate( void )
	{
		this->time = gameManager->GetTimer();
		//��
		second = this->time / MINUTE % 10;
		//�b�񌅖�
		minute[0] = ( this->time / SECOND ) % 60 / 10 % 10;
		//�b�ꌅ��
		minute[1] = this->time / SECOND % 10;
	}

	//	�j���[�X�o�[�֘A����
	void	UI::NewsBarUpdate( void )
	{
		if ( !gameManager->GetNewsFlag() )	return;

		switch ( newsbar.step )
		{
		case 0:
			//	�o�[�o��
			newsbar.left -= static_cast<int>( iexSystem::ScreenWidth * 0.02f );
			if ( newsbar.left <= 0 )
			{
				newsbar.left = 0;
				newsbar.step++;
			}
			break;

		case 1:
			//	�e�L�X�g�o��
			newsbar.textleft--;
			if ( newsbar.textleft <= static_cast<int>( iexSystem::ScreenWidth * -0.23f ) )
			{
				newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * 1.17f );
				newsbar.step++;
			}
			break;

		case 2:
			//	�o�[�ޔ�
			newsbar.right -= static_cast<int>( iexSystem::ScreenWidth * 0.02f );
			if ( newsbar.right <= 0 )
			{
				newsbar.right = 0;
				newsbar.step++;
			}
			break;

		case 3:
			//	������
			gameManager->SetNewsFlag( false );
			newsbar.left = iexSystem::ScreenWidth;
			newsbar.right = iexSystem::ScreenWidth;
			newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * 1.17f );
			newsbar.step = 0;
			break;
		}
	}

	//	�R�C���o�[�֘A����
	void	UI::CoinBarUpdate( void )
	{
		//	�o�[����
		BarControl();

		//	�瓮��
		StateImageControl();
	}

	//	�J�E���g�_�E���E�X�^�[�g���o
	void	UI::StartUpdate( void )
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
				countImage.sx = 256;
				countImage.sy = 0;
				break;

			case 2:
				countImage.sx = 0;
				countImage.sy = 256;
				break;

			case 3:
				countImage.sx = 512;
				countImage.sy = 0;
				countImage.sw = 512;
				countImage.sh = 512;
				countImage.w = static_cast<int>( iexSystem::ScreenWidth * 0.59f );
				countImage.h = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
				SetWave( countImage, 1.5f );
				break;

			case 4:
				waitTimer = 2 * SECOND;
				
				//	�摜�ǂݍ��݈ʒu�E�T�C�Y�ݒ�			
				countImage.w = static_cast<int>( iexSystem::ScreenWidth * 0.47f );
				countImage.h = static_cast<int>( iexSystem::ScreenHeight * 0.51f );
				countImage.sx = 0;
				countImage.sy = 512;
				countImage.sw = 1024;
				countImage.sh = 512;

				//	���C���Q�[����
				changeflag = true;
				break;
			}

		}

		WaveUpdate( countImage );
	}

	//	�^�C���A�b�v���o
	void	UI::FinishUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )	changeflag = true;
	}

	//	�ǂ񂯂��艉�o
	void	UI::DonketsuDirectionUpdate( void )
	{
		/*
			�E�u�ǂ񂯂́`�H�v���ŏ��Ƀo���b!
			�E�S�l�̊���Q�񂸂Ղ�Ղ���ƕ��D���c��ނ悤�Ȋ����ɏo���B������ƊԂ������ĂX��ڂŌ���
			�@���莞�Ɋ�̃o�b�N�ɋ����G�t�F�N�g�A�w�i��g�F�n�ɁB
			�E�����Ɂ��o�I
		*/

		//�@���o�p���ԍX�V
		static int wait = DD_TIMING::WAIT_MAX;
		if (wait <= 0)
		{
			wait = 5 * SECOND + 30;
			changeflag = true;
		}

		//�@�uDonketsuBoooooooooooost!!!�v
		DB_Direction(wait);
		
		//�@�烋�[���b�g
		FaceRoulette(wait);

		//�@�u�HP�v
		P_Direction(wait);

		//�@�X�V
		wait--;
	}

	//	�x�����o
	void	UI::AlertUpdate( void )
	{
		alertInfo.param += D3DX_PI / 30.0f;
		alertInfo.alpha = 0.1f + 0.1f * sinf( alertInfo.param );

		alertInfo.timer++;
		if ( alertInfo.timer % 15 == 0 )	alertImage.renderflag = !alertImage.renderflag;

		//	��b���ŏI��
		if ( alertInfo.timer >= 120 )
		{
			alertInfo.flag = false;
			alertInfo.alpha = 0.0f;
		}
	}

	//	HurryUp���o
	void	UI::HurryUpdate( void )
	{
		hurryInfo.param += D3DX_PI / 30.0f;
		hurryInfo.alpha = 0.1f + 0.1f * sinf( hurryInfo.param );

		hurryInfo.timer++;
		if ( hurryInfo.timer % 15 == 0 )	alertImage.renderflag = !alertImage.renderflag;
	}

	//	���Ԍx�����o
	void	UI::LastProduction( void )
	{
		FlashingUpdate(timer, 0.5f);
	}

	//	�v���C���[�ԍ��ʒu����
	void	UI::PlayerNumberUpdate( void )
	{
		//	�ϐ�������
		Vector3 imagePos = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	up = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�摜��\������ꏊ��ݒ�
			up = characterManager->GetUp( i );
			p_pos = characterManager->GetPos( i );
			imagePos = p_pos + up * 7.0f;

			//	�N���C�A���g���W�ɕϊ�
			WorldToClient( imagePos, out, matView * matProjection );

			//	�\���̂ɏ��ݒ�
			pNumImage[i].x = static_cast<int>( out.x );
			pNumImage[i].y = static_cast<int>( out.y );
		}
	}

//------------------------------------------------------------------------------
//	���C���`��
//------------------------------------------------------------------------------

	//	�R�C���o�[�`��
	void	UI::CoinBarRender( void )
	{
		//�t���[��
		RenderImage(frame, frame.sx, frame.sy, frame.sw, frame.sh, IMAGE_MODE::NORMAL);

		//�D�F�̃o�[
		RenderImage(backgauge, backgauge.sx, backgauge.sy, backgauge.sw, backgauge.sh, IMAGE_MODE::NORMAL);

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			//�F�̃o�[
			gauge.w = bar_sx[i];	gauge.sw = bar_sx[i];

			//	�i����ʒu - �Q�[�W���̔����j�Œ��S�A�Q�[�W���E�����֑��₷����
			RenderImage( gauge, gauge.sx, gauge.sy * i, gauge.sw, gauge.sh, IMAGE_MODE::NORMAL, static_cast<int>( bar_x[i] - (backgauge.w * 0.5f) + (gauge.sw * 0.5f ) ), gauge.y );
																					
			//��
			RenderImage(faceImage, faceImage.sx * state_type[i], faceImage.sy * charatype[i], faceImage.sw, faceImage.sh, IMAGE_MODE::NORMAL, state_x[i] - (backgauge.w / 2), faceImage.y);
		}
	}

	//	�j���[�X�`��
	void	UI::NewsBarRender( void )
	{
		if( !gameManager->GetNewsFlag() )	return;
		
		iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
		IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + static_cast<int>( iexSystem::ScreenHeight * 0.013f ), 500, 200, 0xFFFFFFFF );
	}

	//	�^�C�}�[�`��
	void	UI::TimerRender( void )
	{
		
		RenderImage( timer, timer.sx * second		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 0, timer.y );
		RenderImage( timer, timer.sx * 10			, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 1, timer.y  );
		RenderImage( timer, timer.sx * minute[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 2, timer.y  );
		RenderImage( timer, timer.sx * minute[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 3, timer.y  );

	}

	//	�J�E���g�_�E���E�X�^�[�g���o
	void	UI::StartRender( void )
	{
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::NORMAL );
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::WAVE );
	}

	//	�^�C���A�b�v���o
	void	UI::FinishRender( void )
	{
		RenderImage(countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::NORMAL );
	}

	//	�ǂ񂯂��o
	void	UI::DonketsuDirectionRender( void )
	{
		//�@�O���[�o�b�N
		DWORD	color = 0xD0000000;
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );

		//�@�烋�[���b�g
		RenderImage(ddInfo.face, FACE_INFO::Normal * 256, charatype[ddInfo.f] * 256, 256, 256, IMAGE_MODE::NORMAL);
		
		//�@DonketsuBoooooooooooost!!!
		RenderImage(ddInfo.DB, 0, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );
	
		//�@�HP
		RenderImage(ddInfo.P, gameManager->GetWorst()*128, 256, 128, 128, IMAGE_MODE::NORMAL);

		//�@�f�o�b�O
		if (debug)
		{
			char	str[256];
			int		worst = gameManager->GetWorst();
			wsprintf(str, "�r���� p%d", worst + 1);
			DrawString(str, 200, 70);
		}
	}

	//	�x���`��
	void	UI::AlertRender( void )
	{
		//	�ԃt�B���^�[�`��
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, GetColor( 1.0f, 0.0f, 0.0f, alertInfo.alpha ) );

		//	�x���摜�`��
		RenderImage( alertImage, 0, 0, 256, 256, IMAGE_MODE::NORMAL );
	}

	//	���Ԍx���`��
	void	UI::LastProductionRender( void )
	{
		//	�^�C�}�[�����F��Ԃ�
		timer.sy = 64;

		RenderImage(timer, timer.sx * second	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 0, timer.y);
		RenderImage(timer, timer.sx * 10		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 1, timer.y);
		RenderImage(timer, timer.sx * minute[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 2, timer.y);
		RenderImage(timer, timer.sx * minute[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 3, timer.y);

		//	�^�C�}�[�����F�𔒂�
		timer.sy = 0;

	}

	//	�v���C���[�ԍ��`��
	void	UI::PlayerNumberRender( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			RenderImage( pNumImage[i], pNumImage[i].sx, pNumImage[i].sy, pNumImage[i].sw, pNumImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	�o�[����
	void	UI::BarControl( void )
	{
		//static const int MAX_COIN = 201;

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];

		int num_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = gameManager->GetCoinNum(i);
			bar_sx[i] = backgauge.w * num_coin[i] / gameManager->GetCoinMax();
		}
	}

	//	�o�[����Q
	void		UI::BarControl2( void )
	{
		//	�v���C���[���o�^
		FOR( 0, PLAYER_MAX )
		{
			coinBarInfo.coinNum[value] = gameManager->GetCoinNum( value );
			coinBarInfo.param[value] = 1.0f;
		}

		float		sub[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float		average = 0;
		static const int MAX_COIN = 201;

		//	�R�C�����𒲂ׂ�
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	�R�C�����Ŋ���������
				if ( i == n ) continue;

				if ( coinBarInfo.coinNum[n] == 0 )	sub[n] = 1.0f;
				else sub[n] = ( float )( coinBarInfo.coinNum[i] / coinBarInfo.coinNum[n] );
			}

			//	���ς����߂�
			average = ( sub[0] + sub[1] + sub[2] + sub[3] ) / PLAYER_MAX;
			average /= PLAYER_MAX - 1;
			coinBarInfo.param[i] = average;
		}

		int		originWidth = backgauge.w / PLAYER_MAX;
		FOR( 0, PLAYER_MAX )
		{
			bar_sx[value] = originWidth * ( int )coinBarInfo.param[value];
		}

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];
	}

	//	�o�[�`��
	void		UI::BarRender( void )
	{
	}

	//	�瓮��
	void	UI::StateImageControl( void )
	{
		//�摜�̕`��ꏊ = �e�F�̐擪�@+�@�e�F�̒��S�@-�@�摜�T�C�Y�̔���
		state_x[0] = (bar_x[0] + (bar_x[1] - bar_x[0]) / 2);
		state_x[1] = (bar_x[1] + (bar_x[2] - bar_x[1]) / 2);
		state_x[2] = (bar_x[2] + (bar_x[3] - bar_x[2]) / 2);
		state_x[3] = (bar_x[3] + (bar_x[3] + bar_sx[3] - bar_x[3]) / 2);


		int num_coin[4], temp_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = temp_coin[i] = gameManager->GetCoinNum(i);
		}

		for (int i = 0, temp; i < 4 - 1; i++)
		{
			for (int j = 4 - 1; j > i; j--)
			{
				if (temp_coin[j - 1] > temp_coin[j])
				{
					temp = temp_coin[j];
					temp_coin[j] = temp_coin[j - 1];
					temp_coin[j - 1] = temp;
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (num_coin[i] == temp_coin[j])
				{
					switch (j)
					{
					case 0:
						state_type[i] = FACE_INFO::Sad;
						break;

					case 1:
					case 2:
						state_type[i] = FACE_INFO::Normal;
						break;

					case 3:
						state_type[i] = FACE_INFO::Good;
						break;
					}
				}
			}
		}
	}

	//�@DonketsuBoooooooooooost!!!�̉��o
	void	UI::DB_Direction( int wait )
	{
		const int POS_X = iexSystem::ScreenWidth / 5;		//�@�Œ�ʒu
		const int POS_Y = iexSystem::ScreenHeight / 6;		//�@�Œ�ʒu
		const int TRANS_X = (POS_X - static_cast<int>( iexSystem::ScreenWidth * 0.94f ) ) / 30;	//�@TPF
		const int TRANS_Y = (POS_Y - static_cast<int>( iexSystem::ScreenHeight * -0.14f ) ) / 30;	//�@TPF
		const int SIZE_X = static_cast<int>( iexSystem::ScreenWidth * 0.47f );
		const int SIZE_Y = static_cast<int>( iexSystem::ScreenHeight * 0.41f );
		const int SCALING_X = SIZE_X / 30;
		const int SCALING_Y = SIZE_Y / 30;
		const float ROT = 30 * 0.0175f;	//�@��]��
		
		//�@�^�C�~���O�ɂ��ݒ�
		switch (wait)
		{
			//�@���o�X�^�[�g
		case DD_TIMING::WAIT_MAX:
			ddInfo.DB_step = 0;
			break;

			//�@�Œ�
		case DD_TIMING::DB_LOCK:
			ddInfo.DB_step = 1;
			break;
		}

		//�@���o��
		switch (ddInfo.DB_step)
		{
			//�@�ړ��E��]
		case 0:
			ddInfo.DB.x += TRANS_X;
			ddInfo.DB.y += TRANS_Y;
			ddInfo.DB.p.x = ddInfo.DB.x;
			ddInfo.DB.p.y = ddInfo.DB.y;
			ddInfo.DB.w += SCALING_X;
			ddInfo.DB.h += SCALING_Y;
			ddInfo.DB.angle += ROT;
			break;

			//�@�Œ�
		case 1:
			ddInfo.DB.x = POS_X;
			ddInfo.DB.y = POS_Y;
			ddInfo.DB.w = SIZE_X;
			ddInfo.DB.h = SIZE_Y;
			ddInfo.DB.angle = D3DXToRadian(-30.0f);
			break;
		}
	}

	//�@�ǂ񂯂��o�̊�
	void	UI::FaceRoulette( int face_wait )
	{
		const int INTERVAL = 10;
		const int SPF = 320 / INTERVAL;	// SCALE PER FRAME(�P�t���[���Ɋg�傷�镪)

		switch (face_wait)
		{
			//�@�C���g���@���@�P�o
		case DD_TIMING::FACE_LOCK + INTERVAL * 13:
		case DD_TIMING::FACE_LOCK + INTERVAL * 9:
			ddInfo.face_step = 0;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(0);
			break;

			//�@�P�o�@���@�Q�o
		case DD_TIMING::FACE_LOCK + INTERVAL * 12:
		case DD_TIMING::FACE_LOCK + INTERVAL * 8:
			ddInfo.face_step = 1;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(1);
			break;

			//�@�Q�o�@���@�R�o
		case DD_TIMING::FACE_LOCK + INTERVAL * 11:
		case DD_TIMING::FACE_LOCK + INTERVAL * 7:
			ddInfo.face_step = 2;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(2);
			break;

			//�@�R�o�@���@�S�o
		case DD_TIMING::FACE_LOCK + INTERVAL * 10:
		case DD_TIMING::FACE_LOCK + INTERVAL * 6:
			ddInfo.face_step = 3;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(3);
			break;

			//�@�S�o�@���@���ڂݎn�߂�
		case DD_TIMING::FACE_LOCK + INTERVAL * 5:
			ddInfo.face_step = 4;
			ddInfo.face.w = static_cast<int>( iexSystem::ScreenWidth * 0.25f );
			ddInfo.face.h = static_cast<int>( iexSystem::ScreenHeight * 0.44f );
			break;

			//�@���ڂ݂���@���@�҂�
		case DD_TIMING::FACE_LOCK + INTERVAL * 4:
			ddInfo.face_step = 5;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = -1;
			break;


			//�@�҂��@���@�r��
		case DD_TIMING::FACE_LOCK + INTERVAL * 1:
			ddInfo.face_step = 6;
			ddInfo.f = gameManager->GetWorst();
			break;

			//�@�r���@���@���̂܂ܕ��u
		case DD_TIMING::FACE_LOCK:
			ddInfo.face_step = 7;
			break;
		}

		//�@���o
		switch (ddInfo.face_step)
		{
			//�@�P�`�S�o�g��
		case 0:
		case 1:
		case 2:
		case 3:
			ddInfo.face.renderflag = true;
			ddInfo.face.w += SPF;
			ddInfo.face.h += SPF;
			break;

			//�@�W�l�ڂ̏k��
		case 4:
			ddInfo.face.renderflag = true;
			ddInfo.face.w -= SPF;
			ddInfo.face.h -= SPF;
			break;
		
			//�@�r���g��i�傫�߂Ɂj
		case 6:
			ddInfo.face.renderflag = true;
			ddInfo.face.w += static_cast<int>( iexSystem::ScreenWidth * 0.39f ) / INTERVAL;
			ddInfo.face.h += static_cast<int>( iexSystem::ScreenHeight * 0.69f ) / INTERVAL;
			break;

			//�@���u
		case 7:
			ddInfo.face.renderflag = true;
			ddInfo.face.w = static_cast<int>( iexSystem::ScreenWidth * 0.39f );
			ddInfo.face.h = static_cast<int>( iexSystem::ScreenHeight * 0.69f );
			break;

		default:
			ddInfo.face.renderflag = false;
			break;
		}
	}
	
	//�@�u�H�o�v�̉��o
	void	UI::P_Direction( int wait )
	{
		const int INTERVAL = 5;
		const int SIZE_X = static_cast<int>( iexSystem::ScreenWidth * 0.31f );
		const int SIZE_Y = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		const int SCALING = 200;
		const int SPF = SCALING / INTERVAL;
		
		//�@�^�C�~���O�ɂ��ݒ�
		switch (wait)
		{
			//�@�o��
		case DD_TIMING::P_START:
			ddInfo.P.renderflag = true;
			ddInfo.P_step = 0;
			break;
		case DD_TIMING::P_START - 5:
			ddInfo.P_step = 3;
			break;

			//�@�g��X�^�[�g
		case DD_TIMING::P_LOCK + INTERVAL * 6:
		case DD_TIMING::P_LOCK + INTERVAL * 4:
		case DD_TIMING::P_LOCK + INTERVAL * 2:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			ddInfo.P_step = 1;
			break;

			//�@�k���X�^�[�g
		case DD_TIMING::P_LOCK + INTERVAL * 5:
		case DD_TIMING::P_LOCK + INTERVAL * 3:
		case DD_TIMING::P_LOCK + INTERVAL * 1:
			ddInfo.P.w = SIZE_X + SCALING;
			ddInfo.P.h = SIZE_Y + SCALING;
			ddInfo.P_step = 2;
			break;
	
			//�@�Œ�
		case DD_TIMING::P_LOCK:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			ddInfo.P_step = 3;
			break; 
		}

		//�@���o��
		switch (ddInfo.P_step)
		{
			//�@�o��
		case 0:
			ddInfo.P.w += SIZE_X / 10;
			ddInfo.P.h += SIZE_Y / 10;
			break;

			//�@�g��
		case 1:
			ddInfo.P.w += SPF;
			ddInfo.P.h += SPF;
			break;

			//�@�k��
		case 2:
			ddInfo.P.w -= SPF;
			ddInfo.P.h -= SPF;
			break;

			//�@�Œ�
		case 3:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			break;
		}
	}

//------------------------------------------------------------------------------
//	���ݒ�E�擾
//------------------------------------------------------------------------------
	
	//	���[�h�ύX�t���O�ݒ�
	void	UI::SetChangeFlag( const bool& flag )
	{
		this->changeflag = flag;
	}

	//	�x���t���O�ݒ�
	void	UI::SetAlertInfo( bool flag , int type )
	{
		alertInfo.flag = flag;
		alertInfo.type = type;
	}

	//	HurryUp�t���O�ݒ�
	void	UI::SetHurryFlag( bool flag )
	{
		hurryInfo.flag = flag;
	}

	//	��ѓ���ݒ�
	void	UI::SetFlyingIn( int type )
	{

	}

	//	���[�h�ʓǂݍ��݈ʒu�ݒ�
	void	UI::SetImageSrcPos( int mode )
	{
		if ( mode == TITLE_MODE::TITLE )
		{
			titleInfo.textImage.renderflag = false;
		}
		else
		{
			titleInfo.textImage.renderflag = true;
		}

		switch ( mode ) 
		{
		case TITLE_MODE::MENU:
			SetImageSrcPos( 0, 0 );
			break;

		case TITLE_MODE::OPTION:
			SetImageSrcPos( 0, 128 );
			break;

		case TITLE_MODE::CREDIT:
			SetImageSrcPos( 0, 256 );
			break;

		case TITLE_MODE::PLAY:
			SetImageSrcPos( 0, 384 );
			break;

		case TITLE_MODE::MOVE_MAIN:
			break;
		}
	}

	//	�ǂݍ��݈ʒu�ݒ�
	void	UI::SetImageSrcPos( int sx, int sy )
	{
		titleInfo.textImage.sx = sx;
		titleInfo.textImage.sy = sy;
	}

	//	���[�h�ύX�t���O�擾
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}
