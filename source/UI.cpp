
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
		namespace TITLE_TEXT_POS_INFO
		{
			enum
			{
				IN_START_POS_X = 1980,
				IN_START_POS_Y = -100,
				IN_END_POS_X = 640,
				IN_END_POS_Y = 150,
				OUT_START_POS_X = 640,
				OUT_END_POS_X = -700,
				OUT_END_POS_Y = 0,
				ROLL_POINT_ADJUST_X = 200,
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
		titleInfo.textImage.obj = new iex2DObj( "DATA/UI/menu-int.png" );

		//	構造体初期化
		ImageInitialize( titleInfo.textImage, 1300, 750, 800, 100, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_X, TITLE_TEXT_SRCPOS_INFO::MENU_SRC_POS_Y, 1024, 128 );
		titleInfo.textImage.renderflag = false;
		
		//	変数初期化
		titleInfo.mode = 0;
		titleInfo.step = 0;
		titleInfo.param = 0.0f;
		titleInfo.t = 0.0f;
		titleInfo.moveState = false;
		titleInfo.savePos = 0;
	}

	//	メイン用初期化
	void	UI::MainInitialize( void )
	{
		timer.obj = new iex2DObj("DATA/BG/number.png");
		coinbar = new iex2DObj("DATA/BG/coin_gage.png");
		gauge.obj = coinbar;
		backgauge.obj = coinbar;
		frame.obj = coinbar;
		face = new iex2DObj("DATA/UI/chara_emotion.png");
		faceImage.obj = face;
		countImage.obj = new iex2DObj("DATA/UI/bfUI.png");
		alertImage.obj = new iex2DObj("DATA/UI/alert.png");

		//	共通変数初期化
		changeflag = false;
		for (int i = 0; i < PLAYER_MAX; i++)
			charatype[i] = gameManager->GetCharacterType(i);

		//	各UI情報初期化
		CoinBarInitialize();
		TimerInitialize();
		StartAndTimeUpInitialize();
		NewsBarInitialize();
		DonketsuDirectionInitialize();
		AlertInitialize();
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
		SafeDelete( face );
		SafeDelete( countImage.obj );
		SafeDelete( alertImage.obj );
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
			titleInfo.textImage.y = TITLE_TEXT_POS_INFO::IN_END_POS_Y + static_cast<int>( 10.0f * sinf( titleInfo.t ) );
			titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - TITLE_TEXT_POS_INFO::ROLL_POINT_ADJUST_X, titleInfo.textImage.y  );
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
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartUpdate();
			break;

		case GAME_MODE::MAINGAME:
			TimerUpdate();
			NewsBarUpdate();
			CoinBarUpdate();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			DonketsuDirectionUpdate();
			break;

		case GAME_MODE::CLIMAX:
			TimerUpdate();
			NewsBarUpdate();
			CoinBarUpdate();
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
		switch ( mode )
		{
		case GAME_MODE::GAMESTART:
			StartRender();
			break;

		case GAME_MODE::MAINGAME:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			break;

		case GAME_MODE::DONKETSU_DIRECTION:
			TimerRender();
			NewsBarRender();
			CoinBarRender();
			DonketsuDirectionRender();
			break;

		case GAME_MODE::CLIMAX:
			//TimerRender();
			LastProductionRender();
			NewsBarRender();
			CoinBarRender();

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
		Lerp( titleInfo.textImage.y, TITLE_TEXT_POS_INFO::IN_START_POS_Y, TITLE_TEXT_POS_INFO::IN_END_POS_Y, titleInfo.param );

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, titleInfo.textImage.t );

		//	左移動
		Lerp(titleInfo.textImage.x, TITLE_TEXT_POS_INFO::IN_START_POS_X, TITLE_TEXT_POS_INFO::IN_END_POS_X, titleInfo.param);

		//	回転
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - TITLE_TEXT_POS_INFO::ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
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
		Lerp( titleInfo.textImage.y, startPos, ( int )TITLE_TEXT_POS_INFO::OUT_END_POS_Y, titleInfo.param );

		//	パラメータ調整
		titleInfo.param = GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv3, titleInfo.textImage.t );

		//	左移動
		Lerp( titleInfo.textImage.x, TITLE_TEXT_POS_INFO::OUT_START_POS_X, TITLE_TEXT_POS_INFO::OUT_END_POS_X, titleInfo.param );

		//	回転
		titleInfo.textImage.p = GetPoint( titleInfo.textImage.x - TITLE_TEXT_POS_INFO::ROLL_POINT_ADJUST_X, titleInfo.textImage.y );
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
		ImageInitialize(frame, (1280 / 2), iexSystem::ScreenHeight - 50, iexSystem::ScreenWidth - 300, 80, 0, 32 * 5, 512, 64);
		ImageInitialize(gauge, frame.x, frame.y, 0, 32, 0, 32, 0, 32);
		ImageInitialize(backgauge, frame.x, frame.y, frame.w - (frame.w / 10), frame.h - (frame.h / 2), 0, 32 * 4, 480, 32);
		ImageInitialize(faceImage, 0, frame.y - (frame.h / 2), 32, 32, 256, 256, 256, 256);
		//frame_x = ( 1280 / 2 ) - ( 512 / 2 );
		//frame_y = 600;
		//frame_sx = 512;
		//frame_sy = 64;
		for (int i = 0; i < NUM_BAR; i++)
		{
			bar_x[i] = frame.x;
			bar_sx[i] = backgauge.sx;
			state_x[i] = 0;
		}
	}

	//	タイマー初期化
	void	UI::TimerInitialize( void )
	{

		//	構造体初期化
		ImageInitialize(timer, 512, 30, 64, 64, 64, 0, 64, 64);
		for ( int i = 0; i < 2; i++ )
		{
			minute[i] = 0;
		}
	}

	//	ニュースバー初期化
	void	UI::NewsBarInitialize( void )
	{
		//	ニュースバー初期化
		newsbar.renderflag = false;
		newsbar.left = 1280;
		newsbar.top = 0;
		newsbar.right = 1280;
		newsbar.bottom = 50;
		newsbar.text = GameInfo::NewsText[ gameManager->GetLastBonus() ];
		newsbar.alpha = 0.5f;
		newsbar.color = Vector3( 0.3f, 0.3f, 0.3f );
		newsbar.step = 0;
		newsbar.textleft = 1500;
	}

	//	カウントダウン・スタート・終了演出
	void	UI::StartAndTimeUpInitialize( void )
	{
		ImageInitialize( countImage, 640, 360, 350, 350, 0, 0, 256, 256 );
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
		ImageInitialize(ddInfo.face, 1280 / 2, 720 / 2, 0, 0, FACE_INFO::Normal * 256, 0 * 256, 256, 256);
		ddInfo.f = 0;
		ddInfo.roulette = 0;
		ddInfo.face_step = -1;

		//　DonketsuBoooooooooooost!!!
		ddInfo.DB.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		ImageInitialize(ddInfo.DB, 1200, -100, 0, 0, 0, 0, 512, 256);
		ddInfo.DB.angle = 0;
		ddInfo.DB_step = -1;

		//　？P関連
		ddInfo.P.obj = new iex2DObj("DATA/UI/DonketuUI.png");
		ImageInitialize(ddInfo.P, 1000, 550, 0, 0, 0, 256, 128, 128);
		ddInfo.P.renderflag = false;
		ddInfo.P_step = -1;
	}

	//	警告演出初期化
	void	UI::AlertInitialize( void )
	{
		alertInfo.flag = false;
		alertInfo.alpha = 0.0f;
		alertInfo.timer = 0;
		alertInfo.param = 0.0f;

		//	画像構造体初期化
		ImageInitialize( alertImage, 640, 360, 200, 200, 0, 0, 256, 256 );
		alertImage.renderflag = true;
	}

	//	HurryUp演出初期化
	void	UI::HurryUpInitialize( void )
	{
		hurryInfo.alpha = 0.0f;
		hurryInfo.flag = false;
		hurryInfo.param = 0.0f;
		hurryInfo.timer = 0;
	}
	
