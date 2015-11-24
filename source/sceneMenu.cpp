
#include	"iextreme.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Sound.h"
#include	"Screen.h"
#include	"Camera.h"
#include	"Particle.h"
#include	"Random.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"
#include	"UI.h"
#include	"sceneMain.h"
#include	"sceneLoad.h"
#include	"sceneTitle.h"

#include	"sceneMenu.h"

//*******************************************************************************
//
//	sceneMenu�N���X
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

	namespace
	{
		namespace MENU_MODE
		{
			enum
			{
				INIT,
				SELECT_PLAYERNUM,
				SELECT_CHARACTER,
				SELECT_STAGE,
				SELECT_CHECK,
				MOVE_MAIN,
				MOVE_TITLE,
				OPTION,
			};
		}
	}

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	sceneMenu::sceneMenu( void )
	{
	
	}

	//	�f�X�g���N�^
	sceneMenu::~sceneMenu( void )
	{
		Release();
	}

	//	������
	bool	sceneMenu::Initialize( void )
	{
		//	camera
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		mainView->Set( Vector3( 0.0f, 5.2f, -70.0f ), Vector3( 0.0, 5.2f, 0.0f ) );

		//	random
		Random::Initialize();

		//	screen
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::WIPE_IN, 1.5f );

		//	�Q�[���}�l�[�W��������
		OptionInitialize();
		gameManager->Initialize();
		
		//	�摜�ǂݍ���
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum.png" ) );
		face = new iex2DObj( "DATA/UI/chara_emotion.png" );
		cursor = new iex2DObj( "DATA/UI/cursor.png" );


		Oimage = new iex2DObj("DATA/UI/OptionText.png");
		Otime = new iex2DObj("DATA/UI/number.png");
		OCmax = new iex2DObj("DATA/UI/number.png");
		//	���f���ǂݍ���
		//org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	�R�m
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );		//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Y2009/Y2009.IEM" ) );					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/SQUIRREL/SQUIRREL.IEM"));		//	���X
		org[CHARACTER_TYPE::TIGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );				//	�g��

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.02f );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale(0.04f);		//	���X
		org[CHARACTER_TYPE::TIGER]->SetScale( 0.02f );			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );	//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );	//	���X
		org[CHARACTER_TYPE::TIGER]->SetAngle( D3DX_PI );			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );		//	���X
		org[CHARACTER_TYPE::TIGER]->SetMotion( 0 );			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->Update();	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->Update();		//	�P
		org[CHARACTER_TYPE::THIEF ]->Update();		//	���X
		org[CHARACTER_TYPE::TIGER]->Update();			//	�g��

		deskStage = make_unique<iexMesh>( LPSTR( "DATA/back/stage.IMO" ) );
		forestStage = make_unique<iexMesh>( LPSTR( "DATA/BG/Forest/model/forest.IMO" ) );

		//	�摜�\���̏�����
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu-head.png" );
			int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			ImageInitialize( textImage, x, y, w, h, 512, 0, 512, 256 );
			textImage.angle = D3DXToRadian( 5.0f );
			textImage.renderflag = true;
		}

		//	���[�h�ݒ�
		SetMode( MENU_MODE::INIT );

		//	�S�̍X�V
		Update();
		return	true;
	}

	//	���
	void	sceneMenu::Release( void )
	{
		SafeDelete( mainView );
		SafeDelete( textImage.obj );
		SafeDelete( face );
		SafeDelete( cursor );
		Random::Release();

		//�݌v��
		SafeDelete(Oimage);
		SafeDelete(Otime);
		SafeDelete(OCmax);
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	sceneMenu::Update( void )
	{
		switch ( mode )
		{
		case MENU_MODE::INIT:
			if ( screen->GetScreenState() )
			{
				SetMode( MENU_MODE::SELECT_PLAYERNUM );
			}
			break;

		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumUpdate();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterUpdate();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageUpdate();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckUpdate();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainUpdate();
			break;

		case MENU_MODE::MOVE_TITLE:
			MoveTitleUpdate();
			break;

		case MENU_MODE::OPTION:
			OptionUpdate();
			break;
		}
		if (KEY_Get(KEY_B) == 3){
			tempmode = mode+1;
			SetMode(MENU_MODE::OPTION);
		}

		//	�X�N���[���X�V
		screen->Update();
	}

	//	�`��
	void	sceneMenu::Render( void )
	{
		//	camera
		mainView->Activate();
		mainView->Clear();

		//	�w�i( ��Ԍ��ɕ\�� )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		switch ( mode )
		{
		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumRender();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterRender();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageRender();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckRender();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainRender();
			break;
		case MENU_MODE::OPTION:
			OptionRender();
			break;
		}

		//	�X�N���[��
		screen->Render();
	}

//-------------------------------------------------------------------------------
//	�v���C�l���I�𓮍�
//-------------------------------------------------------------------------------

	//	������
	void	sceneMenu::SelectPlayerNumInitialize( void )
	{
		playerNumSelectInfo.num = 0;
		playerNumSelectInfo.t = 1.0f;
		playerNumSelectInfo.saveY = 0;
		playerNumSelectInfo.sy = 0;
	}

	//	�X�V
	void	sceneMenu::SelectPlayerNumUpdate( void )
	{
		//	�p�����[�^���Z
		playerNumSelectInfo.t += 0.08f;
		if ( playerNumSelectInfo.t >= 1.0f )	playerNumSelectInfo.t = 1.0f;	

		//	�ړ����I�������
		if ( playerNumSelectInfo.t >= 1.0f )
		{
			//	�I��
			if ( input[0]->Get( KEY_DOWN ) == 1 )
			{
				//	���̍��W��ۑ�
				playerNumSelectInfo.saveY =  128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num++;
				if ( playerNumSelectInfo.num >= 4 )
				{
					playerNumSelectInfo.num = 3;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			if ( input[0]->Get( KEY_UP ) == 1 )
			{
				//	���̍��W��ۑ�
				playerNumSelectInfo.saveY = 128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num--;
				if ( playerNumSelectInfo.num < 0 )
				{
					playerNumSelectInfo.num = 0;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			//	����
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				gameManager->SetPlayerNum( playerNumSelectInfo.num + 1 );
				SetMode( MENU_MODE::SELECT_CHARACTER );
			}
		}

		if ( input[0]->Get( KEY_B ) == 3 )
		{
			SetMode( MENU_MODE::MOVE_TITLE );
		}

		//	���
		CubicFunctionInterpolation( playerNumSelectInfo.sy, playerNumSelectInfo.saveY, 128 * playerNumSelectInfo.num, playerNumSelectInfo.t );
	}

	//	�`��
	void	sceneMenu::SelectPlayerNumRender( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.28f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.08f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
		playerNum->Render( x, y, w, h, 0, playerNumSelectInfo.sy, 128, 128 );

		x = static_cast<int>( iexSystem::ScreenWidth * 0.35f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.41f );
		w = static_cast<int>( iexSystem::ScreenWidth * 0.39f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.138f );
		playerNumText->Render( x, y, w, h, 0, 0, 512, 128 );
	}

//-------------------------------------------------------------------------------
//	�L�����N�^�[�I�𓮍�
//-------------------------------------------------------------------------------

	//	�L�����N�^�[�I��������
	void	sceneMenu::SelectCharacterInitialize( void )
	{
		//	���f���A�I����񏉊���
		FOR( 0, PLAYER_MAX )
		{
			//	���f���o�^�A������
			obj[value] = org[value]->Clone();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();

			//	�I����񏉊���
			characterSelectInfo.character[value] = value;
			characterSelectInfo.select[value] = false;
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.04f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.07f );
			ImageInitialize( cursorImage[value], 0, 0, w, h, 0, 0, 0, 0 );
			cursorImage[value].obj = cursor;

			if ( value >= gameManager->GetPlayerNum() )
			{
				cursorImage[value].renderflag = false;
			}
		}

		//	�ϐ�������
		characterSelectInfo.imagePos = 0;
		characterSelectInfo.step = 0;
		characterSelectInfo.timer = 0;
		characterSelectInfo.playerNum = gameManager->GetPlayerNum();

		//	�摜�ʒu������
		textImage.angle = D3DXToRadian( 5.0f );
		textImage.renderflag = true;

		//	��摜������
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			int left = static_cast<int>( iexSystem::ScreenWidth / 5 );
			int x = left + left * value;
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.45f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.155f );
			ImageInitialize( faceImage[value], x, y, w, h, 0, 256 * value, 256, 256 );
			faceImage[value].obj = face;
		}
	}

	//	�L�����N�^�[�I��
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		//	�e�v���C���[����
		FOR( 0, characterSelectInfo.playerNum )
		{
			//	�L�����Z��
			if ( input[value]->Get( KEY_B ) == 3 )		characterSelectInfo.select[value] = false;

			//	�I���ς݂�������L�����Z���̂ݎ�t��
			if ( characterSelectInfo.select[value] )	continue;

			//	�J�[�\���ړ�
			if ( input[value]->Get( KEY_RIGHT ) == 3 )	characterSelectInfo.character[value]++;
			if ( input[value]->Get( KEY_LEFT ) == 3 )		characterSelectInfo.character[value]--;

			//	����E�����ݒ�
			if ( characterSelectInfo.character[value] < 0 )	characterSelectInfo.character[value] = CHARACTER_TYPE::MAX - 1;
			if ( characterSelectInfo.character[value] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[value] = 0;

			//	����
			if ( input[value]->Get( KEY_SPACE ) == 3 || input[value]->Get( KEY_A ) == 3 )
			{
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
				characterSelectInfo.select[value] = true;
			}

			//	���f�������ւ�
			obj[value] = org[characterSelectInfo.character[value]]->Clone();
		}

		//	�I���ςݐl�����J�E���g
		int		selectCheck = 0;
		FOR( 0, gameManager->GetPlayerNum() )
		{
			if ( characterSelectInfo.select[value] )	selectCheck++;
		}

		//	�S�����I�����ɃL�����Z���{�^���������ƃv���C�l���I���ֈڍs
		if ( selectCheck == 0 )
		{
			if ( KEY( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_PLAYERNUM );
			}
		}

		//	�S���I���ς�
		if ( selectCheck >= gameManager->GetPlayerNum() )
		{
			//	�R���s���[�^����K���ɐݒ�
			FOR( gameManager->GetPlayerNum(), PLAYER_MAX )
			{
				characterSelectInfo.character[value] = Random::GetInt( 0, CHARACTER_TYPE::MAX - 1 );
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
			
				//	���f�������ւ�
				obj[value] = org[characterSelectInfo.character[value]]->Clone();
			}

			//	�X�e�[�W�I����
			SetMode( MENU_MODE::SELECT_STAGE );
		}

		//	���f���f�[�^�X�V
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();
		}
	}

	//	�L�����N�^�[�I��`��
	void	sceneMenu::SelectCharacterRender( void )
	{
		//	�e�L�X�g�摜
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );

		//	���f���E��E�J�[�\���`��
		FOR( 0, PLAYER_MAX )
		{
			//	���f���`��
			obj[value]->Render( shader3D, "toon" );
		}
		
		FOR( 0, CHARACTER_TYPE::MAX )
		{
			//	��`��
			RenderImage( faceImage[value], 0, 256 * value, 256, 256, IMAGE_MODE::NORMAL );
		}
			//	�J�[�\���`��
		FOR( 0, PLAYER_MAX )
		{
			cursorImage[value].x = faceImage[characterSelectInfo.character[value]].x;
			cursorImage[value].y = faceImage[characterSelectInfo.character[value]].y - faceImage[characterSelectInfo.character[value]].h / 2;
			RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::NORMAL );
		}

	}

