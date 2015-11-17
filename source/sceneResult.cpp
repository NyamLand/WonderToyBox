
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
//	sceneResultクラス
//
//********************************************************************************

//----------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------

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
				RESTART,						//	再戦
				MOVE_MENU,					//	対戦設定へ	
				MOVE_TITLE,					//	タイトルへ
				END,								//	終端
			};
		}
	}

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------
	
	//	コンストラクタ
	sceneResult::sceneResult( void )
	{
	
	}

	//	デストラクタ
	sceneResult::~sceneResult( void )
	{
		Release();
	}

	//	初期化
	bool	sceneResult::Initialize( void )
	{
		//	乱数初期化
		Random::Initialize();

		//	カメラ設定
		CameraInitialize();

		//	画像読み込み
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		menuHead.obj = new iex2DObj( "DATA/UI/menu-head.png" );
		originNumber = new iex2DObj( "DATA/UI/number.png" );
		menuText = new iex2DObj( "DATA/Result/result-cho.png" );

		//	モデル読み込み
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );			//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Y2009/Y2009.IEM" ) );					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" ) );				//	リス
		org[CHARACTER_TYPE::TIGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );					//	トラ

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );			//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.02f );			//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale( 0.04f );				//	リス
		org[CHARACTER_TYPE::TIGER]->SetScale( 0.02f );				//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );			//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );			//	シーフ
		org[CHARACTER_TYPE::TIGER]->SetAngle( D3DX_PI );			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );				//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );				//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );	 				//	シーフ
		org[CHARACTER_TYPE::TIGER]->SetMotion( 0 );					//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->Update();					//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	姫
		org[CHARACTER_TYPE::THIEF]->Update();						//	リス
		org[CHARACTER_TYPE::TIGER]->Update();						//	トラ

		//	モデル初期化
		ModelInitialize();

		//	構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth / 2 );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );
		menuHead.angle = D3DXToRadian( 7.0f );
		
		//	数値構造体初期化
		FOR( 0, PLAYER_MAX )
		{
			number[value].hundred = 0;
			number[value].ten = 0;
			number[value].one = 0;
		}

		//	変数初期化
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;

		//	ラストボーナス設定
		SetLastBonus();

		//	結果用情報構造体初期化
		ResultInfoInitialize();

		//	数値構造体初期化
		NumberImageInfoInitialize();

		//	ソートかける
		Sort();

		//	ランキング設定
		SetRank();
		
		//	順位画像構造体初期化
		RankImageInitialize();

		//	ルーレット情報初期化
		{
			rouletteInfo.step = 0;
			rouletteInfo.timer = 0;
		}

		//	メニュー情報初期化
		{
			menuInfo.select = MENU::RESTART;
			menuInfo.screenH = 0;
			menuInfo.alpha = 0.5f;
			menuInfo.t = 0.0f;
		}

		//	メニュー画像構造体初期化
		{
			//	メニューの項目数まわす
			for ( int i = 0; i < 3; i++ )
			{
				x = static_cast<int>( iexSystem::ScreenWidth / 2 );
				y = static_cast<int>( iexSystem::ScreenHeight * 0.28f );
				w = static_cast<int>( iexSystem::ScreenWidth * 0.55f );
				h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
				ImageInitialize( menuImage[i], x, y + y * i, w, h, 0, 128 * i, 512, 128 );
				menuImage[i].obj = menuText;
				menuImage[i].renderflag = false;
			}
		}

		//	ランク表示用構造体初期化
		{
			viewRankInOrder.timer = 0;
			viewRankInOrder.step = 3;
		}
		return	true;
	}

	//	解放
	void	sceneResult::Release( void )
	{
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		Random::Release();
	}

