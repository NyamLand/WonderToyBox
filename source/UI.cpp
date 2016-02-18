#include	<memory>

#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"Camera.h"
#include	"sceneTitle.h"
#include	"EventManager.h"
#include	"Particle.h"
#include	"Sound.h"
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
			const int WIDTH		= iexSystem::ScreenWidth;
			const int HEIGHT	= iexSystem::ScreenHeight;
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

		//	�^�C�g���e�L�X�g�ǂݍ��ݍ��W���
		namespace TITLE_TEXT_SRCPOS_INFO
		{
			enum
			{
				MENU_SRC_POS_X,
				MENU_SRC_POS_Y,
			};
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
	bool	UI::Initialize( void )
	{
		MainInitialize();
		return	true;
	}

	//	���
	void	UI::Release( void )
	{
		MainRelease();
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	UI::Update( const int& mode )
	{
		MainUpdate( mode );
	}

	//	�`��
	void	UI::Render( const int& mode )
	{
		MainRender( mode );
	}
	
//------------------------------------------------------------------------------
//	�e�V�[��������
//------------------------------------------------------------------------------

	//	���C���p������
	void	UI::MainInitialize( void )
	{
		timer.obj			= new iex2DObj("DATA/UI/timer.png");
		face				= new iex2DObj("DATA/UI/chara_emotion.png");
		finishImage.obj		= new iex2DObj( "DATA/UI/bfUI.png");
		countImage.obj		= new iex2DObj( "DATA/UI/bfUI_02.png" );
		alertImage.obj		= new iex2DObj( "DATA/UI/alert.png" );
		alert_coinImage.obj = new iex2DObj( "DATA/UI/coin_alert.png" );
		playerNumber		= new iex2DObj( "DATA/UI/number.png" );
		life				= new iex2DObj( "DATA/UI/NLife.png" );
		crown				= new iex2DObj( "DATA/UI/1stCrown.png" );
		pCoinNumImage		= new iex2DObj("DATA/UI/number.png");
		roundImage.obj		= new iex2DObj( "DATA/UI/roundText.png" );
		startNumber			= new iex2DObj("DATA/UI/DonketuUI.png");

		//	�p�[�e�B�N���p�o�b�t�@
		PAR_POS			= Vector3(100.0f, 100.0f, 100.0f);
		target_par		= std::make_unique<iex2DObj>(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);
		particle_camera = std::make_unique<Camera>();
		particle_camera->SetPos(Vector3(0.0f, 10.0f, -10.0f) + PAR_POS);
		
		//	���ʕϐ������� 
		changeflag = false;
		FOR( 0, PLAYER_MAX )
		{
			faceImage[value].obj	= face;
			charatype[value]		= gameManager->GetCharacterType(value);
			startNum[value].obj		= startNumber;
			coin_flg[value]			= false;
			coin_timer[value]		= 0;
		}

		//	�eUI��񏉊���
		CoinNumberInitialize();
		FaceImageInitialize();
		TimerInitialize();
		StartAndTimeUpInitialize();
		StartPlayerNumInitialize();
		AlertInitialize();
		LastProductionInitialize();
		PlayerNumberInitialize();
		LifeInitialize();
		CrownInitialize();
		RoundInitialize();
		EventInitialize();
	}

//------------------------------------------------------------------------------
//	�e�V�[�����
//------------------------------------------------------------------------------

	//	���C���p���
	void	UI::MainRelease( void )
	{
		SafeDelete( timer.obj );
		SafeDelete( face );
		SafeDelete( countImage.obj );
		SafeDelete( finishImage.obj );
		SafeDelete( alertImage.obj );
		SafeDelete( alert_coinImage.obj );
		SafeDelete( playerNumber );
		SafeDelete( life );
		SafeDelete( pCoinNumImage );
		SafeDelete( roundImage.obj );
		SafeDelete( eventInfo.airPlane );
		SafeDelete( startNumber );
	}

//------------------------------------------------------------------------------
//	�e�V�[���X�V
//------------------------------------------------------------------------------

	//	���C���X�V
	void	UI::MainUpdate( int mode )
	{
		PlayerNumberUpdate();
		LifeUpdate();
		CrownUpdate();
		ParticleUpdate();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			StartPlayerNumUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			CoinNumberUpdate();
			AlertUpdate();
			EventUpdate();
			break;
			
		case GAME_MODE::CLIMAX:
			TimerUpdate();
			CoinNumberUpdate();
			LastProduction();
			AlertUpdate();
			EventUpdate();
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}
	}

//------------------------------------------------------------------------------
//	�e�V�[���`��
//------------------------------------------------------------------------------

	//	���C���`��
	void	UI::MainRender( int mode )
	{

		LifeRender();
		CrownRender();
		RoundRender();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartPlayerNumRender();
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			CoinNumberRender();
			EventRender();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			TimerRender();
			CoinNumberRender();
			break;

		case GAME_MODE::CLIMAX:
			LastProductionRender();
			CoinNumberRender();
			EventRender();

			break;

		case GAME_MODE::TIMEUP:
			FinishRender();
			break;
		}

		if ( alertInfo.flag )	AlertRender();
	

	}

