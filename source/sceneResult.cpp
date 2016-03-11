
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"CharacterManager.h"
#include	"Curtain.h"
#include	"sceneTitle.h"
#include	"sceneMenu.h"
#include	"sceneMain.h"
#include	"sceneLoad.h"
#include	"Screen.h"
#include	"Sound.h"

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
#define	CURTAIN_SPEED	0.02f
#define	SCREEN_SPEED	1.5f

	namespace
	{
		namespace MOVE_MODE
		{
			enum
			{
				RESULT,
				SELECT,
				LAST_RESULT,
			};
		}

		namespace RESULT_MODE
		{
			enum
			{
				ROULETTE,
				LASTBONUS,
				RANK,
				LIFE,
				LAST_RESULT,
				RANK_SKIP,
				INPUT_WAIT,
			};
		}

		namespace SELECT_MODE
		{
			enum
			{
				DOWN,
				SELECT,
			};
		}

		namespace LASTRESULT_MODE
		{
			enum
			{
				SET_CLOSE_CURTAIN,
				CLOSE_CURTAIN,		//	カーテン閉じる
				LIGHT_PRODUCTION,	//	ライト演出
				OPEN_CURTAIN,			//	カーテン開ける
				PRODUCTION,	//	演出
				RESULT,			//	ラウンドごとのスコア
				FINALRESULT,
			};
		}

		namespace MENU
		{
			enum
			{
				//RESTART,							//	再戦
				MOVE_MENU,					//	対戦設定へ	
				MOVE_TITLE,					//	タイトルへ
				END,								//	終端
			};
		}

		namespace CURTAIN_MODE
		{
			enum
			{
				OPEN,
				CLOSE,
			};
		}
	}

//----------------------------------------------------------------------------
//	全体初期化・全体解放
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
		//	スクリーン設定
		//screen->SetScreenMode( SCREEN_MODE::FADE_IN , SCREEN_SPEED );

		//	現在のラウンドを取得
		culRound = gameManager->GetRound();
		
		//	再戦用の設定
		FOR( 0, PLAYER_MAX )
		{
			//	現ラウンドの獲得コイン枚数を設定
			gameManager->SetTotalCoin( culRound, value, gameManager->GetCoinNum( value ) );
		}

		//	乱数初期化
		Random::Initialize();

		//	カメラ設定
		CameraInitialize();

		//	読み込み
		Load();

		//	カーテン情報初期化
		CurtainInfoInitialize();

		//	モデル初期化
		ModelInitialize();

		//	結果用情報構造体初期化
		ResultInfoInitialize();

		//	合計情報をソート
		Sort( sortInfo );
		
		//	ランキング設定
		SetRank();

		//	数値構造体初期化
		NumberImageInfoInitialize();
		
		//	順位画像構造体初期化
		RankImageInitialize();

		//	ライフ画像構造体初期化
		LifeInfoInitialize();

		//	ルーレット情報初期化
		RouletteInfoInitialize();

		//	メニュー関連初期化
		MenuInfoInitialize();

		//	レンダーターゲット関連初期化
		RenderTargetTextureInitialize();

		//	ラウンド関連初期化
		RoundInfoInitialize();
		SetRoundCoinNumber();

		//	優勝者設定
		SetWinner();

		//	変数初期化
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;
		bonusPlayer = 0;		
		FOR( 0, PLAYER_MAX )	inputCheck[value] = false;

		//	スクリーン、サウンド初期化
		sound->PlayBGM( BGM::RESULT_BGM );
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );
		return	true;
	}

	//	解放
	void	sceneResult::Release( void )
	{
		SafeDelete( pressButtonImage.obj );
		SafeDelete( waveImage.obj );
		SafeDelete( playerNumImage.obj );
		SafeDelete( faceImage.obj );
		SafeDelete( lifeAnnounceImage.obj );
		SafeDelete( orgCurtain );
		SafeDelete( orgRound );
		SafeDelete( life );
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		SafeDelete( infoScreen );
		SafeDelete( lastResultTest );
		SafeDelete( bgStage );
		SafeDelete( check );
		SafeDelete( winnerBack );
		Random::Release();

		sound->AllStop();
	}

