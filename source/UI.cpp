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

	//	������(���݂̃V�[��)
	bool	UI::Initialize( void )
	{
		//	�摜�Z�b�g
		timer.obj = new iex2DObj("DATA/UI/timer.png");
		finishImage.obj = new iex2DObj("DATA/UI/bfUI.png");
		countImage.obj = new iex2DObj("DATA/UI/bfUI_02.png");
		alertImage.obj = new iex2DObj("DATA/UI/alert.png");
		alert_coinImage.obj = new iex2DObj("DATA/UI/coin_alert.png");
		playerNumber = new iex2DObj("DATA/UI/number.png");
		roundImage.obj = new iex2DObj("DATA/UI/roundText.png");
		startNumber = new iex2DObj("DATA/UI/DonketuUI.png");
		redAlert.obj = new iex2DObj("DATA/UI/last.png");
		
		//	���ʕϐ������� 
		changeflag = false;
		FOR(0, PLAYER_MAX)
		{
			charatype[value] = gameManager->GetCharacterType(value);
			startNum[value].obj = startNumber;
		}

		//	���C�tUI������
		lifeUI = new LifeUI();
		lifeUI->Initialize();

		//	���C�t�������o
		fallLife = new FallLife();
		fallLife->Initialize();

		//	����������
		crown = new Crown();
		crown->Initialize();

		//	�v���C���[�ԍ�UI������
		playerNumUI = new PlayerNumUI();
		playerNumUI->Initialize();

		//	�R�C������UI������
		coinNumUI = new CoinNumUI();
		coinNumUI->Initialize();

		//	��i�w�i�j������
		faceUI = new FaceUI();
		faceUI->Initialize();

		//	�eUI��񏉊���
		TimerInitialize();
		StartAndTimeUpInitialize();
		StartPlayerNumInitialize();
		AlertInitialize();
		LastProductionInitialize();
		PlayerNumberInitialize();
		RoundInitialize();
		EventInitialize();
		return	true;
	}

	//	���
	void	UI::Release( void )
	{
		SafeDelete( playerNumUI );
		SafeDelete( fallLife );
		SafeDelete( crown );
		SafeDelete( lifeUI );
		SafeDelete( timer.obj );
		SafeDelete( countImage.obj );
		SafeDelete( finishImage.obj );
		SafeDelete( alertImage.obj );
		SafeDelete( alert_coinImage.obj );
		SafeDelete( redAlert.obj );
		SafeDelete( playerNumber );
		SafeDelete( roundImage.obj );
		SafeDelete( eventInfo.airPlane );
		SafeDelete( startNumber );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V(���݂̃V�[��)
	void	UI::Update( const int& mode )
	{
		PlayerNumberUpdate();
		playerNumUI->Update();
		lifeUI->Update();
		fallLife->Update();
		crown->Update();
		coinNumUI->CoinEffectUpdate();
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			StartPlayerNumUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			coinNumUI->Update();
			faceUI->Update();
			AlertUpdate();
			EventUpdate();
			break;

		case GAME_MODE::CLIMAX:
			TimerUpdate();
			coinNumUI->Update();
			faceUI->Update();
			LastProduction();
			AlertUpdate();
			EventUpdate();
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}
	}

	//	�`��(���݂̃V�[��)
	void	UI::Render( const int& mode )
	{
		lifeUI->Render();
		fallLife->Render();
		crown->Render();
		playerNumUI->Render();
		RoundRender();

		switch (mode)
		{
		case GAME_MODE::GAMESTART:
			StartPlayerNumRender();
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			faceUI->Render();
			coinNumUI->Render();
			EventRender();
			break;

		case GAME_MODE::CLIMAX:
			LastProductionRender();
			faceUI->Render();
			coinNumUI->Render();
			EventRender();

			break;

		case GAME_MODE::TIMEUP:
			FinishRender();
			break;
		}

		if ( alertInfo.flag )		AlertRender();
	}
	
//------------------------------------------------------------------------------
//	���C�����쏉����
//------------------------------------------------------------------------------

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
		w			= static_cast<int>( iexSystem::ScreenWidth * 0.49f );
		h			= static_cast<int>( iexSystem::ScreenHeight * 0.27f );
		ImageInitialize( finishImage, x, y, w, h, 0, 512, 1024, 512 );
		countInfo.count			= 0;
		countInfo.waitTimer		= 0;
		countInfo.start_pos		= Vector3( ( float )countImage.x, -( (float )countImage.h / 2.0f ), 0.0f );
		countInfo.finish_pos	= Vector3( ( float )countImage.x, ( float )countImage.y, 0.0f );
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
		int w				= static_cast<int>( iexSystem::ScreenWidth * 0.15f );
		int h				= static_cast<int>( iexSystem::ScreenHeight * 0.15f );

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
		//	��ʐԗp������					��ʒ��S��X���W				��ʒ��S��Y���W			
		ImageInitialize( redAlert, iexSystem::ScreenWidth / 2, iexSystem::ScreenHeight / 2, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 512, 512 );
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
		//roundImage.angle = D3DXToRadian( -30.0f );
	}

	//�@�C�x���g��񏉊���
	void	UI::EventInitialize( void )
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
			if ( !countImage.scalingFlag )
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

			changeflag			= true;
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
			alertInfo.alpha = 0.1f + 0.1f * sinf( alertInfo.param );

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

	//	���Ԍx�����o
	void	UI::LastProduction( void )
	{
		//	30�b�`4�b
		if ( !lasttimerInfo.state ){
			ScalingUpdate( timer, 100 );
			FlashingUpdate( redAlert, 0.1f );
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
		//
		RenderImage(redAlert, redAlert.sx, redAlert.sy, redAlert.sw, redAlert.sh, IMAGE_MODE::FLASH);

		//	�^�C�}�[�����F�𔒂�
		timer.sy = 0;

	}

	//	���E���h�`��
	void	UI::RoundRender( void )
	{
		RenderImage( roundImage, roundImage.sx, roundImage.sy, roundImage.sw, roundImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	�C�x���g�֘A���`��i��s�@�\���j
	void	UI::EventRender(void)
	{
		if ( eventInfo.texture.x <= static_cast<int>( iexSystem::ScreenWidth ) ||
			eventInfo.texture.x + eventInfo.texture.sx >= 0 )
			//eventInfo.airPlane->Render();
			RenderImage( eventInfo.texture, eventInfo.texture.sx, eventInfo.texture.sy, eventInfo.texture.sw, eventInfo.texture.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	�����_�[�^�[�Q�b�g�Ńp�[�e�B�N��
	void	UI::RenderTargetParticle( void )
	{
		////	�o�b�N�o�b�t�@�Ƀp�[�e�B�N��
		//target_par->RenderTarget( 0 );

		//particle_camera->Activate();
		//particle_camera->Clear();
		//particle->Render();

		coinNumUI->RenderToBackBuffer();

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

	//	���[�h�ύX�t���O�擾
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

	//	�C�x���g���A���[�h�ݒ�
	void	UI::SetEventInfoMode( int mode )
	{
		this->eventInfo.mode = mode;
	}

	//	�_���[�W�t���O�ݒ�@FallLife�N���X�p
	void	UI::SetDamageFlag( int playerNum, bool flag, int culLife )
	{
		lifeUI->Update();
		fallLife->SetPos( playerNum, lifeUI->GetPosX( playerNum ), lifeUI->GetPosY( playerNum ) );
		fallLife->SetDamageFlag( playerNum, flag, culLife );
	}