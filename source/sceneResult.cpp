
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"CharacterManager.h"
#include	"Curtain.h"
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
#define	SCREEN_SPEED	1.5f

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
				FINALRESULT,
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
		//screen->SetScreenMode( SCREEN_MODE::FADE_IN , SCREEN_SPEED );

		//	���݂̃��E���h���擾
		culRound = gameManager->GetRound();
		
		//	�Đ�p�̐ݒ�
		FOR( 0, PLAYER_MAX )
		{
			//	�����E���h�̊l���R�C��������ݒ�
			gameManager->SetTotalCoin( culRound, value, gameManager->GetCoinNum( value ) );
		}

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

		//	���[���b�g��񏉊���
		RouletteInfoInitialize();

		//	���j���[�֘A������
		MenuInfoInitialize();

		//	�����_�[�^�[�Q�b�g�֘A������
		RenderTargetTextureInitialize();

		//	���E���h�֘A������
		RoundInfoInitialize();
		SetRoundCoinNumber();

		//	�D���Ґݒ�
		SetWinner();

		//	�ϐ�������
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;
		bonusPlayer = 0;		
		FOR( 0, PLAYER_MAX )	inputCheck[value] = false;

		//	�X�N���[���A�T�E���h������
		sound->PlayBGM( BGM::RESULT_BGM );
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );
		return	true;
	}

	//	���
	void	sceneResult::Release( void )
	{
		SafeDelete( pressButtonImage.obj );
		SafeDelete( waveImage.obj );
		SafeDelete( playerNumImage.obj );
		SafeDelete( faceImage.obj );
		SafeDelete( lifeAnnounceImage.obj );
		SafeDelete( orgCurtain );
		SafeDelete( orgRound );
		SafeDelete( life );
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		SafeDelete( infoScreen );
		SafeDelete( lastResultTest );
		SafeDelete( bgStage );
		SafeDelete( check );
		SafeDelete( winnerBack );
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
		life = new iex2DObj("DATA/UI/Nlife.png");
		check = new iex2DObj( "DATA/UI/Result/check.png" );
		orgRound = new iex2DObj( "DATA/UI/roundText.png" );
		orgCurtain = new iex2DObj( "DATA/UI/title/curtain1.png" );
		lifeAnnounceImage.obj = new iex2DObj( "DATA/UI/result/resultText.png" );
		faceImage.obj = new iex2DObj( "DATA/UI/chara_emotion.png" );
		playerNumImage.obj = new iex2DObj( "DATA/UI/DonketuUI.png" );
		waveImage.obj = new iex2DObj( "DATA/UI/Rainbow-circle.png" );
		pressButtonImage.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		winnerBack = new iex2DObj( "DATA/UI/Result/Last-result-back.png" );

		//	���C�t���\�e�L�X�g�摜������
		ImageInitialize( lifeAnnounceImage, static_cast<int>( iexSystem::ScreenWidth * 0.5f ), static_cast<int>( iexSystem::ScreenHeight * 0.35f ), 300, 200, 0, 0, 512, 256 );
		lifeAnnounceImage.alpha = 0.0f;
		lifeAnnounceImage.renderflag = false;

		//	���j���[�w�b�h�摜�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );

		//	PressButtonImage������
		x = static_cast<int>( iexSystem::ScreenWidth * 0.8f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.9f );
		ImageInitialize( pressButtonImage, x, y, w, h, 0, 0, 256, 128 );
		pressButtonImage.flashingRenderflag = false;

		//	���f���ǂݍ���
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>(LPSTR("DATA/CHR/majo/majo.IEM"));			//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>(LPSTR("DATA/CHR/�v�����Z�X/prinsess1.IEM"));					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));				//	���X
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));					//	�g��
		orgCannon = make_unique<iexMesh>( LPSTR( "DATA/CHR/Pirate/pirate_cannon.IMO" ) );
		FOR(0, PLAYER_MAX)	cannon[value] = orgCannon->Clone();
		
		//	�X�e�[�W�ݒ�
		bgStage = new iexMesh("DATA/BG/MenuStage/menustage.IMO");	//	�X�e�[�W
		bgStage->SetScale(0.1f);
		bgStage->SetAngle(D3DX_PI);
		bgStage->Update();

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale(0.015f);	//	����
		org[CHARACTER_TYPE::PRINCESS]->SetScale(0.04f);		//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale(0.025f);				//	����
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.04f);				//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle(D3DX_PI);	//	����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle(D3DX_PI);		//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle(D3DX_PI);				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetAngle(D3DX_PI);			//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(0);		//	����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(0);			//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);	 				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	����
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	�P
		org[CHARACTER_TYPE::THIEF]->Update();							//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->Update();						//	�C��

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
		viewInfo.pos = Vector3( 0.0f, 10.0f, -10.0f );
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
			obj[i]->SetPos( -9.0f + ( 19.0f / 3.0f * i ), 3.0f, 10.0f );
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

			totalSortInfo[i].num = 0;
			totalSortInfo[i].rank = i;
			totalSortInfo[i].sortRank = i;
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
			w = 0;
			h = 0;
			sw = 64;
			sh = 64;
			sx = sw * ( ( 5 - culLife ) % 4 );
			sy = sh * ( ( 5 - culLife ) / 4 );
			ImageInitialize( lifeImage[value], x, y, w, h, sx, sy, sw, sh );

			//	�ŏ��͔�\��
			lifeImage[value].renderflag = false;

			//	���ł�OK�̈ʒu���ݒ�
			w = 300;
			h = 200;
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
		LIGHT_POS[0] = Vector3( 0.0f, 0.0f, 0.0f );
		LIGHT_POS[1] = Vector3( static_cast<float>( iexSystem::ScreenWidth ), 0.0f, 0.0f );
		LIGHT_POS[2] = Vector3( 0.0f, static_cast<float>( iexSystem::ScreenHeight ), 0.0f );
		LIGHT_POS[3] = Vector3( static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight ), 0.0f );

		lightSize[0] = 200.0f;
		lightSize[1] = 150.0f;
		lightSize[2] = 200.0f;
		lightSize[3] = 150.0f;

		shader2D->SetValue("light_Size[0]", lightSize[0]);
		shader2D->SetValue("light_Size[1]", lightSize[1]);
		shader2D->SetValue("light_Size[2]", lightSize[2]);
		shader2D->SetValue("light_Size[3]", lightSize[3]);

		FOR(0, 4)
		{
			lightPos[value] = LIGHT_POS[value];
			MoveLightSet(value, RandomPos());
		}

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

		FOR( 0, Round::END + 1 ) 
		{
			y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.15f ) * value );
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
		int x, y, w, h, sx, sy, sw, sh;
		int x2, y2, w2, h2, sx2, sy2, sw2, sh2;

		//	�e���E���h�̃R�C���������擾
		FOR( 0, PLAYER_MAX )
		{
			//	�e�v���C���[�̊�摜�ݒ�
			y = roundImage[0].y - static_cast<int>( iexSystem::ScreenHeight * 0.15f );
			w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			h = w;
			sw = 256;
			sh = 256;
			sx = 0;
			sy = gameManager->GetCharacterType( value ) * sh;

			for ( int round = 0; round <= Round::END; round++ ) 
			{
				if ( round != Round::END )
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

					x = roundCoinNumberImageInfo[round][value].pos.x;
				}
				else
				{
					//	���W��ݒ�
					totalNumberImageInfo[value].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.4f ) + ( static_cast<int>( iexSystem::ScreenWidth * 0.15f ) * value );
					totalNumberImageInfo[value].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.15f ) * round );
					totalNumberImageInfo[value].scale = 100;
					totalNumberImageInfo[value].one.obj = originNumber;
					totalNumberImageInfo[value].ten.obj = originNumber;
					totalNumberImageInfo[value].hundred.obj = originNumber;
				}
			}

			//	��摜������
			ImageInitialize( faceImageObj[value], x, y, w, h, sx, sy, sw, sh );
			faceImageObj[value].obj = faceImage.obj;

			//	�v���C���[�ԍ�������
			x2 = x - static_cast<int>( iexSystem::ScreenWidth * 0.05f );
			y2 = y + static_cast<int>( iexSystem::ScreenHeight * 0.05f );
			w2 = static_cast<int>( iexSystem::ScreenWidth * 0.07f );
			h2 = w2;
			sw2 = 128;
			sh2 = 128;
			sx2 = value * sw2;
			sy2 = 256;
			ImageInitialize( playerNumImageObj[value], x2, y2, w2, h2, sx2, sy2, sw2, sh2 );
			playerNumImageObj[value].obj = playerNumImage.obj;
		}
	}

	//	�D���Ҍ���
	void	sceneResult::SetWinner( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			for ( int round = 0; round < Round::END; round++ )
			{
				//	�e���E���h�̃R�C���̍��v�����߂�
				totalSortInfo[value].num += gameManager->GetTotalCoin( round, value );
			}
			SetNumberImageInfo( totalNumberImageInfo[value], totalNumber[value], totalSortInfo[value].num );
		}

		Sort( totalSortInfo );
		
		//	��摜�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.55f );
		int w = 0;
		int h = 0;
		int sw = 256;
		int sh = 256;
		int sx = 0;
		int sy = gameManager->GetCharacterType( totalSortInfo[0].rank ) * sh;
		ImageInitialize( faceImage, x, y, w, h, sx, sy, sw, sh );
		faceImage.renderflag = false;

		//	�g��摜������
		w = 900;
		h = 900;
		sx = 0;
		sy = 0;
		sw = 512;
		sh = 512;
		ImageInitialize( waveImage, x, y, w, h, sx, sy, sw, sh );

		//	�v���C���[�ԍ��摜�\���̏�����
		x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
		w = 0;
		h = 0;
		sw = 128;
		sh = 128;
		sx = totalSortInfo[0].rank * sw;
		sy = 256;
		ImageInitialize( playerNumImage, x, y, w, h, sx, sy, sw, sh );
		playerNumImage.renderflag = false;

		//	���X�g���\�\���̏�����
		lastAnnounceInfo.step = 0;
		lastAnnounceInfo.t = 0.0f;

	}