//----------------------------------------------------------------------------
//	各種情報初期化
//----------------------------------------------------------------------------

	//	読み込み
	void	sceneResult::Load( void )
	{
		//	画像読み込み
		back = make_unique<iex2DObj>( LPSTR( "DATA/UI/back.png" ) );
		menuHead.obj = new iex2DObj("DATA/UI/menu/menu-head.png");
		originNumber = new iex2DObj("DATA/UI/number.png");
		menuText = new iex2DObj("DATA/UI/result/result-cho.png");
		life = new iex2DObj("DATA/UI/Nlife.png");
		check = new iex2DObj( "DATA/UI/Result/check.png" );
		orgRound = new iex2DObj( "DATA/UI/roundText.png" );
		orgCurtain = new iex2DObj( "DATA/UI/title/curtain1.png" );
		lifeAnnounceImage.obj = new iex2DObj( "DATA/UI/result/resultText.png" );
		faceImage.obj = new iex2DObj( "DATA/UI/chara_emotion.png" );
		playerNumImage.obj = new iex2DObj( "DATA/UI/DonketuUI.png" );
		waveImage.obj = new iex2DObj( "DATA/UI/Rainbow-circle.png" );
		pressButtonImage.obj = new iex2DObj( "DATA/UI/pressspace.png" );
		winnerBack = new iex2DObj( "DATA/UI/Result/Last-result-back.png" );

		//	ライフ発表テキスト画像初期化
		ImageInitialize( lifeAnnounceImage, static_cast<int>( iexSystem::ScreenWidth * 0.5f ), static_cast<int>( iexSystem::ScreenHeight * 0.35f ), 300, 200, 0, 0, 512, 256 );
		lifeAnnounceImage.alpha = 0.0f;
		lifeAnnounceImage.renderflag = false;

		//	メニューヘッド画像構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );

		//	PressButtonImage初期化
		x = static_cast<int>( iexSystem::ScreenWidth * 0.8f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.9f );
		ImageInitialize( pressButtonImage, x, y, w, h, 0, 0, 256, 128 );
		pressButtonImage.flashingRenderflag = false;

		//	モデル読み込み
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>(LPSTR("DATA/CHR/majo/majo.IEM"));			//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>(LPSTR("DATA/CHR/プリンセス/prinsess1.IEM"));					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));				//	リス
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));					//	トラ
		orgCannon = make_unique<iexMesh>( LPSTR( "DATA/CHR/Pirate/pirate_cannon.IMO" ) );
		FOR(0, PLAYER_MAX)	cannon[value] = orgCannon->Clone();
		
		//	ステージ設定
		bgStage = new iexMesh("DATA/BG/MenuStage/menustage.IMO");	//	ステージ
		bgStage->SetScale(0.1f);
		bgStage->SetAngle(D3DX_PI);
		bgStage->Update();

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale(0.015f);	//	魔女
		org[CHARACTER_TYPE::PRINCESS]->SetScale(0.04f);		//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale(0.025f);				//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.04f);				//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle(D3DX_PI);	//	魔女
		org[CHARACTER_TYPE::PRINCESS]->SetAngle(D3DX_PI);		//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle(D3DX_PI);				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetAngle(D3DX_PI);			//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(0);		//	魔女
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(0);			//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);	 				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	海賊

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	魔女
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	姫
		org[CHARACTER_TYPE::THIEF]->Update();							//	シーフ
		org[CHARACTER_TYPE::PIRATE]->Update();						//	海賊

	}

	//	カメラ初期化
	void	sceneResult::CameraInitialize( void )
	{
		//	バックバッファポインタ退避
		iexSystem::GetDevice()->GetRenderTarget( 0, &backBuffer );

		//	レンダーターゲットスクリーン
		infoScreen = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );
		lastResultTest = new iex2DObj( iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET );

		//	情報表示用カメラ
		viewInfo.pos = Vector3( 0.0f, 5.2f, -70.0f );
		viewInfo.target = Vector3( 0.0f, 5.2f, 0.0f );
		view2D = make_unique<Camera>();
		view2D->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		view2D->Set( viewInfo.pos, viewInfo.target );
		view2D->Activate();

		//	テスト用カメラ
		viewTest = make_unique<Camera>();
		viewTest->SetProjection( D3DXToRadian( 10.0f ), 1.0f, 1000.0f );
		viewTest->Set( viewInfo.pos, viewInfo.target );
		viewTest->Activate();

		//	メインカメラ
		viewInfo.pos = Vector3( 0.0f, 10.0f, -10.0f );
		viewInfo.target = viewInfo.pos + Vector3( 0.0f, 0.0f, 1.0f );
		mainView = new Camera();
		mainView->Set( viewInfo.pos, viewInfo.target );
		mainView->Activate();
	}

	//	モデル初期化
	void	sceneResult::ModelInitialize( void )
	{
		float angle[PLAYER_MAX] = { D3DXToRadian( -15.0f ), D3DXToRadian( -10.0f ), D3DXToRadian( 10.0f ), D3DXToRadian( 15.0f ) };
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	モデル情報設定
			obj[i] = org[gameManager->GetCharacterType( i )]->Clone();
			obj[i]->SetPos( -9.0f + ( 19.0f / 3.0f * i ), 3.0f, 10.0f );
			obj[i]->SetAngle( D3DX_PI + angle[i] );
			obj[i]->Update();
		}
	}

	//	結果用構造体関連初期化
	void	sceneResult::ResultInfoInitialize( void )
	{
		//	数値構造体初期化
		FOR( 0, PLAYER_MAX )
		{
			number[value].hundred = 0;
			number[value].ten = 0;
			number[value].one = 0;
		}

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	ゲーム終了時のデータを格納( ここでボーナスも設定しておく )
			originInfo[i].num = gameManager->GetCoinNum( i );
			originInfo[i].rank = i;
			originInfo[i].bonus = 0;

			//	ランキング計算用に総計データを格納( ボーナス数値が整い次第、元のコイン枚数にボーナスを足す、ランクはソートにかけるため適当に代入 )
			sortInfo[i].num = originInfo[i].num;
			sortInfo[i].sortRank = i;
			sortInfo[i].rank = i;

			totalSortInfo[i].num = 0;
			totalSortInfo[i].rank = i;
			totalSortInfo[i].sortRank = i;
		}
	}

	//	数値構造体初期化
	void	sceneResult::NumberImageInfoInitialize( void )
	{	
		Vector3	out;

		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	座標設定
			out.x = iexSystem::ScreenWidth * 0.2f;
			out.x += out.x * i;

			//	中心座標・初期スケール設定
			numberImageInfo[i].pos.x = static_cast<int>( out.x );
			numberImageInfo[i].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.38f );
			numberImageInfo[i].scale = 100;

			//	各位画像設定
			numberImageInfo[i].one.obj = originNumber;
			numberImageInfo[i].ten.obj = originNumber;
			numberImageInfo[i].hundred.obj = originNumber;
			
			//	数値画像構造体初期化
			SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
		}
	}

	//	順位画像構造体初期化
	void	sceneResult::RankImageInitialize( void )
	{
		Vector3	out;
		
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	座標設定
			out.x = iexSystem::ScreenWidth * 0.2f;
			out.x += out.x * i;

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

		//	ランク表示用構造体初期化
		{
			viewRankInOrder.timer = 0;
			viewRankInOrder.step = 3;
		}
	}

	//	ライフ画像構造体初期化
	void	sceneResult::LifeInfoInitialize( void )
	{
		int	culLife = 0;
		int	x, y, w, h, sx, sy, sw, sh;

		//	次回ライフ設定
		nextLife[0][0] = 2; nextLife[0][1] = 2; nextLife[0][2] = 3; nextLife[0][3] = 3;
		nextLife[1][0] = 2; nextLife[1][1] = 3; nextLife[1][2] = 3; nextLife[1][3] = 4;
		nextLife[2][0] = 2; nextLife[2][1] = 3; nextLife[2][2] = 4; nextLife[2][3] = 5;

		lifeInfo.maxW = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		lifeInfo.maxH = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		FOR( 0, PLAYER_MAX )
		{
			//	画像を登録
			lifeImage[value].obj = life;

			//	現在のライフ最大値を取得
			culLife = gameManager->GetStartLife( value );

			//	画像構造体に設定
			x = rankImage[value].x;
			y = rankImage[value].y;
			w = 0;
			h = 0;
			sw = 64;
			sh = 64;
			sx = sw * ( ( 5 - culLife ) % 4 );
			sy = sh * ( ( 5 - culLife ) / 4 );
			ImageInitialize( lifeImage[value], x, y, w, h, sx, sy, sw, sh );

			//	最初は非表示
			lifeImage[value].renderflag = false;

			//	ついでにOKの位置も設定
			w = 300;
			h = 200;
			y = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
			h = ( int )( w * 0.5f );
			checkImage[value].obj = check;
			ImageInitialize( checkImage[value], x, y, w, h, 0, 0, 256, 128 );
			checkImage[value].renderflag = false;
		}

		//	ライフ発表用構造体設定
		{
			lifeInfo.step = 0;
			lifeInfo.culLife = 0;
			lifeInfo.renderflag = false;
			lifeInfo.isEnd = false;
			lifeInfo.t = 0.0f;
			lifeInfo.waitTimer = 0;
		}
	}

	//	レンダーターゲット関連初期化
	void	sceneResult::RenderTargetTextureInitialize( void )
	{
		//	テクスチャ位置設定
		float	dist = 1000.0f;
		Collision::GetFront( bgStage, viewInfo.pos + Vector3( 0.0f, 2.0f, 0.0f ), viewInfo.texPos );
		viewInfo.texPos.z -= 0.01f;
		Vector3	texSize = Vector3( 30.0f, 20.0f, 0.0f );
		SetVertex( viewInfo.v[0], viewInfo.texPos.x - texSize.x * 0.5f, viewInfo.texPos.y + texSize.y * 0.5f, viewInfo.texPos.z, 0, 0, 0xFFFFFFFF );
		SetVertex( viewInfo.v[1], viewInfo.texPos.x + texSize.x * 0.5f, viewInfo.texPos.y + texSize.y * 0.5f, viewInfo.texPos.z, 1, 0, 0xFFFFFFFF );
		SetVertex( viewInfo.v[2], viewInfo.texPos.x - texSize.x * 0.5f, viewInfo.texPos.y - texSize.y * 0.5f, viewInfo.texPos.z, 0, 1, 0xFFFFFFFF );
		SetVertex( viewInfo.v[3], viewInfo.texPos.x + texSize.x * 0.5f, viewInfo.texPos.y - texSize.y * 0.5f, viewInfo.texPos.z, 1, 1, 0xFFFFFFFF );
	}

	//	メニュー情報構造体
	void	sceneResult::MenuInfoInitialize( void )
	{
		//	メニュー情報初期化
		{
			menuInfo.select = MENU::MOVE_MENU;
			menuInfo.screenH = 0;
			menuInfo.alpha = 0.5f;
			menuInfo.t = 0.0f;
		}

		//	メニュー画像構造体初期化
		{
			//	メニューの項目数まわす
			for ( int i = 0; i < MENU::END; i++ )
			{
				int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
				int y = static_cast<int>( iexSystem::ScreenHeight * 0.35f );
				int w = static_cast<int>( iexSystem::ScreenWidth * 0.55f );
				int h = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
				ImageInitialize( menuImage[i], x, y + y * i, w, h, 0, 128 + 128 * i, 512, 128 );
				menuImage[i].obj = menuText;
				menuImage[i].renderflag = false;
			}
		}
	}

	//	ルーレット構造体初期化
	void	sceneResult::RouletteInfoInitialize( void )
	{

		//	ルーレット情報初期化
		{
			rouletteInfo.step = 0;
			rouletteInfo.timer = 1;
		}
	}

	//	カーテン情報初期化
	void	sceneResult::CurtainInfoInitialize( void )
	{
		//	ポイント設定
		curtainPosInfo.centerPosX = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		curtainPosInfo.leftOutPosX = static_cast<int>( iexSystem::ScreenWidth * -0.5f );
		curtainPosInfo.rightOutPosX = static_cast<int>( iexSystem::ScreenWidth * 1.5f );
		curtainPosInfo.underPosY = static_cast<int>( iexSystem::ScreenHeight );
		curtainPosInfo.rightPos = static_cast<int>( iexSystem::ScreenWidth );
		curtainPosInfo.upPosY = 0;
		curtainPosInfo.leftPos = 0;

		//	カーテン情報初期化
		curtainInfoL.obj = orgCurtain;
		curtainInfoR.obj = orgCurtain;
		curtainInfoL.t = 0.0f;
		curtainInfoR.t = 0.0f;
		curtain_t = 1.0f;
		curtainBrightness = 1.0f;	
		curtainState = false;
		curtainMode = CURTAIN_MODE::OPEN;

		//	シェーダー用変数初期化
		lightMoveNum = 0;
		LIGHT_POS[0] = Vector3( 0.0f, 0.0f, 0.0f );
		LIGHT_POS[1] = Vector3( static_cast<float>( iexSystem::ScreenWidth ), 0.0f, 0.0f );
		LIGHT_POS[2] = Vector3( 0.0f, static_cast<float>( iexSystem::ScreenHeight ), 0.0f );
		LIGHT_POS[3] = Vector3( static_cast<float>( iexSystem::ScreenWidth ), static_cast<float>( iexSystem::ScreenHeight ), 0.0f );

		lightSize[0] = 200.0f;
		lightSize[1] = 150.0f;
		lightSize[2] = 200.0f;
		lightSize[3] = 150.0f;

		shader2D->SetValue("light_Size[0]", lightSize[0]);
		shader2D->SetValue("light_Size[1]", lightSize[1]);
		shader2D->SetValue("light_Size[2]", lightSize[2]);
		shader2D->SetValue("light_Size[3]", lightSize[3]);

		FOR(0, 4)
		{
			lightPos[value] = LIGHT_POS[value];
			MoveLightSet(value, RandomPos());
		}

		//	頂点設定
		SetVertex( curtainInfoL.tlv[0], ( float )curtainPosInfo.leftPos, ( float )curtainPosInfo.upPosY, 0, 0, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[1], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.upPosY, 0, 1, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[2], ( float )curtainPosInfo.leftPos, ( float )curtainPosInfo.underPosY, 0, 0, 1, 0xFFFFFFFF );
		SetVertex( curtainInfoL.tlv[3], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.underPosY, 0, 1, 1, 0xFFFFFFFF );
														
		SetVertex( curtainInfoR.tlv[0], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.upPosY, 0, 1, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[1], ( float )curtainPosInfo.rightPos, ( float )curtainPosInfo.upPosY, 0, 0, 0, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[2], ( float )curtainPosInfo.centerPosX, ( float )curtainPosInfo.underPosY, 0, 1, 1, 0xFFFFFFFF );
		SetVertex( curtainInfoR.tlv[3], ( float )curtainPosInfo.rightPos, ( float )curtainPosInfo.underPosY, 0, 0, 1, 0xFFFFFFFF );
	}

	//	ラウンド関連初期化
	void	sceneResult::RoundInfoInitialize( void )
	{
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.3f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.1f );
		int sx = 0;
		int sy;
		int sw = 512;
		int sh = 128;

		FOR( 0, Round::END + 1 ) 
		{
			y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.15f ) * value );
			sy = sh * value;
			ImageInitialize( roundImage[value], x, y, w, h, sx, sy, sw, sh );
			roundImage[value].obj = orgRound;
		}

		SetRoundCoinNumber();
	}

	//	ラウンドごとの数値を設定
	void	sceneResult::SetRoundCoinNumber( void )
	{
		int	totalCoinNum;
		int x, y, w, h, sx, sy, sw, sh;
		int x2, y2, w2, h2, sx2, sy2, sw2, sh2;

		//	各ラウンドのコイン枚数を取得
		FOR( 0, PLAYER_MAX )
		{
			//	各プレイヤーの顔画像設定
			y = roundImage[0].y - static_cast<int>( iexSystem::ScreenHeight * 0.15f );
			w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			h = w;
			sw = 256;
			sh = 256;
			sx = 0;
			sy = gameManager->GetCharacterType( value ) * sh;

			for ( int round = 0; round <= Round::END; round++ ) 
			{
				if ( round != Round::END )
				{
					//	座標を設定
					roundCoinNumberImageInfo[round][value].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.4f ) + ( static_cast<int>( iexSystem::ScreenWidth * 0.15f ) * value );
					roundCoinNumberImageInfo[round][value].pos.y = roundImage[round].y;
					roundCoinNumberImageInfo[round][value].scale = 100;
					roundCoinNumberImageInfo[round][value].one.obj = originNumber;
					roundCoinNumberImageInfo[round][value].ten.obj = originNumber;
					roundCoinNumberImageInfo[round][value].hundred.obj = originNumber;

					//	数値を設定
					totalCoinNum = gameManager->GetTotalCoin( round, value );
					SetNumberImageInfo( roundCoinNumberImageInfo[round][value], roundCoinNumber[round][value], totalCoinNum );

					x = roundCoinNumberImageInfo[round][value].pos.x;
				}
				else
				{
					//	座標を設定
					totalNumberImageInfo[value].pos.x = static_cast<int>( iexSystem::ScreenWidth * 0.4f ) + ( static_cast<int>( iexSystem::ScreenWidth * 0.15f ) * value );
					totalNumberImageInfo[value].pos.y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.15f ) * round );
					totalNumberImageInfo[value].scale = 100;
					totalNumberImageInfo[value].one.obj = originNumber;
					totalNumberImageInfo[value].ten.obj = originNumber;
					totalNumberImageInfo[value].hundred.obj = originNumber;
				}
			}

			//	顔画像初期化
			ImageInitialize( faceImageObj[value], x, y, w, h, sx, sy, sw, sh );
			faceImageObj[value].obj = faceImage.obj;

			//	プレイヤー番号初期化
			x2 = x - static_cast<int>( iexSystem::ScreenWidth * 0.05f );
			y2 = y + static_cast<int>( iexSystem::ScreenHeight * 0.05f );
			w2 = static_cast<int>( iexSystem::ScreenWidth * 0.07f );
			h2 = w2;
			sw2 = 128;
			sh2 = 128;
			sx2 = value * sw2;
			sy2 = 256;
			ImageInitialize( playerNumImageObj[value], x2, y2, w2, h2, sx2, sy2, sw2, sh2 );
			playerNumImageObj[value].obj = playerNumImage.obj;
		}
	}

	//	優勝者決定
	void	sceneResult::SetWinner( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			for ( int round = 0; round < Round::END; round++ )
			{
				//	各ラウンドのコインの合計を求める
				totalSortInfo[value].num += gameManager->GetTotalCoin( round, value );
			}
			SetNumberImageInfo( totalNumberImageInfo[value], totalNumber[value], totalSortInfo[value].num );
		}

		Sort( totalSortInfo );
		
		//	顔画像構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.55f );
		int w = 0;
		int h = 0;
		int sw = 256;
		int sh = 256;
		int sx = 0;
		int sy = gameManager->GetCharacterType( totalSortInfo[0].rank ) * sh;
		ImageInitialize( faceImage, x, y, w, h, sx, sy, sw, sh );
		faceImage.renderflag = false;

		//	波紋画像初期化
		w = 900;
		h = 900;
		sx = 0;
		sy = 0;
		sw = 512;
		sh = 512;
		ImageInitialize( waveImage, x, y, w, h, sx, sy, sw, sh );

		//	プレイヤー番号画像構造体初期化
		x = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		y = static_cast<int>( iexSystem::ScreenHeight * 0.7f );
		w = 0;
		h = 0;
		sw = 128;
		sh = 128;
		sx = totalSortInfo[0].rank * sw;
		sy = 256;
		ImageInitialize( playerNumImage, x, y, w, h, sx, sy, sw, sh );
		playerNumImage.renderflag = false;

		//	ラスト発表構造体初期化
		lastAnnounceInfo.step = 0;
		lastAnnounceInfo.t = 0.0f;

	}