//-------------------------------------------------------------------------------
//	�X�e�[�W�I�𓮍�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�I��������
	void	sceneMenu::SelectStageInitialize( void )
	{
		//	�����f��������
		deskStage->SetPos( -5.0f, 3.0f, 0.0f );
		deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		deskStage->SetScale( 0.1f );
		deskStage->Update();

		//	�X���f��������
		forestStage->SetPos( 5.0f, 4.0f, 0.0f );
		forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		forestStage->SetScale( 0.04f );
		forestStage->Update();

		//	�p�����[�^������
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;
	}

	//	�X�e�[�W�I��
	void	sceneMenu::SelectStageUpdate( void )
	{
		//	�X�e�[�W��]
		stageSelectInfo.angle += 0.01f;

		//	�X�e�[�W�̌�����ݒ�(�I�𒆉�])
		switch ( stageSelectInfo.stage )
		{
		case 0:
			deskStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			break;

		case 1:
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			break;
		}

		//	���E�őI��
		{
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage++;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage--;
			}

			//	����E�����ݒ�
			if ( stageSelectInfo.stage >= 2 )	stageSelectInfo.stage = 0;
			if ( stageSelectInfo.stage < 0 )		stageSelectInfo.stage = 1;
		}

		//	����
		if ( KEY( KEY_SPACE ) == 3 || KEY( KEY_A ) == 3 )
		{
			//	�}�l�[�W���[�ɏ����Z�b�g
			gameManager->SetStageType( stageSelectInfo.stage );

			//	���f���̊p�x��߂�
			forestStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );
			deskStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );

			//	���̃��[�h��
			SetMode( MENU_MODE::SELECT_CHECK );
		}

		//	�L�����Z��
		if ( KEY( KEY_B ) == 3 )
		{
			SetMode( MENU_MODE::SELECT_CHARACTER );
		}

		//	�X�e�[�W�X�V
		deskStage->Update();
		forestStage->Update();
	}

	//	�X�e�[�W�I��`��
	void	sceneMenu::SelectStageRender( void )
	{
		//	�e�L�X�g�摜�`��(�w�ʂɕ`��)
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		forestStage->Render();
	}

