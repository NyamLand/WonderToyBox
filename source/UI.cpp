
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
		timer = new iex2DObj("DATA/BG/number.png");
		coinbar = new iex2DObj("DATA/BG/coin_gage.png");
		face = new iex2DObj("DATA/UI/chara_emotion.png");
		countImage.obj = new iex2DObj("DATA/UI/bfUI.png");
		alertImage.obj = new iex2DObj("DATA/UI/alert.png");

		//	共通変数初期化
		changeflag = false;

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

	//	メイン用初期化
	void	UI::MainRelease( void )
	{
		SafeDelete( timer );
		SafeDelete( coinbar );
		SafeDelete( face );
		SafeDelete( countImage.obj );
		SafeDelete( alertImage.obj );
	}

	//	リザルト用初期化
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
			TimerRender();
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
		frame_x = ( 1280 / 2 ) - ( 512 / 2 );
		frame_y = 600;
		frame_sx = 512;
		frame_sy = 64;
		for ( int i = 0; i < NUM_BAR; i++ )
		{
			bar_x[i] = frame_x + 16;
			bar_y[i] = frame_y + 16;
			bar_sx[i] = 480;
			bar_sy[i] = 32;
			state_x[i] = 0;
		}
	}

	//	タイマー初期化
	void	UI::TimerInitialize( void )
	{
		x = 200;
		y = 0;
		sx = 64;
		sy = 64;
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
		//　キャラ種類
		for ( int i = 0; i < 4; i++ )
		{
			charatype[i] = gameManager->GetCharacterType( i );
		}
		roulette = 0;
		f = 0;
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
		//　演出用時間更新
		static int wait( 5 * SECOND );
		if ( wait <= 0 )
		{
			//wait = 30;
			wait = 5 * SECOND;
			changeflag = true;
		}

		//　顔ルーレット
		int 	step;
		if (wait <= 5 * SECOND)			step = 0;	//　イントロ
		if (wait <= 4 * SECOND)			step = 1;	//　ルーレット
		if (wait <= 2 * SECOND + 30)	step = 2;	//　決定

		switch ( step )
		{
		case 0:
			break;
		case 1:
			f = roulette % 4;
			roulette++;
			break;
		case 2:
			f = gameManager->GetWorst();
			break;
		}

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

//------------------------------------------------------------------------------
//	メイン描画
//------------------------------------------------------------------------------

	//	コインバー描画
	void	UI::CoinBarRender( void )
	{
		//フレーム
		coinbar->Render(frame_x, frame_y, 512, 64, 0, 32 * 5, frame_sx, frame_sy);

		//灰色のバー
		coinbar->Render(bar_x[0], bar_y[0], 480, 32, 0, 32 * 4, 480, 32);

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			//色のバー
			coinbar->Render(bar_x[i], bar_y[i], bar_sx[i], 32, 0, 32 * i, bar_sx[i], bar_sy[i]);

			//顔
			face->Render(state_x[i], 550, 32, 32, state_type[i] * 256, charatype[i] * 256, 256, 256);
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
		timer->Render( x + 64 * 0, y, 64, 64, second * 64, 0, sx, sy );
		timer->Render( x + 64 * 1, y, 64, 64, 10 * 64, 0, sx, sy );
		timer->Render( x + 64 * 2, y, 64, 64, minute[0] * 64, 0, sx, sy );
		timer->Render( x + 64 * 3, y, 64, 64, minute[1] * 64, 0, sx, sy );

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
		face->Render( 480, 200, 320, 320, FACE_INFO::Normal * 256, charatype[f] * 256, 256, 256 );

		char	str[256];
		int		worst = gameManager->GetWorst();
		DrawString( "どんけつ演出", 200, 50 );
		wsprintf( str, "ビリは p%d", worst + 1 );
		DrawString( str, 200, 70 );
	}

	//	警告描画
	void	UI::AlertRender( void )
	{
		//	赤フィルター描画
		iexPolygon::Rect( 0, 0, 1280, 720, RS_COPY, GetColor( 1.0f, 0.0f, 0.0f, alertInfo.alpha ) );

		//	警告画像描画
		RenderImage( alertImage, 0, 0, 256, 256, IMAGE_MODE::NORMAL );
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
		for ( int i = 0; i < 4; i++ )
		{
			num_coin[i] = gameManager->GetCoinNum( i );
			bar_sx[i] = 480 * num_coin[i] / MAX_COIN;
		}
	}

	//	顔動作
	void	UI::StateImageControl( void )
	{
		//画像の描画場所 = 各色の先頭　+　各色の中心　-　画像サイズの半分
		state_x[0] = (bar_x[0] + (bar_x[1] - bar_x[0]) / 2) - 16;
		state_x[1] = (bar_x[1] + (bar_x[2] - bar_x[1]) / 2) - 16;
		state_x[2] = (bar_x[2] + (bar_x[3] - bar_x[2]) / 2) - 16;
		state_x[3] = (bar_x[3] + (bar_x[3] + bar_sx[3] - bar_x[3]) / 2) - 16;


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

		for ( int i = 0; i < 4; i++ )
		{
			for ( int j = 0; j < 4; j++ )
			{
				if ( num_coin[i] == temp_coin[j] )
				{
					switch ( j )
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

	

