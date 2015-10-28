
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
		screen->SetScreenMode( SCREEN_MODE::WHITE_IN, 1.0f );

		//	�摜�ǂݍ���
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		face = make_unique<iex2DObj>( LPSTR( "DATA/UI/chara_emotion.png" ) );
		cursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/cursor.png" ) );
		frame = make_unique<iex2DObj>( LPSTR( "DATA/UI/frame.png" ) );
		checkBack = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Back.png" ) );
		checkCursor = make_unique<iex2DObj>( LPSTR( "DATA/UI/selectCheck_Cursor.png" ) );
		playerNumText = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum_Text.png" ) );
		playerNum = make_unique<iex2DObj>( LPSTR( "DATA/UI/playerNum.png" ) );

		//	���f���ǂݍ���
		//org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	�R�m
		org[CHARACTER_TYPE::SCAVENGER] = new iex3DObj( "DATA/CHR/Knight/Knight_Dammy.IEM" );		//	�|����
		org[CHARACTER_TYPE::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );					//	�P
		org[CHARACTER_TYPE::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );		//	���X
		org[CHARACTER_TYPE::TIGER] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );				//	�g��
		deskStage = make_unique<iexMesh>( LPSTR( "DATA/back/stage.IMO" ) );
		forestStage = make_unique<iexMesh>( LPSTR( "DATA/BG/Forest/model/forest.IMO" ) );

		//	�摜�\���̏�����
		{
			textImage.obj = new iex2DObj( "DATA/UI/menu-head.png" );
			ImageInitialize( textImage, 640, 150, 370, 150, 512, 0, 512, 256 );
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
		Random::Release();

		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( org[i] );
		}
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
		back->Render( 0, 0, 1280, 720, 0, 0, 1280, 720 );
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
		playerNum->Render( 360, 290, 100, 110, 0, playerNumSelectInfo.sy, 128, 128 );
		playerNumText->Render( 450, 300, 500, 100, 0, 0, 512, 128 );
	}

//-------------------------------------------------------------------------------
//	�L�����N�^�[�I�𓮍�
//-------------------------------------------------------------------------------

	//	�L�����N�^�[�I��������
	void	sceneMenu::SelectCharacterInitialize( void )
	{
		//	�v���C���[�ɂ�������A������
		for ( int i = 0; i < 4; i++ )
		{
			obj[i] = org[i];
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->SetScale( 0.02f );
			obj[i]->Update();
		}

		//	�I����񏉊���
		for ( int i = 0; i < 4; i++ )
		{
			characterSelectInfo.character[i] = i;
			characterSelectInfo.select[i] = false;
		}

		//	�ϐ�������
		characterSelectInfo.imagePos = 0;
		characterSelectInfo.step = 0;
		characterSelectInfo.timer = 0;
		characterSelectInfo.playerNum = gameManager->GetPlayerNum();

		//	�摜�ʒu������
		ImageInitialize( textImage, 640, 150, 370, 150, 512, 0, 512, 256 );
		textImage.renderflag = true;
	}

	//	�L�����N�^�[�I��
	void	sceneMenu::SelectCharacterUpdate( void )
	{
		for ( int i = 0; i < characterSelectInfo.playerNum; i++ )
		{
			//	�L�����Z��
			if ( input[i]->Get( KEY_B ) == 3 )		characterSelectInfo.select[i] = false;

			//	�I���ς݂�������L�����Z���̂ݎ�t��
			if ( characterSelectInfo.select[i] )	continue;

			//	�J�[�\���ړ�
			if ( input[i]->Get( KEY_RIGHT ) == 3 )	characterSelectInfo.character[i]++;
			if ( input[i]->Get( KEY_LEFT ) == 3 )		characterSelectInfo.character[i]--;

			//	����E�����ݒ�
			if ( characterSelectInfo.character[i] < 0 )	characterSelectInfo.character[i] = CHARACTER_TYPE::MAX - 1;
			if ( characterSelectInfo.character[i] >= CHARACTER_TYPE::MAX )	characterSelectInfo.character[i] = 0;

			//	����
			if ( input[i]->Get( KEY_SPACE ) == 3 || input[i]->Get( KEY_A ) == 3 )
			{
				gameManager->SetCharacterType( i, characterSelectInfo.character[i] );
				characterSelectInfo.select[i] = true;
			}

			//	���f�������ւ�
			obj[i] = org[characterSelectInfo.character[i]]->Clone();
		}

		//	�I���`�F�b�N
		int		selectCheck = 0;
		for ( int i = 0; i < gameManager->GetPlayerNum(); i++ )
		{
			if ( characterSelectInfo.select[i] )	selectCheck++;
		}

		//	�S�����I��
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
			for ( int i = gameManager->GetPlayerNum(); i < 4; i++ )
			{
				characterSelectInfo.character[i] = Random::GetInt( 0, CHARACTER_TYPE::MAX - 1 );
				gameManager->SetCharacterType( i, characterSelectInfo.character[i] );
			
				//	���f�������ւ�
				obj[i] = org[characterSelectInfo.character[i]]->Clone();
			}
			SetMode( MENU_MODE::SELECT_STAGE );
		}

		//	���f���f�[�^�X�V
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Animation();
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetScale( 0.02f );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->Update();
		}
	}

	//	�L�����N�^�[�I��`��
	void	sceneMenu::SelectCharacterRender( void )
	{
		//	���f���E��E�J�[�\���`��
		for ( int i = 0; i < 4; i++ )
		{
			//	���f���`��
			obj[i]->Render( shader3D, "toon" );
			
			//	��`��
			int	 x = 230 + ( 670 / 3 * i );
			Vector3	color = Vector3( 1.0f, 1.0f, 1.0f );
			face->Render( x, 280, 150, 150, 0, 256 * i, 256, 256, GetColor( color ) );

			//	�J�[�\���`��
			x = 230 + ( 670 / 3 * characterSelectInfo.character[i] );
			if ( !characterSelectInfo.select[i] )
				cursor->Render( x + 35 * i, 250, 50, 50, 128 * ( i % 2 ), 128 * ( i / 2 ), 128, 128 );
		}

		//	�e�L�X�g�摜
		RenderImage( textImage, 512, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );
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
		forestStage->SetPos( 5.0f, 3.0f, 0.0f );
		forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
		forestStage->SetScale( 0.04f );
		forestStage->Update();

		//	�p�����[�^������
		stageSelectInfo.angle = D3DX_PI;
		stageSelectInfo.stage = 0;

		//	�摜�ʒu������
		ImageInitialize( textImage, 640, 150, 370, 150, 0, 256, 512, 256 );
		textImage.angle = D3DXToRadian( 5.0f );
		textImage.renderflag = true;
	}

	//	�X�e�[�W�I��
	void	sceneMenu::SelectStageUpdate( void )
	{
		//	�X�e�[�W��]
		stageSelectInfo.angle += 0.01f;

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
		//	�e�L�X�g�摜
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		deskStage->Render();
		forestStage->Render();

		//	�e�L�X�g�摜
		RenderImage( textImage, 0, 256, 512, 256, IMAGE_MODE::ADOPTPARAM );
	}