//----------------------------------------------------------------------------
//	�S�̍X�V�E�S�̕`��
//----------------------------------------------------------------------------

	//	�X�V
	void	sceneResult::Update( void ) 
	{
		//	���f���X�V
		ModelUpdate();

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
		//	�J�[�e���X�V
		curtainState = CurtainUpdate();

		//	pressButton�X�V
		PressButtonUpdate();
	
		//	�V�[���ړ��Ǘ�
		MoveScene();
		
		//	�X�N���[���X�V
		screen->Update();
	}

	//	�`��
	void	sceneResult::Render( void ) 
	{
		switch ( mode )
		{
		case MOVE_MODE::RESULT:
			ResultRender();
			break;

		case MOVE_MODE::SELECT:
		case MOVE_MODE::LAST_RESULT:
			LastResultRender();
			break;
		}
		//	pressButton�`��
		PressButtonImageRender();

		//	�J�[�e���`��
		CurtainRender();

		//	�ŏI���ʃe�X�g
		SelectRender();

		screen->Render();
	}

//----------------------------------------------------------------------------
//	�X�V
//----------------------------------------------------------------------------

	//	���U���g���̍X�V
	void	sceneResult::ResultUpdate( void )
	{
		if ( changeScene )	return;
		
		//	�i�K���Ƃ̏���
		switch ( step )
		{
		case RESULT_MODE::ROULETTE:
			ModeRoulette();
			break;
			
		case RESULT_MODE::RANK:
			ModeRank();
			break;

		case	RESULT_MODE::LIFE:
			NextLifeAnnouncing();
			break;

		case RESULT_MODE::LAST_RESULT:
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
			step = LASTRESULT_MODE::LIGHT_PRODUCTION;
			break;

		case LASTRESULT_MODE::LIGHT_PRODUCTION:
			isEnd = WaitTimeUpdate();
			//	���C�g
			LightUpdate();
			MoveLight();

			//	���o�I����J�[�e�����J���D���Ҕ��\��
			if ( isEnd )
			{
				step = LASTRESULT_MODE::RESULT;
				SetCurtainMode( CURTAIN_MODE::OPEN );
			}
			break;

		case LASTRESULT_MODE::RESULT:
			if ( curtainState )		LastRank();
			break;

		case LASTRESULT_MODE::FINALRESULT:
			if ( screen->GetScreenState() )
			{
				pressButtonImage.flashingRenderflag = true;
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) )
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

	//	���C�g�ړ�
	bool	sceneResult::LightUpdate( void )
	{
		FOR(0, 4){
			//	�p�����[�^���Z
			light_t[value] += 1.0f / 30.0f;

			if (light_t[value] >= 1.0f)	light_t[value] = 1.0f;
		}
		return	false;
	}

	//	���̓`�F�b�N�摜�X�V
	bool	sceneResult::InputCheckImageUpdate( void )
	{
		waveImage.flashingRenderflag = true;
		bool	isEnd[PLAYER_MAX] = { false, false, false, false };

		FOR( 0, PLAYER_MAX )
		{
			if ( !inputCheck[value] )	continue;
			isEnd[value] = WaveUpdate( checkImage[value] );
		}

		int count = 0;
		FOR( 0, PLAYER_MAX )
		{
			if ( isEnd[value] )	count++;
		}
		if ( count >= gameManager->GetPlayerNum() )	return	true;
		return	false;
	}

	//	PressButton�X�V
	void	sceneResult::PressButtonUpdate( void )
	{
		FlashingUpdate( pressButtonImage, 0.1f );
	}

	//	���f���X�V
	void	sceneResult::ModelUpdate( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->Update();
		}

		SetCannonPos();
	}

