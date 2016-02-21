
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Crown.h"

//**********************************************************************
//
//	Crownクラス
//
//**********************************************************************

//------------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------------

#define	CROWN_DIST	64

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	Crown::Crown( void ) : orgCrown( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			state[value] = false;
			crownImage[value].obj = nullptr;
		}
	}

	//	デストラクタ
	Crown::~Crown( void )
	{
		Release();
	}

	//	初期化
	bool	Crown::Initialize( void )
	{
		//	読み込み
		Load();

		//	構造体初期化
		FOR( 0, PLAYER_MAX )
		{
			ImageInitialize( crownImage[value], 0, 0, 80, 80, 0, 0, 512, 512 );
			crownImage[value].obj = orgCrown;
			if ( crownImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	解放
	void	Crown::Release( void )
	{
		SafeDelete( orgCrown );
	}

	//	読み込み
	void	Crown::Load( void )
	{
		orgCrown = new iex2DObj( "DATA/UI/1stCrown.png" );
	}

//------------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------------

	//	更新
	void	Crown::Update( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	crownPos;
		Vector3	out;

		FOR( 0, PLAYER_MAX )
		{
			//	1位以外はとばす
			if ( characterManager->GetRank( value ) > 1 )
			{
				state[value] = false;
				continue;
			}

			//	表示状態へ
			state[value] = true;

			//	表示座標算出
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			crownPos = p_Pos + p_Up * 5.0f;
			WorldToClient( crownPos, out, matView *matProjection );

			//	描画位置設定
			crownImage[value].x = static_cast<int>( out.x );
			crownImage[value].y = static_cast<int>( out.y - CROWN_DIST );
		}
	}

	//	描画
	void	Crown::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			if ( state[value] )
			{
				RenderImage( crownImage[value], 
					crownImage[value].sx, crownImage[value].sy, 
					crownImage[value].sw, crownImage[value].sh, 
					IMAGE_MODE::NORMAL );
			}
		}
	}
