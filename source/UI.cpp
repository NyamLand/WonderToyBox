
#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"
#include	"GlobalFunction.h"
#include	"CharacterManager.h"
#include	"GameManager.h"
#include	"sceneTitle.h"
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
			const int WIDTH = iexSystem::ScreenWidth;
			const int HEIGHT = iexSystem::ScreenHeight;
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

		//	タイトルテキスト座標情報


		//	タイトルテキスト読み込み座標情報
		namespace TITLE_TEXT_SRCPOS_INFO
		{
			enum
			{
				MENU_SRC_POS_X,
				MENU_SRC_POS_Y,
			};
		}

		

		

		//　どんけつ演出用
		namespace DD_TIMING
		{
			const int WAIT_MAX		= 7 * SECOND + 30;
			const int DB_LOCK		= WAIT_MAX - 30;
			const int FACE_START	= DB_LOCK - 20;
			const int FACE_LOCK		= FACE_START - 2 * SECOND - 10;
			const int P_START		= FACE_LOCK - 40;
			const int P_LOCK		= P_START - 1 * SECOND - 30;
			const int FIGHT_START	= P_LOCK - 20;
			const int FIGHT_LOCK	= FIGHT_START - 1 * SECOND - 30;
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
	bool	UI::Initialize( int scene )
	{
		this->scene = scene;

		//	モード別初期化
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleInitialize();
			break;

		case UI_MODE::MAIN:
			MainInitialize();
			break;

		case UI_MODE::RESULT:
			ResultInitialize();
			break;
		}

		return	true;
	}

	//	解放
	void	UI::Release( void )
	{
		switch ( scene )
		{
		case UI_MODE::TITLE:
			TitleRelease();
			break;

		case UI_MODE::MAIN:
			MainRelease();
			break;

		case	UI_MODE::RESULT:
			ResultRelease();
			break;
		}
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	UI::Update( const int& mode )
	{
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleUpdate( mode );
			break;

		case UI_MODE::MAIN:
			MainUpdate( mode );
			break;

		case UI_MODE::RESULT:
			ResultUpdate( mode );
			break;
		}
	}

	//	描画
	void	UI::Render( const int& mode )
	{
		switch ( this->scene )
		{
		case UI_MODE::TITLE:
			TitleRender( mode );
			break;

		case UI_MODE::MAIN:
			MainRender( mode );
			break;

		case UI_MODE::RESULT:
			ResultRender( mode );
			break;
		}
		DrawDebug();
	}

	//　デバッグ表示
	void	UI::DrawDebug()
	{
		if (debug)
		{
			
		}
	}

//------------------------------------------------------------------------------
//	各シーン初期化
//------------------------------------------------------------------------------

	//	タイトル用初期化
	void	UI::TitleInitialize( void )
	{
		//	画像初期化
		titleInfo.textImage.obj = new iex2DObj( "DATA/UI/menu/menu-int.png" );

		//	構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 1.01f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.6f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.14f );
		ImageInitialize( titleInfo.textImage, x, y, w, h, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_X, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_Y, 1024, 128 );
		titleInfo.textImage.renderflag = false;
		
		//	変数初期化
		titleInfo.mode = 0;
		titleInfo.step = 0;
		titleInfo.param = 0.0f;
		titleInfo.t = 0.0f;
		titleInfo.moveState = false;
		titleInfo.savePos = 0;

		AirPlaneInitialize();
	}

	//	メイン用初期化
	void	UI::MainInitialize( void )
	{
		timer.obj = new iex2DObj("DATA/UI/timer.png");
		coinbar = new iex2DObj("DATA/BG/coin_gage.png");
		gauge.obj = coinbar;
		backgauge.obj = coinbar;
		frame.obj = coinbar;
		face = new iex2DObj("DATA/UI/chara_emotion.png");
		faceImage.obj = face;
		countImage.obj = new iex2DObj( "DATA/UI/bfUI.png" );
		alertImage.obj = new iex2DObj( "DATA/UI/alert.png" );
		alert_coinImage.obj = new iex2DObj( "DATA/UI/coin_alert.png" );
		playerNumber = new iex2DObj( "DATA/UI/number.png" );
		life = new iex2DObj( "DATA/UI/Life.png" );
		pCoinNumImage = new iex2DObj("DATA/UI/number.png");
		roundImage.obj = new iex2DObj( "DATA/UI/roundText.png" );

		//	共通変数初期化 
		changeflag = false;
		for (int i = 0; i < PLAYER_MAX; i++)
			charatype[i] = gameManager->GetCharacterType(i);

		//	各UI情報初期化
		//CoinBarInitialize();
		CoinNumberInitialize();
		TimerInitialize();
		StartAndTimeUpInitialize();
		NewsBarInitialize();
		DonketsuDirectionInitialize();
		AlertInitialize();
		LastProductionInitialize();
		PlayerNumberInitialize();
		LifeInitialize();
		RoundInitialize();
	}

	//	リザルト用初期化
	void	UI::ResultInitialize( void )
	{

	}

//------------------------------------------------------------------------------
//	各シーン解放
//------------------------------------------------------------------------------

	//	タイトル用解放
	void	UI::TitleRelease( void )
	{
		SafeDelete( titleInfo.textImage.obj );
	}

	//	メイン用解放
	void	UI::MainRelease( void )
	{
		SafeDelete( timer.obj );
		SafeDelete( coinbar );
		SafeDelete( ddInfo.face.obj );
		SafeDelete( ddInfo.DB.obj );
		SafeDelete( ddInfo.P.obj );
		SafeDelete( ddInfo.fight.obj );
		SafeDelete( face );
		SafeDelete( countImage.obj );
		SafeDelete( alertImage.obj );
		SafeDelete( alert_coinImage.obj );
		SafeDelete( playerNumber );
		SafeDelete( life );
		SafeDelete( pCoinNumImage );
		SafeDelete( roundImage.obj );
	}

	//	リザルト用解放
	void	UI::ResultRelease( void )
	{

	}

