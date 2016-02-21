
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"LifeUI.h"

//**********************************************************************
//
//	LifeUIクラス
//
//**********************************************************************

//----------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------
#define	LIFE_MAX		5

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	LifeUI::LifeUI( void ) : orgLife( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			life[value] = 0;
			lifeImage[value].obj = nullptr;
		}
	}

	//	デストラクタ
	LifeUI::~LifeUI( void )
	{
		Release();
	}

	//	読み込み
	void	LifeUI::Load( void )
	{
		orgLife = new iex2DObj( "DATA/UI/NLife.png" );
	}

	//	初期化
	bool	LifeUI::Initialize( void )
	{
		//	画像読み込み
		Load();

		//	構造体初期化
		FOR( 0, PLAYER_MAX )
		{
			lifeImage[value].obj = orgLife;
			life[value] = gameManager->GetStartLife( value );
			ImageInitialize( lifeImage[value], 0, 0, 75, 75, 0, 0, 64, 64 );
			if( lifeImage[value].obj == nullptr )	return	false;	
		}

		return	true;
	}

	//	解放
	void	LifeUI::Release( void )
	{
		SafeDelete( orgLife );
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	LifeUI::Update( void )
	{
		//	変数準備
		Vector3	p_Pos, p_Up, lifePos, out;
		int			culLife;

		//	４人分設定
		FOR( 0, PLAYER_MAX )
		{
			//	表示座標算出
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			lifePos = p_Pos + p_Up * 5.0f;
			WorldToClient( lifePos, out, matView * matProjection );

			//	現在の体力取得
			culLife = characterManager->GetLife( value );

			//	描画位置決定
			lifeImage[value].x = static_cast<int>( out.x );
			lifeImage[value].y = static_cast<int>( out.y );

			//	読み込み位置設定
			lifeImage[value].sx = lifeImage[value] .sw * ( ( LIFE_MAX - culLife )  % 4 );
			lifeImage[value].sy = lifeImage[value].sh * ( ( LIFE_MAX - culLife ) / 4 );
		}
	}

	//	描画
	void	LifeUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( lifeImage[value], 
				lifeImage[value].sx, lifeImage[value].sy,
				lifeImage[value].sw, lifeImage[value].sh,
				IMAGE_MODE::NORMAL );
		}
	}