//------------------------------------------------------------------------------
//	���C�����쏉����
//------------------------------------------------------------------------------

	//	�R�C������������
	void	UI::CoinNumberInitialize( void )
	{
		//	�R�C�������ʒu�i���j
		coinNumInfo[0].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.1 );
		coinNumInfo[1].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.3 );
		coinNumInfo[2].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.7 );
		coinNumInfo[3].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.9 );

		//	�\���F
		coinColor[0] = characterManager->GetDamageColor(0);
		coinColor[1] = characterManager->GetDamageColor(1) + Vector3( 0.7f, 0.7f, 0.0f );		//	���₷�����邽��
		coinColor[2] = characterManager->GetDamageColor(2);
		coinColor[3] = characterManager->GetDamageColor(3);


		FOR( 0, PLAYER_MAX ){
			//	�R�C�������p
			coinNum[value]						= gameManager->GetCoinNum( value );

			//	�R�C�������ʒu�i�c�j
			coinNumInfo[value].pos.y			= static_cast<int>( iexSystem::ScreenHeight * 0.1f );
			coinNumInfo[value].scale			= 100;
			
			//	�摜�ݒ�
			coinNumInfo[value].one.obj			= pCoinNumImage;
			coinNumInfo[value].ten.obj			= pCoinNumImage;
			coinNumInfo[value].hundred.obj		= pCoinNumImage;
			
			//	���ݒ�
			SetCoinImageInfo( coinNumInfo[value], numInfo[value], gameManager->GetCoinNum( value ) );
			
			//	�F�ݒ�	
			coinNumInfo[value].one.color		= coinColor[value];
			coinNumInfo[value].ten.color		= coinColor[value];
			coinNumInfo[value].hundred.color	= coinColor[value];

		
		}
	}

	//	��摜������
	void	UI::FaceImageInitialize( void )
	{
		FOR( 0, PLAYER_MAX ){
			ImageInitialize( faceImage[value], coinNumInfo[value].pos.x, coinNumInfo[value].pos.y,
							 coinNumInfo[value].one.w + coinNumInfo[value].ten.w,
							 coinNumInfo[value].one.h + coinNumInfo[value].ten.h,
							 0, 0, 256, 256 );
		}
	}

	//	�^�C�}�[������
	void	UI::TimerInitialize( void )
	{

		//	�\���̏�����
		int x				= static_cast<int>( iexSystem::ScreenWidth * 0.43f );
		int y				= static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int w				= static_cast<int>( iexSystem::ScreenWidth * 0.05f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.09f );
		ImageInitialize( timer, x, y, w, h, 64, 0, 64, 64 );
		timerInfo.minute	= 0;
		FOR( 0, 2 )
		{
			timerInfo.second[value] = 0;
		}
	}

	//	�J�E���g�_�E���E�X�^�[�g�E�I�����o
	void	UI::StartAndTimeUpInitialize( void )
	{
		int x		= static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y		= static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w		= static_cast<int>( iexSystem::ScreenWidth * 0.27f );
		int h		= static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		ImageInitialize( countImage, x, y, w, h, 0, 0, 512, 512 );
		w			= static_cast<int>( iexSystem::ScreenWidth * 0.49f);
		h			= static_cast<int>( iexSystem::ScreenHeight * 0.27f);
		ImageInitialize( finishImage, x, y, w, h, 0, 512, 1024, 512 );
		countInfo.count			= 0;
		countInfo.waitTimer		= 0;
		countInfo.start_pos		= Vector3((float)countImage.x, -((float)countImage.h / 2.0f), 0.0f);
		countInfo.finish_pos	= Vector3((float)countImage.x, (float)countImage.y, 0.0f);
		countInfo.start_t		= 0.0f;
		countInfo.start_step	= 0;
	}

	//	�J�E���g�_�E�����v���C���[�̔ԍ��\��
	void	UI::StartPlayerNumInitialize( void )
	{
		//	�ϐ�������
		Vector3 imagePos	= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	p_pos		= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	out			= Vector3(0.0f, 0.0f, 0.0f);
		int sx, sy;
		int w				= static_cast<int>(iexSystem::ScreenWidth * 0.15f);
		int h				= static_cast<int>(iexSystem::ScreenHeight * 0.15f);

		FOR( 0, PLAYER_MAX )
		{
			//	�\���̂ɏ��ݒ�
			sx = 0 + 128 * value;
			sy = 256;
 
			ImageInitialize( startNum[value], 0, 0, w, h, sx, sy, 128, 128 );
		}
	}

	//	�x�����o������
	void	UI::AlertInitialize( void )
	{
		alertInfo.flag	= false;
		alertInfo.alpha = 0.0f;
		alertInfo.timer = 0;
		alertInfo.param = 0.0f;
		alertInfo.type	= 0;

		//	�摜�\���̏�����
		int x			= static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y			= static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w			= static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int h			= w;
		ImageInitialize( alertImage, x, y, w, h, 0, 0, 256, 256 );
		ImageInitialize( alert_coinImage, x, y, w, h, 0, 0, 256, 256 );
		alertImage.renderflag		= true;
		alert_coinImage.renderflag	= true;
	}

	//	HurryUp���o������
	void	UI::HurryUpInitialize( void )
	{
		hurryInfo.alpha = 0.0f;
		hurryInfo.flag	= false;
		hurryInfo.param = 0.0f;
		hurryInfo.timer = 0;
	}

	//	�v���C���[�ԍ��摜������
	void	UI::PlayerNumberInitialize( void )
	{
		int rank = 0;
		FOR( 0, PLAYER_MAX )
		{
			rank					= characterManager->GetRank( value );
			pNumImage[value].obj	= playerNumber;
			int w					= static_cast<int>( iexSystem::ScreenWidth * 0.04f );
			int h					= static_cast<int>( iexSystem::ScreenHeight * 0.07f );
			ImageInitialize( pNumImage[value], 0, 0, w, h, rank * 128, 128, 128, 64 );
		}
	}
	
	//	���Ԍx��������
	void	UI::LastProductionInitialize( void )
	{
		SetScaling( timer, 1.0f );
		lasttimerInfo.state = false;
		lasttimerInfo.t		= 0.0f;
		lasttimerInfo.alpha = 1.0f;
	}

	//	�̗͉摜������
	void	UI::LifeInitialize( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			lifeInfo[value].life			= gameManager->GetStartLife( value );
			lifeInfo[value].lifeImage.obj	= life;
			ImageInitialize( lifeInfo[value].lifeImage, 0, 10, 75, 75, 0, 0, 64, 64 );
		}
	}

	//	1�ʉ����摜������
	void	UI::CrownInitialize( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			crownInfo[value].state				= false;
			crownInfo[value].crownImage.obj		= crown;
			ImageInitialize( crownInfo[value].crownImage, 0, 10, 80, 80, 0, 0, 512, 512 );
			crownInfo[value].crownImage.color	= Vector3( 1.0f, 0.5f, 0.5f );
		}
	}

	//	���E���h������
	void	UI::RoundInitialize( void )
	{
		int	x	= static_cast<int>( iexSystem::ScreenWidth * 0.12f );
		int	y	= static_cast<int>( iexSystem::ScreenHeight * 0.9f );
		int	w	= static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int	h	= static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int	sx	= 0;
		int	sy	= gameManager->GetRound() * 128;
		int	sw	= 512;
		int	sh	= 128;
		ImageInitialize( roundImage, x, y, w, h, sx, sy, sw, sh );
	}

	//�@�C�x���g��񏉊���
	void	UI::EventInitialize(void)
	{
		eventInfo.mode			= 0;
		eventInfo.step			= 0;
		eventInfo.state			= 0;
 		eventInfo.airPlane		= new AirPlane();
		eventInfo.texture.obj	= new iex2DObj( "DATA/UI/Event-int.png" );

		int h					= static_cast<int>( iexSystem::ScreenHeight * 0.14f );
		int w					= static_cast<int>( iexSystem::ScreenWidth * 0.6f );
		ImageInitialize( eventInfo.texture, eventInfo.airPlane->IN_START_POS_X, eventInfo.airPlane->IN_START_POS_Y, w, h, 0, 0, 1024, 128 );
	}
	