//----------------------------------------------------------------------------
//	各種情報初期化
//----------------------------------------------------------------------------

	//	カメラ初期化
	void	sceneResult::CameraInitialize( void )
	{
		mainView = new Camera();
		mainView->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		mainView->Set( Vector3( 0.0f, 5.2f, -70.0f ), Vector3( 0.0f, 5.2f, 0.0f ) );
		mainView->Activate();
	}

	//	モデル初期化
	void	sceneResult::ModelInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	モデル情報設定
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 0.0f, 0.0f );
			obj[i]->Update();
		}
	}

	//	結果用構造体関連初期化
	void	sceneResult::ResultInfoInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	ゲーム終了時のデータを格納( ここでボーナスも設定しておく )
			originInfo[i].coin = gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	ランキング計算用に総計データを格納( ボーナス数値が整い次第、元のコイン枚数にボーナスを足す、ランクはソートにかけるため適当に代入 )
			sortInfo[i].coin = originInfo[i].coin;
			sortInfo[i].rank = i;
		}
	}

	//	数値構造体初期化
	void	sceneResult::NumberImageInfoInitialize( void )
	{	
		Vector3	out;

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	クライアント座標に変換
			WorldToClient( obj[i]->GetPos(), out, matView * matProjection );

			//	中心座標・初期スケール設定
			numberImageInfo[i].pos.x = static_cast<int>( out.x );
			numberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.38f );
			numberImageInfo[i].scale = 100;

			//	各位画像設定
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;

			//	数値画像構造体初期化
			SetNumberImageInfo( i, originInfo[i].coin );
		}
	}

	//	順位画像構造体初期化
	void	sceneResult::RankImageInitialize( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			Vector3	out;

			//	クライアント座標に変換
			WorldToClient( obj[i]->GetPos(), out, matView * matProjection );

			//	構造体初期化
			int 	x = static_cast<int>( out.x );
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
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	sceneResult::Update( void ) 
	{
		//	各モデル更新
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

		//	シーン移動管理
		MoveScene();
	}

	//	描画
	void	sceneResult::Render( void ) 
	{
		mainView->Activate();
		mainView->Clear();

		//	背景描画
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	リザルトシール描画
		RenderImage( menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM );

		//	プレイヤー描画
		FOR( 0, PLAYER_MAX )
		{
			//	プレイヤー描画
			obj[value]->Render( shader3D, "toon" );
		}

		//	数値描画
		NumberImageRender();

		//	順位描画
		RankRender();

		//	メニュー用スクリーン描画
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		//	メニュー項目描画
		SelectRender();
	}

	//	リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false;

		switch ( step )
		{
		case 0:		//	ルーレット
			isEnd = Roulette();

			//	決定ボタンでスキップ
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				for ( int i = 0; i < PLAYER_MAX; i++ )
				{
					SetNumberImageInfo( i, originInfo[i].coin );
					SetWave( rankImage[i], 1.5f );
					rankImage[i].renderflag = true;
				}
				
				step = 2;
			}

			//	回転が終了したら
			if ( isEnd )		step++;
			break;
			
		case 1:		//	順位描画
			isFinViewRankInOrder = ViewRankInOrder();
			isEnd = RankWave();

			//	決定ボタンでスキップ
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				for ( int i = 0; i < PLAYER_MAX; i++ )
				{
					SetNumberImageInfo( i, originInfo[i].coin );
					SetWave( rankImage[i], 1.5f );
					rankImage[i].renderflag = true;
				}

				step = 2;
			}

			//	波紋終了後に選択可
			if ( isFinViewRankInOrder && isEnd )
			{
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
				{
					step = 0;
					mode = MOVE_MODE::SELECT;
				}
			}
			break;

		case 2:	//	順位描画(スキップ用)
			for ( int i = 0; i < PLAYER_MAX; i++ )		isEnd = WaveUpdate( rankImage[i] );

			//	波紋終了後に選択可
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

	//	セレクト時の更新
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
				
				//	メニュー項目描画
				for ( int i = 0; i < 3; i++ )		menuImage[i].renderflag = true;
			}
			isEnd = Lerp( menuInfo.screenH, 0, static_cast<int>( iexSystem::ScreenHeight ), menuInfo.t );

			if ( isEnd )step++;
			break;

		case 1:
			//	上下で選択
			if ( input[0]->Get( KEY_UP ) == 3 )		menuInfo.select--;
			if ( input[0]->Get( KEY_DOWN ) == 3 )	menuInfo.select++;
			if ( menuInfo.select < 0 )						menuInfo.select = MENU::END - 1;
			if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::RESTART;
			
			//	読み込み位置変更
			for ( int i = 0; i < 3; i++ )
			{
				if ( i == menuInfo.select )	menuImage[i].sx = 512;
				else									menuImage[i].sx = 0;
			}

			//	決定
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				changeScene = true;
			}
			break;
		}
	}

	//セレクト画面描画
	void	sceneResult::SelectRender( void )
	{
		for ( int i = 0; i < 3; i++ )
		{
			RenderImage( menuImage[i], menuImage[i].sx, menuImage[i].sy, menuImage[i].sw, menuImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	数値画像構造体描画
	void	sceneResult::NumberImageRender( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	１００の位描画
			int		sx = numberImageInfo[i].hundred.sx;
			int		sy = numberImageInfo[i].hundred.sy;
			int		sw = numberImageInfo[i].hundred.sw;
			int		sh = numberImageInfo[i].hundred.sh;

			if ( numberImageInfo[i].hundredRenderFlag )
				RenderImage( numberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	１０の位描画
			sx = numberImageInfo[i].ten.sx;
			sy = numberImageInfo[i].ten.sy;
			sw = numberImageInfo[i].ten.sw;
			sh = numberImageInfo[i].ten.sh;
			RenderImage( numberImageInfo[i].ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	１の位描画
			sx = numberImageInfo[i].one.sx;
			sy = numberImageInfo[i].one.sy;
			sw = numberImageInfo[i].one.sw;
			sh = numberImageInfo[i].one.sh;
			RenderImage( numberImageInfo[i].one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
		}
	}

	//	ランク描画
	void	sceneResult::RankRender( void )
	{
		//	ランク描画
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
//	動作関数
//----------------------------------------------------------------------------

	//	ソート
	void	sceneResult::Sort( void )
	{
		//	退避用
		SORT_INFO temp;

		for ( int i = 0; i < PLAYER_MAX; ++i )
		{
			//	後ろから順番にチェックしていく
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	一つ下の要素と比較
				if ( sortInfo[s].coin >	sortInfo[s - 1].coin )
				{
					//	一時的に退避
					temp = sortInfo[s - 1];

					//	交換
					sortInfo[s - 1] = sortInfo[s];

					//	退避してたやつを戻す
					sortInfo[s] = temp;
				}
			}
		}
	}

	//	ラストボーナス設定
	void	sceneResult::SetLastBonus( void )
	{
		if ( Random::PercentageRandom( 0.3f ) )
		{
			//	ラストボーナスを設定
			lastBonus = gameManager->GetLastBonus();
		}
		else
		{
			//	違う結果が出るまでループ
			while ( lastBonus == gameManager->GetLastBonus() )
			{
				lastBonus = Random::GetInt( 0, 4 );
			}
		}
	}

	//	数値構造体に値をセットする
	void	sceneResult::SetNumberInfo( NUMBER_INFO& number, int coin )
	{
		number.hundred = coin / 100 % 10;
		number.ten = coin / 10 % 10;
		number.one = coin % 10;
	}

	//	ソートかけた順位を元のデータに反映
	void	sceneResult::SetRank( void )
	{
		//	ソートの結果を反映
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				if ( i == sortInfo[n].rank )
				{
					originInfo[i].rank = n;
				}
			}
		}
	}

	//	数値にあわせて数値構造体情報を設定,１００以上かで配置も変更
	void	sceneResult::SetNumberImageInfo( const int& player, const int& coin )
	{
		//	桁数確認
		if ( coin >= 100 )		numberImageInfo[player].hundredRenderFlag = true;
		else							numberImageInfo[player].hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( number[player], coin );

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		if ( numberImageInfo[player].hundredRenderFlag )
		{
			//	１０の位設定
			x = numberImageInfo[player].pos.x;
			y = numberImageInfo[player].pos.y;
			w = h = numberImageInfo[player].scale;
			sx = number[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定
			x = numberImageInfo[player].pos.x - static_cast<int>( numberImageInfo[player].ten.w / 1.5f );
			sx = number[player].hundred * 64;
			ImageInitialize( numberImageInfo[player].hundred, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numberImageInfo[player].pos.x + static_cast<int>( numberImageInfo[player].ten.w / 1.5f );
			sx = number[player].one * 64;
			ImageInitialize( numberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	１０の位設定
			w = h = numberImageInfo[player].scale;
			x = numberImageInfo[player].pos.x - w / 3;
			y = numberImageInfo[player].pos.y;
			sx = number[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numberImageInfo[player].pos.x + w / 3;
			sx = number[player].one * 64;
			ImageInitialize( numberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
	}

	//	数値ルーレット( 終了するとtrueをかえす )
	bool	sceneResult::Roulette( void )
	{
		//	４人分確定してたらtrueをかえす
		if ( rouletteInfo.step > PLAYER_MAX )
		{
			return	true;
		}

		rouletteInfo.timer++;

		for ( int i = rouletteInfo.step; i < PLAYER_MAX; i++ )
		{
			//	適当な数値を入れる
			SetNumberImageInfo( i, Random::GetInt( 100, 999 ) );
		}

		//	初回だけ回転時間ちょっと長く
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

		//	数値決定
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( i < rouletteInfo.step )
			{
				SetNumberImageInfo( i, originInfo[i].coin );
			}
		}

		return	false;
	}

	//	順位を順番に表示
	bool	sceneResult::ViewRankInOrder( void )
	{	
		//	４人分確定してたらtrueをかえす
		if ( viewRankInOrder.step == -1 )
		{
			return	true;
		}

		viewRankInOrder.timer++;

		if ( viewRankInOrder.timer % 30 == 0 )
		{
			//	ランク描画設定
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

	//	順位の波紋の更新と終了チェック
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

	//	シーン移動フラグが立ったら選択中の項目に合ったシーンに移動する（ この関数は絶対に更新の最後に呼ぶ ）
	void	sceneResult::MoveScene( void )
	{
		if ( !changeScene )	return;

		switch ( menuInfo.select )
		{
		case MENU::RESTART:
			//	ゲーム情報初期化
			gameManager->RetryInitialize();
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