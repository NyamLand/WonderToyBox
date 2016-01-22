
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"CharacterManager.h"
#include	"sceneTitle.h"
#include	"sceneMenu.h"
#include	"sceneMain.h"
#include	"sceneLoad.h"
#include	"Screen.h"
#include	"Sound.h"

#include	"sceneResult.h"

//********************************************************************************
//
//	sceneResult�N���X
//
//********************************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

#define	NO_BONUS	-1
#define	CURTAIN_SPEED	0.02f

	namespace
	{
		namespace MOVE_MODE
		{
			enum
			{
				RESULT,
				SELECT,
				LAST_RESULT,
			};
		}

		namespace RESULT_MODE
		{
			enum
			{
				ROULETTE,
				LASTBONUS,
				RANK,
				LIFE,
				LAST_RESULT,
				RANK_SKIP,
				INPUT_WAIT,
			};
		}

		namespace SELECT_MODE
		{
			enum
			{
				DOWN,
				SELECT,
			};
		}

		namespace LASTRESULT_MODE
		{
			enum
			{
				SET_CLOSE_CURTAIN,
				CLOSE_CURTAIN,		//	�J�[�e������
				LIGHT_PRODUCTION,	//	���C�g���o
				OPEN_CURTAIN,			//	�J�[�e���J����
				PRODUCTION,	//	���o
				RESULT,			//	���E���h���Ƃ̃X�R�A
			};
		}

		namespace MENU
		{
			enum
			{
				//RESTART,							//	�Đ�
				MOVE_MENU,					//	�ΐ�ݒ��	
				MOVE_TITLE,					//	�^�C�g����
				END,								//	�I�[
			};
		}

		namespace LASTBONUS
		{
			enum
			{
				MAX_COIN,
				FALL_STAGE,
				COIN77,
				MIN_TOTALCOIN,
				HIT_ATTACK_NUM,
			};
		}	

		namespace CURTAIN_MODE
		{
			enum
			{
				OPEN,
				CLOSE,
			};
		}
	}

//----------------------------------------------------------------------------
//	�S�̏������E�S�̉��
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
		//	�X�N���[���ݒ�
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::FADE_IN , 1.5f );

		//	����������
		Random::Initialize();

		//	�J�����ݒ�
		CameraInitialize();

		//	�ǂݍ���
		Load();

		//	�J�[�e����񏉊���
		CurtainInfoInitialize();

		//	���f��������
		ModelInitialize();

		//	���ʗp���\���̏�����
		ResultInfoInitialize();

		//	���v�����\�[�g
		Sort( sortInfo );
		
		//	�����L���O�ݒ�
		SetRank();

		//	���l�\���̏�����
		NumberImageInfoInitialize();
		
		//	���ʉ摜�\���̏�����
		RankImageInitialize();

		//	���C�t�摜�\���̏�����
		LifeInfoInitialize();

		//	���X�g�{�[�i�X�֘A�\���̏�����
		LastBonusImageInitialize();

		//	���[���b�g��񏉊���
		RouletteInfoInitialize();

		//	���j���[�֘A������
		MenuInfoInitialize();

		//	�����_�[�^�[�Q�b�g�֘A������
		RenderTargetTextureInitialize();

		//	���E���h�֘A������
		RoundInfoInitialize();
		SetRoundCoinNumber();

		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;
		bonusPlayer = 0;
		
		FOR( 0, PLAYER_MAX )	inputCheck[value] = false;

		sound->PlayBGM(BGM::RESULT_BGM);
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );
		return	true;
	}

	//	���
	void	sceneResult::Release( void )
	{
		SafeDelete( orgCurtain );
		SafeDelete( orgRound );
		SafeDelete( life );
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		SafeDelete( infoScreen );
		SafeDelete( lastResultTest );
		SafeDelete( lastBonusText );
		SafeDelete( faceImage.obj );
		SafeDelete( playerNumImage.obj );
		SafeDelete( waveCircleImage.obj );
		SafeDelete( notApplicable.obj );
		SafeDelete( bgStage );
		SafeDelete( check );
		Random::Release();

		sound->AllStop();
	}