//------------------------------------------------------------------------------
//	���C������X�V
//------------------------------------------------------------------------------

	//	�^�C�}�[�֘A����
	void	UI::TimerUpdate( void )
	{
		this->timerInfo.time	= gameManager->GetTimer();
		//��
		timerInfo.minute		= this->timerInfo.time / MINUTE % 10;
		//�b�񌅖�
		timerInfo.second[0]		= ( this->timerInfo.time / SECOND ) % 60 / 10 % 10;
		//�b�ꌅ��
		timerInfo.second[1]		= this->timerInfo.time / SECOND % 10;
	}

	//	�R�C����������
	void	UI::CoinNumberUpdate( void )
	{
		FOR( 0, PLAYER_MAX ){
			CoinCounter( gameManager->GetCoinNum(value),value );
			CoinImageInfoUpdate( coinNumInfo[value], numInfo[value], coinNum[value] );
			FaceImageUpdate( value, gameManager->GetCharacterType( value ) );
			
		}
	}

	//	��摜�\���̍X�V
	void	UI::FaceImageUpdate( int num, int mode )
	{
		faceImage[num].alpha	= 0.5f;
		faceImage[num].sy		= 256 * mode;
	}

	//	�J�E���g�_�E���E�X�^�[�g���o
	void	UI::StartUpdate( void )
	{
		int round = gameManager->GetRound();
		static int	waittime;
		switch ( countInfo.start_step )
		{
		//	��ʏ㕔����ړ�
		case 0:
			if ( StartMove() )
			{
				SetScaling( countImage, 4.0f );
				countInfo.start_step++;
				break;
			}
			break;
		//	���n���Ȃ���t�F�[�h�A�E�g
		case 1:
			if (!countImage.scalingFlag)
			{
				countImage.alpha -= 1.0f / 60.0f;
				if ( countImage.alpha <= 0.0f )
				{
					countImage.sx = 512;
					countImage.sy = 512;
					SetScaling( countImage, 3.0f, false );
					countInfo.start_step++;
					break;
				}
			}
			ScalingLandingUpdate( countImage, 100 );
			break;
		//	GO���t�F�[�h�C��
		case 2:
			sound->PlaySE( SE::GAMESTART_SE );
			if ( !countImage.scalingFlag )
			{
				waittime = 90;		//	������~�t���[����
				countInfo.start_step++;
				break;
			}
			ScalingAlphaUpdate( countImage, 300 );
			break;
		//	������~�t���[������AGo���t�F�[�h�A�E�g
		case 3:
			//	�^�C�}�[�̊Ԃ�break;
			waittime--;
			if ( waittime >= 0 )	break;

			changeflag	= true;
			countInfo.waitTimer = 2 * SECOND;
			break;
		}

		//	FIGHT�`��̂��߃X�L�b�v
		if ( countInfo.start_step > 1 ) return;
		//	�ǂݍ��݈ʒu�E�T�C�Y�ݒ�
		switch ( round )
		{
		case Round::ROUND1:
			countImage.sx = 0;
			countImage.sy = 0;
			
			break;

		case Round::ROUND2:
			countImage.sx = 512;
			countImage.sy = 0;
			break;

		case Round::ROUND_FINAL:
			countImage.sx = 0;
			countImage.sy = 512;
			break;
		}

	}

	//	���E���h�����ړ�
	bool	UI::StartMove( void )
	{
		Vector3 pos;

		//	�p�����[�^���Z
		countInfo.start_t += 1.0f / 30.0f;

		if ( countInfo.start_t >= 1.0f )
		{
			return true;
		}

		Lerp( pos, countInfo.start_pos, countInfo.finish_pos, countInfo.start_t );
		countImage.x = ( int )pos.x;
		countImage.y = ( int )pos.y;

		if ( countInfo.start_t >= 1.0f ) countInfo.start_t = 1.0f;

		return false;
	}

	//	�J�E���g�_�E�����v���C���[�̔ԍ��\��
	void	UI::StartPlayerNumUpdate( void )
	{
		//	�ϐ�������
		Vector3 imagePos	= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos		= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out			= Vector3( 0.0f, 0.0f, 0.0f );
		FOR( 0, PLAYER_MAX )
		{
			//	�摜��\������ꏊ��ݒ�
			p_pos				= characterManager->GetPos(value);
			imagePos			= p_pos;

			//	�N���C�A���g���W�ɕϊ�
			WorldToClient( imagePos, out, matView * matProjection );

			//	�\���̂ɏ��ݒ�
			startNum[value].x	= static_cast<int>( out.x );
			startNum[value].y	= static_cast<int>( out.y );
		}
	}

	//	�^�C���A�b�v���o
	void	UI::FinishUpdate( void )
	{
		countInfo.waitTimer--;

		if ( countInfo.waitTimer <= 0 )	changeflag = true;
	}

	//	�x�����o
	void	UI::AlertUpdate( void )
	{
		if ( !alertInfo.flag ) return;

		{
			alertInfo.param += D3DX_PI / 30.0f;
			alertInfo.alpha = 0.1f + 0.1f * sinf(alertInfo.param);

			alertInfo.timer++;
			if ( alertInfo.timer % 15 == 0 )
			{
				alertImage.renderflag		= !alertImage.renderflag;
				alert_coinImage.renderflag	= !alert_coinImage.renderflag;
			}

			//	��b���ŏI��
			if ( alertInfo.timer >= 120 )
			{
				alertInfo.flag  = false;
				alertInfo.alpha = 0.0f;
				alertInfo.timer = 0;
			}
		}

		//�@��s�@����Ă���
		if ( eventInfo.step == 0 )
		{
			//	�ǂ����������Ă���
			eventInfo.airPlane->SetNext( eventInfo.airPlane->IN_START_POS, eventInfo.airPlane->STAY_POS, AirPlane::FLYING_IN );
			eventInfo.step++;
		}

		eventInfo.airPlane->Update();
		eventInfo.texture.x		= ( int )eventInfo.airPlane->GetPos().x;
		eventInfo.texture.y		= ( int )eventInfo.airPlane->GetPos().y;
		eventInfo.texture.sy	= eventInfo.mode * 128;
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
		//	30�b�`4�b
		if ( !lasttimerInfo.state ){
			ScalingUpdate(timer, 100);
			if ( gameManager->GetTimer() / SECOND <= 3 )
			{
				SetScaling( timer, 1.0f );
				lasttimerInfo.state = true;
			}
			return;
		}
	
		
		//	3�b�`0�b
		if ( !timer.scalingFlag && gameManager->GetTimer() % 60 == 0 )	SetScaling( timer, 1.0f );


		ScalingAlphaUpdate( timer, 1000 );



	}

	//	�v���C���[�ԍ��ʒu����
	void	UI::PlayerNumberUpdate( void )
	{
		//	�ϐ�������
		Vector3 imagePos	= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos		= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	up			= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out			= Vector3( 0.0f, 0.0f, 0.0f );

		FOR( 0,PLAYER_MAX )
		{
			//	�摜��\������ꏊ��ݒ�
			up				= characterManager->GetUp( value );
			p_pos			= characterManager->GetPos( value );
			imagePos		= p_pos + up * 7.0f;

			//	�N���C�A���g���W�ɕϊ�
			WorldToClient( imagePos, out, matView * matProjection );

			//	�\���̂ɏ��ݒ�
			pNumImage[value].x = static_cast<int>( out.x );
			pNumImage[value].y = static_cast<int>( out.y );

			int rank = 0;
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				rank = characterManager->GetRank( i ) - 1;
				pNumImage[i].sx = rank * 128;
			}
		}
	}

	//	���C�t�X�V
	void	UI::LifeUpdate( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	lifePos;
		Vector3	out;
		int			culLife;

		FOR( 0, PLAYER_MAX )
		{
			//	�\�����W�Z�o
			p_Pos		= characterManager->GetPos( value );
			p_Up		= characterManager->GetUp( value );
			lifePos		= p_Pos + p_Up * 5.0f;
			WorldToClient( lifePos, out, matView * matProjection );

			//	���݂̗͎̑擾
			culLife		= characterManager->GetLife( value );

			//	�`��ʒu�ݒ�
			lifeInfo[value].lifeImage.x		= ( int )out.x;
			lifeInfo[value].lifeImage.y		= ( int )out.y;

			//	�ǂݍ��݈ʒu�ݒ�
			lifeInfo[value].lifeImage.sx	= lifeInfo[value].lifeImage.sw * ( ( 5 - culLife ) % 4 );
			lifeInfo[value].lifeImage.sy	= lifeInfo[value].lifeImage.sh * ( ( 5 - culLife ) / 4 );
		}
	}

	//	�����X�V
	void	UI::CrownUpdate( void )
	{

		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	CrownPos;
		Vector3	out;
		int		lifeSize;

		FOR( 0, PLAYER_MAX )
		{
			//	1�ʈȊO�͔�΂�
			if ( characterManager->GetRank(value) > 1 )
			{
				crownInfo[value].state = false;
				continue;
			}

			//	�\����Ԃ�
			crownInfo[value].state = true;

			//	�\�����W�Z�o
			p_Pos		= characterManager->GetPos( value );
			p_Up		= characterManager->GetUp( value );
			CrownPos	= p_Pos + p_Up * 5.0f;
			lifeSize	= lifeInfo[value].lifeImage.sh;
			WorldToClient( CrownPos, out, matView * matProjection );

			//	�`��ʒu�ݒ�
			crownInfo[value].crownImage.x = ( int )out.x;
			crownInfo[value].crownImage.y = ( int )out.y - lifeSize;

		}
	}

	//�@�C�x���g��UI�i��s�@�����j
	void	UI::EventUpdate( void )
	{
		bool isEnd = !eventManager->GetEventFlag();
		if ( isEnd )	return;

		switch ( eventInfo.step )
		{
		case 1:
			eventInfo.airPlane->Update();
			if ( eventManager->GetState() == EVENT_STATE::END )
			{
				eventInfo.airPlane->SetNext( eventInfo.airPlane->GetPos(), eventInfo.airPlane->OUT_END_POS, AirPlane::FLYING_OUT );
				eventInfo.step++;
			}

			break;

		case 2:
			if ( eventInfo.airPlane->Update() ) eventInfo.step = 0;
			break;
		}

		eventInfo.texture.x		= ( int )eventInfo.airPlane->GetPos().x;
		eventInfo.texture.y		= ( int )eventInfo.airPlane->GetPos().y;
		eventInfo.texture.sy	= eventInfo.mode * 128;
	}

