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

	//	初期化(現在のシーン)
	bool	UI::Initialize( void )
	{
		//	画像セット
		timer.obj = new iex2DObj("DATA/UI/timer.png");
		finishImage.obj = new iex2DObj("DATA/UI/bfUI.png");
		countImage.obj = new iex2DObj("DATA/UI/bfUI_02.png");
		alertImage.obj = new iex2DObj("DATA/UI/alert.png");
		alert_coinImage.obj = new iex2DObj("DATA/UI/coin_alert.png");
		playerNumber = new iex2DObj("DATA/UI/number.png");
		roundImage.obj = new iex2DObj("DATA/UI/roundText.png");
		startNumber = new iex2DObj("DATA/UI/DonketuUI.png");
		redAlert.obj = new iex2DObj("DATA/UI/last.png");
		
		//	共通変数初期化 
		changeflag = false;
		FOR(0, PLAYER_MAX)
		{
			charatype[value] = gameManager->GetCharacterType(value);
			startNum[value].obj = startNumber;
		}

		//	ライフUI初期化
		lifeUI = new LifeUI();
		lifeUI->Initialize();

		//	ライフ落下演出
		fallLife = new FallLife();
		fallLife->Initialize();

		//	王冠初期化
		crown = new Crown();
		crown->Initialize();

		//	プレイヤー番号UI初期化
		playerNumUI = new PlayerNumUI();
		playerNumUI->Initialize();

		//	コイン枚数UI初期化
		coinNumUI = new CoinNumUI();
		coinNumUI->Initialize();

		//	顔（背景）初期化
		faceUI = new FaceUI();
		faceUI->Initialize();

		//	各UI情報初期化
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

	//	解放
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
//	更新・描画
//------------------------------------------------------------------------------

	//	更新(現在のシーン)
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

	//	描画(現在のシーン)
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
//	メイン動作初期化
//------------------------------------------------------------------------------

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

	//	カウントダウン中プレイヤーの番号表示
	void	UI::StartPlayerNumInitialize( void )
	{
		//	変数初期化
		Vector3 imagePos	= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	p_pos		= Vector3(0.0f, 0.0f, 0.0f);
		Vector3	out			= Vector3(0.0f, 0.0f, 0.0f);
		int sx, sy;
		int w				= static_cast<int>( iexSystem::ScreenWidth * 0.15f );
		int h				= static_cast<int>( iexSystem::ScreenHeight * 0.15f );

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
		//	画面赤用初期化					画面中心のX座標				画面中心のY座標			
		ImageInitialize( redAlert, iexSystem::ScreenWidth / 2, iexSystem::ScreenHeight / 2, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 512, 512 );
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
		//roundImage.angle = D3DXToRadian( -30.0f );
	}

	//　イベント情報初期化
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

			changeflag			= true;
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
			alertInfo.alpha = 0.1f + 0.1f * sinf( alertInfo.param );

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

	//	時間警告演出
	void	UI::LastProduction( void )
	{
		//	30秒～4秒
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
		//
		RenderImage(redAlert, redAlert.sx, redAlert.sy, redAlert.sw, redAlert.sh, IMAGE_MODE::FLASH);

		//	タイマー文字色を白へ
		timer.sy = 0;

	}

	//	ラウンド描画
	void	UI::RoundRender( void )
	{
		RenderImage( roundImage, roundImage.sx, roundImage.sy, roundImage.sw, roundImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	イベント関連情報描画（飛行機表示）
	void	UI::EventRender(void)
	{
		if ( eventInfo.texture.x <= static_cast<int>( iexSystem::ScreenWidth ) ||
			eventInfo.texture.x + eventInfo.texture.sx >= 0 )
			//eventInfo.airPlane->Render();
			RenderImage( eventInfo.texture, eventInfo.texture.sx, eventInfo.texture.sy, eventInfo.texture.sw, eventInfo.texture.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	レンダーターゲットでパーティクル
	void	UI::RenderTargetParticle( void )
	{
		////	バックバッファにパーティクル
		//target_par->RenderTarget( 0 );

		//particle_camera->Activate();
		//particle_camera->Clear();
		//particle->Render();

		coinNumUI->RenderToBackBuffer();

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

	//	モード変更フラグ取得
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

	//	イベント情報、モード設定
	void	UI::SetEventInfoMode( int mode )
	{
		this->eventInfo.mode = mode;
	}

	//	ダメージフラグ設定　FallLifeクラス用
	void	UI::SetDamageFlag( int playerNum, bool flag, int culLife )
	{
		lifeUI->Update();
		fallLife->SetPos( playerNum, lifeUI->GetPosX( playerNum ), lifeUI->GetPosY( playerNum ) );
		fallLife->SetDamageFlag( playerNum, flag, culLife );
	}