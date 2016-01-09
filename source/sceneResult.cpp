
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

#define	NO_BONUS	-1

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
		menuHead.obj = new iex2DObj( "DATA/UI/menu/menu-head.png" );
		originNumber = new iex2DObj( "DATA/UI/number.png" );
		menuText = new iex2DObj( "DATA/UI/result/result-cho.png" );
		lastBonusText = new iex2DObj( "DATA/UI/Result/LastBonusText.png" );

		//	���f���ǂݍ���
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );			//	�|����
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/�v�����Z�X/prinsess1.IEM" ) );					//	�P
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Thief/Thief.IEM" ) );				//	���X
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );					//	�g��

		//	�I���W�i�����f����񏉊���
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.04f );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetScale( 0.03f );				//	����
		org[CHARACTER_TYPE::PIRATE]->SetScale( 0.02f );				//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );		//	�P
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetAngle( D3DX_PI );			//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );		//	�|����
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );			//	�P
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );	 				//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	�g��

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	�|����
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	�P
		org[CHARACTER_TYPE::THIEF]->Update();							//	�V�[�t
		org[CHARACTER_TYPE::PIRATE]->Update();						//	�g��

		//	���f��������
		ModelInitialize();

		//	�\���̏�����
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
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
		bonusPlayer = 0;

		//	���ʗp���\���̏�����
		ResultInfoInitialize();

		//	�\�[�g������
		//Sort( maxCoinNum );
		//Sort( fallStageNum );
		//ReverseSort( coin77 );
		//ReverseSort( minCoinNum );
		//Sort( hitAttackNum );

		////	���X�g�{�[�i�X�ݒ�
		//SetLastBonus();
		//AddLastBonus();
		Sort( sortInfo );
		
		//	�����L���O�ݒ�
		SetRank();

		//	���񃉃C�t�ݒ�
		SetNextLife();

		//	���l�\���̏�����
		NumberImageInfoInitialize();
		
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
				x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
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

		//	���X�g�{�[�i�X�֘A������
		{
			FOR( 0, PLAYER_MAX )	lastBonusInfo.bonus[value];
			lastBonusInfo.step = 0;
			lastBonusInfo.t = 0.0f;
			
			//	�e�L�X�g������
			int		x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
			int		y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
			int		w =static_cast<int>( iexSystem::ScreenWidth * 0.2f );
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
		return	true;
	}

	//	���
	void	sceneResult::Release( void )
	{
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		SafeDelete( lastBonusText );
		SafeDelete( faceImage.obj );
		SafeDelete( playerNumImage.obj );
		SafeDelete( waveCircleImage.obj );
		SafeDelete( notApplicable.obj );
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
			originInfo[i].num = gameManager->GetCoinTotal( i ) + gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	�����L���O�v�Z�p�ɑ��v�f�[�^���i�[( �{�[�i�X���l����������A���̃R�C�������Ƀ{�[�i�X�𑫂��A�����N�̓\�[�g�ɂ����邽�ߓK���ɑ�� )
			sortInfo[i].num = originInfo[i].num;
			sortInfo[i].rank = i;
			sortInfo[i].sortRank = i;

			//	���X�g�{�[�i�X�̏����ݒ�
			maxCoinNum[i].num = gameManager->GetMaxCoinNum( i );
			maxCoinNum[i].rank = i;
			maxCoinNum[i].sortRank = i;
			fallStageNum[i].num = gameManager->GetFallStageNum( i );
			fallStageNum[i].rank = i;
			fallStageNum[i].sortRank = i;
			coin77[i].num = gameManager->GetSubCoin77( i );
			coin77[i].rank = i;
			coin77[i].sortRank = i;
			minCoinNum[i].num = gameManager->GetTotalCoinNum( i );
			minCoinNum[i].rank = i;
			minCoinNum[i].sortRank = i;
			hitAttackNum[i].num = gameManager->GetHitAttackNum( i );
			hitAttackNum[i].rank = i;
			hitAttackNum[i].sortRank = i;
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
			bonusNumberImageInfo[i].pos.x = static_cast<int>( out.x + ( iexSystem::ScreenWidth * 0.05f ) );
			bonusNumberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.45f );
			bonusNumberImageInfo[i].scale = 70;

			//	�e�ʉ摜�ݒ�
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;
			bonusNumberImageInfo[i].one.obj = originNumber;
			bonusNumberImageInfo[i].ten.obj = originNumber;
			bonusNumberImageInfo[i].hundred.obj = originNumber;
		
			//	���l�摜�\���̏�����
			SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
			SetNumberImageInfo( bonusNumberImageInfo[i], bonusNumber[i], originInfo[i].bonus );
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

		//	���X�g�{�[�i�X�p�|���S���`��
		iexPolygon::Render2D( lastBonusInfo.v, 2, nullptr, RS_COPY );

		//	���X�g�{�[�i�X�e�L�X�g�`��
		int	sx = lastBonusInfo.textImage.sx;
		int	sy = lastBonusInfo.textImage.sy;
		int	sw = lastBonusInfo.textImage.sw;
		int	sh = lastBonusInfo.textImage.sh;
		RenderImage( lastBonusInfo.textImage, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
		RenderImage( lastBonusInfo.textImage, sx, sy, sw, sh, IMAGE_MODE::WAVE );

		//	�~���`��
		RenderImage( waveCircleImage, 0, 0, 512, 512, IMAGE_MODE::WAVE );

		//	�v���C���[��`��
		sx = faceImage.sx;
		sy = faceImage.sy;
		sw = faceImage.sw;
		sh = faceImage.sh;
		RenderImage( faceImage, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

		//	�Y���Ȃ������`��
		sx = notApplicable.sx;
		sy = notApplicable.sy;
		sw = notApplicable.sw;
		sh = notApplicable.sh;
		RenderImage( notApplicable, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

		//	�v���C���[�ԍ��`��
		sx = playerNumImage.sx;
		sy = playerNumImage.sy;
		sw = playerNumImage.sw;
		sh = playerNumImage.sh;
		RenderImage( playerNumImage, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

		//	���j���[�p�X�N���[���`��
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		//	���j���[���ڕ`��
		SelectRender();
	}

	//	���Ԍ��ʔ��\
	void	sceneResult::IntermediateResultsUpdate( void )
	{

	}

	//	���U���g���̍X�V
	void	sceneResult::ResultUpdate( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false; 
		bool	isEndWave = false;

		//	�i�K���Ƃ̏���
		switch ( step )
		{
		case 0:
			//-----------------------------------------------------------------------------------
			//	���[���b�g
			//-----------------------------------------------------------------------------------
				isEnd = Roulette();

				//	����{�^���ŃX�L�b�v
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					FOR( 0, PLAYER_MAX )
					{
						SetNumberImageInfo( value, originInfo[value].num );
						SetWave( rankImage[value], 1.5f );
						rankImage[value].renderflag = true;
					}	
					step = 3;
				}

				//	��]���I��������
				if ( isEnd )		step = 2;
				break;
		
		case 1:
			//-----------------------------------------------------------------------------------
			//	���X�g�{�[�i�X���o
			//-----------------------------------------------------------------------------------
			isEnd = LastBonusUpdate();
			if ( isEnd )	step++;
			break;
			
		case 2:
			//-----------------------------------------------------------------------------------
			//	���ʔ��\
			//-----------------------------------------------------------------------------------
				isFinViewRankInOrder = ViewRankInOrder();
				isEnd = RankWave();

				//	����{�^���ŃX�L�b�v
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					FOR( 0, PLAYER_MAX )
					{
						SetNumberImageInfo( value, originInfo[value].num );
						SetWave( rankImage[value], 1.5f );
						rankImage[value].renderflag = true;
					}
					step = 3;
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

		case 3:
			//-----------------------------------------------------------------------------------
			//	���ʔ��\(�X�L�b�v�p)
			//-----------------------------------------------------------------------------------
				FOR( 0, PLAYER_MAX )	isEnd = WaveUpdate( rankImage[value] );

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
				else										menuImage[i].sx = 0;
			}

			//	����
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				changeScene = true;
			}
			break;
		}
	}

	//	�Z���N�g��ʕ`��
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
			//-----------------------------------------------------------------------------------------------
			//	�R�C�������`��
			//-----------------------------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------------------------
			//	�{�[�i�X���l�`��
			//-----------------------------------------------------------------------------------------------
			//	�P�O�O�̈ʕ`��
			sx = bonusNumberImageInfo[i].hundred.sx;
			sy = bonusNumberImageInfo[i].hundred.sy;
			sw = bonusNumberImageInfo[i].hundred.sw;
			sh = bonusNumberImageInfo[i].hundred.sh;

			if ( bonusNumberImageInfo[i].hundredRenderFlag )
				RenderImage( bonusNumberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	�P�O�̈ʕ`��
			sx = bonusNumberImageInfo[i].ten.sx;
			sy = bonusNumberImageInfo[i].ten.sy;
			sw = bonusNumberImageInfo[i].ten.sw;
			sh = bonusNumberImageInfo[i].ten.sh;
			RenderImage( bonusNumberImageInfo[i].ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	�P�̈ʕ`��
			sx = bonusNumberImageInfo[i].one.sx;
			sy = bonusNumberImageInfo[i].one.sy;
			sw = bonusNumberImageInfo[i].one.sw;
			sh = bonusNumberImageInfo[i].one.sh;
			RenderImage( bonusNumberImageInfo[i].one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
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

	//	���l�ɂ��킹�Đ��l�\���̏���ݒ�,�P�O�O�ȏォ�Ŕz�u���ύX
	void	sceneResult::SetNumberImageInfo( const int& player, const int& coin )
	{
		//	�����m�F
		if ( coin >= 100 )		numberImageInfo[player].hundredRenderFlag = true;
		else							numberImageInfo[player].hundredRenderFlag = false;

		if ( originInfo[player].bonus >= 100 )		bonusNumberImageInfo[player].hundredRenderFlag = true;
		else														bonusNumberImageInfo[player].hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( number[player], coin );
		SetNumberInfo( bonusNumber[player], originInfo[player].bonus );

		//	�e�ʉ摜�\���̏�����
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	�����p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------------------------
		//	�{�[�i�X�p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
		if ( bonusNumberImageInfo[player].hundredRenderFlag )
		{
			//	�P�O�̈ʐݒ�
			x = bonusNumberImageInfo[player].pos.x;
			y = bonusNumberImageInfo[player].pos.y;
			w = h = bonusNumberImageInfo[player].scale;
			sx = bonusNumber[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( bonusNumberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�
			x = bonusNumberImageInfo[player].pos.x - static_cast<int>( bonusNumberImageInfo[player].ten.w / 1.5f );
			sx = bonusNumber[player].hundred * 64;
			ImageInitialize( bonusNumberImageInfo[player].hundred, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = bonusNumberImageInfo[player].pos.x + static_cast<int>( bonusNumberImageInfo[player].ten.w / 1.5f );
			sx = bonusNumber[player].one * 64;
			ImageInitialize( bonusNumberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	�P�O�̈ʐݒ�
			w = h = bonusNumberImageInfo[player].scale;
			x = bonusNumberImageInfo[player].pos.x - w / 3;
			y = bonusNumberImageInfo[player].pos.y;
			sx = bonusNumber[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( bonusNumberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = bonusNumberImageInfo[player].pos.x + w / 3;
			sx = bonusNumber[player].one * 64;
			ImageInitialize( bonusNumberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}

		//	��\���ɂ���
		bonusNumberImageInfo[player].one.renderflag = false;
		bonusNumberImageInfo[player].ten.renderflag = false;
		bonusNumberImageInfo[player].hundred.renderflag = false;
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
				SetNumberImageInfo( i, originInfo[i].num );
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

		int	nextRound = gameManager->GetRound() + 1;

		switch ( menuInfo.select )
		{
		case MENU::RESTART:
			//	�Q�[����񏉊���
			gameManager->RetryInitialize();
			gameManager->SetRound( nextRound );
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

	//	���񃉃C�t�ݒ�
	void	sceneResult::SetNextLife( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			gameManager->SetStartLife( value, originInfo[value].rank + 1 );
		}
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
	