//------------------------------------------------------------------------------
//	各シーン更新
//------------------------------------------------------------------------------

	//	タイトル更新
	void	UI::TitleUpdate( int mode )
	{
		//	画像設定
		SetImageSrcPos( mode );

		bool	isEnd = false;
		switch ( titleInfo.step ) 
		{
		case 0:
			//	どっかからやってくる
			isEnd = FlyingIn();
			
			//	処理が終わるとつぎへ
			if ( isEnd )
			{
				titleInfo.step++;
			}
			break;

		case 1:
			//	ふわふわさせる
			titleInfo.textImage.y = airPlaneInfo.IN_END_POS_Y + static_cast<int>( 10.0f * sinf( titleInfo.t ) );
			titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y  );
			titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * -1.0f ) * cosf( titleInfo.t );
			titleInfo.t += D3DX_PI / 180.0f * 1.0f;

			if ( mode == TITLE_MODE::MOVE_MAIN )
			{
				titleInfo.textImage.t = 0.0f;
				titleInfo.param = 0.0f;
				titleInfo.savePos = titleInfo.textImage.y;
				titleInfo.step++;
			}
			break;

		case 2:
			FlyingOut( titleInfo.savePos );
			break;
		}
	}
	
	//	メイン更新
	void	UI::MainUpdate( int mode )
	{
		PlayerNumberUpdate();
		LifeUpdate();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			NewsBarUpdate();
			//CoinBarUpdate();
			CoinNumberUpdate();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuDirectionUpdate();
			break;

		case GAME_MODE::CLIMAX:
			TimerUpdate();
			NewsBarUpdate();
			//CoinBarUpdate();
			CoinNumberUpdate();
			LastProduction();

			//　どんけつの顔は「怒」に。（時間管理してるとこで↓の処理書きたいけどこれから変更ありそうやからとりあえずここに書いてる許してニャンっ♪）
			state_type[gameManager->GetWorst()] = FACE_INFO::Angry;
			break;

		case GAME_MODE::TIMEUP:
			FinishUpdate();
			break;
		}

		//	警告動作
		if (alertInfo.flag)	AlertUpdate();
	}

	//	リザルト更新
	void	UI::ResultUpdate( int mode )
	{

	}

//------------------------------------------------------------------------------
//	各シーン描画
//------------------------------------------------------------------------------

	//	タイトル描画
	void	UI::TitleRender( int mode )
	{
		RenderImage( titleInfo.textImage, titleInfo.textImage.sx, titleInfo.textImage.sy, 1024, 128, IMAGE_MODE::ADOPTPARAM );
	}

	//	メイン描画
	void	UI::MainRender( int mode )
	{
		PlayerNumberRender();
		LifeRender();
		RoundRender();

		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			NewsBarRender();
			//CoinBarRender();
			CoinNumberRender();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			TimerRender();
			NewsBarRender();
			//CoinBarRender();
			CoinNumberRender();
			DonketsuDirectionRender();
			break;

		case GAME_MODE::CLIMAX:
			//TimerRender();
			LastProductionRender();
			NewsBarRender();
			//CoinBarRender();
			CoinNumberRender();

			break;

		case GAME_MODE::TIMEUP:
			FinishRender();
			break;
		}

		if (alertInfo.flag)	AlertRender();
	}

	//	リザルト描画
	void	UI::ResultRender( int mode )
	{

	}

//------------------------------------------------------------------------------
//	タイトル動作初期化
//------------------------------------------------------------------------------

	//	飛行機初期化
	void	UI::AirPlaneInitialize( void )
	{
		airPlaneInfo.IN_START_POS_X = static_cast<int>(iexSystem::ScreenWidth * 1.5f);
		airPlaneInfo.IN_START_POS_Y = static_cast<int>(iexSystem::ScreenHeight * -0.13f);
		airPlaneInfo.IN_END_POS_X = static_cast<int>(iexSystem::ScreenWidth * 0.5f);
		airPlaneInfo.IN_END_POS_Y = static_cast<int>(iexSystem::ScreenHeight * 0.2f);
		airPlaneInfo.OUT_START_POS_X = static_cast<int>(iexSystem::ScreenWidth * 0.5f);
		airPlaneInfo.OUT_END_POS_X = static_cast<int>(iexSystem::ScreenWidth * -0.55f);
		airPlaneInfo.OUT_END_POS_Y = static_cast<int>( iexSystem::ScreenHeight * -0.13f );
		airPlaneInfo.ROLL_POINT_ADJUST_X = 200;
	}

	//	飛んでくる
	bool	UI::FlyingIn( void )
	{
		if ( titleInfo.textImage.t >= 1.0f )	return	true;

		//	パラメータ加算
		titleInfo.textImage.t += 0.01f;
		if ( titleInfo.textImage.t >= 1.0f )	titleInfo.textImage.t = 1.0f;

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, titleInfo.textImage.t );

		//	上移動
		Lerp( titleInfo.textImage.y, airPlaneInfo.IN_START_POS_Y, airPlaneInfo.IN_END_POS_Y, titleInfo.param );

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.textImage.t );

		//	左移動
		Lerp(titleInfo.textImage.x, airPlaneInfo.IN_START_POS_X, airPlaneInfo.IN_END_POS_X, titleInfo.param);

		//	回転
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
		titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * -1.0f ) * sinf( D3DX_PI  / 2 * titleInfo.textImage.t );

		if ( titleInfo.textImage.t >= 1.0f )	return	true;
		return	false;
	}

	//	飛んでいく
	bool	UI::FlyingOut( int startPos )
	{
		if ( titleInfo.textImage.t >= 1.0f )	return	true;

		//	パラメータ加算
		titleInfo.textImage.t += 0.01f;
		if ( titleInfo.textImage.t >= 1.0f )	titleInfo.textImage.t = 1.0f;

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv5, titleInfo.textImage.t );

		//	上移動
		Lerp( titleInfo.textImage.y, startPos, ( int )airPlaneInfo.OUT_END_POS_Y, titleInfo.param );

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv3, titleInfo.textImage.t );

		//	左移動
		Lerp( titleInfo.textImage.x, airPlaneInfo.OUT_START_POS_X, airPlaneInfo.OUT_END_POS_X, titleInfo.param );

		//	回転
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - airPlaneInfo.ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
		titleInfo.textImage.angle = 0.0f + ( D3DX_PI / 180.0f * - 1.0f ) * sinf( D3DX_PI / 2 * titleInfo.textImage.t );

		if ( titleInfo.textImage.t >= 1.0f )	return	true;
		return	false;
	}