//-------------------------------------------------------------------------------
//	�ŏI�`�F�b�N����
//-------------------------------------------------------------------------------

	//	�ŏI�m�F������
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	�L�����ʃ��[�V�����ݒ�
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 2 );
		
		//	�e�v���C���[���f��������
		FOR( 0, PLAYER_MAX )
		{
			obj[value] = org[gameManager->GetCharacterType( value )]->Clone();
			obj[value]->SetPos( -7.0f + ( 14.0f / 3.0f * value ), 0.0f, 0.0f );
			obj[value]->Update();
		}

		//	�\���̏�����
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	�X�e�[�W���W�A�����ݒ�
		{
			deskStage->SetPos( 0.0f, 5.0f, 5.0f );
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			deskStage->SetScale( 0.08f );
			deskStage->Update();
			forestStage->SetPos( 0.0f, 6.0f, 5.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetScale( 0.03f );
			forestStage->Update();
		}
	}

	//	�ŏI�m�F
	void	sceneMenu::SelectCheckUpdate( void )
	{
		//	����i�͂��F���C���ցA�������F�L�����I���ցj
		if ( input[0]->Get( KEY_A ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
		{
			if ( !checkSelectInfo.check )	checkSelectInfo.check = true;
			else
			{
				if ( !checkSelectInfo.select )
				{
					SetMode( MENU_MODE::MOVE_MAIN );
				}
				else
				{
					SetMode( MENU_MODE::SELECT_CHARACTER );
				}
			}
		}

		//	�I�𒆓���(�J�[�\���ړ�, �L�����Z��)
		if ( checkSelectInfo.check )
		{
			if ( input[0]->Get( KEY_RIGHT ) == 3 || input[0]->Get( KEY_LEFT ) == 3 )
			{
				checkSelectInfo.select = !checkSelectInfo.select;
			}

			if ( input[0]->Get( KEY_B ) == 3 )
			{
				checkSelectInfo.check = false;
				checkSelectInfo.select = false;
			}
		}
		else
		{
			//	�L�����Z���ŃX�e�[�W�I����
			if ( input[0]->Get( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_STAGE );
			}
		}

		//	���f���X�V
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->Update();
		}
	}

	//	�ŏI�m�F�`��
	void	sceneMenu::SelectCheckRender( void )
	{
		//	�X�e�[�W�`��
		switch ( gameManager->GetStageType() )
		{
		case 0:		deskStage->Render();		break;
		case 1:		forestStage->Render();		break;
		}

		//	�v���C���[�`��
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Render( shader3D, "toon" );
		}

		//	�e�L�X�g�摜�`��
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	�`�F�b�N���ڕ`��
		if ( checkSelectInfo.check )
		{
			int x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.17f );
			int w = static_cast<int>( iexSystem::ScreenWidth *  0.39f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			checkBack->Render( x, y, w, h, 0, 0, 512, 512 );

			x = static_cast<int>( iexSystem::ScreenWidth * 0.34f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.62f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.31f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.13f );
			checkCursor->Render( x, y, w, h, 0, checkSelectInfo.select * 128, 512, 128 );
		}
	}

//-------------------------------------------------------------------------------
//	���C���ړ�����
//-------------------------------------------------------------------------------

	//	���C���ړ�������
	void	sceneMenu::MoveMainInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.5f );
	}

	//	���C����
	void	sceneMenu::MoveMainUpdate( void )
	{
		if ( screen->GetScreenState() )
		{
			MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
			return;
		}
	}

	//	���C���ړ��`��
	void	sceneMenu::MoveMainRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	�^�C�g���ړ�����
