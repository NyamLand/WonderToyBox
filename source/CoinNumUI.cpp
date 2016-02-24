
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"NumberUI.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinUIEffect.h"
#include	"CoinNumUI.h"

//**********************************************************************
//
//	CoinNumUIクラス
//
//**********************************************************************

//-------------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------------

#define	NUMBER_SCALE	100

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------

	//	コンストラクタ
	CoinNumUI::CoinNumUI( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			numberUI[value] = nullptr;
		}
	}

	//	デストラクタ
	CoinNumUI::~CoinNumUI( void )
	{
		Release();
	}

	//	初期化
	bool	CoinNumUI::Initialize( void )
	{
		//	座標、色テーブル初期化
		TableInitialize();

		//	エフェクト初期化
		coinUIEffect = new CoinUIEffect();
		coinUIEffect->Initialize();

		//	情報初期化
		FOR( 0, PLAYER_MAX )
		{
			//	コイン枚数初期化
			coinNum[value] = gameManager->GetCoinNum( value );

			//	数値クラス初期化
			numberUI[value] = new NumberUI();
			numberUI[value]->Initialize( posXTable[value],
				static_cast<int>( iexSystem::ScreenHeight * 0.1f ),
				NUMBER_SCALE,
				coinNum[value] );

			//	エフェクトクラスに座標を渡す
			coinUIEffect->SetPos( value, 
				numberUI[value]->GetPosX(), 
				numberUI[value]->GetPosY() );

			if ( numberUI[value] == nullptr )		return	false;
		}

		return	true;
	}

	//	解放
	void	CoinNumUI::Release( void )
	{
		FOR( 0, PLAYER_MAX )	SafeDelete( numberUI[value] );
	}

	//	読み込み
	void	CoinNumUI::Load( void )
	{

	}

	//	情報テーブル初期化
	void	CoinNumUI::TableInitialize( void )
	{
		//	座標テーブル初期化
		posXTable[0] = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		posXTable[1] = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		posXTable[2] = static_cast<int>( iexSystem::ScreenWidth * 0.7f );
		posXTable[3] = static_cast<int>( iexSystem::ScreenWidth * 0.9f );

		//	表示色初期化
		FOR( 0, PLAYER_MAX )
		{
			colorTable[value] = characterManager->GetDamageColor( value );
		}

		//	見やすくするため少し明るく
		colorTable[1] += Vector3( 0.7f, 0.7f, 0.0f );	
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	CoinNumUI::Update( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( gameManager->GetTimer() / SECOND < 15 )
			{
				numberUI[value]->SetSecretFlag( true );
			}
			CoinCount( gameManager->GetCoinNum( value ), value );
			numberUI[value]->SetNumber( coinNum[value] );
			numberUI[value]->Update();
			numberUI[value]->SetColor( colorTable[value] );
		}
	}

	//	エフェクト更新
	void	CoinNumUI::CoinEffectUpdate( void )
	{
		coinUIEffect->Update();
	}

	//	描画
	void	CoinNumUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			numberUI[value]->Render();
			coinUIEffect->Render( value );
		}

	}

	//	エフェクトバックバッファ描画
	void	CoinNumUI::RenderToBackBuffer( void )
	{
		coinUIEffect->RenderToBackBaffer();
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------

	//	コイン枚数を一枚ずつカウントアップダウン( コイン枚数、プレイヤー番号 )
	void	CoinNumUI::CoinCount( int coinNum, int playerNum )
	{
		//	数値に変化がなければスルー
		if ( this->coinNum[playerNum] == coinNum )		return;

		//	コイン減少
		if ( this->coinNum[playerNum] > coinNum )	this->coinNum[playerNum]--;

		//	コイン増加
		if ( this->coinNum[playerNum] < coinNum )
		{
			this->coinNum[playerNum]++;
			coinUIEffect->SetEffectFlag( playerNum, true );
		}
	}
	