//----------------------------------------------------------------------------
//	全体更新・全体描画
//----------------------------------------------------------------------------

	//	更新
	void	sceneResult::Update( void ) 
	{
		//	モデル更新
		ModelUpdate();

		switch ( mode )
		{
		case MOVE_MODE::RESULT:
			ResultUpdate();
			break;

		case MOVE_MODE::SELECT:
			SelectUpdate();
			break;

		case MOVE_MODE::LAST_RESULT:
			LastResultUpdate();
			break;
		}
		//	カーテン更新
		curtainState = CurtainUpdate();

		//	pressButton更新
		PressButtonUpdate();
	
		//	シーン移動管理
		MoveScene();
		
		//	スクリーン更新
		screen->Update();
	}

	//	描画
	void	sceneResult::Render( void ) 
	{
		switch ( mode )
		{
		case MOVE_MODE::RESULT:
			ResultRender();
			break;

		case MOVE_MODE::SELECT:
		case MOVE_MODE::LAST_RESULT:
			LastResultRender();
			break;
		}
		//	pressButton描画
		PressButtonImageRender();

		//	カーテン描画
		CurtainRender();

		//	最終結果テスト
		SelectRender();

		screen->Render();
	}

//----------------------------------------------------------------------------
//	更新
//----------------------------------------------------------------------------

	//	リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		if ( changeScene )	return;
		
		//	段階ごとの処理
		switch ( step )
		{
		case RESULT_MODE::ROULETTE:
			ModeRoulette();
			break;
			
		case RESULT_MODE::RANK:
			ModeRank();
			break;

		case	RESULT_MODE::LIFE:
			NextLifeAnnouncing();
			break;

		case RESULT_MODE::LAST_RESULT:
			ModeLastResult();
			break;

		case RESULT_MODE::RANK_SKIP:
			ModeRankSkip();
			break;

		case	RESULT_MODE::INPUT_WAIT:
			ModeInputWait();
			break;
		}
	}

	//	セレクト時の更新
	void	sceneResult::SelectUpdate( void )
	{
		bool	isEnd = false;

		switch ( step )
		{
		case SELECT_MODE::DOWN:
			isEnd = DownPolygon();
			if ( isEnd )	step++;
			break;

		case SELECT_MODE::SELECT:
			ControlCursor();
			break;
		}
	}

	//	ラスト発表更新
	void	sceneResult::LastResultUpdate( void )
	{
		bool	isEnd = false;
		switch ( step )
		{
		case LASTRESULT_MODE::SET_CLOSE_CURTAIN:
			step = LASTRESULT_MODE::LIGHT_PRODUCTION;
			break;

		case LASTRESULT_MODE::LIGHT_PRODUCTION:
			isEnd = WaitTimeUpdate();
			//	ライト
			LightUpdate();
			MoveLight();

			//	演出終了後カーテンを開き優勝者発表へ
			if ( isEnd )
			{
				step = LASTRESULT_MODE::RESULT;
				SetCurtainMode( CURTAIN_MODE::OPEN );
			}
			break;

		case LASTRESULT_MODE::RESULT:
			if ( curtainState )		LastRank();
			break;

		case LASTRESULT_MODE::FINALRESULT:
			if ( screen->GetScreenState() )
			{
				pressButtonImage.flashingRenderflag = true;
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) )
				{
					step = 0;
					mode = MOVE_MODE::SELECT;
				}
			}
			break;
		}
	}

	//	カーテン更新
	bool	sceneResult::CurtainUpdate( void )
	{
		bool	out = false;

		//	パラメータ更新
		curtain_t += CURTAIN_SPEED;

		//	上限下限設定
		if ( curtain_t >= 1.0f )
		{
			curtain_t = 1.0f;
			out = true;
		}

		//	カーテンのモードでスタート位置と終了位置を変更
		switch ( curtainMode )
		{
		case CURTAIN_MODE::OPEN:
			//	左カーテン
			Lerp( curtainInfoL.tlv[0].sx, curtainPosInfo.leftPos, curtainPosInfo.leftOutPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	左上
			Lerp( curtainInfoL.tlv[1].sx, curtainPosInfo.centerPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	右上
			Lerp( curtainInfoL.tlv[2].sx, curtainPosInfo.leftPos, curtainPosInfo.leftOutPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	左下
			Lerp( curtainInfoL.tlv[3].sx, curtainPosInfo.centerPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	右下

			//	右カーテン
			Lerp( curtainInfoR.tlv[0].sx, curtainPosInfo.centerPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	左上
			Lerp( curtainInfoR.tlv[1].sx, curtainPosInfo.rightPos, curtainPosInfo.rightOutPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	右上
			Lerp( curtainInfoR.tlv[2].sx, curtainPosInfo.centerPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	左下
			Lerp( curtainInfoR.tlv[3].sx, curtainPosInfo.rightPos, curtainPosInfo.rightOutPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	右下	
			break;

		case CURTAIN_MODE::CLOSE:
			//	左カーテン
			Lerp( curtainInfoL.tlv[0].sx, curtainPosInfo.leftOutPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	左上
			Lerp( curtainInfoL.tlv[1].sx, curtainPosInfo.leftPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	右上
			Lerp( curtainInfoL.tlv[2].sx, curtainPosInfo.leftOutPosX, curtainPosInfo.leftPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	左下
			Lerp( curtainInfoL.tlv[3].sx, curtainPosInfo.leftPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	右下

			//	右カーテン
			Lerp( curtainInfoR.tlv[0].sx, curtainPosInfo.rightPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );		//	左上
			Lerp( curtainInfoR.tlv[1].sx, curtainPosInfo.rightOutPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv1, ePrm_t::eSlow_Lv1, curtain_t ) );	//	右上
			Lerp( curtainInfoR.tlv[2].sx, curtainPosInfo.rightPos, curtainPosInfo.centerPosX, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );		//	左下
			Lerp( curtainInfoR.tlv[3].sx, curtainPosInfo.rightOutPosX, curtainPosInfo.rightPos, GetBezier( ePrm_t::eSlow_Lv5, ePrm_t::eSlow_Lv5, curtain_t ) );	//	右下	
			break;
		}

		return	out;
	}

	//	ライト移動
	bool	sceneResult::LightUpdate( void )
	{
		FOR(0, 4){
			//	パラメータ加算
			light_t[value] += 1.0f / 30.0f;

			if (light_t[value] >= 1.0f)	light_t[value] = 1.0f;
		}
		return	false;
	}

	//	入力チェック画像更新
	bool	sceneResult::InputCheckImageUpdate( void )
	{
		waveImage.flashingRenderflag = true;
		bool	isEnd[PLAYER_MAX] = { false, false, false, false };

		FOR( 0, PLAYER_MAX )
		{
			if ( !inputCheck[value] )	continue;
			isEnd[value] = WaveUpdate( checkImage[value] );
		}

		int count = 0;
		FOR( 0, PLAYER_MAX )
		{
			if ( isEnd[value] )	count++;
		}
		if ( count >= gameManager->GetPlayerNum() )	return	true;
		return	false;
	}

	//	PressButton更新
	void	sceneResult::PressButtonUpdate( void )
	{
		FlashingUpdate( pressButtonImage, 0.1f );
	}

	//	モデル更新
	void	sceneResult::ModelUpdate( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			obj[value]->Animation();
			obj[value]->Update();
		}

		SetCannonPos();
	}

//----------------------------------------------------------------------------
//	描画
//----------------------------------------------------------------------------
	
	//	セレクト画面描画
	void	sceneResult::SelectRender( void )
	{
		//	メニュー用スクリーン描画
		iexPolygon::Rect( 0, 0, iexSystem::ScreenWidth, menuInfo.screenH, RS_COPY, GetColor( 0.0f, 0.0f, 0.0f, menuInfo.alpha ) );

		for ( int i = 0; i < MENU::END; i++ )
		{
			RenderImage( menuImage[i], menuImage[i].sx, menuImage[i].sy, menuImage[i].sw, menuImage[i].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	リザルト描画
	void	sceneResult::ResultRender( void )
	{
		//	カメラ
		mainView->Activate();
		mainView->Clear();

		//	背景( 一番後ろに表示 )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	プレイヤー描画
		FOR( 0, PLAYER_MAX )
		{
			//	トゥーン輪郭線色設定
			shader3D->SetValue( "OutlineColor", gameManager->GetPlayerColor( value ) );
			obj[value]->Render( shader3D, "toon" );
		}

		//	大砲描画
		CannonRender();

		//	リザルトシール描画
		RenderImage( menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM );

		//	数値描画
		NumberImageRender();

		//	順位描画
		RankRender();

		//	ライフ描画
		LifeRender();

		//	入力チェック描画
		InputCheckRender();		
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

			if (numberImageInfo[i].hundredRenderFlag)
			{
				RenderImage( numberImageInfo[i].hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
			}

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

	//	数値画像構造体描画
	void	sceneResult::NumberImageRender( sceneResult::NUMBERIMAGE_INFO numberImageInfo )
	{
		//	１００の位描画
		int		sx = numberImageInfo.hundred.sx;
		int		sy = numberImageInfo.hundred.sy;
		int		sw = numberImageInfo.hundred.sw;
		int		sh = numberImageInfo.hundred.sh;

		if ( numberImageInfo.hundredRenderFlag )
		{
			RenderImage( numberImageInfo.hundred, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
		}

		//	１０の位描画
		sx = numberImageInfo.ten.sx;
		sy = numberImageInfo.ten.sy;
		sw = numberImageInfo.ten.sw;
		sh = numberImageInfo.ten.sh;
		RenderImage( numberImageInfo.ten, sx, sy, sw, sh, IMAGE_MODE::NORMAL );

		//	１の位描画
		sx = numberImageInfo.one.sx;
		sy = numberImageInfo.one.sy;
		sw = numberImageInfo.one.sw;
		sh = numberImageInfo.one.sh;
		RenderImage( numberImageInfo.one, sx, sy, sw, sh, IMAGE_MODE::NORMAL );
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
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::ADOPTPARAM );
			RenderImage( rankImage[i], sx, sy, sw, sh, IMAGE_MODE::WAVE );
		}
	}
	
	//	ライフ描画
	void	sceneResult::LifeRender( void )
	{
		//	テキスト描画
		RenderImage( lifeAnnounceImage, lifeAnnounceImage.sx, lifeAnnounceImage.sy, lifeAnnounceImage.sw, lifeAnnounceImage.sh, IMAGE_MODE::ADOPTPARAM );

		//	ライフ描画
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			int		sx = lifeImage[i].sx;
			int		sy = lifeImage[i].sy;
			int		sw = lifeImage[i].sw;
			int		sh = lifeImage[i].sh;
			RenderImage( lifeImage[i], sx, sy, sw, sh, IMAGE_MODE::NORMAL );
			RenderImage( lifeImage[i], sx, sy, sw, sh, IMAGE_MODE::WAVE );
		}
	}

	//	入力チェック描画
	void	sceneResult::InputCheckRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::NORMAL );
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::WAVE );
		}
	}

	//	ラスト発表描画
	void sceneResult::LastResultRender( void )
	{
		//-------------UI関連---------------//
		mainView->Activate();
		mainView->Clear();

		if ( step == LASTRESULT_MODE::RESULT )
		{
			WinnerRender();
			return;
		}

		//	背景描画
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		
		//	ラウンドごとの
		FOR( 0, Round::END )
		{
			//	ラウンド描画
			RenderImage( roundImage[value], roundImage[value].sx, roundImage[value].sy, roundImage[value].sw, roundImage[value].sh, IMAGE_MODE::NORMAL );
			
			//	各ラウンド・プレイヤーのコイン枚数描画
			for ( int player = 0; player < PLAYER_MAX; player++ )
			{
				NumberImageRender( roundCoinNumberImageInfo[value][player] );
			}
		}

		RenderImage( roundImage[3],roundImage[3].sx, roundImage[3].sy, roundImage[3].sw, roundImage[3].sh, IMAGE_MODE::NORMAL );

		FOR( 0, PLAYER_MAX )
		{
			NumberImageRender( totalNumberImageInfo[value] );
		}

		//	顔、プレイヤー番号描画
		AllPlayerFaceRender();
		AllPlayerNumRender();
	}

	//	カーテン描画
	void	sceneResult::CurtainRender( void )
	{
		LPSTR technique = "copy";

		if (step == LASTRESULT_MODE::LIGHT_PRODUCTION)
		{
			technique = "SpotLight";
			shader2D->SetValue( "lightPos[0]", lightPos[0] );
			shader2D->SetValue( "lightPos[1]", lightPos[1] );
			shader2D->SetValue( "lightPos[2]", lightPos[2] );
			shader2D->SetValue( "lightPos[3]", lightPos[3] );
		}
		iexPolygon::Render2D( curtainInfoL.tlv, 2, curtainInfoL.obj, shader2D, technique );
		iexPolygon::Render2D( curtainInfoR.tlv, 2, curtainInfoR.obj, shader2D,technique );
	}

	//	優勝者関連描画
	void	sceneResult::WinnerRender( void )
	{
		winnerBack->Render( 0, 0, ( int )iexSystem::ScreenWidth, ( int )iexSystem::ScreenHeight, 0, 0, 1280, 720 );

		//	波紋描画
		RenderImage( waveImage, waveImage.sx, waveImage.sy, waveImage.sw, waveImage.sh, IMAGE_MODE::WAVE );

		//	顔画像描画
		RenderImage( faceImage, faceImage.sx, faceImage.sy, faceImage.sw, faceImage.sh, IMAGE_MODE::ADOPTPARAM );

		//	プレイヤー番号描画
		RenderImage( playerNumImage, playerNumImage.sx, playerNumImage.sy, playerNumImage.sw, playerNumImage.sh, IMAGE_MODE::ADOPTPARAM );
	}

	//	PressButtonRender
	void	sceneResult::PressButtonImageRender( void )
	{
		RenderImage( pressButtonImage, pressButtonImage.sx, pressButtonImage.sy, pressButtonImage.sw, pressButtonImage.sh, IMAGE_MODE::FLASH );
	}

	//	大砲描画
	void	sceneResult::CannonRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetCharacterType( value ) == CHARACTER_TYPE::PIRATE )
			{
				//	トゥーン輪郭線色設定
				shader3D->SetValue( "OutlineColor", gameManager->GetPlayerColor( value ) );
				cannon[value]->Render( shader3D, "toon" );
			}
		}
	}

	//	全プレイヤー顔描画
	void	sceneResult::AllPlayerFaceRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( faceImageObj[value], faceImageObj[value].sx, faceImageObj[value].sy, faceImageObj[value].sw, faceImageObj[value].sh, IMAGE_MODE::NORMAL );
		}
	}

	//	全プレイヤー番号描画
	void	sceneResult::AllPlayerNumRender( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( playerNumImageObj[value], playerNumImageObj[value].sx, playerNumImageObj[value].sy, playerNumImageObj[value].sw, playerNumImageObj[value].sh, IMAGE_MODE::NORMAL );
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	ライトと画面のあたり判定
	bool	sceneResult::WallLightCheck( int num )
	{
		if (lightPos[num].x - lightSize[num] <= 0.0f ||		//	左
			lightPos[num].x + lightSize[num] >= iexSystem::ScreenWidth ||		//	右
			lightPos[num].y - lightSize[num] <= 0.0f ||		//	上
			lightPos[num].y + lightSize[num] >= iexSystem::ScreenHeight)		//	下
		{
			return true;
		}
			return false;

	}

	//	画面上のランダムポジションを獲得
	Vector3	sceneResult::RandomPos( void )
	{
		return Vector3( Random::GetFloat( 0, static_cast<float>( iexSystem::ScreenWidth ) ), Random::GetFloat( 0, static_cast<float>( iexSystem::ScreenHeight ) ), 0 );
	}

	//	ライトの情報設定
	bool	sceneResult::MoveLightSet( int num, Vector3 pos )
	{
		light_t[num] = 0.0f;
		lightMove_start[num] = lightPos[num];
		lightMove_finish[num] = pos;

		return false;
	}

	//	ライトの移動
	void	sceneResult::MoveLight( void )
	{
		FOR(0, 4)
		{
			Lerp(lightPos[value], lightMove_start[value], lightMove_finish[value], light_t[value]);
			if ((WallLightCheck(value) == true || light_t[value] >= 1.0f) && waitTimer > 30)
			{
				MoveLightSet(value, RandomPos());
			}
		}
		//	残り30秒に初期位置へ移動
		if (waitTimer == 30)
		{
			MoveLightSet(0, LIGHT_POS[0]);
			MoveLightSet(1, LIGHT_POS[1]);
			MoveLightSet(2, LIGHT_POS[2]);
			MoveLightSet(3, LIGHT_POS[3]);
		}

	}

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
		if ( rouletteInfo.step > PLAYER_MAX )		return	true;

		//	画面切り替え終了後タイマー加算
		if ( screen->GetScreenState() )	rouletteInfo.timer++;

		for ( int i = rouletteInfo.step; i < PLAYER_MAX; i++ )
		{
			//	適当な数値を入れる
			SetNumberImageInfo( numberImageInfo[i], number[i], Random::GetInt( 100, 999 ) );
		}

		//	初回だけ回転時間ちょっと長く
		if ( rouletteInfo.step == 0 )
		{
			if ( rouletteInfo.timer % 90 == 0 )
			{
				rouletteInfo.step++;
				sound->PlaySE( SE::RESULT_JAN );
				rouletteInfo.timer = 1;
			}
		}
		else
		{
			if ( rouletteInfo.timer % 50 == 0 )
			{
				if ( rouletteInfo.step != PLAYER_MAX )sound->PlaySE(SE::RESULT_JAN);
				rouletteInfo.step++;
				rouletteInfo.timer = 1;
			}
		}

		//	数値決定
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( i < rouletteInfo.step )
			{
				SetNumberImageInfo( numberImageInfo[i], number[i], originInfo[i].num );
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
		bool	screenState = screen->GetScreenState();
		if ( !screenState )	return;

		//	現在のラウンドを取得
		int	round = gameManager->GetRound();

		if ( round != Round::ROUND_FINAL )
		{
			//	再戦用の設定
			FOR( 0, PLAYER_MAX )
			{
				//	現ラウンドの獲得コイン枚数を設定
				gameManager->SetTotalCoin( round, value, gameManager->GetCoinNum( value ) );
			}
			gameManager->RetryInitialize();
			gameManager->SetRound( round + 1 );

			//	メインへ
			MainFrame->ChangeScene( new sceneLoad( new sceneMain() ) );
			return;
		}
		else
		{
			switch ( menuInfo.select )
			{
			case MENU::MOVE_MENU:
				MainFrame->ChangeScene( new sceneMenu() );
				return;
				break;

			case	MENU::MOVE_TITLE:
				MainFrame->ChangeScene( new sceneTitle() );
				return;
				break;
			}
		}
	}

	//	次回ライフ設定
	void	sceneResult::SetNextLife( void )
	{
		int	rank;
		int	maxLife = gameManager->GetMaxLife();
		FOR( 0, PLAYER_MAX )
		{
			rank = originInfo[value].rank;
			gameManager->SetStartLife( value, nextLife[maxLife][rank] );
		}
	}

	//	カーテン状態設定
	void	sceneResult::SetCurtainMode( int mode )
	{
		curtainMode = mode;
		curtain_t = 0.0f;
	}

	//	待ちタイマー設定
	void	sceneResult::SetWaitTimer( int time )
	{
		waitTimer = time;
	}

	//	待ちタイマー減算
	bool	sceneResult::WaitTimeUpdate( void )
	{
		waitTimer--;
		if ( waitTimer <= 0 )
		{
			waitTimer = 0;
			return	true;
		}

		return	false;
	}

	//	ライフ発表
	bool	sceneResult::NextLifeAnnouncing( void )
	{
		//	変数準備
		bool	isEnd = false;

		//	パラメータ更新
		lifeInfo.t += 0.03f;
		if ( lifeInfo.t >= 1.0f )	lifeInfo.t = 1.0f;

		switch ( lifeInfo.step )
		{
		//	ライフ発表テキスト表示
		case 0:
			lifeAnnounceImage.renderflag = true;
			isEnd = Lerp( lifeAnnounceImage.alpha, 0.0f, 1.0f, lifeInfo.t );
			Lerp( lifeAnnounceImage.w, 0, 500, lifeInfo.t );
			Lerp( lifeAnnounceImage.h, 0, 400, lifeInfo.t );

			if ( isEnd )
			{
				lifeInfo.step++;
				lifeInfo.t = 0.0f;
			}
			break;

		//	ランク消去
		case 1:
			//	透明度更新
			FOR( 0, PLAYER_MAX )
			{
				Lerp( rankImage[value].alpha, 1.0f, 0.0f, lifeInfo.t );
			}

			if ( lifeInfo.t >= 1.0f )
			{
				//	順位非表示＆ライフ描画開始
				FOR( 0, PLAYER_MAX )
				{
					rankImage[value].renderflag = false;
					lifeImage[value].renderflag = true;
				}
				lifeInfo.t = 0.0f;

				//	つぎのステップへ
				lifeInfo.step++;
			}
			break;

		case 2:
			if ( lifeInfo.t >= 1.0f )
			{
				FOR( 0, PLAYER_MAX )	SetWave( lifeImage[value], 1.0f );
				lifeInfo.step++;
			}
			//	ライフ出現
			FOR( 0, PLAYER_MAX )
			{
				Lerp( lifeImage[value].w, 0, lifeInfo.maxW, lifeInfo.t );
				Lerp( lifeImage[value].h, 0, lifeInfo.maxH, lifeInfo.t );
			}
			break;

		case 3:
			FOR( 0, PLAYER_MAX )
			{
				isEnd = WaveUpdate( lifeImage[value] );
			}
			if ( isEnd )
			{
				lifeInfo.t = 0.0f;
				lifeInfo.step++;
			}
			break;

		case 4:
			SetNextLife();
			//	ライフ情報更新
			FOR( 0, PLAYER_MAX )
			{
				lifeInfo.culLife = gameManager->GetStartLife( value );
				lifeImage[value].sx = lifeImage[value].sw * ( ( 5 - lifeInfo.culLife ) % 4 );
				lifeImage[value].sy = lifeImage[value].sh * ( ( 5 - lifeInfo.culLife ) / 4 );
			}
			//lifeAnnounceImage.renderflag = false;

			isEnd = Lerp( lifeAnnounceImage.alpha, 1.0f, 0.0f, lifeInfo.t );

			if ( isEnd )
			{
				step = RESULT_MODE::INPUT_WAIT;
				return	true;
			}
			break;
		}

		return	false;
	}

	//	演出スキップ
	void	sceneResult::ProductionSkip( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			SetNumberImageInfo( numberImageInfo[value], number[value], originInfo[value].num );
			SetWave( rankImage[value], 1.5f );
			rankImage[value].renderflag = true;
		}
		step = RESULT_MODE::RANK_SKIP;
	}

	//	大砲位置設定
	void	sceneResult::SetCannonPos( void )
	{
		Matrix	mat;
		Matrix	cannonMat;
		Vector3	up = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	cannonPos = Vector3( 0.0f, 0.0f, 0.0f );
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetCharacterType( value ) != CHARACTER_TYPE::PIRATE )	continue;
			mat = *obj[value]->GetBone( 7 ) * obj[value]->TransMatrix;
			cannon[value]->TransMatrix = mat;
			up = Vector3( mat._21, mat._22, mat._23 );
			up.Normalize();
			cannonPos = Vector3( cannon[value]->TransMatrix._41, cannon[value]->TransMatrix._42, cannon[value]->TransMatrix._43 );
			cannonPos += up * 0.5f;
			cannon[value]->TransMatrix._41 = cannonPos.x;
			cannon[value]->TransMatrix._42 = cannonPos.y;
			cannon[value]->TransMatrix._43 = cannonPos.z;
		}
	}

//----------------------------------------------------------------------------
//	リザルトモード関数
//----------------------------------------------------------------------------

	//	ルーレット
	void	sceneResult::ModeRoulette( void )
	{
		bool	isEnd = false;

		isEnd = Roulette();

		//	決定ボタンでスキップ
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
		{
			ProductionSkip();
		}

		//	回転が終了したら
		if ( isEnd )		step = RESULT_MODE::RANK;
	}

	//	ランク発表
	void	sceneResult::ModeRank( void )
	{
		bool	isEnd = false;
		bool	isFinViewRankInOrder = false;
		bool	isEndWave = false;
		int	round = gameManager->GetRound();

		//-----------------------------------------------------------------------------------
		//	順位発表
		//-----------------------------------------------------------------------------------
		isFinViewRankInOrder = ViewRankInOrder();
		isEnd = RankWave();

		//	波紋終了後に選択可
		if ( isFinViewRankInOrder && isEnd )
		{
			pressButtonImage.flashingRenderflag = true;
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else															step = RESULT_MODE::LAST_RESULT;
				pressButtonImage.flashingRenderflag = false;
			}
		}
		else
		{
			//	決定ボタンでスキップ
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				ProductionSkip();
			}
		}
	}

	//	最終ランク発表
	void	sceneResult::LastRank( void )
	{
		bool	isEnd = false;

		faceImage.renderflag = true;
		playerNumImage.renderflag = true;

		lastAnnounceInfo.t += 0.03f;
		if ( lastAnnounceInfo.t >= 1.0f )	lastAnnounceInfo.t = 1.0f;

		switch ( lastAnnounceInfo.step )
		{
		case 0:
			//	拡大
			CubicFunctionInterpolation( faceImage.w, 0, 700, lastAnnounceInfo.t );
			isEnd = CubicFunctionInterpolation( faceImage.h, 0, 700, lastAnnounceInfo.t );
			if ( isEnd )
			{
				sound->PlaySE( SE::RESULT_JAN );
				SetWave( waveImage, 1.0f );
				lastAnnounceInfo.t = 0.0f;
				lastAnnounceInfo.step++;
			}
			break;

		case 1:
			isEnd = WaveUpdate( waveImage );
			if ( isEnd )
			{
				lastAnnounceInfo.t = 0.0f;
				lastAnnounceInfo.step++;
			}
			break;

		case 2:
			//	拡大
			Lerp( playerNumImage.w, 0, 200, lastAnnounceInfo.t );
			isEnd = Lerp( playerNumImage.h, 0, 200, lastAnnounceInfo.t );
			if ( isEnd )
			{
				pressButtonImage.flashingRenderflag = true;
				if ( input[0]->Get( KEY_B ) == 3 || input[0]->Get( KEY_SPACE ) == 3 )
				{
					screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 2.0f );
					pressButtonImage.flashingRenderflag = false;
					lastAnnounceInfo.step++;
				}
			}
			break;

		case 3:
			if ( screen->GetScreenState() )
			{
				step = LASTRESULT_MODE::FINALRESULT;
				screen->SetScreenMode( SCREEN_MODE::FADE_IN, 2.0f );
			}
			break;
		}

	}

	//	ランク発表スキップ
	void	sceneResult::ModeRankSkip( void )
	{
		bool	isEnd = false;
		int	round = gameManager->GetRound();

		FOR( 0, PLAYER_MAX )	isEnd = WaveUpdate( rankImage[value] );

		//	波紋終了後に選択可
		if ( isEnd )
		{
			pressButtonImage.flashingRenderflag = true;
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_B ) == 3 )
			{
				pressButtonImage.flashingRenderflag = false;
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else
				{
					step = RESULT_MODE::LAST_RESULT;
				}
			}
		}
	}

	//	ラスト結果発表
	void	sceneResult::ModeLastResult( void )
	{
		static int result_step = 0;

		switch (result_step)
		{
		case LASTRESULT_MODE::SET_CLOSE_CURTAIN:
			SetCurtainMode(CURTAIN_MODE::CLOSE);
			sound->PlaySE( SE::DRAMROLL_SE );
			result_step++;
			break;

		case LASTRESULT_MODE::CLOSE_CURTAIN:
			if ( curtainState )
			{
				//sound->StopSE( SE::DRAMROLL_SE );
				SetWaitTimer(150);
				step = 0;
				mode = MOVE_MODE::LAST_RESULT;
				result_step = 0;
			}
			break;
		}
	}

	//	入力待ち
	void	sceneResult::ModeInputWait( void )
	{
		bool	isEnd = false;
		pressButtonImage.flashingRenderflag = true;
		FOR( 0, PLAYER_MAX )
		{
			//	入力を受け付けていたらスキップ
			if ( inputCheck[value] )	continue;
	
			//	キー入力取得
			int key_space = input[value]->Get( KEY_SPACE );
			int key_a = input[value]->Get( KEY_B );

			//	入力受付
			if ( key_space == 3 || key_a == 3 )
			{
				if ( !inputCheck[value] )	sound->PlaySE( SE::DECIDE_SE );
				inputCheck[value] = true;
				checkImage[value].renderflag = true;
				SetWave( checkImage[value], 1.0f );
			}
		}

		isEnd = InputCheckImageUpdate();

		//	全員の入力が終わっていたら
		if ( isEnd )
		{
			pressButtonImage.flashingRenderflag = false;
			screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
			changeScene = true;
		}
	}