//------------------------------------------------------------------------------
//	メイン動作初期化
//------------------------------------------------------------------------------

	//	コインバー初期化
	void	UI::CoinBarInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.93f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.77f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.11f );
		ImageInitialize( frame, x, y, w, h, 0, 32 * 5, 512, 64);
		ImageInitialize( backgauge, frame.x, frame.y, frame.w - (frame.w / 10), frame.h - (frame.h / 2), 0, 32 * 4, 480, 32);
		ImageInitialize( gauge, frame.x, frame.y, 0, backgauge.h, 0, 32, 0, 32);
		w = static_cast<int>( iexSystem::ScreenWidth * 0.025f );
		h = static_cast<int>( iexSystem::ScreenHeight * 0.04f );
		ImageInitialize(faceImage, 0, frame.y - (frame.h / 2), w, h, 256, 256, 256, 256);

		for (int i = 0; i < NUM_BAR; i++)
		{
			bar_x[i] = frame.x;
			bar_sx[i] = backgauge.sx;
			state_x[i] = 0;
		}
	}

	//	コイン枚数初期化
	void	UI::CoinNumberInitialize( void )
	{
		//	コイン枚数位置（横）
		coinNumInfo[0].pos.x = static_cast<int>(iexSystem::ScreenWidth * 0.1);
		coinNumInfo[1].pos.x = static_cast<int>(iexSystem::ScreenWidth * 0.3);
		coinNumInfo[2].pos.x = static_cast<int>(iexSystem::ScreenWidth * 0.7);
		coinNumInfo[3].pos.x = static_cast<int>(iexSystem::ScreenWidth * 0.9);

		//	表示色
		coinColor[0] = Vector3(1.0f, 0.0f, 0.0f);
		coinColor[1] = Vector3(0.0f, 0.0f, 1.0f);
		coinColor[2] = Vector3(1.0f, 1.0f, 0.0f);
		coinColor[3] = Vector3(0.0f, 1.0f, 0.0f);


		FOR(0, PLAYER_MAX){
			//	コイン枚数用
			coinNum[value] = gameManager->GetCoinNum(value);

			//	コイン枚数位置（縦）
			coinNumInfo[value].pos.y = static_cast<int>(iexSystem::ScreenHeight * 0.1f);
			coinNumInfo[value].scale = 100;
			
			//	画像設定
			coinNumInfo[value].one.obj			= pCoinNumImage;
			coinNumInfo[value].ten.obj			= pCoinNumImage;
			coinNumInfo[value].hundred.obj		= pCoinNumImage;
			
			//	情報設定
			SetCoinImageInfo(coinNumInfo[value], numInfo[value], gameManager->GetCoinNum(value));
			
			//	色設定	
			coinNumInfo[value].one.color		= coinColor[value];
			coinNumInfo[value].ten.color		= coinColor[value];
			coinNumInfo[value].hundred.color	= coinColor[value];

		
		}
	}

	//	タイマー初期化
	void	UI::TimerInitialize( void )
	{

		//	構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.43f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.05f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.09f );
		ImageInitialize( timer, x, y, w, h, 64, 0, 64, 64 );
		minute = 0;
		for ( int i = 0; i < 2; i++ )
		{
			second[i] = 0;
		}
	}

	//	ニュースバー初期化
	void	UI::NewsBarInitialize( void )
	{
		//	ニュースバー初期化
		newsbar.renderflag = false;
		newsbar.left = iexSystem::ScreenWidth;
		newsbar.top = 0;
		newsbar.right = iexSystem::ScreenWidth;
		newsbar.bottom = static_cast<int>( iexSystem::ScreenHeight * ( 0.07f ) );
		newsbar.text = GameInfo::NewsText[ gameManager->GetLastBonus() ];
		newsbar.alpha = 0.5f;
		newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
		newsbar.step = 0;
		newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * ( 1.17f ) );
	}

	//	カウントダウン・スタート・終了演出
	void	UI::StartAndTimeUpInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.27f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		ImageInitialize( countImage, x, y, w, h, 0, 0, 256, 256 );
		countImage.renderflag = true;
		count = 0;
		waitTimer = 0;
	}

	//	どんけつ演出初期化
	void	UI::DonketsuDirectionInitialize( void )
	{
		//　構造体初期化
		
		//　顔ルーレット関連
		ddInfo.face.obj = new iex2DObj("DATA/UI/chara_emotion.png");
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		ImageInitialize(ddInfo.face, x, y, 0, 0, FACE_INFO::Normal * 256, 0 * 256, 256, 256);
		ddInfo.f = 0;
		ddInfo.roulette = 0;
		ddInfo.face_step = -1;

		//　DonketsuBoooooooooooost!!!
		ddInfo.DB.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		x = static_cast<int>( iexSystem::ScreenWidth * 0.94f );
		y = static_cast<int>( iexSystem::ScreenHeight * -0.14f );
		ImageInitialize(ddInfo.DB, x, y, 0, 0, 0, 0, 512, 256);
		ddInfo.DB.angle = 0;
		ddInfo.DB_step = -1;

		//　？P関連
		x = static_cast<int>( iexSystem::ScreenWidth * 0.78f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.76f );
		ddInfo.P.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		ImageInitialize(ddInfo.P, x, y, 0, 0, 0, 256, 128, 128);
		ddInfo.P.renderflag = false;
		ddInfo.P_step = -1;

		//　FightLast
		ddInfo.fight.obj = new iex2DObj("DATA/UI/L_coin_alert.png");
		x = static_cast<int>(iexSystem::ScreenWidth * 0.5f);
		y = static_cast<int>(iexSystem::ScreenHeight * 0.5f);
		ImageInitialize(ddInfo.fight, x, y, 256, 256, 0, 0, 256, 256);
		ddInfo.fight.renderflag = false;
		ddInfo.fight_step = -1;
	}

	//	警告演出初期化
	void	UI::AlertInitialize( void )
	{
		alertInfo.flag = false;
		alertInfo.alpha = 0.0f;
		alertInfo.timer = 0;
		alertInfo.param = 0.0f;
		alertInfo.type = 0;

		//	画像構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.15f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.27f );
		ImageInitialize( alertImage, x, y, w, h, 0, 0, 256, 256 );
		alertImage.renderflag = true;
		ImageInitialize(alert_coinImage, x, y, w, h, 0, 0, 256, 256);
		alert_coinImage.renderflag = true;
	}

	//	HurryUp演出初期化
	void	UI::HurryUpInitialize( void )
	{
		hurryInfo.alpha = 0.0f;
		hurryInfo.flag = false;
		hurryInfo.param = 0.0f;
		hurryInfo.timer = 0;
	}

	//	プレイヤー番号画像初期化
	void	UI::PlayerNumberInitialize( void )
	{
		int rank = 0;
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			rank = characterManager->GetRank( i );
			pNumImage[i].obj = playerNumber;
			int w = static_cast<int>( iexSystem::ScreenWidth * 0.04f );
			int h = static_cast<int>( iexSystem::ScreenHeight * 0.07f );
			ImageInitialize( pNumImage[i], 0, 0, w, h, rank * 128, 128, 128, 64 );
		}
	}
	
	//	時間警告初期化
	void	UI::LastProductionInitialize( void )
	{
		SetScaling(timer, 1.0f);
		last_state = false;
		last_t = 0.0f;
		last_alpha = 1.0f;
	}

	//	体力画像初期化
	void	UI::LifeInitialize( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			lifeInfo[value].life = gameManager->GetStartLife( value );
			for ( int i = 0; i < lifeInfo[value].life; i++ )
			{
				lifeInfo[value].lifeImage[i].obj = life;
				ImageInitialize( lifeInfo[value].lifeImage[i], 0, 0, 30, 30, 0, 0, 256, 256 );
			}
		}
	}

	//	ラウンド初期化
	void	UI::RoundInitialize( void )
	{
		int	x = static_cast<int>( iexSystem::ScreenWidth * 0.07f );
		int	y = static_cast<int>( iexSystem::ScreenHeight * 0.9f );
		int	w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		int	h = static_cast<int>( iexSystem::ScreenHeight * 0.05f );
		int	sx = 0;
		int	sy = gameManager->GetRound() * 128;
		int	sw = 512;
		int	sh = 128;
		ImageInitialize( roundImage, x, y, w, h, sx, sy, sw, sh );
		//roundImage.angle = D3DXToRadian( -30.0f );
	}
	
