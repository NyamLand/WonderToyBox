
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
		namespace CAMERA_TARGET
		{
			Vector3 front(0.0f, 0.0f, -1.0f);
			Vector3 back(0.0f, 0.0f, 1.0f);
			Vector3 right(-1.0f, 0.0f, 0.0f);
			Vector3 left(1.0f, 0.0f, 0.0f);
			Vector3 up(0.0f, 1.0f, 0.0f);
		}
		namespace CHARA_ANGLE
		{
			float CharaSelectPlayer_ANGLE[4] = {
				(D3DXToRadian(60.0f)),
				(D3DXToRadian(80.0f)),
				(D3DXToRadian(100.0f)),
				(D3DXToRadian(120.0f)),
			};

			float SelectCheckPlayer_ANGLE[4] = {
				(D3DXToRadian(-120.0f)),
				(D3DXToRadian(-100.0f)),
				(D3DXToRadian(-80.0f)),
				(D3DXToRadian(-60.0f)),
			};

		}
		namespace CHECK_MODE
		{
			enum
			{
				SELECT,
				MOVETARGET,
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
		//�w�i�p�\���̏�����
		bgInfo.t = 1.0f;
		bgInfo.mt = 0.0f;
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 70.0f ), 1.0f, 1000.0f );
		bgInfo.cpos = Vector3(0.0f, 15.0f, 0.0f);
		bgInfo.cspos = bgInfo.cpos;
		bgInfo.cepos = Vector3( 1.0f,30.0f, 0.0f );
		bgInfo.target = CAMERA_TARGET::front;
		bgInfo.start	= bgInfo.target;
		bgInfo.end		= bgInfo.target;
		bgInfo.moveflg = false;
		mainView->Set( bgInfo.cpos, bgInfo.target );

		changeScene = false;

		//	random
		Random::Initialize();

		//	screen
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::WIPE_IN, 1.5f );

		//	�Q�[���}�l�[�W��������
		gameManager->Initialize();
		OptionInitialize();
	
		//	�摜�ǂݍ���
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/menu/playerNum.png" ) );
		face = new iex2DObj( "DATA/UI/chara_emotion.png" );
		cursor = new iex2DObj( "DATA/UI/cursor.png" );
		decidecursor = new iex2DObj("DATA/UI/decide-cursor.png");
		cpuCursor = new iex2DObj( "DATA/UI/cpuIcon.png" );
		selectCheckCursor = new iex2DObj("DATA/UI/menu/cursor.png");
		triangleCursor = new iex2DObj("DATA/UI/tryangle.png");
		
		//	�I�v�V�����֌W�摜�ǂݍ���
		optionImage =		new iex2DObj( "DATA/UI/OptionText.png" );
		optionTime	=		new iex2DObj( "DATA/UI/number.png" );
		optionLife	=		new iex2DObj( "DATA/UI/number.png" );
		optionMenu = new iex2DObj("DATA/UI/option-int.png");

		//	���f���ǂݍ���
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/majo/majo.IEM" ) );		//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/�v�����Z�X/prin2.IEM" ) );					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));		//	����
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));				//	�C��

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.015f );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.04f );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale(0.025f);		//	����
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.04f);			//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );	//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );	//	����
		org[CHARACTER_TYPE::PIRATE]->SetAngle( D3DX_PI );			//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 0 );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 0 );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );		//	����
		org[CHARACTER_TYPE::PIRATE]->SetMotion( 0 );			//	�C��

		org[CHARACTER_TYPE::SCAVENGER]->Update();	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->Update();		//	�P
		org[CHARACTER_TYPE::THIEF ]->Update();		//	����
		org[CHARACTER_TYPE::PIRATE]->Update();			//	�C��

		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( 0.0f, 0.0f, 0.0f );
			modelAngle[value] = Vector3( 0.0f, 0.0f, 0.0f );
			modelScale[value] = Vector3( 0.0f, 0.0f, 0.0f );
			obj[value] = org[value]->Clone();
		}

		deskStage = make_unique<iexMesh>( LPSTR( "DATA/BG/stage-desk/stage.IMO" ) );
		toyStage = make_unique<iexMesh>(LPSTR("DATA/BG/stage_toy/stage_toy.IMO"));
		BG = make_unique<iexMesh>( LPSTR( "DATA/BG/MenuStage/menustage.IMO" ) );

		//	�����f��������
		BG->SetPos(0.0f, 0.0f, -1.0f);
		BG->SetAngle(0.0f, 0.0f, 0.0f);
		BG->SetScale(0.1f);
		BG->Update();
		//	�摜�\���̏�����
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu/menu-head.png" );
			int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
			ImageInitialize( textImage, x, y, w, h, 512, 0, 512, 256 );
			textImage.angle = D3DXToRadian( 5.0f );
			textImage.renderflag = true;
		}

		//	���[�h�ݒ�
		tempmode = 0;
		SetMode( MENU_MODE::INIT );

		//BGM�ݒ�Đ�
		sound->PlayBGM(BGM::MENU_BGM);

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
		SafeDelete(decidecursor);
		SafeDelete( selectCheckCursor );
		SafeDelete( cpuCursor );
		SafeDelete(triangleCursor);
		Random::Release();
		sound->AllStop();

		//�݌v��
		SafeDelete(optionImage);
		SafeDelete(optionTime);
		SafeDelete(optionLife);
		SafeDelete(optionMenu);
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	sceneMenu::Update( void )
	{
		if ( screen->GetScreenState() )
		{
			//	�I�v�V�����Ƃ̐؂�ւ�
			ChangeToOption();
		}
		//	�J�����X�V
		CameraUpdate();	

		//	�e���[�h�X�V
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
			if ( changeScene )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				return;
			}

			break;

		case MENU_MODE::MOVE_TITLE:
			MoveTitleUpdate();
			if ( changeScene )
			{
				MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
				return;
			}
			break;

		case MENU_MODE::OPTION:
			OptionUpdate();
			break;
		}

		//	���f���X�V
		ModelUpdate();

		//	�X�N���[���X�V
		screen->Update();
	}

	//	�`��
	void	sceneMenu::Render( void )
	{
		//	camera
		mainView->Activate();
		mainView->Clear();

		//	���C�g�ݒ�
		dir = bgInfo.target;
		shader3D->SetValue("DirLightVec", dir);
		iexLight::DirLight(shader, 0, &dir, 1.0f, 1.0f, 1.0f);
		dir.Normalize();

		//	�w�i�`��
		BG->Render();
		////�w�i( ��Ԍ��ɕ\�� )
		//iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		//back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		//iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

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
		if (mode != MENU_MODE::OPTION){
			optionMenu->Render(80, 50, 256, 64, 0, 0, 256, 64);
		}
		
		//	�X�N���[��
		screen->Render();
	}

	//	�J�����X�V
	void	sceneMenu::CameraUpdate( void )
	{
		//	�J�����p�p�����[�^���Z
		bgInfo.t += 0.02f;
		if ( bgInfo.t >= 1.0f )	bgInfo.t = 1.0f;

		//	��]���
		CubicFunctionInterpolation( bgInfo.target, bgInfo.start, bgInfo.end, bgInfo.t );

		//	�J�����X�V
		mainView->Set( bgInfo.cpos, bgInfo.cpos + bgInfo.target );
	}

	//	�I�v�V�����ɐ؂�ւ������̋t
	void	sceneMenu::ChangeToOption( void )
	{
		if ( KEY_Get( KEY_B6 ) == 3 )
		{
			//	���݂̃��[�h��ۑ����ăI�v�V�����ֈڍs
			if ( mode != MENU_MODE::OPTION )
			{
				tempmode = mode;
				SetMode( MENU_MODE::OPTION );
			}
			else
			{
				//	�ۑ����Ă��������[�h�ֈڍs
				SetMode( tempmode );
			}
		}
	}

	//	���f���X�V
	void	sceneMenu::ModelUpdate( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->SetPos( modelPos[value] );
			obj[value]->SetAngle( modelAngle[value] );
			obj[value]->Animation();
			obj[value]->Update();
		}

		printf("1 frame = %d, 2 frame = %d, 3 frame = %d, 4 frame = %d\n", obj[0]->GetFrame(), obj[1]->GetFrame(), obj[2]->GetFrame(), obj[3]->GetFrame());
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
		if (bgInfo.t < 1.0f)return;
		//	�p�����[�^���Z
		playerNumSelectInfo.t += 0.08f;
		if ( playerNumSelectInfo.t >= 1.0f )	playerNumSelectInfo.t = 1.0f;	

		//	�ړ����I�������
		if ( playerNumSelectInfo.t >= 1.0f )
		{
			//	�I��
			if ( input[0]->Get( KEY_DOWN ) == 1 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				//	���̍��W��ۑ�
				playerNumSelectInfo.saveY =  128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num++;
				if ( playerNumSelectInfo.num >= 4 )
				{
					playerNumSelectInfo.num = -1;
					playerNumSelectInfo.t = 1.0f;
				}
				else
				{
					playerNumSelectInfo.t = 0.0f;
				}
			}

			if ( input[0]->Get( KEY_UP ) == 1 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				//	���̍��W��ۑ�
				playerNumSelectInfo.saveY = 128 * playerNumSelectInfo.num;
				playerNumSelectInfo.num--;
				if ( playerNumSelectInfo.num < 0 )
				{
					playerNumSelectInfo.num = 4;
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
				bgInfo.start = CAMERA_TARGET::front;
				bgInfo.end = CAMERA_TARGET::right;
				bgInfo.t = 0.0f;
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
		if (bgInfo.t < 1.0f)return;
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.28f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.08f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.15f );
		playerNum->Render( x, y, w, h, 0, playerNumSelectInfo.sy, 128, 128 );

		triangleCursor->Render(x+10.0f, y - 130.0f, 128, 128, 0, 0, 256, 256);
		triangleCursor->Render(x + 10.0f, y + 140.0f, 128, 128, 256, 0, 256, 256);

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
		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(0);	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(0);		//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);		//	����
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);			//	�C��
		//	���f���A�I����񏉊���
		FOR( 0, PLAYER_MAX )
		{
			//	���f���o�^�A������
			obj[value] = org[value]->Clone();
			modelAngle[value] = Vector3( 0.0f, D3DXToRadian( 100.0f ), 0.0f );

			//	�I����񏉊���
			characterSelectInfo.character[value] = value;
			characterSelectInfo.select[value] = false;
			
			//	�J�[�\��������
			int w = static_cast<int>(iexSystem::ScreenWidth * (0.04f));
			int h = static_cast<int>(iexSystem::ScreenHeight * (0.07f));
			ImageInitialize(cursorImage[value], 0, 0, w, h, 0, 0, 0, 0);
			ImageInitialize(decidecursorImage[value], 0, 0, w, h, 0, 0, 0, 0);
			cursorImage[value].obj = cursor;
			decidecursorImage[value].obj = decidecursor;
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
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.37f );
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.155f );
			ImageInitialize( faceImage[value], x, y, w, h, 0, 256 * value, 256, 256 );
			faceImage[value].obj = face;
		}
	}

	//	�L�����N�^�[�I��
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	�e�v���C���[����
		FOR( 0, characterSelectInfo.playerNum )
		{
			//	�L�����Z��
			if ( input[value]->Get( KEY_B ) == 3 )		characterSelectInfo.select[value] = false;

			//	�I���ς݂�������L�����Z���̂ݎ�t��
			if ( characterSelectInfo.select[value] )	continue;

			//	�J�[�\���ړ�
			if ( input[value]->Get( KEY_RIGHT ) == 3 )	
			{
				sound->PlaySE( SE::CHOICE_SE );
				characterSelectInfo.character[value]++;
				//	�����ݒ�
				if ( characterSelectInfo.character[value] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[value] = 0;
				//	���f�������ւ�
				obj[value] = org[characterSelectInfo.character[value]]->Clone();
			}
			if ( input[value]->Get( KEY_LEFT ) == 3 )
			{
				sound->PlaySE( SE::CHOICE_SE );
				characterSelectInfo.character[value]--;
				//	����ݒ�
				if ( characterSelectInfo.character[value] < 0 )	characterSelectInfo.character[value] = CHARACTER_TYPE::MAX - 1;
				//	���f�������ւ�
				obj[value] = org[characterSelectInfo.character[value]]->Clone();
			}


			//	����
			if ( input[value]->Get( KEY_SPACE ) == 3 || input[value]->Get( KEY_A ) == 3 )
			{
				gameManager->SetCharacterType( value, characterSelectInfo.character[value] );
				characterSelectInfo.select[value] = true;
			}


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
				bgInfo.t = 0.0f;
				bgInfo.start = CAMERA_TARGET::right;
				bgInfo.end = CAMERA_TARGET::front;
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
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::back;
			SetMode( MENU_MODE::SELECT_STAGE );
		}

		//	���f���f�[�^�X�V
		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( -12.0f, 10.0f, -9.0f + 6.0f * value );
			modelAngle[value] = Vector3( 0.0f, CHARA_ANGLE::CharaSelectPlayer_ANGLE[value], 0.0f );
		}
	}

	//	�L�����N�^�[�I��`��
	void	sceneMenu::SelectCharacterRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	�e�L�X�g�摜
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );

		//	���f��
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
		//FOR( 0, PLAYER_MAX )
		//{
		//	cursorImage[value].x = faceImage[characterSelectInfo.character[value]].x-faceImage[characterSelectInfo.character[value]].w/2;
		//	cursorImage[value].y = faceImage[characterSelectInfo.character[value]].y - faceImage[characterSelectInfo.character[value]].h / 2;
		//	if ( characterSelectInfo.select[value] )	cursorImage[value].color = Vector3( 0.5f, 0.5f, 0.5f );	//	���莞���x������
		//	RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::NORMAL );
		//	//RenderImage(cursorImage[value], 128 * ( value % 2 ), 128 * value, 128, 128, IMAGE_MODE::NORMAL);
		//}
		//�v���C���[1
		cursorImage[0].x = faceImage[characterSelectInfo.character[0]].x - faceImage[characterSelectInfo.character[0]].w / 2;
		cursorImage[0].y = faceImage[characterSelectInfo.character[0]].y - faceImage[characterSelectInfo.character[0]].h / 2;
		if (characterSelectInfo.select[0])	{
			decidecursorImage[0].x = cursorImage[0].x;	//	���莞���x������
			decidecursorImage[0].y = cursorImage[0].y;
		}
		

		//�v���C���[2
		cursorImage[1].x = faceImage[characterSelectInfo.character[1]].x + faceImage[characterSelectInfo.character[1]].w / 2;
		cursorImage[1].y = faceImage[characterSelectInfo.character[1]].y - faceImage[characterSelectInfo.character[1]].h / 2;
		if (characterSelectInfo.select[1])	{
			decidecursorImage[1].x = cursorImage[1].x;	//	���莞���x������
			decidecursorImage[1].y = cursorImage[1].y;
		}


		//�v���C���[3
		cursorImage[2].x = faceImage[characterSelectInfo.character[2]].x - faceImage[characterSelectInfo.character[2]].w / 2;
		cursorImage[2].y = faceImage[characterSelectInfo.character[2]].y + faceImage[characterSelectInfo.character[2]].h / 2;
		if (characterSelectInfo.select[2])	{
			decidecursorImage[2].x = cursorImage[2].x;	//	���莞���x������
			decidecursorImage[2].y = cursorImage[2].y;
		}

		//�v���C���[4
		cursorImage[3].x = faceImage[characterSelectInfo.character[3]].x + faceImage[characterSelectInfo.character[3]].w / 2;
		cursorImage[3].y = faceImage[characterSelectInfo.character[3]].y + faceImage[characterSelectInfo.character[3]].h / 2;
		if (characterSelectInfo.select[3])	{
			decidecursorImage[3].x = cursorImage[3].x;	//	���莞���x������
			decidecursorImage[3].y = cursorImage[3].y;
		}

		FOR(0, PLAYER_MAX){
			if (characterSelectInfo.select[value]){
				RenderImage(decidecursorImage[value], 128 * (value % 2), 128 * (value / 2), 128, 128, IMAGE_MODE::NORMAL);
			}
			else
			{
				RenderImage(cursorImage[value], 128 * (value % 2), 128 * (value / 2), 128, 128, IMAGE_MODE::NORMAL);
			}
		}
		
	}