//----------------------------------------------------------------------------
//	選択モード関数
//----------------------------------------------------------------------------

	//	幕下ろし
	bool	sceneResult::DownPolygon( void )
	{
		bool	isEnd = false;
		menuInfo.t += 0.03f;
		if ( menuInfo.t >= 1.0f )
		{
			menuInfo.t = 1.0f;

			//	メニュー項目描画
			static	int	MENU_MAX = 3;	//	メニュー項目数
			for ( int i = 0; i < MENU_MAX; i++ )		menuImage[i].renderflag = true;
		}
		isEnd = Lerp( menuInfo.screenH, 0, static_cast<int>( iexSystem::ScreenHeight ), menuInfo.t );

		return	isEnd;
	}

	//	カーソル操作
	void	sceneResult::ControlCursor( void )
	{
		//	上下で選択
		int	keyUp = input[0]->Get( KEY_UP );
		int	keyDown = input[0]->Get( KEY_DOWN );
		if ( keyUp == 3 )
		{
			sound->PlaySE( SE::CHOICE_SE );
			menuInfo.select--;
		}
		if ( keyDown == 3 )
		{
			sound->PlaySE( SE::CHOICE_SE );
			menuInfo.select++;
		}
		if ( menuInfo.select < 0 )							menuInfo.select = MENU::END - 1;
		if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::MOVE_MENU;

		//	読み込み位置変更
		int	textMax = 2;
		FOR( 0, textMax )
		{
			if ( value == menuInfo.select )	menuImage[value].sx = 512;
			else												menuImage[value].sx = 0;
		}

		// 決定ボタンでシーン移行フラグを立てる
		int keySpace = input[0]->Get( KEY_SPACE );
		int keyA = input[0]->Get(KEY_B);
		
		//	決定キーで決定＆シーン移行
		if ( !changeScene )
		{
			if ( keySpace == 3 || keyA == 3 )
			{
				sound->PlaySE( SE::DECIDE_SE );
				switch ( menuInfo.select )
				{
				case MENU::MOVE_MENU:
					screen->SetScreenMode( SCREEN_MODE::WIPE_OUT, 1.0f );
					break;

				case MENU::MOVE_TITLE:
					screen->SetScreenMode( SCREEN_MODE::FADE_OUT, 1.0f );
					break;
				}
				changeScene = true;
			}
		}
	}
	