//------------------------------------------------------------------------------
//	メイン動作更新
//------------------------------------------------------------------------------

	//	タイマー関連動作
	void	UI::TimerUpdate( void )
	{
		this->time = gameManager->GetTimer();
		//分
		minute = this->time / MINUTE % 10;
		//秒二桁目
		second[0] = ( this->time / SECOND ) % 60 / 10 % 10;
		//秒一桁目
		second[1] = this->time / SECOND % 10;
	}

	//	ニュースバー関連動作
	void	UI::NewsBarUpdate( void )
	{
		if ( !gameManager->GetNewsFlag() )	return;

		switch ( newsbar.step )
		{
		case 0:
			//	バー出現
			newsbar.left -= static_cast<int>( iexSystem::ScreenWidth * 0.02f );
			if ( newsbar.left <= 0 )
			{
				newsbar.left = 0;
				newsbar.step++;
			}
			break;

		case 1:
			//	テキスト出現
			newsbar.textleft--;
			if ( newsbar.textleft <= static_cast<int>( iexSystem::ScreenWidth * -0.23f ) )
			{
				newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * 1.17f );
				newsbar.step++;
			}
			break;

		case 2:
			//	バー退避
			newsbar.right -= static_cast<int>( iexSystem::ScreenWidth * 0.02f );
			if ( newsbar.right <= 0 )
			{
				newsbar.right = 0;
				newsbar.step++;
			}
			break;

		case 3:
			//	初期化
			gameManager->SetNewsFlag( false );
			newsbar.left = iexSystem::ScreenWidth;
			newsbar.right = iexSystem::ScreenWidth;
			newsbar.textleft = static_cast<int>( iexSystem::ScreenWidth * 1.17f );
			newsbar.step = 0;
			break;
		}
	}

	//	コインバー関連動作
	void	UI::CoinBarUpdate( void )
	{
		//	バー動作
		BarControl();

		//	顔動作
		StateImageControl();
	}

	//	コイン枚数動作
	void	UI::CoinNumberUpdate( void )
	{
		FOR(0, PLAYER_MAX){
			CoinCounter(gameManager->GetCoinNum(value),value);
			CoinImageInfoUpdate(coinNumInfo[value], numInfo[value], coinNum[value]);
		}
	}

	//	カウントダウン・スタート演出
	void	UI::StartUpdate( void )
	{
		//	タイマー更新
		waitTimer++;

		//	一秒ごとに画像進める
		if ( waitTimer % SECOND == 0 )
		{
			//	カウントダウン
			count++;

			//	読み込み位置・サイズ設定
			switch ( count )
			{
			case 1:
				countImage.sx = 256;
				countImage.sy = 0;
				break;

			case 2:
				countImage.sx = 0;
				countImage.sy = 256;
				break;

			case 3:
				countImage.sx = 512;
				countImage.sy = 0;
				countImage.sw = 512;
				countImage.sh = 512;
				countImage.w = static_cast<int>( iexSystem::ScreenWidth * 0.59f );
				countImage.h = static_cast<int>( iexSystem::ScreenHeight * 1.04f );
				SetWave( countImage, 1.5f );
				break;

			case 4:
				waitTimer = 2 * SECOND;
				
				//	画像読み込み位置・サイズ設定			
				countImage.w = static_cast<int>( iexSystem::ScreenWidth * 0.47f );
				countImage.h = static_cast<int>( iexSystem::ScreenHeight * 0.51f );
				countImage.sx = 0;
				countImage.sy = 512;
				countImage.sw = 1024;
				countImage.sh = 512;

				//	メインゲームへ
				changeflag = true;
				break;
			}

		}

		WaveUpdate( countImage );
	}

	//	タイムアップ演出
	void	UI::FinishUpdate( void )
	{
		waitTimer--;

		if ( waitTimer <= 0 )	changeflag = true;
	}

	//	どんけつ決定演出
	void	UI::DonketsuDirectionUpdate( void )
	{
		/*
			・「どんけつは～？」を最初にバンッ!
			・４人の顔を２回ずつぷわぷわっと風船が膨らむような感じに出す。ちょっと間をおいて９回目で決定
			　決定時に顔のバックに強調エフェクト、背景を暖色系に。
			・下方に○Ｐ！
		*/

		//　演出用時間更新
		static int wait = DD_TIMING::WAIT_MAX;
		if (wait <= 0)
		{
			wait = 5 * SECOND + 30;
			changeflag = true;
		}

		//　「DonketsuBoooooooooooost!!!」
		DB_Direction(wait);
		
		//　顔ルーレット
		FaceRoulette(wait);

		//　「？P」
		P_Direction(wait);

		//　がんばれ！のやつ
		Fight_Direction(wait);

		//　更新
		wait--;
	}

	//	警告演出
	void	UI::AlertUpdate( void )
	{
		alertInfo.param += D3DX_PI / 30.0f;
		alertInfo.alpha = 0.1f + 0.1f * sinf( alertInfo.param );

		alertInfo.timer++;
		if (alertInfo.timer % 15 == 0)
		{
			alertImage.renderflag = !alertImage.renderflag;
			alert_coinImage.renderflag = !alert_coinImage.renderflag;
		}

		//	二秒半で終了
		if ( alertInfo.timer >= 120 )
		{
			alertInfo.flag = false;
			alertInfo.alpha = 0.0f;
		}
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
		ScalingUpdate(timer,100);

		if (gameManager->GetTimer() / SECOND <= 3) last_state = true;
		if (!last_state)	return;
		last_t += D3DX_PI / 180 * 1.0f;
		
		//	パラメータ上限設定
		if (last_t >= 1.0f)
		{
			last_t = 1.0f;
		}
		
		Lerp(last_alpha, 1.0f, 0.0f, last_t);

		if (gameManager->GetTimer() % SECOND == 0) last_t = 0.0f;



	}

	//	プレイヤー番号位置決定
	void	UI::PlayerNumberUpdate( void )
	{
		//	変数初期化
		Vector3 imagePos = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	p_pos = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	up = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	out = Vector3( 0.0f, 0.0f, 0.0f );

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	画像を表示する場所を設定
			up = characterManager->GetUp( i );
			p_pos = characterManager->GetPos( i );
			imagePos = p_pos + up * 7.0f;

			//	クライアント座標に変換
			WorldToClient( imagePos, out, matView * matProjection );

			//	構造体に情報設定
			pNumImage[i].x = static_cast<int>( out.x );
			pNumImage[i].y = static_cast<int>( out.y );

			int rank = 0;
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				rank = characterManager->GetRank(i) - 1;
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

		FOR( 0, PLAYER_MAX )
		{
			//	表示座標算出
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			lifePos = p_Pos + p_Up * 5.0f;
			WorldToClient( lifePos, out, matView * matProjection );

			//	構造体に設定
			lifeInfo[value].life = characterManager->GetLife( value );
			for ( int i = 0; i < lifeInfo[value].life; i++ )
			{
				switch ( lifeInfo[value].life )
				{
				case 1:
					//	表示位置設定
					lifeInfo[value].lifeImage[i].x = static_cast<int>( out.x );
					lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 30 );
					break;

				case 2:
					//	表示位置設定
					lifeInfo[value].lifeImage[i].x = static_cast<int>( ( out.x - 15 ) + 30 * i );
					lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 30 );
					break;

				case 3:
					//	表示位置設定
					lifeInfo[value].lifeImage[i].x = static_cast<int>( ( out.x - 30 ) + 30 * i );
					lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 30 * ( i % 2 ) );
					break;

				case 4:
					//	表示位置設定
					lifeInfo[value].lifeImage[i].x = static_cast<int>( out.x - 40 + ( 80 / 3 ) * i );
					if ( i == 1 || i == 2 )	lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 30 );
					else								lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y );
					break;

				case 5:
					lifeInfo[value].lifeImage[i].x = static_cast<int>( out.x - 40 + ( 100 / 4 ) * i );
					if ( i == 2 )							lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 40 );
					else	if ( i == 1 || i == 3 )	lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y - 20 );
					else										lifeInfo[value].lifeImage[i].y = static_cast<int>( out.y );
					break;
				}
				
				//	読み込み位置設定
				if ( lifeInfo[value].life > i )	lifeInfo[value].lifeImage[i].sx = 256;
				else										lifeInfo[value].lifeImage[i].sx = 0;
			}
		}
	}