//----------------------------------------------------------------------------
//	�`��
//----------------------------------------------------------------------------
	
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
		//	�J����
		mainView->Activate();
		mainView->Clear();

		//	�w�i( ��Ԍ��ɕ\�� )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	�v���C���[�`��
		FOR( 0, PLAYER_MAX )
		{
			//	�g�D�[���֊s���F�ݒ�
			shader3D->SetValue( "OutlineColor", gameManager->GetPlayerColor( value ) );
			obj[value]->Render( shader3D, "toon" );
		}

		//	��C�`��
		CannonRender();

		//	���U���g�V�[���`��
		RenderImage( menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM );

		//	���l�`��
		NumberImageRender();

		//	���ʕ`��
		RankRender();

		//	���C�t�`��
		LifeRender();

		//	���̓`�F�b�N�`��
		InputCheckRender();		
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
		//	�e�L�X�g�`��
		RenderImage( lifeAnnounceImage, lifeAnnounceImage.sx, lifeAnnounceImage.sy, lifeAnnounceImage.sw, lifeAnnounceImage.sh, IMAGE_MODE::ADOPTPARAM );

		//	���C�t�`��
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
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::NORMAL );
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::WAVE );
		}
	}

	//	���X�g���\�`��
	void sceneResult::LastResultRender( void )
	{
		//-------------UI�֘A---------------//
		mainView->Activate();
		mainView->Clear();

		if ( step == LASTRESULT_MODE::RESULT )
		{
			WinnerRender();
			return;
		}

		//	�w�i�`��
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		
		//	���E���h���Ƃ�
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

		RenderImage( roundImage[3],roundImage[3].sx, roundImage[3].sy, roundImage[3].sw, roundImage[3].sh, IMAGE_MODE::NORMAL );

		FOR( 0, PLAYER_MAX )
		{
			NumberImageRender( totalNumberImageInfo[value] );
		}

		//	��A�v���C���[�ԍ��`��
		AllPlayerFaceRender();
		AllPlayerNumRender();
	}

	//	�J�[�e���`��
	void	sceneResult::CurtainRender( void )
	{
		LPSTR technique = "copy";

		if (step == LASTRESULT_MODE::LIGHT_PRODUCTION)
		{
			technique = "SpotLight";
			shader2D->SetValue( "lightPos[0]", lightPos[0] );
			shader2D->SetValue( "lightPos[1]", lightPos[1] );
			shader2D->SetValue( "lightPos[2]", lightPos[2] );
			shader2D->SetValue( "lightPos[3]", lightPos[3] );
		}
		iexPolygon::Render2D( curtainInfoL.tlv, 2, curtainInfoL.obj, shader2D, technique );
		iexPolygon::Render2D( curtainInfoR.tlv, 2, curtainInfoR.obj, shader2D,technique );
	}

	//	�D���Ҋ֘A�`��
	void	sceneResult::WinnerRender( void )
	{
		winnerBack->Render( 0, 0, ( int )iexSystem::ScreenWidth, ( int )iexSystem::ScreenHeight, 0, 0, 1280, 720 );

		//	�g��`��
		RenderImage( waveImage, waveImage.sx, waveImage.sy, waveImage.sw, waveImage.sh, IMAGE_MODE::WAVE );

		//	��摜�`��
		RenderImage( faceImage, faceImage.sx, faceImage.sy, faceImage.sw, faceImage.sh, IMAGE_MODE::ADOPTPARAM );

		//	�v���C���[�ԍ��`��
		RenderImage( playerNumImage, playerNumImage.sx, playerNumImage.sy, playerNumImage.sw, playerNumImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	PressButtonRender
	void	sceneResult::PressButtonImageRender( void )
	{
		RenderImage( pressButtonImage, pressButtonImage.sx, pressButtonImage.sy, pressButtonImage.sw, pressButtonImage.sh, IMAGE_MODE::FLASH );
	}

	//	��C�`��
	void	sceneResult::CannonRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetCharacterType( value ) == CHARACTER_TYPE::PIRATE )
			{
				//	�g�D�[���֊s���F�ݒ�
				shader3D->SetValue( "OutlineColor", gameManager->GetPlayerColor( value ) );
				cannon[value]->Render( shader3D, "toon" );
			}
		}
	}

	//	�S�v���C���[��`��
	void	sceneResult::AllPlayerFaceRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( faceImageObj[value], faceImageObj[value].sx, faceImageObj[value].sy, faceImageObj[value].sw, faceImageObj[value].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	�S�v���C���[�ԍ��`��
	void	sceneResult::AllPlayerNumRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( playerNumImageObj[value], playerNumImageObj[value].sx, playerNumImageObj[value].sy, playerNumImageObj[value].sw, playerNumImageObj[value].sh, IMAGE_MODE::NORMAL );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���C�g�Ɖ�ʂ̂����蔻��
	bool	sceneResult::WallLightCheck( int num )
	{
		if (lightPos[num].x - lightSize[num] <= 0.0f ||		//	��
			lightPos[num].x + lightSize[num] >= iexSystem::ScreenWidth ||		//	�E
			lightPos[num].y - lightSize[num] <= 0.0f ||		//	��
			lightPos[num].y + lightSize[num] >= iexSystem::ScreenHeight)		//	��
		{
			return true;
		}
			return false;

	}

	//	��ʏ�̃����_���|�W�V�������l��
	Vector3	sceneResult::RandomPos( void )
	{
		return Vector3( Random::GetFloat( 0, static_cast<float>( iexSystem::ScreenWidth ) ), Random::GetFloat( 0, static_cast<float>( iexSystem::ScreenHeight ) ), 0 );
	}

	//	���C�g�̏��ݒ�
	bool	sceneResult::MoveLightSet( int num, Vector3 pos )
	{
		light_t[num] = 0.0f;
		lightMove_start[num] = lightPos[num];
		lightMove_finish[num] = pos;

		return false;
	}

	//	���C�g�̈ړ�
	void	sceneResult::MoveLight( void )
	{
		FOR(0, 4)
		{
			Lerp(lightPos[value], lightMove_start[value], lightMove_finish[value], light_t[value]);
			if ((WallLightCheck(value) == true || light_t[value] >= 1.0f) && waitTimer > 30)
			{
				MoveLightSet(value, RandomPos());
			}
		}
		//	�c��30�b�ɏ����ʒu�ֈړ�
		if (waitTimer == 30)
		{
			MoveLightSet(0, LIGHT_POS[0]);
			MoveLightSet(1, LIGHT_POS[1]);
			MoveLightSet(2, LIGHT_POS[2]);
			MoveLightSet(3, LIGHT_POS[3]);
		}

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
				sound->PlaySE( SE::RESULT_JAN );
				rouletteInfo.timer = 1;
			}
		}
		else
		{
			if ( rouletteInfo.timer % 50 == 0 )
			{
				if ( rouletteInfo.step != PLAYER_MAX )sound->PlaySE(SE::RESULT_JAN);
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
		bool	screenState = screen->GetScreenState();
		if ( !screenState )	return;

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

	//	���C�t���\
	bool	sceneResult::NextLifeAnnouncing( void )
	{
		//	�ϐ�����
		bool	isEnd = false;

		//	�p�����[�^�X�V
		lifeInfo.t += 0.03f;
		if ( lifeInfo.t >= 1.0f )	lifeInfo.t = 1.0f;

		switch ( lifeInfo.step )
		{
		//	���C�t���\�e�L�X�g�\��
		case 0:
			lifeAnnounceImage.renderflag = true;
			isEnd = Lerp( lifeAnnounceImage.alpha, 0.0f, 1.0f, lifeInfo.t );
			Lerp( lifeAnnounceImage.w, 0, 500, lifeInfo.t );
			Lerp( lifeAnnounceImage.h, 0, 400, lifeInfo.t );

			if ( isEnd )
			{
				lifeInfo.step++;
				lifeInfo.t = 0.0f;
			}
			break;

		//	�����N����
		case 1:
			//	�����x�X�V
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

		case 2:
			if ( lifeInfo.t >= 1.0f )
			{
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

		case 3:
			FOR( 0, PLAYER_MAX )
			{
				isEnd = WaveUpdate( lifeImage[value] );
			}
			if ( isEnd )
			{
				lifeInfo.t = 0.0f;
				lifeInfo.step++;
			}
			break;

		case 4:
			SetNextLife();
			//	���C�t���X�V
			FOR( 0, PLAYER_MAX )
			{
				lifeInfo.culLife = gameManager->GetStartLife( value );
				lifeImage[value].sx = lifeImage[value].sw * ( ( 5 - lifeInfo.culLife ) % 4 );
				lifeImage[value].sy = lifeImage[value].sh * ( ( 5 - lifeInfo.culLife ) / 4 );
			}
			//lifeAnnounceImage.renderflag = false;

			isEnd = Lerp( lifeAnnounceImage.alpha, 1.0f, 0.0f, lifeInfo.t );

			if ( isEnd )
			{
				step = RESULT_MODE::INPUT_WAIT;
				return	true;
			}
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

	//	��C�ʒu�ݒ�
	void	sceneResult::SetCannonPos( void )
	{
		Matrix	mat;
		Matrix	cannonMat;
		Vector3	up = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	cannonPos = Vector3( 0.0f, 0.0f, 0.0f );
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetCharacterType( value ) != CHARACTER_TYPE::PIRATE )	continue;
			mat = *obj[value]->GetBone( 7 ) * obj[value]->TransMatrix;
			cannon[value]->TransMatrix = mat;
			up = Vector3( mat._21, mat._22, mat._23 );
			up.Normalize();
			cannonPos = Vector3( cannon[value]->TransMatrix._41, cannon[value]->TransMatrix._42, cannon[value]->TransMatrix._43 );
			cannonPos += up * 0.5f;
			cannon[value]->TransMatrix._41 = cannonPos.x;
			cannon[value]->TransMatrix._42 = cannonPos.y;
			cannon[value]->TransMatrix._43 = cannonPos.z;
		}
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
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
		{
			ProductionSkip();
		}

		//	��]���I��������
		if ( isEnd )		step = RESULT_MODE::RANK;
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
			pressButtonImage.flashingRenderflag = true;
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else															step = RESULT_MODE::LAST_RESULT;
				pressButtonImage.flashingRenderflag = false;
			}
		}
		else
		{
			//	����{�^���ŃX�L�b�v
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				ProductionSkip();
			}
		}
	}

	//	�ŏI�����N���\
	void	sceneResult::LastRank( void )
	{
		bool	isEnd = false;

		faceImage.renderflag = true;
		playerNumImage.renderflag = true;

		lastAnnounceInfo.t += 0.03f;
		if ( lastAnnounceInfo.t >= 1.0f )	lastAnnounceInfo.t = 1.0f;

		switch ( lastAnnounceInfo.step )
		{
		case 0:
			//	�g��
			CubicFunctionInterpolation( faceImage.w, 0, 700, lastAnnounceInfo.t );
			isEnd = CubicFunctionInterpolation( faceImage.h, 0, 700, lastAnnounceInfo.t );
			if ( isEnd )
			{
				sound->PlaySE( SE::RESULT_JAN );
				SetWave( waveImage, 1.0f );
				lastAnnounceInfo.t = 0.0f;
				lastAnnounceInfo.step++;
			}
			break;

		case 1:
			isEnd = WaveUpdate( waveImage );
			if ( isEnd )
			{
				lastAnnounceInfo.t = 0.0f;
				lastAnnounceInfo.step++;
			}
			break;

		case 2:
			//	�g��
			Lerp( playerNumImage.w, 0, 200, lastAnnounceInfo.t );
			isEnd = Lerp( playerNumImage.h, 0, 200, lastAnnounceInfo.t );
			if ( isEnd )
			{
				pressButtonImage.flashingRenderflag = true;
				if ( input[0]->Get( KEY_B ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
				{
					screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 2.0f );
					pressButtonImage.flashingRenderflag = false;
					lastAnnounceInfo.step++;
				}
			}
			break;

		case 3:
			if ( screen->GetScreenState() )
			{
				step = LASTRESULT_MODE::FINALRESULT;
				screen->SetScreenMode( SCREEN_MODE::FADE_IN, 2.0f );
			}
			break;
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
			pressButtonImage.flashingRenderflag = true;
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				pressButtonImage.flashingRenderflag = false;
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
		static int result_step = 0;

		switch (result_step)
		{
		case LASTRESULT_MODE::SET_CLOSE_CURTAIN:
			SetCurtainMode(CURTAIN_MODE::CLOSE);
			sound->PlaySE( SE::DRAMROLL_SE );
			result_step++;
			break;

		case LASTRESULT_MODE::CLOSE_CURTAIN:
			if ( curtainState )
			{
				//sound->StopSE( SE::DRAMROLL_SE );
				SetWaitTimer(150);
				step = 0;
				mode = MOVE_MODE::LAST_RESULT;
				result_step = 0;
			}
			break;
		}
	}

	//	���͑҂�
	void	sceneResult::ModeInputWait( void )
	{
		bool	isEnd = false;
		pressButtonImage.flashingRenderflag = true;
		FOR( 0, PLAYER_MAX )
		{
			//	���͂��󂯕t���Ă�����X�L�b�v
			if ( inputCheck[value] )	continue;
	
			//	�L�[���͎擾
			int key_space = input[value]->Get( KEY_SPACE );
			int key_a = input[value]->Get( KEY_B );

			//	���͎�t
			if ( key_space == 3 || key_a == 3 )
			{
				if ( !inputCheck[value] )	sound->PlaySE( SE::DECIDE_SE );
				inputCheck[value] = true;
				checkImage[value].renderflag = true;
				SetWave( checkImage[value], 1.0f );
			}
		}

		isEnd = InputCheckImageUpdate();

		//	�S���̓��͂��I����Ă�����
		if ( isEnd )
		{
			pressButtonImage.flashingRenderflag = false;
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
		int	keyUp = input[0]->Get( KEY_UP );
		int	keyDown = input[0]->Get( KEY_DOWN );
		if ( keyUp == 3 )
		{
			sound->PlaySE( SE::CHOICE_SE );
			menuInfo.select--;
		}
		if ( keyDown == 3 )
		{
			sound->PlaySE( SE::CHOICE_SE );
			menuInfo.select++;
		}
		if ( menuInfo.select < 0 )							menuInfo.select = MENU::END - 1;
		if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::MOVE_MENU;

		//	�ǂݍ��݈ʒu�ύX
		int	textMax = 2;
		FOR( 0, textMax )
		{
			if ( value == menuInfo.select )	menuImage[value].sx = 512;
			else												menuImage[value].sx = 0;
		}

		// ����{�^���ŃV�[���ڍs�t���O�𗧂Ă�
		int keySpace = input[0]->Get( KEY_SPACE );
		int keyA = input[0]->Get(KEY_B);
		
		//	����L�[�Ō��聕�V�[���ڍs
		if ( !changeScene )
		{
			if ( keySpace == 3 || keyA == 3 )
			{
				sound->PlaySE( SE::DECIDE_SE );
				switch ( menuInfo.select )
				{
				case MENU::MOVE_MENU:
					screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.0f );
					break;

				case MENU::MOVE_TITLE:
					screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
					break;
				}
				changeScene = true;
			}
		}
	}
	