//-------------------------------------------------------------------------------
//	�X�e�[�W�I�𓮍�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�I��������
	void	sceneMenu::SelectStageInitialize( void )
	{
		//	�����f��������
		deskStage->SetPos( 0.0f,0.0f,0.0f );
		deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		deskStage->SetScale( 0.1f );
		deskStage->Update();

		
		//�@�������Ⴢ�f��������
		toyStage->SetPos(0.0f,0.0f,0.0f);
		toyStage->SetAngle(D3DXToRadian(30.0f), D3DX_PI, 0.0f);
		toyStage->SetScale(0.05f);
		toyStage->Update();

		//	�p�����[�^������
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;
	}

	//	�X�e�[�W�I��
	void	sceneMenu::SelectStageUpdate( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	�X�e�[�W��]
		static float atai = 0.01f;
		stageSelectInfo.angle += atai;
		if (stageSelectInfo.angle>3.8f || stageSelectInfo.angle < 2.4f)
		{
			atai *= -1.0f;
		}//stageSelectInfo.angle = 3.5f;
		//stageSelectInfo.angle = 1.8f;

		//	�X�e�[�W�̌�����ݒ�(�I�𒆉�])
		switch ( stageSelectInfo.stage )
		{
		case 0:
			deskStage->SetPos(-2.0f, 14.0f, 8.0f);
			toyStage->SetPos(7.0f, 14.0f, 15.0f);
			deskStage->SetAngle( D3DXToRadian( 30.0f ), stageSelectInfo.angle, 0.0f );
			toyStage->SetAngle(D3DXToRadian(30.0f), D3DX_PI, 0.0f);
			break;

		case 1:
			deskStage->SetPos(-7.0f, 14.0f, 15.0f);
			toyStage->SetPos(2.0f, 14.0f, 8.0f);
			deskStage->SetAngle(D3DXToRadian(30.0f), D3DX_PI, 0.0f);
			toyStage->SetAngle(D3DXToRadian(30.0f), stageSelectInfo.angle, 0.0f);
			break;
		}

		//	���E�őI��
		{
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				stageSelectInfo.angle = D3DX_PI;
				stageSelectInfo.stage++;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
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
			deskStage->SetAngle( D3DXToRadian( 5.0f ), D3DX_PI, 0.0f );
			toyStage->SetAngle(D3DXToRadian(5.0f), D3DX_PI, 0.0f);

			//	���̃��[�h��
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::left;
			SetMode( MENU_MODE::SELECT_CHECK );
		}

		//	�L�����Z��
		if ( KEY( KEY_B ) == 3 )
		{
			bgInfo.t = 0.0f;
			bgInfo.start = bgInfo.end;
			bgInfo.end = CAMERA_TARGET::right;
			SetMode( MENU_MODE::SELECT_CHARACTER );
		}

		//	�X�e�[�W�X�V
		deskStage->Update();
		toyStage->Update();
	}

	//	�X�e�[�W�I��`��
	void	sceneMenu::SelectStageRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	�e�L�X�g�摜�`��(�w�ʂɕ`��)
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		toyStage->Render();
	}