//------------------------------------------------------------------------------
//	メイン描画
//------------------------------------------------------------------------------

	//	コインバー描画
	void	UI::CoinBarRender( void )
	{
		//フレーム
		RenderImage(frame, frame.sx, frame.sy, frame.sw, frame.sh, IMAGE_MODE::NORMAL);

		//灰色のバー
		RenderImage(backgauge, backgauge.sx, backgauge.sy, backgauge.sw, backgauge.sh, IMAGE_MODE::NORMAL);

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			//色のバー
			gauge.w = bar_sx[i];	gauge.sw = bar_sx[i];

			//	（左上位置 - ゲージ幅の半分）で中心、ゲージを右向きへ増やすため
			RenderImage( gauge, gauge.sx, gauge.sy * i, gauge.sw, gauge.sh, IMAGE_MODE::NORMAL, static_cast<int>( bar_x[i] - (backgauge.w * 0.5f) + (gauge.sw * 0.5f ) ), gauge.y );
																					
			//顔
			RenderImage(faceImage, faceImage.sx * state_type[i], faceImage.sy * charatype[i], faceImage.sw, faceImage.sh, IMAGE_MODE::NORMAL, state_x[i] - (backgauge.w / 2), faceImage.y);
		}
	}

	//	コイン枚数描画
	void	UI::CoinNumberRender( void )
	{
		FOR(0, PLAYER_MAX)
		{
			//	１００の位描画
			int		sx = coinNumInfo[value].hundred.sx;
			int		sy = coinNumInfo[value].hundred.sy;
			int		sw = coinNumInfo[value].hundred.sw;
			int		sh = coinNumInfo[value].hundred.sh;

			if (coinNumInfo[value].hundredRenderFlag)
				RenderImage(coinNumInfo[value].hundred, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM);

			//	１０の位描画
			sx = coinNumInfo[value].ten.sx;
			sy = coinNumInfo[value].ten.sy;
			sw = coinNumInfo[value].ten.sw;
			sh = coinNumInfo[value].ten.sh;
			RenderImage(coinNumInfo[value].ten, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM);

			//	１の位描画
			sx = coinNumInfo[value].one.sx;
			sy = coinNumInfo[value].one.sy;
			sw = coinNumInfo[value].one.sw;
			sh = coinNumInfo[value].one.sh;
			RenderImage(coinNumInfo[value].one, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM);
		}
	}

	//	ニュース描画
	void	UI::NewsBarRender( void )
	{
		if( !gameManager->GetNewsFlag() )	return;
		
		iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
		IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + static_cast<int>( iexSystem::ScreenHeight * 0.013f ), 500, 200, 0xFFFFFFFF );
	}

	//	タイマー描画
	void	UI::TimerRender( void )
	{
		
		RenderImage( timer, timer.sx * minute		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 0, timer.y );
		RenderImage( timer, timer.sx * 10			, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 1, timer.y  );
		RenderImage( timer, timer.sx * second[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 2, timer.y  );
		RenderImage( timer, timer.sx * second[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 3, timer.y  );

	}

	//	カウントダウン・スタート演出
	void	UI::StartRender( void )
	{
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::NORMAL );
		RenderImage( countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::WAVE );
	}

	//	タイムアップ演出
	void	UI::FinishRender( void )
	{
		RenderImage(countImage, countImage.sx, countImage.sy, countImage.sw, countImage.sh, IMAGE_MODE::NORMAL );
	}

	//	どんけつ演出
	void	UI::DonketsuDirectionRender( void )
	{
		//　グレーバック
		DWORD	color = 0xD0000000;
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color );

		//　顔ルーレット
		RenderImage(ddInfo.face, FACE_INFO::Normal * 256, charatype[ddInfo.f] * 256, 256, 256, IMAGE_MODE::NORMAL);
		
		//　DonketsuBoooooooooooost!!!
		RenderImage(ddInfo.DB, 0, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );
	
		//　？P
		RenderImage(ddInfo.P, gameManager->GetWorst()*128, 256, 128, 128, IMAGE_MODE::NORMAL);

		//　FightLast
		RenderImage(ddInfo.fight, 0, 0, 256, 256, IMAGE_MODE::NORMAL);

		//　デバッグ
		if (debug)
		{
			char	str[256];
			int		worst = gameManager->GetWorst();
			wsprintf(str, "ビリは p%d", worst + 1);
			DrawString(str, 200, 70);
		}
	}

	//	警告描画
	void	UI::AlertRender( void )
	{
		int color = (alertInfo.type == ALERT_TYPE_INFO::JAM) ?
			GetColor(1.0f, 0.0f, 0.0f, alertInfo.alpha) :	//　赤
			GetColor(1.0f, 1.0f, 0.0f, alertInfo.alpha);	//　黄
		
		//	フィルター描画
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, RS_COPY, color);

		//	警告画像描画
		(alertInfo.type == ALERT_TYPE_INFO::JAM) ?
			RenderImage(alertImage, 0, 0, 256, 256, IMAGE_MODE::NORMAL) :
			RenderImage(alert_coinImage, 0, 0, 256, 256, IMAGE_MODE::NORMAL);
	}

	//	時間警告描画
	void	UI::LastProductionRender( void )
	{
		//	タイマー文字色を赤へ
		timer.sy = 64;
		if (!last_state){
			RenderImage(timer, timer.sx * minute, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 0, timer.y);
			RenderImage(timer, timer.sx * 10, timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 1, timer.y);
			RenderImage(timer, timer.sx * second[0], timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 2, timer.y);
			RenderImage(timer, timer.sx * second[1], timer.sy, timer.sw, timer.sh, IMAGE_MODE::SCALING, timer.x + timer.w * 3, timer.y);
		}
		else
		{
			timer.alpha = last_alpha;
			timer.x = iexSystem::ScreenWidth / 2;	timer.y = iexSystem::ScreenHeight / 2;
			RenderImage(timer, timer.sx * second[1], timer.sy, timer.sw, timer.sh, IMAGE_MODE::ADOPTPARAM);
		}
		//	タイマー文字色を白へ
		timer.sy = 0;

	}

	//	プレイヤー番号描画
	void	UI::PlayerNumberRender( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			RenderImage( pNumImage[i], pNumImage[i].sx, pNumImage[i].sy, pNumImage[i].sw, pNumImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	ライフ描画
	void	UI::LifeRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			for ( int i = 0; i < lifeInfo[value].life; i++ )
			{
				RenderImage(lifeInfo[value].lifeImage[i], lifeInfo[value].lifeImage[i].sx, lifeInfo[value].lifeImage[i].sy, lifeInfo[value].lifeImage[i].sw, lifeInfo[value].lifeImage[i].sh, IMAGE_MODE::NORMAL );
			}
		}
	}

	//	ラウンド描画
	void	UI::RoundRender( void )
	{
		RenderImage( roundImage, roundImage.sx, roundImage.sy, roundImage.sw, roundImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	コイン枚数を1枚ずつカウントアップダウン
	void	UI::CoinCounter( int coin, int num )
	{
		if (coinNum[num] == coin) return;

		//	コイン減少
		if( coinNum[num] > coin )
		{
			coinNum[num]--;
		}

		//	コイン増加
		if (coinNum[num] < coin)
		{
			coinNum[num]++;
		}
	}

	//	バー動作
	void	UI::BarControl( void )
	{
		//static const int MAX_COIN = 201;

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];

		int num_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = gameManager->GetCoinNum(i);
			bar_sx[i] = backgauge.w * num_coin[i] / gameManager->GetCoinMax();
		}
	}

	//	バー動作２
	void		UI::BarControl2( void )
	{
		//	プレイヤー情報登録
		FOR( 0, PLAYER_MAX )
		{
			coinBarInfo.coinNum[value] = gameManager->GetCoinNum( value );
			coinBarInfo.param[value] = 1.0f;
		}

		float		sub[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float		average = 0;
		static const int MAX_COIN = 201;

		//	コイン差を調べる
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	コイン差で割合をだす
				if ( i == n ) continue;

				if ( coinBarInfo.coinNum[n] == 0 )	sub[n] = 1.0f;
				else sub[n] = ( float )( coinBarInfo.coinNum[i] / coinBarInfo.coinNum[n] );
			}

			//	平均を求める
			average = ( sub[0] + sub[1] + sub[2] + sub[3] ) / PLAYER_MAX;
			average /= PLAYER_MAX - 1;
			coinBarInfo.param[i] = average;
		}

		int		originWidth = backgauge.w / PLAYER_MAX;
		FOR( 0, PLAYER_MAX )
		{
			bar_sx[value] = originWidth * ( int )coinBarInfo.param[value];
		}

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];
	}

	//	バー描画
	void		UI::BarRender( void )
	{
	}

	//	顔動作
	void	UI::StateImageControl( void )
	{
		//画像の描画場所 = 各色の先頭　+　各色の中心　-　画像サイズの半分
		state_x[0] = (bar_x[0] + (bar_x[1] - bar_x[0]) / 2);
		state_x[1] = (bar_x[1] + (bar_x[2] - bar_x[1]) / 2);
		state_x[2] = (bar_x[2] + (bar_x[3] - bar_x[2]) / 2);
		state_x[3] = (bar_x[3] + (bar_x[3] + bar_sx[3] - bar_x[3]) / 2);


		int num_coin[4], temp_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = temp_coin[i] = gameManager->GetCoinNum(i);
		}

		for (int i = 0, temp; i < 4 - 1; i++)
		{
			for (int j = 4 - 1; j > i; j--)
			{
				if (temp_coin[j - 1] > temp_coin[j])
				{
					temp = temp_coin[j];
					temp_coin[j] = temp_coin[j - 1];
					temp_coin[j - 1] = temp;
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (num_coin[i] == temp_coin[j])
				{
					switch (j)
					{
					case 0:
						state_type[i] = FACE_INFO::Sad;
						break;

					case 1:
					case 2:
						state_type[i] = FACE_INFO::Normal;
						break;

					case 3:
						state_type[i] = FACE_INFO::Good;
						break;
					}
				}
			}
		}
	}

	//　DonketsuBoooooooooooost!!!の演出
	void	UI::DB_Direction( int wait )
	{
		const int POS_X = iexSystem::ScreenWidth / 5;		//　固定位置
		const int POS_Y = iexSystem::ScreenHeight / 6;		//　固定位置
		const int TRANS_X = (POS_X - static_cast<int>( iexSystem::ScreenWidth * 0.94f ) ) / 30;	//　TPF
		const int TRANS_Y = (POS_Y - static_cast<int>( iexSystem::ScreenHeight * -0.14f ) ) / 30;	//　TPF
		const int SIZE_X = static_cast<int>( iexSystem::ScreenWidth * 0.47f );
		const int SIZE_Y = static_cast<int>( iexSystem::ScreenHeight * 0.41f );
		const int SCALING_X = SIZE_X / 30;
		const int SCALING_Y = SIZE_Y / 30;
		const float ROT = 30 * 0.0175f;	//　回転量
		
		//　タイミングによる設定
		switch (wait)
		{
			//　演出スタート
		case DD_TIMING::WAIT_MAX:
			ddInfo.DB_step = 0;
			break;

			//　固定
		case DD_TIMING::DB_LOCK:
			ddInfo.DB_step = 1;
			break;
		}

		//　演出部
		switch (ddInfo.DB_step)
		{
			//　移動・回転
		case 0:
			ddInfo.DB.x += TRANS_X;
			ddInfo.DB.y += TRANS_Y;
			ddInfo.DB.p.x = ddInfo.DB.x;
			ddInfo.DB.p.y = ddInfo.DB.y;
			ddInfo.DB.w += SCALING_X;
			ddInfo.DB.h += SCALING_Y;
			ddInfo.DB.angle += ROT;
			break;

			//　固定
		case 1:
			ddInfo.DB.x = POS_X;
			ddInfo.DB.y = POS_Y;
			ddInfo.DB.w = SIZE_X;
			ddInfo.DB.h = SIZE_Y;
			ddInfo.DB.angle = D3DXToRadian(-30.0f);
			break;
		}
	}

	//　どんけつ演出の顔
	void	UI::FaceRoulette( int face_wait )
	{
		const int INTERVAL = 10;
		const int SPF = 320 / INTERVAL;	// SCALE PER FRAME(１フレームに拡大する分)

		switch (face_wait)
		{
			//　イントロ　→　１Ｐ
		case DD_TIMING::FACE_LOCK + INTERVAL * 13:
		case DD_TIMING::FACE_LOCK + INTERVAL * 9:
			ddInfo.face_step = 0;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(0);
			break;

			//　１Ｐ　→　２Ｐ
		case DD_TIMING::FACE_LOCK + INTERVAL * 12:
		case DD_TIMING::FACE_LOCK + INTERVAL * 8:
			ddInfo.face_step = 1;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(1);
			break;

			//　２Ｐ　→　３Ｐ
		case DD_TIMING::FACE_LOCK + INTERVAL * 11:
		case DD_TIMING::FACE_LOCK + INTERVAL * 7:
			ddInfo.face_step = 2;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(2);
			break;

			//　３Ｐ　→　４Ｐ
		case DD_TIMING::FACE_LOCK + INTERVAL * 10:
		case DD_TIMING::FACE_LOCK + INTERVAL * 6:
			ddInfo.face_step = 3;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = characterManager->GetPlayerNum(3);
			break;

			//　４Ｐ　→　しぼみ始める
		case DD_TIMING::FACE_LOCK + INTERVAL * 5:
			ddInfo.face_step = 4;
			ddInfo.face.w = static_cast<int>( iexSystem::ScreenWidth * 0.25f );
			ddInfo.face.h = static_cast<int>( iexSystem::ScreenHeight * 0.44f );
			break;

			//　しぼみきる　→　待ち
		case DD_TIMING::FACE_LOCK + INTERVAL * 4:
			ddInfo.face_step = 5;
			ddInfo.face.w = ddInfo.face.h = 0;
			ddInfo.f = -1;
			break;


			//　待ち　→　ビリ
		case DD_TIMING::FACE_LOCK + INTERVAL * 1:
			ddInfo.face_step = 6;
			ddInfo.f = gameManager->GetWorst();
			break;

			//　ビリ　→　そのまま放置
		case DD_TIMING::FACE_LOCK:
			ddInfo.face_step = 7;
			break;

		case DD_TIMING::FIGHT_START:
			ddInfo.face_step = 8;
			break;
		}

		//　演出
		switch (ddInfo.face_step)
		{
			//　１～４Ｐ拡大
		case 0:
		case 1:
		case 2:
		case 3:
			ddInfo.face.renderflag = true;
			ddInfo.face.w += SPF;
			ddInfo.face.h += SPF;
			break;

			//　８人目の縮小
		case 4:
			ddInfo.face.renderflag = true;
			ddInfo.face.w -= SPF;
			ddInfo.face.h -= SPF;
			break;
		
			//　ビリ拡大（大きめに）
		case 6:
			ddInfo.face.renderflag = true;
			ddInfo.face.w += static_cast<int>( iexSystem::ScreenWidth * 0.39f ) / INTERVAL;
			ddInfo.face.h += static_cast<int>( iexSystem::ScreenHeight * 0.69f ) / INTERVAL;
			break;

			//　放置
		case 7:
			ddInfo.face.renderflag = true;
			ddInfo.face.w = static_cast<int>( iexSystem::ScreenWidth * 0.39f );
			ddInfo.face.h = static_cast<int>( iexSystem::ScreenHeight * 0.69f );
			break;

		default:
			ddInfo.face.renderflag = false;
			break;
		}
	}
	
	//　「？Ｐ」の演出
	void	UI::P_Direction( int wait )
	{
		const int INTERVAL = 5;
		const int SIZE_X = static_cast<int>( iexSystem::ScreenWidth * 0.31f );
		const int SIZE_Y = static_cast<int>( iexSystem::ScreenHeight * 0.49f );
		const int SCALING = 200;
		const int SPF = SCALING / INTERVAL;
		
		//　タイミングによる設定
		switch (wait)
		{
			//　出現
		case DD_TIMING::P_START:
			ddInfo.P.renderflag = true;
			ddInfo.P_step = 0;
			break;
		case DD_TIMING::P_START - 5:
			ddInfo.P_step = 3;
			break;

			//　拡大スタート
		case DD_TIMING::P_LOCK + INTERVAL * 6:
		case DD_TIMING::P_LOCK + INTERVAL * 4:
		case DD_TIMING::P_LOCK + INTERVAL * 2:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			ddInfo.P_step = 1;
			break;

			//　縮小スタート
		case DD_TIMING::P_LOCK + INTERVAL * 5:
		case DD_TIMING::P_LOCK + INTERVAL * 3:
		case DD_TIMING::P_LOCK + INTERVAL * 1:
			ddInfo.P.w = SIZE_X + SCALING;
			ddInfo.P.h = SIZE_Y + SCALING;
			ddInfo.P_step = 2;
			break;
	
			//　固定
		case DD_TIMING::P_LOCK:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			ddInfo.P_step = 3;
			break; 
		}

		//　演出部
		switch (ddInfo.P_step)
		{
			//　出現
		case 0:
			ddInfo.P.w += SIZE_X / 10;
			ddInfo.P.h += SIZE_Y / 10;
			break;

			//　拡大
		case 1:
			ddInfo.P.w += SPF;
			ddInfo.P.h += SPF;
			break;

			//　縮小
		case 2:
			ddInfo.P.w -= SPF;
			ddInfo.P.h -= SPF;
			break;

			//　固定
		case 3:
			ddInfo.P.w = SIZE_X;
			ddInfo.P.h = SIZE_Y;
			break;
		}
	}

	//　FightLastの演出
	void	UI::Fight_Direction(int wait)
	{
		//　タイミングによる設定
		switch (wait)
		{
		case DD_TIMING::FIGHT_START:
			ddInfo.fight.renderflag = true;
			ddInfo.fight_step = 0;

			ddInfo.DB.renderflag = false;
			ddInfo.face.renderflag = false;
			ddInfo.P.renderflag = false;
			break;
		}

		//　演出部
		switch (ddInfo.fight_step)
		{
		case 0:
			
			break;
		}
	}
	
	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更
	void	UI::CoinImageInfoUpdate(NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num)
	{
		//	桁数確認
		if (num >= 100)		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo(numInfo, num);

		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if (numImageInfo.hundredRenderFlag)
		{
			//	１０の位設定
			numImageInfo.ten.x = numImageInfo.pos.x;
			numImageInfo.ten.sx = numInfo.ten * 64;

			//	１００の位設定
			numImageInfo.hundred.sx = numInfo.hundred * 64;

			//	１の位設定
			numImageInfo.one.x = numImageInfo.pos.x + static_cast<int>(numImageInfo.ten.w / 1.5f);
			numImageInfo.one.sx = numInfo.one * 64;
		}
		else
		{
			//	１０の位設定
			numImageInfo.ten.x = numImageInfo.pos.x - numImageInfo.scale / 3;
			numImageInfo.ten.sx = numInfo.ten * 64;

			//	１の位設定
			numImageInfo.one.x = numImageInfo.pos.x + numImageInfo.scale / 3;
			numImageInfo.one.sx = numInfo.one * 64;
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

	//	飛び入り設定
	void	UI::SetFlyingIn( int type )
	{

	}

	//	モード別読み込み位置設定
	void	UI::SetImageSrcPos( int mode )
	{
		if ( mode == TITLE_MODE::TITLE )
		{
			titleInfo.textImage.renderflag = false;
		}
		else
		{
			titleInfo.textImage.renderflag = true;
		}

		switch ( mode ) 
		{
		case TITLE_MODE::MENU:
			SetImageSrcPos( 0, 0 );
			break;

		case TITLE_MODE::OPTION:
			SetImageSrcPos( 0, 128 );
			break;

		case TITLE_MODE::CREDIT:
			SetImageSrcPos( 0, 256 );
			break;

		case TITLE_MODE::PLAY:
			SetImageSrcPos( 0, 384 );
			break;

		case TITLE_MODE::MOVE_MAIN:
			break;
		}
	}

	//	読み込み位置設定
	void	UI::SetImageSrcPos( int sx, int sy )
	{
		titleInfo.textImage.sx = sx;
		titleInfo.textImage.sy = sy;
	}

	//	モード変更フラグ取得
	bool	UI::GetChangeFlag( void )
	{
		bool	out = this->changeflag;
		return	out;
	}

	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更
	void	UI::SetCoinImageInfo(NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num)
	{
		//	桁数確認
		if (num >= 100)		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo(numInfo, num);

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if (numImageInfo.hundredRenderFlag)
		{
			//	１０の位設定
			x = numImageInfo.pos.x;
			y = numImageInfo.pos.y;
			w = h = numImageInfo.scale;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize(numImageInfo.ten, x, y, w, h, sx, sy, sw, sh);

			//	１００の位設定
			x = numImageInfo.pos.x - static_cast<int>(numImageInfo.ten.w / 1.5f);
			sx = numInfo.hundred * 64;
			ImageInitialize(numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh);

			//	１の位設定
			x = numImageInfo.pos.x + static_cast<int>(numImageInfo.ten.w / 1.5f);
			sx = numInfo.one * 64;
			ImageInitialize(numImageInfo.one, x, y, w, h, sx, sy, sw, sh);
		}
		else
		{
			//	１０の位設定
			w = h = numImageInfo.scale;
			x = numImageInfo.pos.x - w / 3;
			y = numImageInfo.pos.y;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize(numImageInfo.ten, x, y, w, h, sx, sy, sw, sh);

			//	１の位設定
			x = numImageInfo.pos.x + w / 3;
			sx = numInfo.one * 64;
			ImageInitialize(numImageInfo.one, x, y, w, h, sx, sy, sw, sh);

			//	１００の位設定(初期化の時のみセットするため）
			x = numImageInfo.pos.x - static_cast<int>(numImageInfo.ten.w / 1.5f);
			sx = numInfo.hundred * 64;
			ImageInitialize(numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh);
		}
	}

	//	数値構造体に値をセットする
	void	UI::SetNumberInfo(NUMBER_INFO& number, int coin)
	{
		number.hundred = coin / 100 % 10;
		number.ten = coin / 10 % 10;
		number.one = coin % 10;
	}