//-------------------------------------------------------------------------------

	//	�^�C�g���ړ�������
	void	sceneMenu::MoveTitleInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
	}

	//	�^�C�g����
	void	sceneMenu::MoveTitleUpdate( void )
	{
		if ( screen->GetScreenState() )
		{
			MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
			return;
		}
	}

	//	�^�C�g���ړ��`��
	void	sceneMenu::MoveTitleRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	�I�v�V�����֐�
//-------------------------------------------------------------------------------
	//	�I�v�V����
	void	sceneMenu::OptionInitialize( void )
	{
		//�@�\���̏�����
		optionInfo.itemflg = true;
		optionInfo.coinMAX = 200;
		optionInfo.minute = 1;
		optionInfo.second = 30;
		optionInfo.step = 0;
		gameManager->SetItemFlg(optionInfo.itemflg);
		gameManager->SetCoinMax(optionInfo.coinMAX);
		gameManager->SetTime(optionInfo.minute, optionInfo.second);
	}

	void	sceneMenu::OptionUpdate( void )
	{
		if (KEY_Get(KEY_DOWN) == 3){
			if (optionInfo.step<3)
			optionInfo.step++;
		}
		if (KEY_Get(KEY_UP) == 3){
			if (optionInfo.step>0){
				optionInfo.step--;
			}
		}

		switch (optionInfo.step){
		case 0:
			if (KEY_Get(KEY_RIGHT) == 3 || KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.itemflg == false){
					optionInfo.itemflg = true;
				}
				else{
					optionInfo.itemflg = false;
				}
			}
			break;
		case 1:
			if (KEY_Get(KEY_RIGHT) == 3 ){
				if (optionInfo.coinMAX<500){
					optionInfo.coinMAX += 50;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.coinMAX>200){
					optionInfo.coinMAX -= 50;
				}
			}
			break;
		case 2:
			if (KEY_Get(KEY_RIGHT) == 3){
				if (optionInfo.minute<5){
					optionInfo.minute++;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.minute>1){
					optionInfo.minute--;
				}
			}
			break;
		case 3:
			if (KEY_Get(KEY_RIGHT) == 3){
				optionInfo.second = 30;
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				optionInfo.second = 0;
			}
			break;
		}
			gameManager->SetItemFlg(optionInfo.itemflg);
			gameManager->SetCoinMax(optionInfo.coinMAX);
			gameManager->SetTime(optionInfo.minute,optionInfo.second);
		if (KEY_Get(KEY_A) == 3){
			SetMode(tempmode);
		}
	}

	void	sceneMenu::OptionRender( void )
	{
		Oimage->Render(300, 150, 512, 128, 0, 128*2, 512, 128);
		Oimage->Render(300, 350, 512, 128, 0, 128*0, 512, 128);
		Oimage->Render(400, 550, 256, 128, 0, 128*1, 256, 128);

		//�A�C�e���̗L���`��
		if (optionInfo.itemflg){
			Oimage->Render(950, 150, 256, 128, 0, 128 * 3, 256, 128);
		}
		else{
			Oimage->Render(950, 150, 256, 128, 256, 128 * 3, 256, 128);
		}

		OCmax->Render(950,			350, 128, 128,			(optionInfo.coinMAX/100)*64, 128 * 0, 64, 64);
		OCmax->Render(950+128,		350, 128, 128,	((optionInfo.coinMAX / 10)%10) * 64, 128 * 0, 64, 64);
		OCmax->Render(950+(128*2),	350, 128, 128,									  0, 128 * 0, 64, 64);

		TimerRender();
	}
	void	sceneMenu::TimerRender(void)
	{
		//Oimage->Render(970, 550, 256, 128, 256, 128 * 1, 256, 128);
		OCmax->Render(950 , 550, 128, 128, optionInfo.minute*64, 128 * 0, 64, 64);
		OCmax->Render(950 + 128, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 128 * 0, 64, 64);
		OCmax->Render(950 + (128 * 2), 550, 128, 128, 0, 0, 64, 64);

	}
//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���[�h�؂�ւ�
	void	sceneMenu::SetMode( int mode )
	{
		//	����������
		switch ( mode )
		{
		case MENU_MODE::SELECT_PLAYERNUM:
			SelectPlayerNumInitialize();
			break;

		case MENU_MODE::SELECT_CHARACTER:
			SelectCharacterInitialize();
			break;

		case MENU_MODE::SELECT_STAGE:
			SelectStageInitialize();
			break;

		case MENU_MODE::SELECT_CHECK:
			SelectCheckInitialize();
			break;

		case MENU_MODE::MOVE_MAIN:
			MoveMainInitialize();
			break;
		case MENU_MODE::OPTION:
			OptionInitialize();
			break;
		}

		//	���[�h�ؑ�
		this->mode = mode;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------