//-------------------------------------------------------------------------------
//	�ŏI�`�F�b�N����
//-------------------------------------------------------------------------------

	//	�ŏI�m�F������
	void	sceneMenu::SelectCheckInitialize( void )
	{
		//	�v���C���[�ɂ�������A������
		for ( int i = 0; i < 4; i++ )
		{
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetAngle( D3DX_PI );
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->SetScale( 0.02f );
			obj[i]->Update();
		}

		//	�\���̏�����
		{
			checkSelectInfo.check = false;
			checkSelectInfo.select = false;
			checkSelectInfo.step = 0;
		}

		//	�X�e�[�W���W�ݒ�
		{
			deskStage->SetPos( 0.0f, 5.0f, 0.0f );
			deskStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			deskStage->SetScale( 0.08f );
			deskStage->Update();
			forestStage->SetPos( 0.0f, 5.0f, 0.0f );
			forestStage->SetAngle( D3DXToRadian( 30.0f ), D3DX_PI, 0.0f );
			forestStage->SetScale( 0.03f );
			forestStage->Update();
		}
	}

	//	�ŏI�m�F
	void	sceneMenu::SelectCheckUpdate( void )
	{
		//	����
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

		//	�I��
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
			//	�L�����Z��
			if ( input[0]->Get( KEY_B ) == 3 )
			{
				SetMode( MENU_MODE::SELECT_STAGE );
			}
		}

		//	���f���X�V
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Animation();
			obj[i]->Update();
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
		for ( int i = 0; i < 4; i++ )
		{
			obj[i]->Render( shader3D, "toon" );
		}

		//	�e�L�X�g�摜�`��
		RenderImage( textImage, 0, 512, 512, 256, IMAGE_MODE::ADOPTPARAM );
		
		//	�`�F�b�N���ڕ`��
		if ( checkSelectInfo.check )
		{
			checkBack->Render( 390, 120, 500, 500, 0, 0, 512, 512 );
			checkCursor->Render( 440, 450, 400, 100, 0, checkSelectInfo.select * 128, 512, 128 );
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
		screen->SetScreenMode( SCREEN_MODE::WHITE_OUT, 1.0f );
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
		}

		//	���[�h�ؑ�
		this->mode = mode;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------