
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/Framework.h"
#include	"system/System.h"
#include	"Random.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"CharacterManager.h"
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
		screen->Initialize();
		screen->SetScreenMode( SCREEN_MODE::FADE_IN , 1.5f );

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

		//	ラストボーナス関連構造体初期化
		LastBonusImageInitialize();

		//	ルーレット情報初期化
		RouletteInfoInitialize();

		//	メニュー関連初期化
		MenuInfoInitialize();

		//	レンダーターゲット関連初期化
		RenderTargetTextureInitialize();

		//	ラウンド関連初期化
		RoundInfoInitialize();
		SetRoundCoinNumber();

		//	変数初期化
		lastBonus = 0;
		step = 0;
		mode = MOVE_MODE::RESULT;
		changeScene = false;
		bonusPlayer = 0;
		
		FOR( 0, PLAYER_MAX )	inputCheck[value] = false;

		sound->PlayBGM(BGM::RESULT_BGM);
		screen->SetScreenMode( SCREEN_MODE::FADE_IN, 1.0f );
		return	true;
	}

	//	解放
	void	sceneResult::Release( void )
	{
		SafeDelete( orgCurtain );
		SafeDelete( orgRound );
		SafeDelete( life );
		SafeDelete( menuHead.obj );
		SafeDelete( originNumber );
		SafeDelete( menuText );
		SafeDelete( mainView );
		SafeDelete( infoScreen );
		SafeDelete( lastResultTest );
		SafeDelete( lastBonusText );
		SafeDelete( faceImage.obj );
		SafeDelete( playerNumImage.obj );
		SafeDelete( waveCircleImage.obj );
		SafeDelete( notApplicable.obj );
		SafeDelete( bgStage );
		SafeDelete( check );
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
		lastBonusText = new iex2DObj("DATA/UI/Result/LastBonusText.png");
		life = new iex2DObj("DATA/UI/Nlife.png");
		check = new iex2DObj( "DATA/UI/Result/check.png" );
		orgRound = new iex2DObj( "DATA/UI/roundText.png" );
		orgCurtain = new iex2DObj( "DATA/UI/title/curtain1.png" );

		//	構造体初期化
		int x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int y = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		int w = static_cast<int>( iexSystem::ScreenWidth * 0.29f );
		int h = static_cast<int>( iexSystem::ScreenHeight * 0.2f );
		ImageInitialize( menuHead, x, y, w, h, 0, 0, 512, 256 );
		menuHead.angle = D3DXToRadian( 7.0f );

		//	モデル読み込み
		org[CHARACTER_TYPE::SCAVENGER] = make_unique<iex3DObj>(LPSTR("DATA/CHR/majo/majo.IEM"));			//	掃除屋
		org[CHARACTER_TYPE::PRINCESS] = make_unique<iex3DObj>(LPSTR("DATA/CHR/プリンセス/prinsess1.IEM"));					//	姫
		org[CHARACTER_TYPE::THIEF] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Thief/Thief.IEM"));				//	リス
		org[CHARACTER_TYPE::PIRATE] = make_unique<iex3DObj>(LPSTR("DATA/CHR/Pirate/Pirate.IEM"));					//	トラ

		//	ステージ設定
		bgStage = new iexMesh("DATA/BG/MenuStage/menustage.IMO");	//	ステージ
		bgStage->SetScale(0.1f);
		bgStage->SetAngle(D3DX_PI);
		bgStage->Update();

		//	オリジナルモデル情報初期化
		org[CHARACTER_TYPE::SCAVENGER]->SetScale(0.01f);	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetScale(0.04f);		//	姫
		org[CHARACTER_TYPE::THIEF]->SetScale(0.03f);				//	怪盗
		org[CHARACTER_TYPE::PIRATE]->SetScale(0.02f);				//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetAngle(D3DX_PI);	//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetAngle(D3DX_PI);		//	姫
		org[CHARACTER_TYPE::THIEF]->SetAngle(D3DX_PI);				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetAngle(D3DX_PI);			//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->SetMotion(2);		//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->SetMotion(1);			//	姫
		org[CHARACTER_TYPE::THIEF]->SetMotion(0);	 				//	シーフ
		org[CHARACTER_TYPE::PIRATE]->SetMotion(0);					//	トラ

		org[CHARACTER_TYPE::SCAVENGER]->Update();				//	掃除屋
		org[CHARACTER_TYPE::PRINCESS]->Update();					//	姫
		org[CHARACTER_TYPE::THIEF]->Update();							//	シーフ
		org[CHARACTER_TYPE::PIRATE]->Update();						//	トラ

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
		viewInfo.pos = Vector3( 0.0f, 15.0f, -10.0f );
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
			obj[i]->SetPos( -7.0f + ( 14.0f / 3.0f * i ), 10.0f, 10.0f );
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
			w = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
			h = w;
			sw = 64;
			sh = 64;
			sx = sw * ( ( 5 - culLife ) % 4 );
			sy = sh * ( ( 5 - culLife ) / 4 );
			ImageInitialize( lifeImage[value], x, y, w, h, sx, sy, sw, sh );

			//	最初は非表示
			lifeImage[value].renderflag = false;

			//	ついでにOKの位置も設定
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

	//	ラストボーナス関連構造体初期化
	void	sceneResult::LastBonusImageInitialize( void )
	{
		//	ラストボーナス関連初期化
		FOR( 0, PLAYER_MAX )	lastBonusInfo.bonus[value];
		lastBonusInfo.step = 0;
		lastBonusInfo.t = 0.0f;

		//	テキスト初期化
		int		x = static_cast<int>( iexSystem::ScreenWidth * 0.5f );
		int		y = static_cast<int>( iexSystem::ScreenHeight * 0.4f );
		int		w = static_cast<int>( iexSystem::ScreenWidth * 0.2f );
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
		light_t = 0.0f;
		lightPos[0] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[1] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[2] = Vector3( 0.0f, 0.0f, 0.0f );
		lightPos[3] = Vector3( 0.0f, 0.0f, 0.0f );

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

		FOR( 0, Round::END ) 
		{
			y = static_cast<int>( iexSystem::ScreenHeight * 0.3f ) + ( static_cast<int>( iexSystem::ScreenHeight * 0.2f ) * value );
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

		//	各ラウンドのコイン枚数を取得
		FOR( 0, PLAYER_MAX )
		{
			for ( int round = 0; round < Round::END; round++ ) 
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
			}
		}
	}

//----------------------------------------------------------------------------
//	全体更新・全体描画
//----------------------------------------------------------------------------

	//	更新
	void	sceneResult::Update( void ) 
	{
		//	カーテン更新
		curtainState = CurtainUpdate();

		//	スクリーン更新
		screen->Update();

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

		case MOVE_MODE::LAST_RESULT:
			LastResultUpdate();
			break;
		}

		//	シーン移動管理
		MoveScene();
	}

	//	描画
	void	sceneResult::Render( void ) 
	{

		switch ( mode )
		{
		case MOVE_MODE::RESULT:
		case MOVE_MODE::SELECT:
			ResultRender();
			break;
		case MOVE_MODE::LAST_RESULT:
			LastResultRender();
			lastResultTest->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight );
			break;
		}

		//	カーテン描画
		CurtainRender();

		//	最終結果テスト
		SelectRender();
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	リザルト時の更新
	void	sceneResult::ResultUpdate( void )
	{
		//	段階ごとの処理
		switch ( step )
		{
		case RESULT_MODE::ROULETTE:
			ModeRoulette();
			break;
		
		case RESULT_MODE::LASTBONUS:
			ModeLastBonus();
			break;
			
		case RESULT_MODE::RANK:
			ModeRank();
			break;

		case	RESULT_MODE::LIFE:
			NextLifeAnnouncing();
			break;

		case	RESULT_MODE::LAST_RESULT:
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
			SetCurtainMode( CURTAIN_MODE::CLOSE );
			step++;
			break;

		case LASTRESULT_MODE::CLOSE_CURTAIN:
			if ( curtainState ) SetWaitTimer( 60 );
			step = LASTRESULT_MODE::LIGHT_PRODUCTION;
			break;

		case LASTRESULT_MODE::LIGHT_PRODUCTION:
			isEnd = WaitTimeUpdate();
			if ( isEnd )
			{
				step = LASTRESULT_MODE::OPEN_CURTAIN;
				SetCurtainMode( CURTAIN_MODE::OPEN );
			}
			break;

		case LASTRESULT_MODE::OPEN_CURTAIN:
			if ( curtainState )
			{
				if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) )
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
		//	レンダーターゲットを設定
		infoScreen->RenderTarget( 0 );

		//	カメラ
		view2D->Activate();
		view2D->Clear();

		//	背景( 一番後ろに表示 )
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

		//	リザルトシール描画
		RenderImage(menuHead, menuHead.sx, menuHead.sy, menuHead.sw, menuHead.sh, IMAGE_MODE::ADOPTPARAM);

		//	数値描画
		NumberImageRender();

		//	順位描画
		RankRender();

		//	ライフ描画
		LifeRender();

		//	入力チェック描画
		InputCheckRender();

		//	フレームバッファへ切り替え
		iexSystem::GetDevice()->SetRenderTarget(0, backBuffer);
		mainView->Activate();
		mainView->Clear();

		//	ステージ描画
		bgStage->Render();

		//	ポリゴン描画
		iexPolygon::Render3D(viewInfo.v, 2, infoScreen, shader3D, "alpha");
		
		//	プレイヤー描画
		FOR( 0, PLAYER_MAX )
		{
			//	プレイヤー描画
			obj[value]->Render( shader3D, "toon" );
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
		//	ランク描画
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
			RenderImage( checkImage[value], checkImage[value].sx, checkImage[value].sy, checkImage[value].sw, checkImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
	}

	//	ラスト発表描画
	void sceneResult::LastResultRender( void )
	{
		//-------------UI関連---------------//
		//	別のテクスチャに書き出し
		lastResultTest->RenderTarget();
		viewTest->Activate();
		viewTest->Clear();

		//	背景描画
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		back->Render( 0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1280, 720 );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		
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

		//	フレームバッファへ切り替え
		iexSystem::GetDevice()->SetRenderTarget( 0, backBuffer );
		mainView->Activate();
		mainView->Clear();
	}

	//	カーテン描画
	void	sceneResult::CurtainRender( void )
	{
		LPSTR technique = "copy";

		if ( step == LASTRESULT_MODE::LIGHT_PRODUCTION )
		{
			technique = "SpotLight";
		}
		iexPolygon::Render2D( curtainInfoL.tlv, 2, curtainInfoL.obj, shader2D, technique );
		iexPolygon::Render2D( curtainInfoR.tlv, 2, curtainInfoR.obj, shader2D,technique );
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	ライト移動
	bool	sceneResult::LightUpdate( void )
	{
		bool isEnd = false;

		//	パラメータ加算
		light_t += 0.1f;
		if ( light_t >= 1.0f )	light_t = 1.0f;
		return	false;
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
				rouletteInfo.timer = 1;
			}
		}
		else
		{
			if ( rouletteInfo.timer % 50 == 0 )
			{
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
		//switch ( menuInfo.select )
		//{
		//case MENU::RESTART:
		//	//	ゲーム情報初期化
		//	gameManager->RetryInitialize();
		//	gameManager->SetRound( nextRound );
		//	MainFrame->ChangeScene( new sceneMain() );
		//	return;
		//	break;

		//case MENU::MOVE_MENU:
		//	MainFrame->ChangeScene( new sceneMenu() );
		//	return;
		//	break;

		//case MENU::MOVE_TITLE:
		//	MainFrame->ChangeScene( new sceneTitle() );
		//	return;
		//	break;
		//}
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

	//	ライフ発表
	bool	sceneResult::NextLifeAnnouncing( void )
	{
		//	変数準備
		bool	isEnd = false;

		switch ( lifeInfo.step )
		{
		case 0:
			lifeInfo.t += 0.01f;
			if ( lifeInfo.t >= 1.0f ) lifeInfo.t = 1.0f;

			//	情報適用
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

		case 1:
			lifeInfo.t += 0.1f;
			if ( lifeInfo.t >= 1.0f )
			{
				lifeInfo.t = 1.0f;
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

		case 2:
			FOR( 0, PLAYER_MAX )
			{
				isEnd = WaveUpdate( lifeImage[value] );
			}
			if ( isEnd )	lifeInfo.step++;
			break;

		case 3:
			SetNextLife();
			//	ライフ情報更新
			FOR( 0, PLAYER_MAX )
			{
				lifeInfo.culLife = gameManager->GetStartLife( value );
				lifeImage[value].sx = lifeImage[value].sw * ( ( 5 - lifeInfo.culLife ) % 4 );
				lifeImage[value].sy = lifeImage[value].sh * ( ( 5 - lifeInfo.culLife ) / 4 );
			}

			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				step = RESULT_MODE::INPUT_WAIT;
			}
			return	true;
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

//----------------------------------------------------------------------------
//	リザルトモード関数
//----------------------------------------------------------------------------

	//	ルーレット
	void	sceneResult::ModeRoulette( void )
	{
		bool	isEnd = false;

		isEnd = Roulette();

		//	決定ボタンでスキップ
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
		{
			ProductionSkip();
		}

		//	回転が終了したら
		if ( isEnd )		step = RESULT_MODE::RANK;
	}
	
	//	ラストボーナス発表
	void	sceneResult::ModeLastBonus( void )
	{
		bool	isEnd = false;
		//-----------------------------------------------------------------------------------
		//	ラストボーナス演出
		//-----------------------------------------------------------------------------------
		isEnd = LastBonusUpdate();
		if ( isEnd )	step++;
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
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				if ( round != Round::ROUND_FINAL )		step = RESULT_MODE::LIFE;
				else															step = RESULT_MODE::LAST_RESULT;
			}
		}
		else
		{
			//	決定ボタンでスキップ
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
				ProductionSkip();
			}
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
			if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
			{
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
		step = 0;
		mode = MOVE_MODE::LAST_RESULT;
	}

	//	入力待ち
	void	sceneResult::ModeInputWait( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			//	入力を受け付けていたらスキップ
			if ( inputCheck[value] )	continue;
	
			//	キー入力取得
			int key_space = input[value]->Get( KEY_SPACE );
			int key_a = input[value]->Get( KEY_A );

			//	入力受付
			if ( key_space == 3 || key_a == 3 )
			{
				checkImage[value].renderflag = true;
				inputCheck[value] = true;
			}
		}

		//	全員の入力が終わっていたら
		if ( inputCheck[0] && inputCheck[1] && inputCheck[2] && inputCheck[3] )
		{
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
		if ( input[0]->Get( KEY_UP ) == 3 )		menuInfo.select--;
		if ( input[0]->Get( KEY_DOWN ) == 3 )	menuInfo.select++;
		if ( menuInfo.select < 0 )							menuInfo.select = MENU::END - 1;
		if ( menuInfo.select >= MENU::END )		menuInfo.select = MENU::MOVE_MENU;

		//	読み込み位置変更
		for ( int i = 0; i < 2; i++ )
		{
			if ( i == menuInfo.select )	menuImage[i].sx = 512;
			else										menuImage[i].sx = 0;
		}

		//	決定
		if ( input[0]->Get( KEY_SPACE ) == 3 || input[0]->Get( KEY_A ) == 3 )
		{
			changeScene = true;
		}
	}
	