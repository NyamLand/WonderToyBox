
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
				RESTART,						//	再戦
				MOVE_MENU,					//	対戦設定へ	
				MOVE_TITLE,					//	タイトルへ
				END,								//	終端
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
		menuHead.obj = new iex2DObj( "DATA/UI/menu/menu-head.png" );
		originNumber = new iex2DObj( "DATA/UI/number.png" );
		menuText = new iex2DObj( "DATA/UI/result/result-cho.png" );
		lastBonusText = new iex2DObj( "DATA/UI/Result/LastBonusText.png" );

		//	モデル読み込み
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Knight/Knight_Dammy.IEM" ) );			//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/プリンセス/prinsess1.IEM" ) );					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/Thief/Thief.IEM" ) );				//	リス
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>( LPSTR( "DATA/CHR/ECCMAN/ECCMAN.IEM" ) );					//	トラ

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale( 0.05f );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetScale( 0.04f );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale( 0.03f );				//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetScale( 0.02f );				//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle( D3DX_PI );	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetAngle( D3DX_PI );		//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle( D3DX_PI );				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetAngle( D3DX_PI );			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion( 2 );		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion( 1 );			//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion( 0 );	 				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	姫
		org[CHARACTER_TYPE::THIEF]->Update();							//	シーフ
		org[CHARACTER_TYPE::PIRATE]->Update();						//	トラ

		//	モデル初期化
		ModelInitialize();

		//	構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
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
		bonusPlayer = 0;

		//	結果用情報構造体初期化
		ResultInfoInitialize();

		//	ソートかける
		//Sort( maxCoinNum );
		//Sort( fallStageNum );
		//ReverseSort( coin77 );
		//ReverseSort( minCoinNum );
		//Sort( hitAttackNum );

		////	ラストボーナス設定
		//SetLastBonus();
		//AddLastBonus();
		Sort( sortInfo );
		
		//	ランキング設定
		SetRank();

		//	次回ライフ設定
		SetNextLife();

		//	数値構造体初期化
		NumberImageInfoInitialize();
		
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
				x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
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

		//	ラストボーナス関連初期化
		{
			FOR( 0, PLAYER_MAX )	lastBonusInfo.bonus[value];
			lastBonusInfo.step = 0;
			lastBonusInfo.t = 0.0f;
			
			//	テキスト初期化
			int		x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
			int		y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
			int		w =static_cast<int>( iexSystem::ScreenWidth * 0.2f );
			int		h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
			ImageInitialize( lastBonusInfo.textImage, x, y, w, h, 0, lastBonus * 128, 512, 128 );
			lastBonusInfo.textImage.renderflag = false;
			lastBonusInfo.textImage.obj = lastBonusText;
			
			//	頂点設定
			SetVertex( lastBonusInfo.v[0], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.2f ), 0.0f, 0.0f, 0.0f, 0x77333333 );
			SetVertex( lastBonusInfo.v[1], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.2f ), 0.0f, 1.0f, 0.0f, 0x77333333 );
			SetVertex( lastBonusInfo.v[2], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.6f ), 0.0f, 0.0f, 1.0f, 0x77333333 );
			SetVertex( lastBonusInfo.v[3], static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight * 0.6f ), 0.0f, 1.0f, 1.0f, 0x77333333 );

			//	顔画像初期化
			y = static_cast<int>( iexSystem::ScreenHeight * 0.6f );
			w = 0;
			h = 0;
			ImageInitialize( faceImage, x, y, w, h, 0, 256 * bonusPlayer, 256, 256 );
			faceImage.obj = new iex2DObj( "DATA/UI/chara_emotion.png" );
			faceImage.renderflag = false;

			//	虹円画像初期化
			w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			h = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			ImageInitialize( waveCircleImage, x, y, w, h, 0, 0, 512, 512 );
			waveCircleImage.obj = new iex2DObj( "DATA/UI/Rainbow-circle.png" );
			waveCircleImage.renderflag = false;
			
			//	該当なし画像初期化
			w = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
			h = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
			ImageInitialize( notApplicable, x, y, w, h, 0, 0, 512, 256 );
			notApplicable.obj = new iex2DObj( "DATA/UI/Result/resultText.png" );
			notApplicable.renderflag = false;
			notApplicable.alpha = 0.0f;
			notApplicable.angle = -D3DX_PI * 0.035f;
		
			//	プレイヤー番号初期化
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

	//	解放
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
			originInfo[i].num = gameManager->GetCoinTotal( i ) + gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	ランキング計算用に総計データを格納( ボーナス数値が整い次第、元のコイン枚数にボーナスを足す、ランクはソートにかけるため適当に代入 )
			sortInfo[i].num = originInfo[i].num;
			sortInfo[i].rank = i;
			sortInfo[i].sortRank = i;

			//	ラストボーナスの情報も設定
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
			bonusNumberImageInfo[i].pos.x = static_cast<int>( out.x + ( iexSystem::ScreenWidth * 0.05f ) );
			bonusNumberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.45f );
			bonusNumberImageInfo[i].scale = 70;

			//	各位画像設定
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;
			bonusNumberImageInfo[i].one.obj = originNumber;
			bonusNumberImageInfo[i].ten.obj = originNumber;
			bonusNumberImageInfo[i].hundred.obj = originNumber;
		
			//	数値画像構造体初期化
			SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
			SetNumberImageInfo( bonusNumberImageInfo[i], bonusNumber[i], originInfo[i].bonus );
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

		//	ラストボーナス用ポリゴン描画
		iexPolygon::Render2D( lastBonusInfo.v, 2, nullptr, RS_COPY );

		//	ラストボーナステキスト描画
		int	sx = lastBonusInfo.textImage.sx;
		int	sy = lastBonusInfo.textImage.sy;
		int	sw = lastBonusInfo.textImage.sw;
		int	sh = lastBonusInfo.textImage.sh;
		RenderImage( lastBonusInfo.textImage, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
		RenderImage( lastBonusInfo.textImage, sx, sy, sw, sh, IMAGE_MODE::WAVE );

		//	円虹描画
		RenderImage( waveCircleImage, 0, 0, 512, 512, IMAGE_MODE::WAVE );

		//	プレイヤー顔描画
		sx = faceImage.sx;
		sy = faceImage.sy;
		sw = faceImage.sw;
		sh = faceImage.sh;
		RenderImage( faceImage, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

		//	該当なし文字描画
		sx = notApplicable.sx;
		sy = notApplicable.sy;
		sw = notApplicable.sw;
		sh = notApplicable.sh;
		RenderImage( notApplicable, sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );

		//	プレイヤー番号描画
		sx = playerNumImage.sx;
		sy = playerNumImage.sy;
		sw = playerNumImage.sw;
		sh = playerNumImage.sh;
		RenderImage( playerNumImage, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

		//	メニュー用スクリーン描画
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		//	メニュー項目描画
		SelectRender();
	}

	//	中間結果発表
	void	sceneResult::IntermediateResultsUpdate( void )
	{

	}

	//	リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false; 
		bool	isEndWave = false;

		//	段階ごとの処理
		switch ( step )
		{
		case 0:
			//-----------------------------------------------------------------------------------
			//	ルーレット
			//-----------------------------------------------------------------------------------
				isEnd = Roulette();

				//	決定ボタンでスキップ
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

				//	回転が終了したら
				if ( isEnd )		step = 2;
				break;
		
		case 1:
			//-----------------------------------------------------------------------------------
			//	ラストボーナス演出
			//-----------------------------------------------------------------------------------
			isEnd = LastBonusUpdate();
			if ( isEnd )	step++;
			break;
			
		case 2:
			//-----------------------------------------------------------------------------------
			//	順位発表
			//-----------------------------------------------------------------------------------
				isFinViewRankInOrder = ViewRankInOrder();
				isEnd = RankWave();

				//	決定ボタンでスキップ
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

		case 3:
			//-----------------------------------------------------------------------------------
			//	順位発表(スキップ用)
			//-----------------------------------------------------------------------------------
				FOR( 0, PLAYER_MAX )	isEnd = WaveUpdate( rankImage[value] );

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
				else										menuImage[i].sx = 0;
			}

			//	決定
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				changeScene = true;
			}
			break;
		}
	}

	//	セレクト画面描画
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
			//-----------------------------------------------------------------------------------------------
			//	コイン枚数描画
			//-----------------------------------------------------------------------------------------------
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

			//-----------------------------------------------------------------------------------------------
			//	ボーナス数値描画
			//-----------------------------------------------------------------------------------------------
			//	１００の位描画
			sx = bonusNumberImageInfo[i].hundred.sx;
			sy = bonusNumberImageInfo[i].hundred.sy;
			sw = bonusNumberImageInfo[i].hundred.sw;
			sh = bonusNumberImageInfo[i].hundred.sh;

			if ( bonusNumberImageInfo[i].hundredRenderFlag )
				RenderImage( bonusNumberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	１０の位描画
			sx = bonusNumberImageInfo[i].ten.sx;
			sy = bonusNumberImageInfo[i].ten.sy;
			sw = bonusNumberImageInfo[i].ten.sw;
			sh = bonusNumberImageInfo[i].ten.sh;
			RenderImage( bonusNumberImageInfo[i].ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

			//	１の位描画
			sx = bonusNumberImageInfo[i].one.sx;
			sy = bonusNumberImageInfo[i].one.sy;
			sw = bonusNumberImageInfo[i].one.sw;
			sh = bonusNumberImageInfo[i].one.sh;
			RenderImage( bonusNumberImageInfo[i].one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
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
	void	sceneResult::Sort( SORT_INFO ( &sort_info )[4]  )
	{
		//	退避用
		SORT_INFO temp;

		for ( int i = 0; i < PLAYER_MAX; ++i )
		{
			//	後ろから順番にチェックしていく
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	一つ下の要素と比較
				if ( sort_info[s].num >	sort_info[s - 1].num )
				{
					//	一時的に退避
					temp = sort_info[s - 1];

					//	交換
					sort_info[s - 1] = sort_info[s];

					//	退避してたやつを戻す
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

	//	逆向きソート
	void	sceneResult::ReverseSort( SORT_INFO ( &sort_info )[4] )
	{
		//	退避用
		SORT_INFO temp;

		for (int i = 0; i < PLAYER_MAX; ++i)
		{
			//	後ろから順番にチェックしていく
			for ( int s = PLAYER_MAX - 1; s > i; --s )
			{
				//	一つ下の要素と比較
				if ( sort_info[s].num <	sort_info[s - 1].num ) 
				{
					//	一時的に退避
					temp = sort_info[s - 1];

					//	交換
					sort_info[s - 1] = sort_info[s];

					//	退避してたやつを戻す
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
				lastBonus = Random::GetInt( 0, 3 );
			}
		}
	}

	//	ラストボーナス数値加算、全員同じ値　or　０だった場合ボーナスに最終値を入れ該当なしにする
	void	sceneResult::AddLastBonus( void )
	{
		int	bonus = 0;
		int	result = 0;
		switch ( lastBonus )
		{
		case LASTBONUS::MAX_COIN:
			//	該当なしかチェック
			FOR( 0, PLAYER_MAX ) 	result += maxCoinNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	最大コイン枚数
			bonus = Random::GetInt( 10, 30 );
			sortInfo[maxCoinNum[0].rank].num += bonus;
			originInfo[maxCoinNum[0].rank].bonus = bonus;
			bonusPlayer = maxCoinNum[0].rank;
			break;

		case LASTBONUS::FALL_STAGE:
			//	該当なしかチェック
			FOR( 0, PLAYER_MAX ) 	result += fallStageNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	ステージからの落下回数
			bonus = Random::GetInt( 10, 30 );
			sortInfo[fallStageNum[0].rank].num += bonus;
			originInfo[maxCoinNum[0].rank].bonus = bonus;
			bonusPlayer = maxCoinNum[0].rank;
			break;

		case LASTBONUS::COIN77:
			//	該当なしかチェック
			FOR( 0, PLAYER_MAX ) 	result += coin77[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}
			
			//	７７枚とコイン枚数の差
			bonus = Random::GetInt( 10, 30 );
			sortInfo[coin77[0].rank].num += bonus;
			originInfo[coin77[0].rank].bonus = bonus;
			bonusPlayer = coin77[0].rank;
			break;

		case LASTBONUS::MIN_TOTALCOIN:
			//	該当なしかチェック
			FOR( 0, PLAYER_MAX ) 	result += minCoinNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	取得コイン総数が一番少ない
			bonus = Random::GetInt( 10, 30 );
			sortInfo[minCoinNum[0].rank].num += bonus;
			originInfo[minCoinNum[0].rank].bonus = bonus;
			bonusPlayer = minCoinNum[0].rank;
			break;

		case LASTBONUS::HIT_ATTACK_NUM:
			//	該当なしかチェック
			FOR( 0, PLAYER_MAX ) 	result += hitAttackNum[value].num;
			if ( result == 0 )
			{
				bonusPlayer = NO_BONUS;
				return;
			}

			//	攻撃を当てた回数
			bonus = Random::GetInt( 10, 30 );
			sortInfo[hitAttackNum[0].rank].num += bonus;
			originInfo[hitAttackNum[0].rank].bonus = bonus;
			bonusPlayer = hitAttackNum[0].rank;
			break;
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
		FOR( 0, PLAYER_MAX )
		{
			originInfo[sortInfo[value].rank].rank = value;

			if ( value != 0 )
			{
				//	上位の人と同じコイン枚数だったら同ランクにする
				if ( originInfo[sortInfo[value].rank].num == originInfo[sortInfo[value - 1].rank].num )
				{
					originInfo[sortInfo[value].rank].rank = originInfo[sortInfo[value - 1].rank].rank;
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

		if ( originInfo[player].bonus >= 100 )		bonusNumberImageInfo[player].hundredRenderFlag = true;
		else														bonusNumberImageInfo[player].hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( number[player], coin );
		SetNumberInfo( bonusNumber[player], originInfo[player].bonus );

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
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

		//------------------------------------------------------------------------------------------------
		//	ボーナス用構造体設定
		//------------------------------------------------------------------------------------------------
		if ( bonusNumberImageInfo[player].hundredRenderFlag )
		{
			//	１０の位設定
			x = bonusNumberImageInfo[player].pos.x;
			y = bonusNumberImageInfo[player].pos.y;
			w = h = bonusNumberImageInfo[player].scale;
			sx = bonusNumber[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( bonusNumberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定
			x = bonusNumberImageInfo[player].pos.x - static_cast<int>( bonusNumberImageInfo[player].ten.w / 1.5f );
			sx = bonusNumber[player].hundred * 64;
			ImageInitialize( bonusNumberImageInfo[player].hundred, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = bonusNumberImageInfo[player].pos.x + static_cast<int>( bonusNumberImageInfo[player].ten.w / 1.5f );
			sx = bonusNumber[player].one * 64;
			ImageInitialize( bonusNumberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	１０の位設定
			w = h = bonusNumberImageInfo[player].scale;
			x = bonusNumberImageInfo[player].pos.x - w / 3;
			y = bonusNumberImageInfo[player].pos.y;
			sx = bonusNumber[player].ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( bonusNumberImageInfo[player].ten, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = bonusNumberImageInfo[player].pos.x + w / 3;
			sx = bonusNumber[player].one * 64;
			ImageInitialize( bonusNumberImageInfo[player].one, x, y, w, h, sx, sy, sw, sh );
		}

		//	非表示にする
		bonusNumberImageInfo[player].one.renderflag = false;
		bonusNumberImageInfo[player].ten.renderflag = false;
		bonusNumberImageInfo[player].hundred.renderflag = false;
	}

	//	設定した数値にあわせて構造体情報を設定、１００以上かで配置も変更
	void	sceneResult::SetNumberImageInfo( NUMBERIMAGE_INFO& numImageInfo, NUMBER_INFO& numInfo, const int& num )
	{
		//	桁数確認
		if ( num >= 100 )		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	数字構造体設定
		SetNumberInfo( numInfo, num );

		//	各位画像構造体初期化
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	総数用構造体設定
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	１０の位設定
			x = numImageInfo.pos.x;
			y = numImageInfo.pos.y;
			w = h = numImageInfo.scale;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１００の位設定
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
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
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	１の位設定
			x = numImageInfo.pos.x + w / 3;
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
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
				SetNumberImageInfo( i, originInfo[i].num );
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

		int	nextRound = gameManager->GetRound() + 1;

		switch ( menuInfo.select )
		{
		case MENU::RESTART:
			//	ゲーム情報初期化
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

	//	次回ライフ設定
	void	sceneResult::SetNextLife( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			gameManager->SetStartLife( value, originInfo[value].rank + 1 );
		}
	}

//----------------------------------------------------------------------------
//	ラストボーナス関数( ボード出現→文字→コイン枚数更新→ボード退却 )
//----------------------------------------------------------------------------

	//	ラストボーナス動作更新
	bool	sceneResult::LastBonusUpdate( void )
	{
		bool	isEnd = false;
		bool	isEndWave = false;

		switch ( lastBonusInfo.step )
		{
		case 0:
			//	黒ボード出現
			isEnd = InBoard();

			//	黒ボードの動作終了後ラストボーナスのテキストの描画をONにする
			if ( isEnd )
			{
				lastBonusInfo.textImage.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 1:
			//	ラストボーナス発表
			isEnd = BonusAnnouncing();
			
			//	発表直後波紋設定
			if ( isEnd )		isEndWave = WaveUpdate( lastBonusInfo.textImage );

			//	波紋終了後対象プレイヤーの描画をONにして次のステップへ
			if ( isEndWave )
			{
				if ( bonusPlayer != NO_BONUS )		faceImage.renderflag = true;
				else								notApplicable.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 2:
			//	対象プレイヤー発表
			isEnd = PlayerAnnouncing();

			//	プレイヤー発表後、波紋を設定
			if ( isEnd )
			{
				if ( bonusPlayer != NO_BONUS )		isEndWave = WaveUpdate( waveCircleImage, 140 );
				else	isEndWave = true;
			}
			
			//	波紋動作終了後プレイヤー番号表示
			if ( isEndWave )
			{
				if ( bonusPlayer != NO_BONUS )		playerNumImage.renderflag = true;
				lastBonusInfo.t = 0.0f;
				lastBonusInfo.step++;
			}
			break;

		case 3:
			//	ボーナス加算演出
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
	
	//	ボード出現
	bool	sceneResult::InBoard( void )
	{
		//	パラメータ更新
		lastBonusInfo.t += 0.07f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;
		
		//	頂点移動
		CubicFunctionInterpolation( lastBonusInfo.v[0].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[2].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[0].sy, static_cast<int>( iexSystem::ScreenHeight * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.4f ), lastBonusInfo.t );
		CubicFunctionInterpolation( lastBonusInfo.v[2].sy, static_cast<int>( iexSystem::ScreenHeight * 0.6f ), static_cast<int>( iexSystem::ScreenHeight * 0.8f ), lastBonusInfo.t );

		//	処理が終了してたらtrueをかえす
		if ( lastBonusInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	ボード退却
	bool	sceneResult::OutBoard( void )
	{		
		//	パラメータ更新
		lastBonusInfo.t += 0.07f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

		//	頂点移動
		Lerp( lastBonusInfo.v[1].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		Lerp( lastBonusInfo.v[3].sx, static_cast<int>( iexSystem::ScreenWidth ), 0, lastBonusInfo.t );
		Lerp( lastBonusInfo.v[1].sy, static_cast<int>( iexSystem::ScreenHeight * 0.2f ), static_cast<int>( iexSystem::ScreenHeight * 0.4f ), lastBonusInfo.t );
		Lerp( lastBonusInfo.v[3].sy, static_cast<int>( iexSystem::ScreenHeight * 0.6f ), static_cast<int>( iexSystem::ScreenHeight * 0.8f ), lastBonusInfo.t );

		//	処理が終了してたらtrueをかえす
		if ( lastBonusInfo.t >= 1.0f )	return	true;
		return	false;
	}

	//	ボーナス発表
	bool	sceneResult::BonusAnnouncing( void )
	{
		if ( lastBonusInfo.t >= 1.0f )	return	true;

		//	パラメータ更新
		lastBonusInfo.t += 0.01f;
		if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

		//	文字回転
		static	float startAngle = -D3DX_PI * 0.035f;
		static	float	endAngle = startAngle + ( ( D3DX_PI * 2 ) * 10.0f );
		Lerp( lastBonusInfo.textImage.angle, startAngle, endAngle, lastBonusInfo.t );

		//	文字サイズ変更
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

	//	プレイヤー発表
	bool	sceneResult::PlayerAnnouncing( void )
	{
		if ( lastBonusInfo.t >= 1.0f )	return	true;

		//	顔画像拡大
		if ( bonusPlayer != NO_BONUS )
		{
			//	パラメータ更新
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
			//	該当なし
			//	パラメータ更新
			lastBonusInfo.t += 0.01f;
			if ( lastBonusInfo.t >= 1.0f )	lastBonusInfo.t = 1.0f;

			float	t = GetBezier( ePrm_t::eRapid_Lv3, ePrm_t::eSlow_Lv5, lastBonusInfo.t );
			int	startPos = static_cast<int>( iexSystem::ScreenHeight * 0.5f );
			int	endPos = static_cast<int>( iexSystem::ScreenHeight * 0.55f );
			Lerp( notApplicable.alpha, 0.0f, 1.0f, lastBonusInfo.t );
			Lerp( notApplicable.y, startPos, endPos, lastBonusInfo.t );
		}

		//	処理が終了してたらtrueをかえす
		if ( lastBonusInfo.t >= 1.0f )
		{
			if ( bonusPlayer != NO_BONUS )	SetWave( waveCircleImage, 2.0f );
			return	true;
		}
		return	false;
	}

	//	ボーナス加算演出
	bool	sceneResult::AddBonus( void )
	{
		static	const		int WAIT_TIME	=	90;		//	カウント時間
		static	int			addBonusStep	=	0;			//	switch文用ステップ
		static	int			waitTimer			=	0;			//	待機時間カウント
		static	int			temp[4] = { 0, 0, 0, 0 };	//	退避用
		bool					isEnd					=	false;	//	終了チェック用変数

		switch ( addBonusStep )
		{
		case	0:
			//	一定時間でボーナスを非表示にする
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
			//	ボード退避
			isEnd = OutBoard();

			//	ボード退避終了後、ボーナスの数値の描画をONにする
			if ( isEnd )
			{
				FOR( 0, PLAYER_MAX )
				{
					//	退避用変数にボーナスをコピー
					temp[value] = originInfo[value].bonus;

					//	ボーナスがあれば表示
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
			//	待機
			waitTimer++;
			if ( waitTimer == WAIT_TIME )
			{
				waitTimer = 0;
				FOR( 0, PLAYER_MAX )
				{
					//	ボーナス非表示
					bonusNumberImageInfo[value].hundred.renderflag = false;
					bonusNumberImageInfo[value].one.renderflag = false;
					bonusNumberImageInfo[value].ten.renderflag = false;
				}

				addBonusStep++;
			}
			break;

		case	3:
			//	数値にボーナス分を加算する
			FOR( 0, PLAYER_MAX )
			{
				if ( temp[value] == 0 )	continue;
				
				//	元の数値に加算して退避用から減算
				originInfo[value].num++;
				temp[value]--;

				SetNumberImageInfo( numberImageInfo[value], number[value], originInfo[value].num );
				SetNumberImageInfo( bonusNumberImageInfo[value], bonusNumber[value], temp[value] );
			}

			if ( temp[0] + temp[1] + temp[2] + temp[3] == 0 )		addBonusStep++;
			break;

		case	4:
			//	待機
			waitTimer++;
			if ( waitTimer == WAIT_TIME )
			{
				waitTimer = 0;	
				FOR( 0, PLAYER_MAX )
				{
					//	ボーナス非表示
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
	