//-------------------------------------------------------------------------------
//	�ŏI�`�F�b�N����
//-------------------------------------------------------------------------------

	//	�ŏI�m�F������
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	�L�����ʃ��[�V�����ݒ�
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 0 );
		
		//	�e�v���C���[���f��������
		static	Vector3	cursorPos[4];
		FOR( 0, PLAYER_MAX )
		{
			obj[value] = org[gameManager->GetCharacterType( value )]->Clone();
			modelPos[value] = Vector3( 10.0f, 14.0f, 4.0f - 3.0f * value );
			obj[value]->SetPos( modelPos[value] );
			obj[value]->Update();
			WorldToClient( obj[value]->GetPos(), cursorPos[value], matView * matProjection );
		}

		//	�\���̏�����
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	�X�e�[�W���W�A�����ݒ�
		{
			deskStage->SetPos( 7.0f, 15.5f, 2.5f );
			deskStage->SetAngle(D3DXToRadian(45.0f), D3DXToRadian(-90.0f), 0.0f);
			deskStage->SetScale( 0.03f );
			deskStage->Update();
		
			toyStage->SetPos(7.0f, 15.5f, 2.5f);
			toyStage->SetAngle(D3DXToRadian(45.0f), D3DXToRadian(-90.0f), 0.0f);
			toyStage->SetScale(0.02f);
			toyStage->Update();
		}

		//	�J�[�\��������
		{
			FOR( 0, PLAYER_MAX )
			{
				//	���W������
				cursorImage[value].x = static_cast<int>( cursorPos[value].x - ( iexSystem::ScreenWidth * 0.05f ) );
				cursorImage[value].y = static_cast<int>( cursorPos[value].y - ( iexSystem::ScreenHeight * 0.3f ) );
				cursorImage[value].renderflag = true;
				
				//	CPU���v���C���[�ŉ摜�����ւ�
				if ( value >= gameManager->GetPlayerNum() )
					cursorImage[value].obj = cpuCursor;
			}
		}
	}

	//	�ŏI�m�F
	void	sceneMenu::SelectCheckUpdate( void )
	{
		Vector3	vec;	//	�J��������ړ���ւ̃x�N�g���ۑ��p
		if ( bgInfo.t < 1.0f )return;
		switch (checkSelectInfo.step)
		{
		case CHECK_MODE::SELECT:
		//	����i�͂��F���C���ցA�������F�L�����I���ցj
		if ( input[0]->Get( KEY_A ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
		{
			
			//	�m�F�\��
			if ( !checkSelectInfo.check )
			{
				checkSelectInfo.check = true;
			}
			else
			{
				//	�͂��E������
				if ( !checkSelectInfo.select )	//	�͂�
				{
					bgInfo.t = 0.0f;
					bgInfo.start = bgInfo.end;
					vec = bgInfo.cepos - bgInfo.cpos;
					vec.Normalize();
					bgInfo.end = vec;
					checkSelectInfo.step = CHECK_MODE::MOVETARGET;
				}				
				else //������
				{
					bgInfo.t = 0.0f;
					bgInfo.start = CAMERA_TARGET::left;
					bgInfo.end = CAMERA_TARGET::back;
					SetMode( MENU_MODE::SELECT_STAGE );
				}
			}
		}


		//	�I�𒆓���(�J�[�\���ړ�, �L�����Z��)
		if ( checkSelectInfo.check )
		{
			if ( input[0]->Get( KEY_RIGHT ) == 3 || input[0]->Get( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
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
				bgInfo.t = 0.0f;
				bgInfo.start = CAMERA_TARGET::left;
				bgInfo.end = CAMERA_TARGET::back;
				SetMode( MENU_MODE::SELECT_STAGE );
				return;
			}
		}

		//	���f���X�V
		FOR( 0, PLAYER_MAX )
		{
			modelPos[value] = Vector3( 12.0f, 10.0f, 8.0f - 6.0f * value );
			modelAngle[value] = Vector3( 0.0f, CHARA_ANGLE::SelectCheckPlayer_ANGLE[value], 0.0f );
		}
		break;

		case  CHECK_MODE::MOVETARGET:
			if (bgInfo.t >= 1.0f)
			{
				SetMode(MENU_MODE::MOVE_MAIN);
			}
			break;
		}
	}

	//	�ŏI�m�F�`��
	void	sceneMenu::SelectCheckRender( void )
	{
		if (bgInfo.t < 1.0f)return;
		//	�X�e�[�W�`��
		switch ( gameManager->GetStageType() )
		{
		case 0:		deskStage->Render();		break;
		case 1:		toyStage->Render();		break;
		}

		//	�v���C���[�`��
		FOR( 0, PLAYER_MAX )
		{
			//	���f���`��
			obj[value]->Render( shader3D, "toon" );

			if ( value < gameManager->GetPlayerNum() )
				RenderImage( cursorImage[value], 128 * ( value % 2 ), 128 * ( value / 2 ), 128, 128, IMAGE_MODE::NORMAL );
			else
				RenderImage( cursorImage[value], 0, 0, 64, 64, IMAGE_MODE::NORMAL );
		}
		//	�I�v�V�����m�F�`��
		OptionSelectRender();

		//	�e�L�X�g�摜�`��
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	�`�F�b�N���ڕ`��
		if ( checkSelectInfo.check )
		{
			//	�m�F�̔w�i�`��
			int x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
			int y = static_cast<int>( iexSystem::ScreenHeight * 0.17f );
			int w = static_cast<int>( iexSystem::ScreenWidth *  0.39f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			checkBack->Render( x, y, w, h, 0, 0, 512, 512 );

			//	�m�F���ڕ`��
			x = static_cast<int>( iexSystem::ScreenWidth * 0.34f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.62f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.31f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.13f );
			checkCursor->Render( x, y, w, h, 0, checkSelectInfo.select * 128, 512, 128 );

			//	�m�F�J�[�\���`��
			if ( !checkSelectInfo.select )		x = static_cast<int>( iexSystem::ScreenWidth * 0.369f );
			else											x = static_cast<int>( iexSystem::ScreenWidth * 0.51f );
			y = static_cast<int>( iexSystem::ScreenHeight * 0.64f );
			w = static_cast<int>( iexSystem::ScreenWidth *  0.11f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
			selectCheckCursor->Render( x, y, w, h, 0, 0, 256, 256 );
		}

	}

//-------------------------------------------------------------------------------
//	���C���ړ�����
//-------------------------------------------------------------------------------

	//	���C���ړ�������
	void	sceneMenu::MoveMainInitialize( void )
	{
		screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );
	}

	//	���C����
	void	sceneMenu::MoveMainUpdate( void ) 
	{
		//	��]���I�����Ă��Ȃ�������X�L�b�v
		if ( bgInfo.t < 1.0f )return;

		//	�ϐ�����
		bool	isEnd = false;

		//	�p�����[�^���Z
		bgInfo.mt += 0.01f;
		if ( bgInfo.mt >= 1.0f )	bgInfo.mt = 1.0f;

		isEnd = CubicFunctionInterpolation( bgInfo.cpos, bgInfo.cspos, bgInfo.cepos, bgInfo.mt );
		if ( isEnd )
		{
			if ( screen->GetScreenState() )
			{
				changeScene = true;
				//MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
				//return;
			}
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
			changeScene = true;
			//MainFrame->ChangeScene( new sceneLoad( new sceneTitle() ) );
			//return;
		}
	}

	//	�^�C�g���ړ��`��
	void	sceneMenu::MoveTitleRender( void )
	{

	}

//-------------------------------------------------------------------------------
//	�I�v�V�����֐�
//-------------------------------------------------------------------------------
	
	//	�I�v�V����������
	void	sceneMenu::OptionInitialize( void )
	{
		//�@�\���̏�����
		optionInfo.itemflg = true;
		optionInfo.life = LIFE_MAX_NUM::LIFE_5;
		optionInfo.minute = 1;
		optionInfo.second = 0;
		optionInfo.step = 0;
		gameManager->SetItemFlg(optionInfo.itemflg);
		gameManager->SetMaxLife(optionInfo.life);
		gameManager->SetTime(optionInfo.minute, optionInfo.second);
	}

	//	�I�v�V�����X�V
	void	sceneMenu::OptionUpdate( void )
	{
		if (input[0]->Get(KEY_DOWN) == 3)
		{
			sound->PlaySE(SE::DECIDE_SE);
			if (optionInfo.step<3)
			optionInfo.step++;
		}
		if (input[0]->Get(KEY_UP) == 3)
		{
			sound->PlaySE(SE::DECIDE_SE);
			if (optionInfo.step>0){
				optionInfo.step--;
			}
		}

		switch (optionInfo.step)
		{
		case 0:
			if (input[0]->Get(KEY_RIGHT) == 3 || input[0]->Get(KEY_LEFT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				if (optionInfo.itemflg == false){
					optionInfo.itemflg = true;
				}
				else{
					optionInfo.itemflg = false;
				}
			}
			break;
		case 1:
			if ( KEY( KEY_RIGHT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.life++;
				if ( optionInfo.life >= LIFE_MAX_NUM::END )		optionInfo.life = LIFE_MAX_NUM::LIFE_3;
			}

			if ( KEY( KEY_LEFT ) == 3 )
			{
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.life--;
				if ( optionInfo.life < LIFE_MAX_NUM::LIFE_3 )	optionInfo.life = LIFE_MAX_NUM::LIFE_5;
			}
			break;
		case 2:
			if (KEY_Get(KEY_RIGHT) == 3){
				if (optionInfo.minute<5){
					sound->PlaySE(SE::CHOICE_SE);
					optionInfo.minute++;
				}
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				if (optionInfo.minute>1){
					sound->PlaySE(SE::CHOICE_SE);
					optionInfo.minute--;
				}
			}
			break;
		case 3:
			if (KEY_Get(KEY_RIGHT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.second = 30;
			}
			else if (KEY_Get(KEY_LEFT) == 3){
				sound->PlaySE(SE::CHOICE_SE);
				optionInfo.second = 0;
			}
			break;
		}
			gameManager->SetItemFlg(optionInfo.itemflg);
			gameManager->SetMaxLife(optionInfo.life);
			gameManager->SetTime(optionInfo.minute,optionInfo.second);
	}

	//	�I�v�V�����`��
	void	sceneMenu::OptionRender( void )
	{
		optionImage->Render(300, 150, 512, 128, 0, 128*2, 512, 128);
		optionImage->Render(300, 350, 512, 128, 0, 128*0, 512, 128);
		optionImage->Render(400, 550, 256, 128, 0, 128*1, 256, 128);

		//�A�C�e���̗L���`��
		if (optionInfo.itemflg){
			optionImage->Render(950, 150, 256, 128, 0, 128 * 3, 256, 128);
		}
		else{
			optionImage->Render(950, 150, 256, 128, 256, 128 * 3, 256, 128);

		}
		//lifeMAX�̕`��
		optionLife->Render(940,	350, 128, 128,(optionInfo.life+3)*64, 128 * 0, 64, 64);

		TimerRender();
		ArrowRender();

		optionMenu->Render(80, 50, 256, 128, 0, 64, 256, 128);
	}

	//	�^�C�}�[�`��
	void	sceneMenu::TimerRender( void )
	{
		optionLife->Render(930 , 550, 128, 128, optionInfo.minute*64, 64 * 0, 64, 64);
		optionLife->Render(1140, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 64 * 0, 64, 64);
		optionLife->Render(1220, 550, 128, 128, 0, 64*0, 64, 64);
		optionImage->Render(1020, 550, 128, 128, 256, 128 * 1, 128, 128);
		optionImage->Render(1300, 550, 128, 128, 384, 128 * 1, 128, 128);

	}

	//	���ڕ`��
	void	sceneMenu::OptionSelectRender( void )
	{
		
		//���ڕ`��
		optionImage->Render(800, 320, 256, 64, 0, 128 * 2, 512, 128);
		optionImage->Render(800, 420, 256, 64, 0, 128 * 0, 512, 128);
		optionImage->Render(800, 520, 128, 64, 0, 128 * 1, 256, 128);
		//�A�C�e���̗L���`��
		if (optionInfo.itemflg){
			optionImage->Render(1100, 320, 128, 64, 0, 128 * 3, 256, 128);
		}
		else{
			optionImage->Render(1100, 320, 128, 64, 256, 128 * 3, 256, 128);

		}
		//�R�C��MAX�̕`��
		optionLife->Render(1100, 420, 64, 64, ( optionInfo.life + 3 ) * 64, 128 * 0, 64, 64);
		//�^�C��
		optionLife->Render(1100, 520, 64, 64, optionInfo.minute * 64, 64 * 0, 64, 64);
		optionLife->Render(1220, 520, 64, 64, ((optionInfo.second / 10) % 10) * 64, 64 * 0, 64, 64);
		optionLife->Render(1260, 520, 64, 64, 0, 64 * 0, 64, 64);
		optionImage->Render(1160, 520, 64, 64, 256, 128 * 1, 128, 128);
		optionImage->Render(1320, 520, 64, 64, 384, 128 * 1, 128, 128);
	}

	//	���`��
	void	sceneMenu::ArrowRender( void )
	{
		switch (optionInfo.step){
		case 0:
			optionLife->Render(830, 150, 128, 128, 64*4, 64 * 3, 64, 64);
			break;
		case 1:
			optionLife->Render(830, 350, 128, 128, 64 * 4, 64 * 3, 64, 64);
			break;
		case 2:
			optionLife->Render(830, 550, 128, 128, 64 * 4, 64 * 3, 64, 64);
			optionLife->Render(930, 550, 128, 128, optionInfo.minute * 64, 64 * 1, 64, 64);
			break;
		case 3:
			optionLife->Render(830, 550, 128, 128, 64 * 4, 64 * 3, 64, 64);
			optionLife->Render(1140, 550, 128, 128, ((optionInfo.second / 10) % 10) * 64, 64 * 1, 64, 64);
			optionLife->Render(1220, 550, 128, 128, 0, 64 * 1, 64, 64);
			break;
		}
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
			//OptionInitialize();
			break;
		}

		//	���[�h�ؑ�
		this->mode = mode;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------