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
//	UIクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------

	namespace
	{
		namespace SCREEN_SIZE
		{
			const int WIDTH		= iexSystem::ScreenWidth;
			const int HEIGHT	= iexSystem::ScreenHeight;
		}

		//    顔情報
		namespace FACE_INFO
		{
			//　「楽・喜・哀・怒」
			enum
			{
				Normal,
				Good,
				Sad,
				Angry
			};
		}

		//	タイトルテキスト読み込み座標情報
		namespace TITLE_TEXT_SRCPOS_INFO
		{
			enum
			{
				MENU_SRC_POS_X,
				MENU_SRC_POS_Y,
			};
		}	
	}

	//	実体
	UI*	ui = nullptr;

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	UI::UI( void )
	{

	}

	//	デストラクタ
	UI::~UI( void )
	{
		Release();
	}

	//	初期化
	bool	UI::Initialize( void )
	{
		MainInitialize();
		return	true;
	}

	//	解放
	void	UI::Release( void )
	{
		MainRelease();
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	UI::Update( const int& mode )
	{
		MainUpdate( mode );
	}

	//	描画
	void	UI::Render( const int& mode )
	{
		MainRender( mode );
	}
	
//------------------------------------------------------------------------------
//	各シーン初期化
//------------------------------------------------------------------------------

	//	メイン用初期化
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

		//	パーティクル用バッファ
		PAR_POS			= Vector3(100.0f, 100.0f, 100.0f);
		target_par		= std::make_unique<iex2DObj>(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET);
		particle_camera = std::make_unique<Camera>();
		particle_camera->SetPos(Vector3(0.0f, 10.0f, -10.0f) + PAR_POS);
		
		//	共通変数初期化 
		changeflag = false;
		FOR( 0, PLAYER_MAX )
		{
			faceImage[value].obj	= face;
			charatype[value]		= gameManager->GetCharacterType(value);
			startNum[value].obj		= startNumber;
			coin_flg[value]			= false;
			coin_timer[value]		= 0;
		}

		//	各UI情報初期化
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
//	各シーン解放
//------------------------------------------------------------------------------

	//	メイン用解放
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
//	各シーン更新
//------------------------------------------------------------------------------

	//	メイン更新
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
//	各シーン描画
//------------------------------------------------------------------------------

	//	メイン描画
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
//	メイン動作初期化
//------------------------------------------------------------------------------

	//	コイン枚数初期化
	void	UI::CoinNumberInitialize( void )
	{
		//	コイン枚数位置（横）
		coinNumInfo[0].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.1 );
		coinNumInfo[1].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.3 );
		coinNumInfo[2].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.7 );
		coinNumInfo[3].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.9 );

		//	表示色
		coinColor[0] = characterManager->GetDamageColor(0);
		coinColor[1] = characterManager->GetDamageColor(1) + Vector3( 0.7f, 0.7f, 0.0f );		//	見やすくするため
		coinColor[2] = characterManager->GetDamageColor(2);
		coinColor[3] = characterManager->GetDamageColor(3);


		FOR( 0, PLAYER_MAX ){
			//	コイン枚数用
			coinNum[value]						= gameManager->GetCoinNum( value );

			//	コイン枚数位置（縦）
			coinNumInfo[value].pos.y			= static_cast<int>( iexSystem::ScreenHeight * 0.1f );
			coinNumInfo[value].scale			= 100;
			
			//	画像設定
			coinNumInfo[value].one.obj			= pCoinNumImage;
			coinNumInfo[value].ten.obj			= pCoinNumImage;
			coinNumInfo[value].hundred.obj		= pCoinNumImage;
			
			//	情報設定
			SetCoinImageInfo( coinNumInfo[value], numInfo[value], gameManager->GetCoinNum( value ) );
			
			//	色設定	
			coinNumInfo[value].one.color		= coinColor[value];
			coinNumInfo[value].ten.color		= coinColor[value];
			coinNumInfo[value].hundred.color	= coinColor[value];

		
		}
	}

	//	顔画像初期化
	void	UI::FaceImageInitialize( void )
	{
		FOR( 0, PLAYER_MAX ){
			ImageInitialize( faceImage[value], coinNumInfo[value].pos.x, coinNumInfo[value].pos.y,
							 coinNumInfo[value].one.w + coinNumInfo[value].ten.w,
							 coinNumInfo[value].one.h + coinNumInfo[value].ten.h,
							 0, 0, 256, 256 );
		}
	}

	//	タイマー初期化
	void	UI::TimerInitialize( void )
	{

		//	構造体初期化
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

	//	カウントダウン・スタート・終了演出
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

	//	カウントダウン中プレイヤーの番号表示
	void	UI::StartPlayerNumInitialize( void )
	{
		//	変数初期化
		Vector3 imagePos	= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	p_pos		= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	out			= Vector3(0.0f, 0.0f, 0.0f);
		int sx, sy;
		int w				= static_cast<int>(iexSystem::ScreenWidth * 0.15f);
		int h				= static_cast<int>(iexSystem::ScreenHeight * 0.15f);

		FOR( 0, PLAYER_MAX )
		{
			//	構造体に情報設定
			sx = 0 + 128 * value;
			sy = 256;
 
			ImageInitialize( startNum[value], 0, 0, w, h, sx, sy, 128, 128 );
		}
	}

	//	警告演出初期化
	void	UI::AlertInitialize( void )
	{
		alertInfo.flag	= false;
		alertInfo.alpha = 0.0f;
		alertInfo.timer = 0;
		alertInfo.param = 0.0f;
		alertInfo.type	= 0;

		//	画像構造体初期化
		int x			= static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y			= static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w			= static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int h			= w;
		ImageInitialize( alertImage, x, y, w, h, 0, 0, 256, 256 );
		ImageInitialize( alert_coinImage, x, y, w, h, 0, 0, 256, 256 );
		alertImage.renderflag		= true;
		alert_coinImage.renderflag	= true;
	}

	//	HurryUp演出初期化
	void	UI::HurryUpInitialize( void )
	{
		hurryInfo.alpha = 0.0f;
		hurryInfo.flag	= false;
		hurryInfo.param = 0.0f;
		hurryInfo.timer = 0;
	}

	//	プレイヤー番号画像初期化
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
	
	//	時間警告初期化
	void	UI::LastProductionInitialize( void )
	{
		SetScaling( timer, 1.0f );
		lasttimerInfo.state = false;
		lasttimerInfo.t		= 0.0f;
		lasttimerInfo.alpha = 1.0f;
	}

	//	体力画像初期化
	void	UI::LifeInitialize( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			lifeInfo[value].life			= gameManager->GetStartLife( value );
			lifeInfo[value].lifeImage.obj	= life;
			ImageInitialize( lifeInfo[value].lifeImage, 0, 10, 75, 75, 0, 0, 64, 64 );
		}
	}

	//	1位王冠画像初期化
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

	//	ラウンド初期化
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

	//　イベント情報初期化
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
//	メイン動作更新
//------------------------------------------------------------------------------

	//	タイマー関連動作
	void	UI::TimerUpdate( void )
	{
		this->timerInfo.time	= gameManager->GetTimer();
		//分
		timerInfo.minute		= this->timerInfo.time / MINUTE % 10;
		//秒二桁目
		timerInfo.second[0]		= ( this->timerInfo.time / SECOND ) % 60 / 10 % 10;
		//秒一桁目
		timerInfo.second[1]		= this->timerInfo.time / SECOND % 10;
	}

	//	コイン枚数動作
	void	UI::CoinNumberUpdate( void )
	{
		FOR( 0, PLAYER_MAX ){
			CoinCounter( gameManager->GetCoinNum(value),value );
			CoinImageInfoUpdate( coinNumInfo[value], numInfo[value], coinNum[value] );
			FaceImageUpdate( value, gameManager->GetCharacterType( value ) );
			
		}
	}

	//	顔画像構造体更新
	void	UI::FaceImageUpdate( int num, int mode )
	{
		faceImage[num].alpha	= 0.5f;
		faceImage[num].sy		= 256 * mode;
	}

	//	カウントダウン・スタート演出
	void	UI::StartUpdate( void )
	{
		int round = gameManager->GetRound();
		static int	waittime;
		switch ( countInfo.start_step )
		{
		//	画面上部から移動
		case 0:
			if ( StartMove() )
			{
				SetScaling( countImage, 4.0f );
				countInfo.start_step++;
				break;
			}
			break;
		//	着地しながらフェードアウト
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
		//	GOがフェードイン
		case 2:
			sound->PlaySE( SE::GAMESTART_SE );
			if ( !countImage.scalingFlag )
			{
				waittime = 90;		//	文字停止フレーム数
				countInfo.start_step++;
				break;
			}
			ScalingAlphaUpdate( countImage, 300 );
			break;
		//	文字停止フレーム数後、Goがフェードアウト
		case 3:
			//	タイマーの間はbreak;
			waittime--;
			if ( waittime >= 0 )	break;

			changeflag	= true;
			countInfo.waitTimer = 2 * SECOND;
			break;
		}

		//	FIGHT描画のためスキップ
		if ( countInfo.start_step > 1 ) return;
		//	読み込み位置・サイズ設定
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

	//	ラウンド文字移動
	bool	UI::StartMove( void )
	{
		Vector3 pos;

		//	パラメータ加算
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

	//	カウントダウン中プレイヤーの番号表示
	void	UI::StartPlayerNumUpdate( void )
	{
		//	変数初期化
		Vector3 imagePos	= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos		= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out			= Vector3( 0.0f, 0.0f, 0.0f );
		FOR( 0, PLAYER_MAX )
		{
			//	画像を表示する場所を設定
			p_pos				= characterManager->GetPos(value);
			imagePos			= p_pos;

			//	クライアント座標に変換
			WorldToClient( imagePos, out, matView * matProjection );

			//	構造体に情報設定
			startNum[value].x	= static_cast<int>( out.x );
			startNum[value].y	= static_cast<int>( out.y );
		}
	}

	//	タイムアップ演出
	void	UI::FinishUpdate( void )
	{
		countInfo.waitTimer--;

		if ( countInfo.waitTimer <= 0 )	changeflag = true;
	}

	//	警告演出
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

			//	二秒半で終了
			if ( alertInfo.timer >= 120 )
			{
				alertInfo.flag  = false;
				alertInfo.alpha = 0.0f;
				alertInfo.timer = 0;
			}
		}

		//　飛行機やってくる
		if ( eventInfo.step == 0 )
		{
			//	どっかからやってくる
			eventInfo.airPlane->SetNext( eventInfo.airPlane->IN_START_POS, eventInfo.airPlane->STAY_POS, AirPlane::FLYING_IN );
			eventInfo.step++;
		}

		eventInfo.airPlane->Update();
		eventInfo.texture.x		= ( int )eventInfo.airPlane->GetPos().x;
		eventInfo.texture.y		= ( int )eventInfo.airPlane->GetPos().y;
		eventInfo.texture.sy	= eventInfo.mode * 128;
	}

	//	HurryUp演出
	void	UI::HurryUpdate( void )
	{
		hurryInfo.param += D3DX_PI / 30.0f;
		hurryInfo.alpha = 0.1f + 0.1f * sinf( hurryInfo.param );

		hurryInfo.timer++;
		if ( hurryInfo.timer % 15 == 0 )	alertImage.renderflag = !alertImage.renderflag;
	}

	//	時間警告演出
	void	UI::LastProduction( void )
	{
		//	30秒～4秒
		if ( !lasttimerInfo.state ){
			ScalingUpdate(timer, 100);
			if ( gameManager->GetTimer() / SECOND <= 3 )
			{
				SetScaling( timer, 1.0f );
				lasttimerInfo.state = true;
			}
			return;
		}
	
		
		//	3秒～0秒
		if ( !timer.scalingFlag && gameManager->GetTimer() % 60 == 0 )	SetScaling( timer, 1.0f );


		ScalingAlphaUpdate( timer, 1000 );



	}

	//	プレイヤー番号位置決定
	void	UI::PlayerNumberUpdate( void )
	{
		//	変数初期化
		Vector3 imagePos	= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos		= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	up			= Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out			= Vector3( 0.0f, 0.0f, 0.0f );

		FOR( 0,PLAYER_MAX )
		{
			//	画像を表示する場所を設定
			up				= characterManager->GetUp( value );
			p_pos			= characterManager->GetPos( value );
			imagePos		= p_pos + up * 7.0f;

			//	クライアント座標に変換
			WorldToClient( imagePos, out, matView * matProjection );

			//	構造体に情報設定
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

	//	ライフ更新
	void	UI::LifeUpdate( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	lifePos;
		Vector3	out;
		int			culLife;

		FOR( 0, PLAYER_MAX )
		{
			//	表示座標算出
			p_Pos		= characterManager->GetPos( value );
			p_Up		= characterManager->GetUp( value );
			lifePos		= p_Pos + p_Up * 5.0f;
			WorldToClient( lifePos, out, matView * matProjection );

			//	現在の体力取得
			culLife		= characterManager->GetLife( value );

			//	描画位置設定
			lifeInfo[value].lifeImage.x		= ( int )out.x;
			lifeInfo[value].lifeImage.y		= ( int )out.y;

			//	読み込み位置設定
			lifeInfo[value].lifeImage.sx	= lifeInfo[value].lifeImage.sw * ( ( 5 - culLife ) % 4 );
			lifeInfo[value].lifeImage.sy	= lifeInfo[value].lifeImage.sh * ( ( 5 - culLife ) / 4 );
		}
	}

	//	王冠更新
	void	UI::CrownUpdate( void )
	{

		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	CrownPos;
		Vector3	out;
		int		lifeSize;

		FOR( 0, PLAYER_MAX )
		{
			//	1位以外は飛ばす
			if ( characterManager->GetRank(value) > 1 )
			{
				crownInfo[value].state = false;
				continue;
			}

			//	表示状態へ
			crownInfo[value].state = true;

			//	表示座標算出
			p_Pos		= characterManager->GetPos( value );
			p_Up		= characterManager->GetUp( value );
			CrownPos	= p_Pos + p_Up * 5.0f;
			lifeSize	= lifeInfo[value].lifeImage.sh;
			WorldToClient( CrownPos, out, matView * matProjection );

			//	描画位置設定
			crownInfo[value].crownImage.x = ( int )out.x;
			crownInfo[value].crownImage.y = ( int )out.y - lifeSize;

		}
	}

	//　イベントのUI（飛行機挙動）
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
//	メイン描画
//------------------------------------------------------------------------------

	//	コイン枚数描画
	void	UI::CoinNumberRender( void )
	{
		FOR( 0, PLAYER_MAX ){
			RenderImage( faceImage[value], faceImage[value].sx, faceImage[value].sy, faceImage[value].sw, faceImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
		//	15秒まで数字表示
		if ( gameManager->GetTimer() / SECOND >= 15 )
		{
			FOR(0, PLAYER_MAX)
			{
				//	１００の位描画
				int		sx = coinNumInfo[value].hundred.sx;
				int		sy = coinNumInfo[value].hundred.sy;
				int		sw = coinNumInfo[value].hundred.sw;
				int		sh = coinNumInfo[value].hundred.sh;

				if ( coinNumInfo[value].hundredRenderFlag )
					RenderImage( coinNumInfo[value].hundred, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	１０の位描画
				sx = coinNumInfo[value].ten.sx;
				sy = coinNumInfo[value].ten.sy;
				sw = coinNumInfo[value].ten.sw;
				sh = coinNumInfo[value].ten.sh;
				RenderImage( coinNumInfo[value].ten, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	１の位描画
				sx = coinNumInfo[value].one.sx;
				sy = coinNumInfo[value].one.sy;
				sw = coinNumInfo[value].one.sw;
				sh = coinNumInfo[value].one.sh;
				RenderImage( coinNumInfo[value].one, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				ParticleRender( value );
			}
		}
		//	ラスト15秒は自分の持ち枚数が分からなくなる
		else
		{
			FOR( 0, PLAYER_MAX )
			{

				//	１０の位描画
				int		sx = 12 * 64;		//	？
				int		sy = coinNumInfo[value].ten.sy;
				int		sw = coinNumInfo[value].ten.sw;
				int		sh = coinNumInfo[value].ten.sh;

				RenderImage( coinNumInfo[value].ten, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

				//	１の位描画
				sy = coinNumInfo[value].one.sy;
				sw = coinNumInfo[value].one.sw;
				sh = coinNumInfo[value].one.sh;
				RenderImage( coinNumInfo[value].one, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
			
				ParticleRender( value );
			}
		}


	}

	//	パーティクル描画(コイン枚数)
	void	UI::ParticleRender( int value )
	{
		if ( !coin_flg[value] ) return;

		target_par->Render( (int)( coinNumInfo[value].pos.x - coinNumInfo[value].scale * 1.5f ), (int)( coinNumInfo[value].pos.y - coinNumInfo[value].scale ),
			coinNumInfo[value].scale * 3, coinNumInfo[value].scale * 2,
			0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, shader2D, "add" );
	
	}

	//	タイマー描画
	void	UI::TimerRender( void )
	{
		RenderImage( timer, timer.sx * timerInfo.minute		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 0, timer.y );
		RenderImage( timer, timer.sx * 10					, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 1, timer.y );
		RenderImage( timer, timer.sx * timerInfo.second[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 2, timer.y );
		RenderImage( timer, timer.sx * timerInfo.second[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 3, timer.y );

	}

	//	カウントダウン・スタート演出
	void	UI::StartRender( void )
	{
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::SCALING );
	}

	//	カウントダウン中プレイヤーの番号表示
	void	UI::StartPlayerNumRender( void )
	{
		FOR( 0,PLAYER_MAX )
		{
			RenderImage( startNum[value], startNum[value].sx, startNum[value].sy, startNum[value].sw, startNum[value].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	タイムアップ演出
	void	UI::FinishRender( void )
	{
		RenderImage( finishImage, finishImage.sx, finishImage.sy, finishImage.sw, finishImage.sh, IMAGE_MODE::NORMAL );
	}

	//	警告描画
	void	UI::AlertRender( void )
	{
		int color;
		switch ( alertInfo.type )
		{
		case ALERT_TYPE_INFO::JAM:
			//　フィルタ色設定、フィルタ描画、警告画像描画
			color = GetColor( 1.0f, 0.0f, 0.0f, alertInfo.alpha );	//　赤
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );
			RenderImage( alertImage, alert_coinImage.sx, alert_coinImage.sy, alertImage.sw, alertImage.sh, IMAGE_MODE::NORMAL );
			break;

		case ALERT_TYPE_INFO::COIN:
			color = GetColor( 1.0f, 1.0f, 0.0f, alertInfo.alpha );	//　黄
			iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );
			RenderImage( alert_coinImage, alert_coinImage.sx, alert_coinImage.sy, alert_coinImage.sw, alert_coinImage.sh, IMAGE_MODE::NORMAL );
			break;

		default:
			break;
		}

		//　飛行機
		EventRender();
	}

	//	時間警告描画
	void	UI::LastProductionRender( void )
	{
		//	タイマー文字色を赤へ
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
		//	タイマー文字色を白へ
		timer.sy = 0;

	}

	//	ライフ描画
	void	UI::LifeRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( lifeInfo[value].lifeImage, lifeInfo[value].lifeImage.sx, lifeInfo[value].lifeImage.sy, lifeInfo[value].lifeImage.sw, lifeInfo[value].lifeImage.sh, IMAGE_MODE::NORMAL );
		}
	}

	//	王冠描画
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

	//	ラウンド描画
	void	UI::RoundRender( void )
	{
		RenderImage( roundImage, roundImage.sx, roundImage.sy, roundImage.sw, roundImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	イベント関連情報描画（飛行機表示）
	void	UI::EventRender( void )
	{
		if ( eventInfo.texture.x <= static_cast<int>( iexSystem::ScreenWidth ) ||
			eventInfo.texture.x + eventInfo.texture.sx >= 0 )
			RenderImage( eventInfo.texture, eventInfo.texture.sx, eventInfo.texture.sy, eventInfo.texture.sw, eventInfo.texture.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	レンダーターゲットでパーティクル
	void	UI::RenderTargetParticle( void )
	{
		//	バックバッファにパーティクル
		target_par->RenderTarget( 0 );

		particle_camera->Activate();
		particle_camera->Clear();
		particle->Render();

	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	コイン枚数を1枚ずつカウントアップダウン
	void	UI::CoinCounter( int coin, int num )
	{
		if (coinNum[num] == coin)	return;

		//	コイン減少
		if( coinNum[num] > coin )
		{
			coinNum[num]--;
		}

		//	コイン増加
		if ( coinNum[num] < coin )
		{
			coinNum[num]++;
			coin_flg[num] = true;
			particle->CoinGet( PAR_POS , 0.5f );
		}
	
	}

	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更
	void	UI::CoinImageInfoUpdate( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	桁数確認
		if ( num >= 100 )				numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( numInfo, num );

		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	１０の位設定
			numImageInfo.ten.x		= numImageInfo.pos.x;
			numImageInfo.ten.sx		= numInfo.ten * 64;

			//	１００の位設定
			numImageInfo.hundred.sx = numInfo.hundred * 64;

			//	１の位設定
			numImageInfo.one.x		= numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			numImageInfo.one.sx		= numInfo.one * 64;
		}
		else
		{
			//	１０の位設定
			numImageInfo.ten.x		= numImageInfo.pos.x - numImageInfo.scale / 3;
			numImageInfo.ten.sx		= numInfo.ten * 64;

			//	１の位設定
			numImageInfo.one.x		= numImageInfo.pos.x + numImageInfo.scale / 3;
			numImageInfo.one.sx		= numInfo.one * 64;
		}
	}

	//	パーティクル更新
	void	UI::ParticleUpdate( void )
	{
		particle_camera->Update( VIEW_MODE::FIX, PAR_POS );
		particle->Update();

		FOR( 0, PLAYER_MAX )
		{
			//	パーティクルを30フレーム間だけ描画許可する
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
//	情報設定・取得
//------------------------------------------------------------------------------
	
	//	モード変更フラグ設定
	void	UI::SetChangeFlag( const bool& flag )
	{
		this->changeflag = flag;
	}

	//	警告フラグ設定
	void	UI::SetAlertInfo( bool flag , int type )
	{
		alertInfo.flag = flag;
		alertInfo.type = type;
	}

	//	HurryUpフラグ設定
	void	UI::SetHurryFlag( bool flag )
	{
		hurryInfo.flag = flag;
	}

	//	モード変更フラグ取得
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更
	void	UI::SetCoinImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	桁数確認
		if ( num >= 100 )				numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( numInfo, num );

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if (numImageInfo.hundredRenderFlag)
		{
			//	１０の位設定
			x	= numImageInfo.pos.x;
			y	= numImageInfo.pos.y;
			w	= h = numImageInfo.scale;
			sx	= numInfo.ten * 64;
			sy	= 0;
			sw	= sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定
			x	= numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x	= numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	１０の位設定
			w	= h = numImageInfo.scale;
			x	= numImageInfo.pos.x - w / 3;
			y	= numImageInfo.pos.y;
			sx	= numInfo.ten * 64;
			sy	= 0;
			sw	= sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x	= numImageInfo.pos.x + w / 3;
			sx	= numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定(初期化の時のみセットするため）
			x	= numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx	= numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );
		}
	}

	//	数値構造体に値をセットする
	void	UI::SetNumberInfo( NumberInfo& number, int coin )
	{
		number.hundred	= coin / 100 % 10;
		number.ten		= coin / 10 % 10;
		number.one		= coin % 10;
	}

	//	イベント情報、モード設定
	void	UI::SetEventInfoMode( int mode )
	{
		this->eventInfo.mode = mode;
	}