//------------------------------------------------------------------------------
//	���C���`��
//------------------------------------------------------------------------------

	//	�R�C�������`��
	void	UI::CoinNumberRender( void )
	{
		FOR( 0, PLAYER_MAX ){
			RenderImage( faceImage[value], faceImage[value].sx, faceImage[value].sy, faceImage[value].sw, faceImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
		//	15�b�܂Ő����\��
		if ( gameManager->GetTimer() / SECOND >= 15 )
		{
			FOR(0, PLAYER_MAX)
			{
				//	�P�O�O�̈ʕ`��
				int		sx = coinNumInfo[value].hundred.sx;
				int		sy = coinNumInfo[value].hundred.sy;
				int		sw = coinNumInfo[value].hundred.sw;
				int		sh = coinNumInfo[value].hundred.sh;

				if ( coinNumInfo[value].hundredRenderFlag )
					RenderImage( coinNumInfo[value].hundred, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	�P�O�̈ʕ`��
				sx = coinNumInfo[value].ten.sx;
				sy = coinNumInfo[value].ten.sy;
				sw = coinNumInfo[value].ten.sw;
				sh = coinNumInfo[value].ten.sh;
				RenderImage( coinNumInfo[value].ten, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	�P�̈ʕ`��
				sx = coinNumInfo[value].one.sx;
				sy = coinNumInfo[value].one.sy;
				sw = coinNumInfo[value].one.sw;
				sh = coinNumInfo[value].one.sh;
				RenderImage( coinNumInfo[value].one, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				ParticleRender( value );
			}
		}
		//	���X�g15�b�͎����̎���������������Ȃ��Ȃ�
		else
		{
			FOR( 0, PLAYER_MAX )
			{

				//	�P�O�̈ʕ`��
				int		sx = 12 * 64;		//	�H
				int		sy = coinNumInfo[value].ten.sy;
				int		sw = coinNumInfo[value].ten.sw;
				int		sh = coinNumInfo[value].ten.sh;

				RenderImage( coinNumInfo[value].ten, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	�P�̈ʕ`��
				sy = coinNumInfo[value].one.sy;
				sw = coinNumInfo[value].one.sw;
				sh = coinNumInfo[value].one.sh;
				RenderImage( coinNumInfo[value].one, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
			
				ParticleRender( value );
			}
		}


	}

	//	�p�[�e�B�N���`��(�R�C������)
	void	UI::ParticleRender( int value )
	{
		if ( !coin_flg[value] ) return;

		target_par->Render( (int)( coinNumInfo[value].pos.x - coinNumInfo[value].scale * 1.5f ), (int)( coinNumInfo[value].pos.y - coinNumInfo[value].scale ),
			coinNumInfo[value].scale * 3, coinNumInfo[value].scale * 2,
			0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "add" );
	
	}

	//	�^�C�}�[�`��
	void	UI::TimerRender( void )
	{
		RenderImage( timer, timer.sx * timerInfo.minute		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 0, timer.y );
		RenderImage( timer, timer.sx * 10					, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 1, timer.y );
		RenderImage( timer, timer.sx * timerInfo.second[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 2, timer.y );
		RenderImage( timer, timer.sx * timerInfo.second[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 3, timer.y );

	}

	//	�J�E���g�_�E���E�X�^�[�g���o
	void	UI::StartRender( void )
	{
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::SCALING );
	}

	//	�J�E���g�_�E�����v���C���[�̔ԍ��\��
	void	UI::StartPlayerNumRender( void )
	{
		FOR( 0,PLAYER_MAX )
		{
			RenderImage( startNum[value], startNum[value].sx, startNum[value].sy, startNum[value].sw, startNum[value].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	�^�C���A�b�v���o
	void	UI::FinishRender( void )
	{
		RenderImage( finishImage, finishImage.sx, finishImage.sy, finishImage.sw, finishImage.sh, IMAGE_MODE::NORMAL );
	}

	//	�x���`��
	void	UI::AlertRender( void )
	{
		int color;
		switch ( alertInfo.type )
		{
		case ALERT_TYPE_INFO::JAM:
			//�@�t�B���^�F�ݒ�A�t�B���^�`��A�x���摜�`��
			color = GetColor( 1.0f, 0.0f, 0.0f, alertInfo.alpha );	//�@��
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );
			RenderImage( alertImage, alert_coinImage.sx, alert_coinImage.sy, alertImage.sw, alertImage.sh, IMAGE_MODE::NORMAL );
			break;

		case ALERT_TYPE_INFO::COIN:
			color = GetColor( 1.0f, 1.0f, 0.0f, alertInfo.alpha );	//�@��
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );
			RenderImage( alert_coinImage, alert_coinImage.sx, alert_coinImage.sy, alert_coinImage.sw, alert_coinImage.sh, IMAGE_MODE::NORMAL );
			break;

		default:
			break;
		}

		//�@��s�@
		EventRender();
	}

	//	���Ԍx���`��
	void	UI::LastProductionRender( void )
	{
		//	�^�C�}�[�����F��Ԃ�
		timer.sy = 64;
		if ( !lasttimerInfo.state ){
			RenderImage( timer, timer.sx * timerInfo.minute		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 0, timer.y );
			RenderImage( timer, timer.sx * 10					, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 1, timer.y );
			RenderImage( timer, timer.sx * timerInfo.second[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 2, timer.y );
			RenderImage( timer, timer.sx * timerInfo.second[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 3, timer.y );
		}
		else
		{
			if ( !timer.scalingFlag ) return;

			timer.x = iexSystem::ScreenWidth / 2;	timer.y = iexSystem::ScreenHeight / 2;
			RenderImage( timer, timer.sx * timerInfo.second[1], timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING );
		}
		//	�^�C�}�[�����F�𔒂�
		timer.sy = 0;

	}

	//	���C�t�`��
	void	UI::LifeRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( lifeInfo[value].lifeImage, lifeInfo[value].lifeImage.sx, lifeInfo[value].lifeImage.sy, lifeInfo[value].lifeImage.sw, lifeInfo[value].lifeImage.sh, IMAGE_MODE::NORMAL );
		}
	}

	//	�����`��
	void	UI::CrownRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( crownInfo[value].state )
			{
				RenderImage( crownInfo[value].crownImage, crownInfo[value].crownImage.sx, crownInfo[value].crownImage.sy, crownInfo[value].crownImage.sw, crownInfo[value].crownImage.sh, IMAGE_MODE::ADOPTPARAM );
			}
		}
	}

	//	���E���h�`��
	void	UI::RoundRender( void )
	{
		RenderImage( roundImage, roundImage.sx, roundImage.sy, roundImage.sw, roundImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	�C�x���g�֘A���`��i��s�@�\���j
	void	UI::EventRender( void )
	{
		if ( eventInfo.texture.x <= static_cast<int>( iexSystem::ScreenWidth ) ||
			eventInfo.texture.x + eventInfo.texture.sx >= 0 )
			RenderImage( eventInfo.texture, eventInfo.texture.sx, eventInfo.texture.sy, eventInfo.texture.sw, eventInfo.texture.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	�����_�[�^�[�Q�b�g�Ńp�[�e�B�N��
	void	UI::RenderTargetParticle( void )
	{
		//	�o�b�N�o�b�t�@�Ƀp�[�e�B�N��
		target_par->RenderTarget( 0 );

		particle_camera->Activate();
		particle_camera->Clear();
		particle->Render();

	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	�R�C��������1�����J�E���g�A�b�v�_�E��
	void	UI::CoinCounter( int coin, int num )
	{
		if (coinNum[num] == coin)	return;

		//	�R�C������
		if( coinNum[num] > coin )
		{
			coinNum[num]--;
		}

		//	�R�C������
		if ( coinNum[num] < coin )
		{
			coinNum[num]++;
			coin_flg[num] = true;
			particle->CoinGet( PAR_POS , 0.5f );
		}
	
	}

	//	�ݒ肵�����l�ɂ��킹�č\���̏���ݒ�A�P�O�O�ȏォ�Ŕz�u���ύX
	void	UI::CoinImageInfoUpdate( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	�����m�F
		if ( num >= 100 )				numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( numInfo, num );

		//------------------------------------------------------------------------------------------------
		//	�����p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	�P�O�̈ʐݒ�
			numImageInfo.ten.x		= numImageInfo.pos.x;
			numImageInfo.ten.sx		= numInfo.ten * 64;

			//	�P�O�O�̈ʐݒ�
			numImageInfo.hundred.sx = numInfo.hundred * 64;

			//	�P�̈ʐݒ�
			numImageInfo.one.x		= numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			numImageInfo.one.sx		= numInfo.one * 64;
		}
		else
		{
			//	�P�O�̈ʐݒ�
			numImageInfo.ten.x		= numImageInfo.pos.x - numImageInfo.scale / 3;
			numImageInfo.ten.sx		= numInfo.ten * 64;

			//	�P�̈ʐݒ�
			numImageInfo.one.x		= numImageInfo.pos.x + numImageInfo.scale / 3;
			numImageInfo.one.sx		= numInfo.one * 64;
		}
	}

	//	�p�[�e�B�N���X�V
	void	UI::ParticleUpdate( void )
	{
		particle_camera->Update( VIEW_MODE::FIX, PAR_POS );
		particle->Update();

		FOR( 0, PLAYER_MAX )
		{
			//	�p�[�e�B�N����30�t���[���Ԃ����`�拖����
			if ( coin_flg[value] )
			{
				coin_timer[value]++;
				if ( coin_timer[value] > 30 )
				{
					coin_flg[value]		= false;
					coin_timer[value]	= 0;
				}
			}
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

	//	���[�h�ύX�t���O�擾
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

	//	�ݒ肵�����l�ɂ��킹�č\���̏���ݒ�A�P�O�O�ȏォ�Ŕz�u���ύX
	void	UI::SetCoinImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	�����m�F
		if ( num >= 100 )				numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( numInfo, num );

		//	�e�ʉ摜�\���̏�����
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	�����p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
		if (numImageInfo.hundredRenderFlag)
		{
			//	�P�O�̈ʐݒ�
			x	= numImageInfo.pos.x;
			y	= numImageInfo.pos.y;
			w	= h = numImageInfo.scale;
			sx	= numInfo.ten * 64;
			sy	= 0;
			sw	= sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�
			x	= numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x	= numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	�P�O�̈ʐݒ�
			w	= h = numImageInfo.scale;
			x	= numImageInfo.pos.x - w / 3;
			y	= numImageInfo.pos.y;
			sx	= numInfo.ten * 64;
			sy	= 0;
			sw	= sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x	= numImageInfo.pos.x + w / 3;
			sx	= numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�(�������̎��̂݃Z�b�g���邽�߁j
			x	= numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );
		}
	}

	//	���l�\���̂ɒl���Z�b�g����
	void	UI::SetNumberInfo( NumberInfo& number, int coin )
	{
		number.hundred	= coin / 100 % 10;
		number.ten		= coin / 10 % 10;
		number.one		= coin % 10;
	}

	//	�C�x���g���A���[�h�ݒ�
	void	UI::SetEventInfoMode( int mode )
	{
		this->eventInfo.mode = mode;
	}