//----------------------------------------------------------------------------
//	�e���񏉊���
//----------------------------------------------------------------------------

	//	�ǂݍ���
	void	sceneResult::Load( void )
	{
		//	�摜�ǂݍ���
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		menuHead.obj = new iex2DObj("DATA/UI/menu/menu-head.png");
		originNumber = new iex2DObj("DATA/UI/number.png");
		menuText = new iex2DObj("DATA/UI/result/result-cho.png");
		lastBonusText = new iex2DObj("DATA/UI/Result/LastBonusText.png");
		life = new iex2DObj("DATA/UI/Nlife.png");
		check = new iex2DObj( "DATA/UI/Result/check.png" );
		orgRound = new iex2DObj( "DATA/UI/roundText.png" );
		orgCurtain = new iex2DObj( "DATA/UI/title/curtain1.png" );

		//	�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );
		menuHead.angle = D3DXToRadian( 7.0f );

		//	���f���ǂݍ���
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>(LPSTR("DATA/CHR/majo/majo.IEM"));			//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>(LPSTR("DATA/CHR/�v�����Z�X/prinsess1.IEM"));					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));				//	���X
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));					//	�g��

		//	�X�e�[�W�ݒ�
		bgStage = new iexMesh("DATA/BG/MenuStage/menustage.IMO");	//	�X�e�[�W
		bgStage->SetScale(0.1f);
		bgStage->SetAngle(D3DX_PI);
		bgStage->Update();

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale(0.01f);	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetScale(0.04f);		//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale(0.03f);				//	����
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.02f);				//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle(D3DX_PI);	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle(D3DX_PI);		//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle(D3DX_PI);				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetAngle(D3DX_PI);			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(2);		//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(1);			//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);	 				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	�|����
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	�P
		org[CHARACTER_TYPE::THIEF]->Update();							//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->Update();						//	�g��

	}

	//	�J����������
	void	sceneResult::CameraInitialize( void )
	{
		//	�o�b�N�o�b�t�@�|�C���^�ޔ�
		iexSystem::GetDevice()->GetRenderTarget( 0, &backBuffer );

		//	�����_�[�^�[�Q�b�g�X�N���[��
		infoScreen = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
		lastResultTest = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );

		//	���\���p�J����
		viewInfo.pos = Vector3( 0.0f, 5.2f, -70.0f );
		viewInfo.target = Vector3( 0.0f, 5.2f, 0.0f );
		view2D = make_unique<Camera>();
		view2D->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		view2D->Set( viewInfo.pos, viewInfo.target );
		view2D->Activate();

		//	�e�X�g�p�J����
		viewTest = make_unique<Camera>();
		viewTest->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		viewTest->Set( viewInfo.pos, viewInfo.target );
		viewTest->Activate();

		//	���C���J����
		viewInfo.pos = Vector3( 0.0f, 15.0f, -10.0f );
		viewInfo.target = viewInfo.pos + Vector3( 0.0f, 0.0f, 1.0f );
		mainView = new Camera();
		mainView->Set( viewInfo.pos, viewInfo.target );
		mainView->Activate();
	}

	//	���f��������
	void	sceneResult::ModelInitialize( void )
	{
		float angle[PLAYER_MAX] = { D3DXToRadian( -15.0f ), D3DXToRadian( -10.0f ), D3DXToRadian( 10.0f ), D3DXToRadian( 15.0f ) };
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	���f�����ݒ�
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 10.0f, 10.0f );
			obj[i]->SetAngle( D3DX_PI + angle[i] );
			obj[i]->Update();
		}
	}

	//	���ʗp�\���̊֘A������
	void	sceneResult::ResultInfoInitialize( void )
	{
		//	���l�\���̏�����
		FOR( 0, PLAYER_MAX )
		{
			number[value].hundred = 0;
			number[value].ten = 0;
			number[value].one = 0;
		}

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�Q�[���I�����̃f�[�^���i�[( �����Ń{�[�i�X���ݒ肵�Ă��� )
			originInfo[i].num = gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	�����L���O�v�Z�p�ɑ��v�f�[�^���i�[( �{�[�i�X���l����������A���̃R�C�������Ƀ{�[�i�X�𑫂��A�����N�̓\�[�g�ɂ����邽�ߓK���ɑ�� )
			sortInfo[i].num = originInfo[i].num;
			sortInfo[i].sortRank = i;
			sortInfo[i].rank = i;
		}
	}

	//	���l�\���̏�����
	void	sceneResult::NumberImageInfoInitialize( void )
	{	
		Vector3	out;

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	���W�ݒ�
			out.x = iexSystem::ScreenWidth * 0.2f;
			out.x += out.x * i;

			//	���S���W�E�����X�P�[���ݒ�
			numberImageInfo[i].pos.x = static_cast<int>( out.x );
			numberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.38f );
			numberImageInfo[i].scale = 100;

			//	�e�ʉ摜�ݒ�
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;
			
			//	���l�摜�\���̏�����
			SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
		}
	}

	//	���ʉ摜�\���̏�����
	void	sceneResult::RankImageInitialize( void )
	{
		Vector3	out;
		
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	���W�ݒ�
			out.x = iexSystem::ScreenWidth * 0.2f;
			out.x += out.x * i;

			//	�\���̏�����
			int 		x = static_cast<int>( out.x );
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

		//	�����N�\���p�\���̏�����
		{
			viewRankInOrder.timer = 0;
			viewRankInOrder.step = 3;
		}
	}

	//	���C�t�摜�\���̏�����
	void	sceneResult::LifeInfoInitialize( void )
	{
		int	culLife = 0;
		int	x, y, w, h, sx, sy, sw, sh;

		//	���񃉃C�t�ݒ�
		nextLife[0][0] = 2; nextLife[0][1] = 2; nextLife[0][2] = 3; nextLife[0][3] = 3;
		nextLife[1][0] = 2; nextLife[1][1] = 3; nextLife[1][2] = 3; nextLife[1][3] = 4;
		nextLife[2][0] = 2; nextLife[2][1] = 3; nextLife[2][2] = 4; nextLife[2][3] = 5;

		lifeInfo.maxW = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		lifeInfo.maxH = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		FOR( 0, PLAYER_MAX )
		{
			//	�摜��o�^
			lifeImage[value].obj = life;

			//	���݂̃��C�t�ő�l���擾
			culLife = gameManager->GetStartLife( value );

			//	�摜�\���̂ɐݒ�
			x = rankImage[value].x;
			y = rankImage[value].y;
			w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			h = w;
			sw = 64;
			sh = 64;
			sx = sw * ( ( 5 - culLife ) % 4 );
			sy = sh * ( ( 5 - culLife ) / 4 );
			ImageInitialize( lifeImage[value], x, y, w, h, sx, sy, sw, sh );

			//	�ŏ��͔�\��
			lifeImage[value].renderflag = false;

			//	���ł�OK�̈ʒu���ݒ�
			y = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			h = ( int )( w * 0.5f );
			checkImage[value].obj = check;
			ImageInitialize( checkImage[value], x, y, w, h, 0, 0, 256, 128 );
			checkImage[value].renderflag = false;
		}

		//	���C�t���\�p�\���̐ݒ�
		{
			lifeInfo.step = 0;
			lifeInfo.culLife = 0;
			lifeInfo.renderflag = false;
			lifeInfo.isEnd = false;
			lifeInfo.t = 0.0f;
			lifeInfo.waitTimer = 0;
		}
	}

	//	�����_�[�^�[�Q�b�g�֘A������
	void	sceneResult::RenderTargetTextureInitialize( void )
	{
		//	�e�N�X�`���ʒu�ݒ�
		float	dist = 1000.0f;
		Collision::GetFront( bgStage, viewInfo.pos + Vector3( 0.0f, 2.0f, 0.0f ), viewInfo.texPos );
		viewInfo.texPos.z -= 0.01f;
		Vector3	texSize = Vector3( 30.0f, 20.0f, 0.0f );
		SetVertex( viewInfo.v[0], viewInfo.texPos.x - texSize.x * 0.5f, viewInfo.texPos.y + texSize.y * 0.5f, viewInfo.texPos.z, 0, 0, 0xFFFFFFFF );
		SetVertex( viewInfo.v[1], viewInfo.texPos.x + texSize.x * 0.5f, viewInfo.texPos.y + texSize.y * 0.5f, viewInfo.texPos.z, 1, 0, 0xFFFFFFFF );
		SetVertex( viewInfo.v[2], viewInfo.texPos.x - texSize.x * 0.5f, viewInfo.texPos.y - texSize.y * 0.5f, viewInfo.texPos.z, 0, 1, 0xFFFFFFFF );
		SetVertex( viewInfo.v[3], viewInfo.texPos.x + texSize.x * 0.5f, viewInfo.texPos.y - texSize.y * 0.5f, viewInfo.texPos.z, 1, 1, 0xFFFFFFFF );

	}

	//	���X�g�{�[�i�X�֘A�\���̏�����
	void	sceneResult::LastBonusImageInitialize( void )
	{
		//	���X�g�{�[�i�X�֘A������
		FOR( 0, PLAYER_MAX )	lastBonusInfo.bonus[value];
		lastBonusInfo.step = 0;
		lastBonusInfo.t = 0.0f;

		//	�e�L�X�g������
		int		x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int		y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int		w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int		h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		ImageInitialize( lastBonusInfo.textImage, x, y, w, h, 0, lastBonus * 128, 512, 128 );
		lastBonusInfo.textImage.renderflag = false;
		lastBonusInfo.textImage.obj = lastBonusText;

		//	���_�ݒ�
		SetVertex( lastBonusInfo.v[0], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.2f ), 0.0f, 0.0f, 0.0f, 0x77333333 );
		SetVertex( lastBonusInfo.v[1], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.2f ), 0.0f, 1.0f, 0.0f, 0x77333333 );
		SetVertex( lastBonusInfo.v[2], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.6f ), 0.0f, 0.0f, 1.0f, 0x77333333 );
		SetVertex( lastBonusInfo.v[3], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.6f ), 0.0f, 1.0f, 1.0f, 0x77333333 );

		//	��摜������
		y = static_cast<int>( iexSystem::ScreenHeight * 0.6f );
		w = 0;
		h = 0;
		ImageInitialize( faceImage, x, y, w, h, 0, 256 * bonusPlayer, 256, 256 );
		faceImage.obj = new iex2DObj( "DATA/UI/chara_emotion.png" );
		faceImage.renderflag = false;

		//	���~�摜������
		w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		h = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		ImageInitialize( waveCircleImage, x, y, w, h, 0, 0, 512, 512 );
		waveCircleImage.obj = new iex2DObj( "DATA/UI/Rainbow-circle.png" );
		waveCircleImage.renderflag = false;

		//	�Y���Ȃ��摜������
		w = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
		ImageInitialize( notApplicable, x, y, w, h, 0, 0, 512, 256 );
		notApplicable.obj = new iex2DObj( "DATA/UI/Result/resultText.png" );
		notApplicable.renderflag = false;
		notApplicable.alpha = 0.0f;
		notApplicable.angle = -D3DX_PI * 0.035f;

		//	�v���C���[�ԍ�������
		x = static_cast<int>( iexSystem::ScreenWidth * 0.45f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.52f );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.05f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.075f );
		ImageInitialize( playerNumImage, x, y, w, h, 128 * ( bonusPlayer % 2 ), 128 * ( bonusPlayer / 2 ), 128, 128 );
		playerNumImage.obj = new iex2DObj( "DATA/UI/cursor.png" );
		playerNumImage.renderflag = false;	
	}

	//	���j���[���\����
	void	sceneResult::MenuInfoInitialize( void )
	{
		//	���j���[��񏉊���
		{
			menuInfo.select = MENU::MOVE_MENU;
			menuInfo.screenH = 0;
			menuInfo.alpha = 0.5f;
			menuInfo.t = 0.0f;
		}

		//	���j���[�摜�\���̏�����
		{
			//	���j���[�̍��ڐ��܂킷
			for ( int i = 0; i < MENU::END; i++ )
			{
				int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
				int y = static_cast<int>( iexSystem::ScreenHeight * 0.35f );
				int w = static_cast<int>( iexSystem::ScreenWidth * 0.55f );
				int h = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
				ImageInitialize( menuImage[i], x, y + y * i, w, h, 0, 128 + 128 * i, 512, 128 );
				menuImage[i].obj = menuText;
				menuImage[i].renderflag = false;
			}
		}
	}

	//	���[���b�g�\���̏�����
	void	sceneResult::RouletteInfoInitialize( void )
	{

		//	���[���b�g��񏉊���
		{
			rouletteInfo.step = 0;
			rouletteInfo.timer = 1;
		}
	}

	//	�J�[�e����񏉊���
	void	sceneResult::CurtainInfoInitialize( void )
	{
		//	�|�C���g�ݒ�
		curtainPosInfo.centerPosX = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		curtainPosInfo.leftOutPosX = static_cast<int>( iexSystem::ScreenWidth * -0.5f );
		curtainPosInfo.rightOutPosX = static_cast<int>( iexSystem::ScreenWidth * 1.5f );
		curtainPosInfo.underPosY = static_cast<int>( iexSystem::ScreenHeight );
		curtainPosInfo.rightPos = static_cast<int>( iexSystem::ScreenWidth );
		curtainPosInfo.upPosY = 0;
		curtainPosInfo.leftPos = 0;

		//	�J�[�e����񏉊���
		curtainInfoL.obj = orgCurtain;
		curtainInfoR.obj = orgCurtain;
		curtainInfoL.t = 0.0f;
		curtainInfoR.t = 0.0f;
		curtain_t = 1.0f;
		curtainBrightness = 1.0f;	
		curtainState = false;
		curtainMode = CURTAIN_MODE::OPEN;

		//	�V�F�[�_�[�p�ϐ�������
		lightMoveNum = 0;
		light_t = 0.0f;
		lightPos[0] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[1] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[2] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[3] = Vector3( 0.0f, 0.0f, 0.0f );

		//	���_�ݒ�
		SetVertex( curtainInfoL.tlv[0], ( float )curtainPosInfo.leftPos, ( float )curtainPosInfo.upPosY, 0, 0, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[1], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.upPosY, 0, 1, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[2], ( float )curtainPosInfo.leftPos, ( float )curtainPosInfo.underPosY, 0, 0, 1, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[3], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.underPosY, 0, 1, 1, 0xFFFFFFFF );
														
		SetVertex( curtainInfoR.tlv[0], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.upPosY, 0, 1, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[1], ( float )curtainPosInfo.rightPos, ( float )curtainPosInfo.upPosY, 0, 0, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[2], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.underPosY, 0, 1, 1, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[3], ( float )curtainPosInfo.rightPos, ( float )curtainPosInfo.underPosY, 0, 0, 1, 0xFFFFFFFF );
	}

	//	���E���h�֘A������
	void	sceneResult::RoundInfoInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int sx = 0;
		int sy;
		int sw = 512;
		int sh = 128;

		FOR( 0, Round::END ) 
		{
			y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.2f ) * value );
			sy = sh * value;
			ImageInitialize( roundImage[value], x, y, w, h, sx, sy, sw, sh );
			roundImage[value].obj = orgRound;
		}

		SetRoundCoinNumber();
	}

	//	���E���h���Ƃ̐��l��ݒ�
	void	sceneResult::SetRoundCoinNumber( void )
	{
		int	totalCoinNum;

		//	�e���E���h�̃R�C���������擾
		FOR( 0, PLAYER_MAX )
		{
			for ( int round = 0; round < Round::END; round++ ) 
			{
				//	���W��ݒ�
				roundCoinNumberImageInfo[round][value].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.4f ) + ( static_cast<int>( iexSystem::ScreenWidth * 0.15f ) * value );
				roundCoinNumberImageInfo[round][value].pos.y = roundImage[round].y;
				roundCoinNumberImageInfo[round][value].scale = 100;
				roundCoinNumberImageInfo[round][value].one.obj = originNumber;
				roundCoinNumberImageInfo[round][value].ten.obj = originNumber;
				roundCoinNumberImageInfo[round][value].hundred.obj = originNumber;

				//	���l��ݒ�
				totalCoinNum = gameManager->GetTotalCoin( round, value );
				SetNumberImageInfo( roundCoinNumberImageInfo[round][value], roundCoinNumber[round][value], totalCoinNum );
			}
		}
	}