//------------------------------------------------------------------------------
//	メイン動作更新
//------------------------------------------------------------------------------

	//	タイマー関連動作
	void	UI::TimerUpdate( void )
	{
		this->time = gameManager->GetTimer();
		//分
		second = this->time / MINUTE % 10;
		//秒二桁目
		minute[0] = ( this->time / SECOND ) % 60 / 10 % 10;
		//秒一桁目
		minute[1] = this->time / SECOND % 10;
	}

	//	ニュースバー関連動作
	void	UI::NewsBarUpdate( void )
	{
		if ( !gameManager->GetNewsFlag() )	return;

		switch ( newsbar.step )
		{
		case 0:
			//	バー出現
			newsbar.left -= 30;
			if ( newsbar.left <= 0 )
			{
				newsbar.left = 0;
				newsbar.step++;
			}
			break;

		case 1:
			//	テキスト出現
			newsbar.textleft--;
			if ( newsbar.textleft <= -320 )
			{
				newsbar.textleft = 1500;
				newsbar.step++;
			}
			break;

		case 2:
			//	バー退避
			newsbar.right -= 30;
			if ( newsbar.right <= 0 )
			{
				newsbar.right = 0;
				newsbar.step++;
			}
			break;

		case 3:
			//	初期化
			gameManager->SetNewsFlag( false );
			newsbar.left = 1280;
			newsbar.right = 1280;
			newsbar.textleft = 1500;
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
				countImage.w = 750;
				countImage.h = 750;
				SetWave( countImage, 1.5f );
				break;

			case 4:
				waitTimer = 2 * SECOND;
				
				//	画像読み込み位置・サイズ設定			
				countImage.w = 600;
				countImage.h = 370;
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
			・「どんけつは〜？」を最初にバンッ!
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

		//　更新
		wait--;
	}

	//	警告演出
	void	UI::AlertUpdate( void )
	{
		alertInfo.param += D3DX_PI / 30.0f;
		alertInfo.alpha = 0.1f + 0.1f * sinf( alertInfo.param );

		alertInfo.timer++;
		if ( alertInfo.timer % 15 == 0 )	alertImage.renderflag = !alertImage.renderflag;

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
		FlashingUpdate(timer, 0.5f);
	}

//------------------------------------------------------------------------------
//	メイン描画
//------------------------------------------------------------------------------

	//	コインバー描画
	void	UI::CoinBarRender( void )
	{
		//フレーム
		RenderImage(frame, frame.sx, frame.sy, frame.sw, frame.sh, IMAGE_MODE::NORMAL);
		//coinbar->Render(frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy);

		//灰色のバー
		//	RenderImage(backgauge, backgauge.sx, backgauge.sy, backgauge.sw, backgauge.sh, IMAGE_MODE::NORMAL);
		//coinbar->Render(bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32);

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			//色のバー
			gauge.w = bar_sx[i];	gauge.sw = bar_sx[i];
			//	（左上位置 - ゲージ幅の半分）で中心、ゲージを右向きへ増やすため
			RenderImage(gauge, gauge.sx, gauge.sy * i, gauge.sw, gauge.sh, IMAGE_MODE::NORMAL, bar_x[i] - (backgauge.w / 2) + (gauge.sw / 2), gauge.y);
			//	coinbar->Render(bar_x[i], bar_y[i], bar_sx[i], 32, 0, 32 * i, bar_sx[i], bar_sy[i]);
			//																										
			//顔
			RenderImage(faceImage, faceImage.sx * state_type[i], faceImage.sy * charatype[i], faceImage.sw, faceImage.sh, IMAGE_MODE::NORMAL, state_x[i] - (backgauge.w / 2), faceImage.y);
			//face->Render(state_x[i], 550, 32, 32, state_type[i] * 256, charatype[i] * 256, 256, 256);
		}
	}

	//	ニュース描画
	void	UI::NewsBarRender( void )
	{
		if( !gameManager->GetNewsFlag() )	return;
		
		iexPolygon::Rect( newsbar.left, newsbar.top, newsbar.right - newsbar.left, newsbar.bottom - newsbar.top, RS_COPY, GetColor( newsbar.color, newsbar.alpha ) );
		IEX_DrawText( newsbar.text, newsbar.textleft, newsbar.top + 10, 500, 200, 0xFFFFFFFF );
	}

	//	タイマー描画
	void	UI::TimerRender( void )
	{
		
		RenderImage( timer, timer.sx * second		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 0, timer.y );
		RenderImage( timer, timer.sx * 10			, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 1, timer.y  );
		RenderImage( timer, timer.sx * minute[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 2, timer.y  );
		RenderImage( timer, timer.sx * minute[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::NORMAL, timer.x + timer.w * 3, timer.y  );

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
		iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, color );

		//　顔ルーレット
		RenderImage(ddInfo.face, FACE_INFO::Normal * 256, charatype[ddInfo.f] * 256, 256, 256, IMAGE_MODE::NORMAL);
		
		//　DonketsuBoooooooooooost!!!
		RenderImage(ddInfo.DB, 0, 0, 512, 256, IMAGE_MODE::ADOPTPARAM );
	
		//　？P
		RenderImage(ddInfo.P, gameManager->GetWorst()*128, 256, 128, 128, IMAGE_MODE::NORMAL);

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
		//	赤フィルター描画
		iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, GetColor( 1.0f, 0.0f, 0.0f, alertInfo.alpha ) );

		//	警告画像描画
		RenderImage( alertImage, 0, 0, 256, 256, IMAGE_MODE::NORMAL );
	}

	//	時間警告描画
	void	UI::LastProductionRender( void )
	{
		//	タイマー文字色を赤へ
		timer.sy = 64;

		RenderImage(timer, timer.sx * second	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 0, timer.y);
		RenderImage(timer, timer.sx * 10		, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 1, timer.y);
		RenderImage(timer, timer.sx * minute[0]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 2, timer.y);
		RenderImage(timer, timer.sx * minute[1]	, timer.sy, timer.sw, timer.sh, IMAGE_MODE::FLASH, timer.x + timer.w * 3, timer.y);

		//	タイマー文字色を白へ
		timer.sy = 0;

	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	バー動作
	void	UI::BarControl( void )
	{
		static const int MAX_COIN = 201;

		bar_x[1] = bar_x[0] + bar_sx[0];
		bar_x[2] = bar_x[1] + bar_sx[1];
		bar_x[3] = bar_x[2] + bar_sx[2];

		int num_coin[4];
		for (int i = 0; i < 4; i++)
		{
			num_coin[i] = gameManager->GetCoinNum(i);
			bar_sx[i] = backgauge.w * num_coin[i] / MAX_COIN;
		}
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
		const int POS_X = 1280 / 4;		//　固定位置
		const int POS_Y = 720 / 4;		//　固定位置
		const int TRANS_X = (POS_X - 1200) / 30;	//　TPF
		const int TRANS_Y = (POS_Y - -100) / 30;	//　TPF
		const int SIZE_X = 600;
		const int SIZE_Y = 300;
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
			ddInfo.DB.angle = 0;
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
			ddInfo.face.w = ddInfo.face.h = 320;
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
		}

		//　演出
		switch (ddInfo.face_step)
		{
			//　１〜４Ｐ拡大
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
			ddInfo.face.w += 500/INTERVAL;
			ddInfo.face.h += 500/INTERVAL;
			break;

			//　放置
		case 7:
			ddInfo.face.renderflag = true;
			ddInfo.face.w = ddInfo.face.h = 500;
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
		const int SIZE_X = 400;
		const int SIZE_Y = 350;
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

//------------------------------------------------------------------------------
//	情報設定・取得
//------------------------------------------------------------------------------
	
	//	モード変更フラグ設定
	void	UI::SetChangeFlag( const bool& flag )
	{
		this->changeflag = flag;
	}

	//	警告フラグ設定
	void	UI::SetAlertFlag( bool flag )
	{
		alertInfo.flag = flag;
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

	

