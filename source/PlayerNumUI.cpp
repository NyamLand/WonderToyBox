
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"PlayerNumUI.h"

//***********************************************************************
//
//	PlayerNumUIクラス
//
//***********************************************************************

//------------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------------
#define	PLAYER_NUM_DIST	50

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------

	//	コンストラクタ
	PlayerNumUI::PlayerNumUI( void ) : orgPlayerNum( nullptr )
	{
		FOR( 0, PLAYER_MAX )
		{
			playerNumImage[value].obj = nullptr;
		}
	}

	//	デストラクタ
	PlayerNumUI::~PlayerNumUI( void )
	{
		Release();
	}

	//	初期化
	bool	PlayerNumUI::Initialize( void )
	{
		Load();

		FOR( 0, PLAYER_MAX )
		{
			//	構造体初期化
			ImageInitialize( playerNumImage[value], 
				0, 0, 40, 40, 
				128 * ( value % 2 ),
				128 * ( value / 2 ), 
				128,	128 );

			//	画像設定
			playerNumImage[value].obj = orgPlayerNum;

			if ( playerNumImage[value].obj == nullptr )	return	false;
		}

		return	true;
	}

	//	解放
	void	PlayerNumUI::Release( void )
	{
		SafeDelete( orgPlayerNum );
	}

	//	読み込み
	void	PlayerNumUI::Load( void )
	{
		orgPlayerNum = new iex2DObj( "DATA/UI/playerNum.png" );
	}

//------------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------------

	//	更新
	void	PlayerNumUI::Update( void )
	{
		Vector3	p_Pos;
		Vector3	p_Up;
		Vector3	crownPos;
		Vector3	out;

		FOR( 0, PLAYER_MAX )
		{
			//	表示座標算出
			p_Pos = characterManager->GetPos( value );
			p_Up = characterManager->GetUp( value );
			crownPos = p_Pos + p_Up * 5.0f;
			WorldToClient( crownPos, out, matView *matProjection );

			//	描画位置設定
			playerNumImage[value].x = static_cast<int>( out.x );
			playerNumImage[value].y = static_cast<int>( out.y - PLAYER_NUM_DIST );
		}
	}

	//	描画
	void	PlayerNumUI::Render( void )
	{
		FOR( 0, PLAYER_MAX )
		{
			RenderImage( playerNumImage[value], 
				playerNumImage[value].sx, playerNumImage[value].sy, 
				playerNumImage[value].sw, playerNumImage[value].sh, 
				IMAGE_MODE::NORMAL );
		}
	}

//------------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//	情報設定
//------------------------------------------------------------------------------------

	//	座標設定
	void	PlayerNumUI::SetPos( int playerNum, int posX, int posY )
	{

	}

//------------------------------------------------------------------------------------
//	情報取得
//------------------------------------------------------------------------------------