//----------------------------------------------------------------------------
//	�S�̍X�V�E�S�̕`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{
		//	�J�[�e���X�V
		curtainState = CurtainUpdate();

		//	�X�N���[���X�V
		screen->Update();

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

		case MOVE_MODE::LAST_RESULT:
			LastResultUpdate();
			break;
		}

		//	�V�[���ړ��Ǘ�
		MoveScene();
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{

		switch ( mode )
		{
		case MOVE_MODE::RESULT:
		case MOVE_MODE::SELECT:
			ResultRender();
			break;
		case MOVE_MODE::LAST_RESULT:
			LastResultRender();
			lastResultTest->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight );
			break;
		}

		//	�J�[�e���`��
		CurtainRender();

		//	�ŏI���ʃe�X�g
		SelectRender();
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	���U���g���̍X�V
	void	sceneResult::ResultUpdate( void )
	{
		//	�i�K���Ƃ̏���
		switch ( step )
		{
		case RESULT_MODE::ROULETTE:
			ModeRoulette();
			break;
		
		case RESULT_MODE::LASTBONUS:
			ModeLastBonus();
			break;
			
		case RESULT_MODE::RANK:
			ModeRank();
			break;

		case	RESULT_MODE::LIFE:
			NextLifeAnnouncing();
			break;

		case	RESULT_MODE::LAST_RESULT:
			ModeLastResult();
			break;

		case RESULT_MODE::RANK_SKIP:
			ModeRankSkip();
			break;

		case	RESULT_MODE::INPUT_WAIT:
			ModeInputWait();
			break;
		}
	}

	//	�Z���N�g���̍X�V
	void	sceneResult::SelectUpdate( void )
	{
		bool	isEnd = false;

		switch ( step )
		{
		case SELECT_MODE::DOWN:
			isEnd = DownPolygon();
			if ( isEnd )	step++;
			break;

		case SELECT_MODE::SELECT:
			ControlCursor();
			break;
		}
	}

	//	���X�g���\�X�V
	void	sceneResult::LastResultUpdate( void )
	{
		bool	isEnd = false;
		switch ( step )
		{
		case LASTRESULT_MODE::SET_CLOSE_CURTAIN:
			SetCurtainMode( CURTAIN_MODE::CLOSE );
			step++;
			break;

		case LASTRESULT_MODE::CLOSE_CURTAIN:
			if ( curtainState ) SetWaitTimer( 60 );
			step = LASTRESULT_MODE::LIGHT_PRODUCTION;
			break;

		case LASTRESULT_MODE::LIGHT_PRODUCTION:
			isEnd = WaitTimeUpdate();
			if ( isEnd )
			{
				step = LASTRESULT_MODE::OPEN_CURTAIN;
				SetCurtainMode( CURTAIN_MODE::OPEN );
			}
			break;

		case LASTRESULT_MODE::OPEN_CURTAIN:
			if ( curtainState )
			{
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) )
				{
					step = 0;
					mode = MOVE_MODE::SELECT;
				}
			}
			break;
		}
	}

	//	�J�[�e���X�V
	bool	sceneResult::CurtainUpdate( void )
	{
		bool	out = false;

		//	�p�����[�^�X�V
		curtain_t += CURTAIN_SPEED;

		//	��������ݒ�
		if ( curtain_t >= 1.0f )
		{
			curtain_t = 1.0f;
			out = true;
		}

		//	�J�[�e���̃��[�h�ŃX�^�[�g�ʒu�ƏI���ʒu��ύX
		switch ( curtainMode )
		{
		case CURTAIN_MODE::OPEN:
			//	���J�[�e��
			Lerp( curtainInfoL.tlv[0].sx, curtainPosInfo.leftPos, curtainPosInfo.leftOutPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	����
			Lerp( curtainInfoL.tlv[1].sx, curtainPosInfo.centerPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	�E��
			Lerp( curtainInfoL.tlv[2].sx, curtainPosInfo.leftPos, curtainPosInfo.leftOutPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	����
			Lerp( curtainInfoL.tlv[3].sx, curtainPosInfo.centerPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	�E��

			//	�E�J�[�e��
			Lerp( curtainInfoR.tlv[0].sx, curtainPosInfo.centerPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	����
			Lerp( curtainInfoR.tlv[1].sx, curtainPosInfo.rightPos, curtainPosInfo.rightOutPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	�E��
			Lerp( curtainInfoR.tlv[2].sx, curtainPosInfo.centerPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	����
			Lerp( curtainInfoR.tlv[3].sx, curtainPosInfo.rightPos, curtainPosInfo.rightOutPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	�E��	
			break;

		case CURTAIN_MODE::CLOSE:
			//	���J�[�e��
			Lerp( curtainInfoL.tlv[0].sx, curtainPosInfo.leftOutPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	����
			Lerp( curtainInfoL.tlv[1].sx, curtainPosInfo.leftPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	�E��
			Lerp( curtainInfoL.tlv[2].sx, curtainPosInfo.leftOutPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	����
			Lerp( curtainInfoL.tlv[3].sx, curtainPosInfo.leftPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	�E��

			//	�E�J�[�e��
			Lerp( curtainInfoR.tlv[0].sx, curtainPosInfo.rightPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	����
			Lerp( curtainInfoR.tlv[1].sx, curtainPosInfo.rightOutPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	�E��
			Lerp( curtainInfoR.tlv[2].sx, curtainPosInfo.rightPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	����
			Lerp( curtainInfoR.tlv[3].sx, curtainPosInfo.rightOutPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	�E��	
			break;
		}

		return	out;
	}

	//	�Z���N�g��ʕ`��
	void	sceneResult::SelectRender( void )
	{
		//	���j���[�p�X�N���[���`��
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		for ( int i = 0; i < MENU::END; i++ )
		{
			RenderImage( menuImage[i], menuImage[i].sx, menuImage[i].sy, menuImage[i].sw, menuImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	���U���g�`��
	void	sceneResult::ResultRender( void )
	{
		//	�����_�[�^�[�Q�b�g��ݒ�
		infoScreen->RenderTarget( 0 );

		//	�J����
		view2D->Activate();
		view2D->Clear();

		//	�w�i( ��Ԍ��ɕ\�� )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	���U���g�V�[���`��
		RenderImage(menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM);

		//	���l�`��
		NumberImageRender();

		//	���ʕ`��
		RankRender();

		//	���C�t�`��
		LifeRender();

		//	���̓`�F�b�N�`��
		InputCheckRender();

		//	�t���[���o�b�t�@�֐؂�ւ�
		iexSystem::GetDevice()->SetRenderTarget(0, backBuffer);
		mainView->Activate();
		mainView->Clear();

		//	�X�e�[�W�`��
		bgStage->Render();

		//	�|���S���`��
		iexPolygon::Render3D(viewInfo.v, 2, infoScreen, shader3D, "alpha");
		
		//	�v���C���[�`��
		FOR( 0, PLAYER_MAX )
		{
			//	�v���C���[�`��
			obj[value]->Render( shader3D, "toon" );
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

			if (numberImageInfo[i].hundredRenderFlag)
			{
				RenderImage( numberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			}

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

	//	���l�摜�\���̕`��
	void	sceneResult::NumberImageRender( sceneResult::NUMBERIMAGE_INFO numberImageInfo )
	{
		//	�P�O�O�̈ʕ`��
		int		sx = numberImageInfo.hundred.sx;
		int		sy = numberImageInfo.hundred.sy;
		int		sw = numberImageInfo.hundred.sw;
		int		sh = numberImageInfo.hundred.sh;

		if ( numberImageInfo.hundredRenderFlag )
		{
			RenderImage( numberImageInfo.hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
		}

		//	�P�O�̈ʕ`��
		sx = numberImageInfo.ten.sx;
		sy = numberImageInfo.ten.sy;
		sw = numberImageInfo.ten.sw;
		sh = numberImageInfo.ten.sh;
		RenderImage( numberImageInfo.ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

		//	�P�̈ʕ`��
		sx = numberImageInfo.one.sx;
		sy = numberImageInfo.one.sy;
		sw = numberImageInfo.one.sw;
		sh = numberImageInfo.one.sh;
		RenderImage( numberImageInfo.one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
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
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::WAVE );
		}
	}
	
	//	���C�t�`��
	void	sceneResult::LifeRender( void )
	{
		//	�����N�`��
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			int		sx = lifeImage[i].sx;
			int		sy = lifeImage[i].sy;
			int		sw = lifeImage[i].sw;
			int		sh = lifeImage[i].sh;
			RenderImage( lifeImage[i], sx, sy, sw, sh, IMAGE_MODE::NORMAL );
			RenderImage( lifeImage[i], sx, sy, sw, sh, IMAGE_MODE::WAVE );
		}
	}

	//	���̓`�F�b�N�`��
	void	sceneResult::InputCheckRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
	}

	//	���X�g���\�`��
	void sceneResult::LastResultRender( void )
	{
		//-------------UI�֘A---------------//
		//	�ʂ̃e�N�X�`���ɏ����o��
		lastResultTest->RenderTarget();
		viewTest->Activate();
		viewTest->Clear();

		//	�w�i�`��
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		
		FOR( 0, Round::END )
		{
			//	���E���h�`��
			RenderImage( roundImage[value], roundImage[value].sx, roundImage[value].sy, roundImage[value].sw, roundImage[value].sh, IMAGE_MODE::NORMAL );
			
			//	�e���E���h�E�v���C���[�̃R�C�������`��
			for ( int player = 0; player < PLAYER_MAX; player++ )
			{
				NumberImageRender( roundCoinNumberImageInfo[value][player] );
			}
		}

		//	�t���[���o�b�t�@�֐؂�ւ�
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		mainView->Activate();
		mainView->Clear();
	}

	//	�J�[�e���`��
	void	sceneResult::CurtainRender( void )
	{
		LPSTR technique = "copy";

		if ( step == LASTRESULT_MODE::LIGHT_PRODUCTION )
		{
			technique = "SpotLight";
		}
		iexPolygon::Render2D( curtainInfoL.tlv, 2, curtainInfoL.obj, shader2D, technique );
		iexPolygon::Render2D( curtainInfoR.tlv, 2, curtainInfoR.obj, shader2D,technique );
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���C�g�ړ�
	bool	sceneResult::LightUpdate( void )
	{
		bool isEnd = false;

		//	�p�����[�^���Z
		light_t += 0.1f;
		if ( light_t >= 1.0f )	light_t = 1.0f;
		return	false;
	}

	//	�\�[�g
	void	sceneResult::Sort( SORT_INFO ( &sort_info )[4]  )
	{
		//	�ޔ�p
		SORT_INFO temp;

		for ( int i = 0; i < PLAYER_MAX; ++i )
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( sort_info[s].num >	sort_info[s - 1].num )
				{
					//	�ꎞ�I�ɑޔ�
					temp = sort_info[s - 1];

					//	����
					sort_info[s - 1] = sort_info[s];

					//	�ޔ����Ă����߂�
					sort_info[s] = temp;
				}
			}
		}

		FOR( 1, PLAYER_MAX )
		{
			if ( sort_info[value].num == sort_info[value - 1].num )
				sort_info[value].sortRank = sort_info[value - 1].sortRank;
		}
	}

	//	�t�����\�[�g
	void	sceneResult::ReverseSort( SORT_INFO ( &sort_info )[4] )
	{
		//	�ޔ�p
		SORT_INFO temp;

		for (int i = 0; i < PLAYER_MAX; ++i)
		{
			//	��납�珇�ԂɃ`�F�b�N���Ă���
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	����̗v�f�Ɣ�r
				if ( sort_info[s].num <	sort_info[s - 1].num ) 
				{
					//	�ꎞ�I�ɑޔ�
					temp = sort_info[s - 1];

					//	����
					sort_info[s - 1] = sort_info[s];

					//	�ޔ����Ă����߂�
					sort_info[s] = temp;
				}
			}
		}

		FOR( 1, PLAYER_MAX )
		{
			if ( sort_info[value].num == sort_info[value - 1].num ) 
				sort_info[value].sortRank = sort_info[value - 1].sortRank;
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
				lastBonus = Random::GetInt( 0, 3 );
			}
		}
	}

	//	���X�g�{�[�i�X���l���Z�A�S�������l�@or�@�O�������ꍇ�{�[�i�X�ɍŏI�l�����Y���Ȃ��ɂ���
	void	sceneResult::AddLastBonus( void )
	{
		int	bonus = 0;
		int	result = 0;
		switch ( lastBonus )
		{
		case LASTBONUS::MAX_COIN:
			//	�Y���Ȃ����`�F�b�N
			FOR( 0, PLAYER_MAX ) 	result += maxCoinNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	�ő�R�C������
			bonus = Random::GetInt( 10, 30 );
			sortInfo[maxCoinNum[0].rank].num += bonus;
			originInfo[maxCoinNum[0].rank].bonus = bonus;
			bonusPlayer = maxCoinNum[0].rank;
			break;

		case LASTBONUS::FALL_STAGE:
			//	�Y���Ȃ����`�F�b�N
			FOR( 0, PLAYER_MAX ) 	result += fallStageNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	�X�e�[�W����̗�����
			bonus = Random::GetInt( 10, 30 );
			sortInfo[fallStageNum[0].rank].num += bonus;
			originInfo[maxCoinNum[0].rank].bonus = bonus;
			bonusPlayer = maxCoinNum[0].rank;
			break;

		case LASTBONUS::COIN77:
			//	�Y���Ȃ����`�F�b�N
			FOR( 0, PLAYER_MAX ) 	result += coin77[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}
			
			//	�V�V���ƃR�C�������̍�
			bonus = Random::GetInt( 10, 30 );
			sortInfo[coin77[0].rank].num += bonus;
			originInfo[coin77[0].rank].bonus = bonus;
			bonusPlayer = coin77[0].rank;
			break;

		case LASTBONUS::MIN_TOTALCOIN:
			//	�Y���Ȃ����`�F�b�N
			FOR( 0, PLAYER_MAX ) 	result += minCoinNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	�擾�R�C����������ԏ��Ȃ�
			bonus = Random::GetInt( 10, 30 );
			sortInfo[minCoinNum[0].rank].num += bonus;
			originInfo[minCoinNum[0].rank].bonus = bonus;
			bonusPlayer = minCoinNum[0].rank;
			break;

		case LASTBONUS::HIT_ATTACK_NUM:
			//	�Y���Ȃ����`�F�b�N
			FOR( 0, PLAYER_MAX ) 	result += hitAttackNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	�U���𓖂Ă���
			bonus = Random::GetInt( 10, 30 );
			sortInfo[hitAttackNum[0].rank].num += bonus;
			originInfo[hitAttackNum[0].rank].bonus = bonus;
			bonusPlayer = hitAttackNum[0].rank;
			break;
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
		FOR( 0, PLAYER_MAX )
		{
			originInfo[sortInfo[value].rank].rank = value;

			if ( value != 0 )
			{
				//	��ʂ̐l�Ɠ����R�C�������������瓯�����N�ɂ���
				if ( originInfo[sortInfo[value].rank].num == originInfo[sortInfo[value - 1].rank].num )
				{
					originInfo[sortInfo[value].rank].rank = originInfo[sortInfo[value - 1].rank].rank;
				}
			}
		}
	}

	//	�ݒ肵�����l�ɂ��킹�č\���̏���ݒ�A�P�O�O�ȏォ�Ŕz�u���ύX
	void	sceneResult::SetNumberImageInfo( NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num )
	{
		//	�����m�F
		if ( num >= 100 )		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( numInfo, num );

		//	�e�ʉ摜�\���̏�����
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	�����p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	�P�O�̈ʐݒ�
			x = numImageInfo.pos.x;
			y = numImageInfo.pos.y;
			w = h = numImageInfo.scale;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	�P�O�̈ʐݒ�
			w = h = numImageInfo.scale;
			x = numImageInfo.pos.x - w / 3;
			y = numImageInfo.pos.y;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numImageInfo.pos.x + w / 3;
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
	}

	//	���l���[���b�g( �I�������true�������� )
	bool	sceneResult::Roulette( void )
	{
		//	�S�l���m�肵�Ă���true��������
		if ( rouletteInfo.step > PLAYER_MAX )		return	true;

		//	��ʐ؂�ւ��I����^�C�}�[���Z
		if ( screen->GetScreenState() )	rouletteInfo.timer++;

		for ( int i = rouletteInfo.step; i < PLAYER_MAX; i++ )
		{
			//	�K���Ȑ��l������
			SetNumberImageInfo( numberImageInfo[i], number[i], Random::GetInt( 100, 999 ) );
		}

		//	���񂾂���]���Ԃ�����ƒ���
		if ( rouletteInfo.step == 0 )
		{
			if ( rouletteInfo.timer % 90 == 0 )
			{
				rouletteInfo.step++;
				rouletteInfo.timer = 1;
			}
		}
		else
		{
			if ( rouletteInfo.timer % 50 == 0 )
			{
				rouletteInfo.step++;
				rouletteInfo.timer = 1;
			}
		}

		//	���l����
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( i < rouletteInfo.step )
			{
				SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
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

		//	���݂̃��E���h���擾
		int	round = gameManager->GetRound();

		if ( round != Round::ROUND_FINAL )
		{
			//	�Đ�p�̐ݒ�
			FOR( 0, PLAYER_MAX )
			{
				//	�����E���h�̊l���R�C��������ݒ�
				gameManager->SetTotalCoin( round, value, gameManager->GetCoinNum( value ) );
			}
			gameManager->RetryInitialize();
			gameManager->SetRound( round + 1 );

			//	���C����
			MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
			return;
		}
		else
		{
			switch ( menuInfo.select )
			{
			case MENU::MOVE_MENU:
				MainFrame->ChangeScene( new sceneMenu() );
				return;
				break;

			case	MENU::MOVE_TITLE:
				MainFrame->ChangeScene( new sceneTitle() );
				return;
				break;
			}
		}
		//switch ( menuInfo.select )
		//{
		//case MENU::RESTART:
		//	//	�Q�[����񏉊���
		//	gameManager->RetryInitialize();
		//	gameManager->SetRound( nextRound );
		//	MainFrame->ChangeScene( new sceneMain() );
		//	return;
		//	break;

		//case MENU::MOVE_MENU:
		//	MainFrame->ChangeScene( new sceneMenu() );
		//	return;
		//	break;

		//case MENU::MOVE_TITLE:
		//	MainFrame->ChangeScene( new sceneTitle() );
		//	return;
		//	break;
		//}
	}

	//	���񃉃C�t�ݒ�
	void	sceneResult::SetNextLife( void )
	{
		int	rank;
		int	maxLife = gameManager->GetMaxLife();
		FOR( 0, PLAYER_MAX )
		{
			rank = originInfo[value].rank;
			gameManager->SetStartLife( value, nextLife[maxLife][rank] );
		}
	}

	//	�J�[�e����Ԑݒ�
	void	sceneResult::SetCurtainMode( int mode )
	{
		curtainMode = mode;
		curtain_t = 0.0f;
	}

	//	�҂��^�C�}�[�ݒ�
	void	sceneResult::SetWaitTimer( int time )
	{
		waitTimer = time;
	}

	//	�҂��^�C�}�[���Z
	bool	sceneResult::WaitTimeUpdate( void )
	{
		waitTimer--;
		if ( waitTimer <= 0 )
		{
			waitTimer = 0;
			return	true;
		}

		return	false;
	}

//----------------------------------------------------------------------------
//	���X�g�{�[�i�X�֐�( �{�[�h�o�����������R�C�������X�V���{�[�h�ދp )
//----------------------------------------------------------------------------

	//	���X�g�{�[�i�X����X�V
	bool	sceneResult::LastBonusUpdate( void )
	{
		bool	isEnd = false;
		bool	isEndWave = false;

		switch ( lastBonusInfo.step )
		{
		case 0:
			//	���{�[�h�o��
			isEnd = InBoard();

			//	���{�[�h�̓���I���ド�X�g�{�[�i�X�̃e�L�X�g�̕`���ON�ɂ���
			if ( isEnd )
			{
				lastBonusInfo.textImage.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 1:
			//	���X�g�{�[�i�X���\
			isEnd = BonusAnnouncing();
			
			//	���\����g��ݒ�
			if ( isEnd )		isEndWave = WaveUpdate( lastBonusInfo.textImage );

			//	�g��I����Ώۃv���C���[�̕`���ON�ɂ��Ď��̃X�e�b�v��
			if ( isEndWave )
			{
				if ( bonusPlayer != NO_BONUS )		faceImage.renderflag = true;
				else								notApplicable.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 2:
			//	�Ώۃv���C���[���\
			isEnd = PlayerAnnouncing();

			//	�v���C���[���\��A�g���ݒ�
			if ( isEnd )
			{
				if ( bonusPlayer != NO_BONUS )		isEndWave = WaveUpdate( waveCircleImage, 140 );
				else	isEndWave = true;
			}
			
			//	�g�䓮��I����v���C���[�ԍ��\��
			if ( isEndWave )
			{
				if ( bonusPlayer != NO_BONUS )		playerNumImage.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 3:
			//	�{�[�i�X���Z���o
			isEnd = AddBonus();

			if ( isEnd )
			{
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;
		case 4:
				return	true;
			break;
		}
		return	false;
	}
	
	//	�{�[�h�o��
	bool	sceneResult::InBoard( void )
	{
		//	�p�����[�^�X�V
		lastBonusInfo.t += 0.07f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;
		
		//	���_�ړ�
		CubicFunctionInterpolation( lastBonusInfo.v[0].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[2].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[0].sy, static_cast<int>( iexSystem::ScreenHeight * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.4f ), lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[2].sy, static_cast<int>( iexSystem::ScreenHeight * 0.6f ), static_cast<int>( iexSystem::ScreenHeight * 0.8f ), lastBonusInfo.t );

		//	�������I�����Ă���true��������
		if ( lastBonusInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	�{�[�h�ދp
	bool	sceneResult::OutBoard( void )
	{		
		//	�p�����[�^�X�V
		lastBonusInfo.t += 0.07f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

		//	���_�ړ�
		Lerp( lastBonusInfo.v[1].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		Lerp( lastBonusInfo.v[3].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		Lerp( lastBonusInfo.v[1].sy, static_cast<int>( iexSystem::ScreenHeight * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.4f ), lastBonusInfo.t );
		Lerp( lastBonusInfo.v[3].sy, static_cast<int>( iexSystem::ScreenHeight * 0.6f ), static_cast<int>( iexSystem::ScreenHeight * 0.8f ), lastBonusInfo.t );

		//	�������I�����Ă���true��������
		if ( lastBonusInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	�{�[�i�X���\
	bool	sceneResult::BonusAnnouncing( void )
	{
		if ( lastBonusInfo.t >= 1.0f )	return	true;

		//	�p�����[�^�X�V
		lastBonusInfo.t += 0.01f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

		//	������]
		static	float startAngle = -D3DX_PI * 0.035f;
		static	float	endAngle = startAngle + ( ( D3DX_PI * 2 ) * 10.0f );
		Lerp( lastBonusInfo.textImage.angle, startAngle, endAngle, lastBonusInfo.t );

		//	�����T�C�Y�ύX
		static	int		startWidth = static_cast<int>( iexSystem::ScreenWidth * 0.35f );
		static	int		maxWidth = startWidth + static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		static	int		startHeight = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		static	int		maxHeight = startHeight + static_cast<int>( iexSystem::ScreenHeight * 0.05f );
		CubicFunctionInterpolation( lastBonusInfo.textImage.w, startWidth, maxWidth, lastBonusInfo.t * 1.5f );
		CubicFunctionInterpolation( lastBonusInfo.textImage.h, startHeight, maxHeight, lastBonusInfo.t * 1.5f );
		if ( lastBonusInfo.t >= 1.0f )
		{
			SetWave( lastBonusInfo.textImage, 1.0f );
			return	true;
		}
		return	false;
	}

	//	�v���C���[���\
	bool	sceneResult::PlayerAnnouncing( void )
	{
		if ( lastBonusInfo.t >= 1.0f )	return	true;

		//	��摜�g��
		if ( bonusPlayer != NO_BONUS )
		{
			//	�p�����[�^�X�V
			lastBonusInfo.t += 0.1f;
			if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;
			float	t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, lastBonusInfo.t );
			int		startWidth = static_cast<int>( iexSystem::ScreenWidth * 0.08f );
			int		startHeight = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
			int		endWidth = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			int		endHeight = static_cast<int>( iexSystem::ScreenHeight * 0.18f );
			CubicFunctionInterpolation( faceImage.w, startWidth, endWidth, lastBonusInfo.t );
			CubicFunctionInterpolation( faceImage.h, startHeight, endHeight, lastBonusInfo.t );
		}
		else
		{
			//	�Y���Ȃ�
			//	�p�����[�^�X�V
			lastBonusInfo.t += 0.01f;
			if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

			float	t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, lastBonusInfo.t );
			int	startPos = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
			int	endPos = static_cast<int>( iexSystem::ScreenHeight * 0.55f );
			Lerp( notApplicable.alpha, 0.0f, 1.0f, lastBonusInfo.t );
			Lerp( notApplicable.y, startPos, endPos, lastBonusInfo.t );
		}

		//	�������I�����Ă���true��������
		if ( lastBonusInfo.t >= 1.0f )
		{
			if ( bonusPlayer != NO_BONUS )	SetWave( waveCircleImage, 2.0f );
			return	true;
		}
		return	false;
	}

	//	���C�t���\
	bool	sceneResult::NextLifeAnnouncing( void )
	{
		//	�ϐ�����
		bool	isEnd = false;

		switch ( lifeInfo.step )
		{
		case 0:
			lifeInfo.t += 0.01f;
			if ( lifeInfo.t >= 1.0f ) lifeInfo.t = 1.0f;

			//	���K�p
			FOR( 0, PLAYER_MAX )
			{
				Lerp( rankImage[value].alpha, 1.0f, 0.0f, lifeInfo.t );
			}

			if ( lifeInfo.t >= 1.0f )
			{
				//	���ʔ�\�������C�t�`��J�n
				FOR( 0, PLAYER_MAX )
				{
					rankImage[value].renderflag = false;
					lifeImage[value].renderflag = true;
				}
				lifeInfo.t = 0.0f;

				//	���̃X�e�b�v��
				lifeInfo.step++;
			}
			break;

		case 1:
			lifeInfo.t += 0.1f;
			if ( lifeInfo.t >= 1.0f )
			{
				lifeInfo.t = 1.0f;
				FOR( 0, PLAYER_MAX )	SetWave( lifeImage[value], 1.0f );
				lifeInfo.step++;
			}
			//	���C�t�o��
			FOR( 0, PLAYER_MAX )
			{
				Lerp( lifeImage[value].w, 0, lifeInfo.maxW, lifeInfo.t );
				Lerp( lifeImage[value].h, 0, lifeInfo.maxH, lifeInfo.t );
			}
			break;

		case 2:
			FOR( 0, PLAYER_MAX )
			{
				isEnd = WaveUpdate( lifeImage[value] );
			}
			if ( isEnd )	lifeInfo.step++;
			break;

		case 3:
			SetNextLife();
			//	���C�t���X�V
			FOR( 0, PLAYER_MAX )
			{
				lifeInfo.culLife = gameManager->GetStartLife( value );
				lifeImage[value].sx = lifeImage[value].sw * ( ( 5 - lifeInfo.culLife ) % 4 );
				lifeImage[value].sy = lifeImage[value].sh * ( ( 5 - lifeInfo.culLife ) / 4 );
			}

			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				step = RESULT_MODE::INPUT_WAIT;
			}
			return	true;
			break;
		}

		return	false;
	}

	//	���o�X�L�b�v
	void	sceneResult::ProductionSkip( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			SetNumberImageInfo( numberImageInfo[value], number[value], originInfo[value].num );
			SetWave( rankImage[value], 1.5f );
			rankImage[value].renderflag = true;
		}
		step = RESULT_MODE::RANK_SKIP;
	}

	//	�{�[�i�X���Z���o
	bool	sceneResult::AddBonus( void )
	{
		static	const		int WAIT_TIME	=	90;		//	�J�E���g����
		static	int			addBonusStep	=	0;			//	switch���p�X�e�b�v
		static	int			waitTimer			=	0;			//	�ҋ@���ԃJ�E���g
		static	int			temp[4] = { 0, 0, 0, 0 };	//	�ޔ�p
		bool					isEnd					=	false;	//	�I���`�F�b�N�p�ϐ�

		switch ( addBonusStep )
		{
		case	0:
			//	��莞�ԂŃ{�[�i�X���\���ɂ���
			waitTimer++;
			if ( waitTimer == WAIT_TIME )
			{
				waitTimer = 0;
				faceImage.renderflag = false;
				playerNumImage.renderflag = false;
				notApplicable.renderflag = false;
				lastBonusInfo.textImage.renderflag = false;
				addBonusStep++;
			}
			break;

		case 1:
			//	�{�[�h�ޔ�
			isEnd = OutBoard();

			//	�{�[�h�ޔ��I����A�{�[�i�X�̐��l�̕`���ON�ɂ���
			if ( isEnd )
			{
				FOR( 0, PLAYER_MAX )
				{
					//	�ޔ�p�ϐ��Ƀ{�[�i�X���R�s�[
					temp[value] = originInfo[value].bonus;

					//	�{�[�i�X������Ε\��
					if ( originInfo[value].bonus > 0 )
					{
						if ( bonusNumberImageInfo[value].hundredRenderFlag )	bonusNumberImageInfo[value].hundred.renderflag = true;
						bonusNumberImageInfo[value].one.renderflag = true;
						bonusNumberImageInfo[value].ten.renderflag = true;
					}
				}
				addBonusStep++;
			}
			break;

		case 2:
			//	�ҋ@
			waitTimer++;
			if ( waitTimer == WAIT_TIME )
			{
				waitTimer = 0;
				FOR( 0, PLAYER_MAX )
				{
					//	�{�[�i�X��\��
					bonusNumberImageInfo[value].hundred.renderflag = false;
					bonusNumberImageInfo[value].one.renderflag = false;
					bonusNumberImageInfo[value].ten.renderflag = false;
				}

				addBonusStep++;
			}
			break;

		case	3:
			//	���l�Ƀ{�[�i�X�������Z����
			FOR( 0, PLAYER_MAX )
			{
				if ( temp[value] == 0 )	continue;
				
				//	���̐��l�ɉ��Z���đޔ�p���猸�Z
				originInfo[value].num++;
				temp[value]--;

				SetNumberImageInfo( numberImageInfo[value], number[value], originInfo[value].num );
				SetNumberImageInfo( bonusNumberImageInfo[value], bonusNumber[value], temp[value] );
			}

			if ( temp[0] + temp[1] + temp[2] + temp[3] == 0 )		addBonusStep++;
			break;

		case	4:
			//	�ҋ@
			waitTimer++;
			if ( waitTimer == WAIT_TIME )
			{
				waitTimer = 0;	
				FOR( 0, PLAYER_MAX )
				{
					//	�{�[�i�X��\��
					bonusNumberImageInfo[value].hundred.renderflag = true;
					bonusNumberImageInfo[value].one.renderflag = false;
					bonusNumberImageInfo[value].ten.renderflag = false;
				}
				
				addBonusStep++;
			}
			break;

		case	5:
			addBonusStep = 0;
			return	true;
			break;
		}

		return	false;
	}

//----------------------------------------------------------------------------
//	���U���g���[�h�֐�
//----------------------------------------------------------------------------

	//	���[���b�g
	void	sceneResult::ModeRoulette( void )
	{
		bool	isEnd = false;

		isEnd = Roulette();

		//	����{�^���ŃX�L�b�v
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
		{
			ProductionSkip();
		}

		//	��]���I��������
		if ( isEnd )		step = RESULT_MODE::RANK;
	}
	
	//	���X�g�{�[�i�X���\
	void	sceneResult::ModeLastBonus( void )
	{
		bool	isEnd = false;
		//-----------------------------------------------------------------------------------
		//	���X�g�{�[�i�X���o
		//-----------------------------------------------------------------------------------
		isEnd = LastBonusUpdate();
		if ( isEnd )	step++;
	}

	//	�����N���\
	void	sceneResult::ModeRank( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false;
		bool	isEndWave = false;
		int	round = gameManager->GetRound();

		//-----------------------------------------------------------------------------------
		//	���ʔ��\
		//-----------------------------------------------------------------------------------
		isFinViewRankInOrder = ViewRankInOrder();
		isEnd = RankWave();

		//	�g��I����ɑI����
		if ( isFinViewRankInOrder && isEnd )
		{
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else															step = RESULT_MODE::LAST_RESULT;
			}
		}
		else
		{
			//	����{�^���ŃX�L�b�v
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				ProductionSkip();
			}
		}
	}

	//	�����N���\�X�L�b�v
	void	sceneResult::ModeRankSkip( void )
	{
		bool	isEnd = false;
		int	round = gameManager->GetRound();

		FOR( 0, PLAYER_MAX )	isEnd = WaveUpdate( rankImage[value] );

		//	�g��I����ɑI����
		if ( isEnd )
		{
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else
				{
					step = RESULT_MODE::LAST_RESULT;
				}
			}
		}
	}

	//	���X�g���ʔ��\
	void	sceneResult::ModeLastResult( void )
	{
		step = 0;
		mode = MOVE_MODE::LAST_RESULT;
	}

	//	���͑҂�
	void	sceneResult::ModeInputWait( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			//	���͂��󂯕t���Ă�����X�L�b�v
			if ( inputCheck[value] )	continue;
	
			//	�L�[���͎擾
			int key_space = input[value]->Get( KEY_SPACE );
			int key_a = input[value]->Get( KEY_A );

			//	���͎�t
			if ( key_space == 3 || key_a == 3 )
			{
				checkImage[value].renderflag = true;
				inputCheck[value] = true;
			}
		}

		//	�S���̓��͂��I����Ă�����
		if ( inputCheck[0] && inputCheck[1] && inputCheck[2] && inputCheck[3] )
		{
			screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
			changeScene = true;
		}
	}

//----------------------------------------------------------------------------
//	�I�����[�h�֐�
//----------------------------------------------------------------------------

	//	�����낵
	bool	sceneResult::DownPolygon( void )
	{
		bool	isEnd = false;
		menuInfo.t += 0.03f;
		if ( menuInfo.t >= 1.0f )
		{
			menuInfo.t = 1.0f;

			//	���j���[���ڕ`��
			static	int	MENU_MAX = 3;	//	���j���[���ڐ�
			for ( int i = 0; i < MENU_MAX; i++ )		menuImage[i].renderflag = true;
		}
		isEnd = Lerp( menuInfo.screenH, 0, static_cast<int>( iexSystem::ScreenHeight ), menuInfo.t );

		return	isEnd;
	}

	//	�J�[�\������
	void	sceneResult::ControlCursor( void )
	{
		//	�㉺�őI��
		if ( input[0]->Get( KEY_UP ) == 3 )		menuInfo.select--;
		if ( input[0]->Get( KEY_DOWN ) == 3 )	menuInfo.select++;
		if ( menuInfo.select < 0 )							menuInfo.select = MENU::END - 1;
		if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::MOVE_MENU;

		//	�ǂݍ��݈ʒu�ύX
		for ( int i = 0; i < 2; i++ )
		{
			if ( i == menuInfo.select )	menuImage[i].sx = 512;
			else										menuImage[i].sx = 0;
		}

		//	����
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
		{
			changeScene = true;
		}
	}
	