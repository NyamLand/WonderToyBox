
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"PlayerManager.h"

//****************************************************************************************
//
//	PlayerManagerクラス
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------

	//	実体の宣言
	PlayerManager*		m_Player = NULL;

//------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------

	//	コンストラクタ
	PlayerManager::PlayerManager( void )
	{
	
	}
	
	//	デストラクタ
	PlayerManager::~PlayerManager( void )
	{
		for (int i = 0; i < PLAYER_NUM; i++){
			SafeDelete( c_Player[i] );
		}
	}

	//	初期化
	void	PlayerManager::Initialize( int input, int type, Vector3 pos )
	{
		c_Player[input] = new Player();
		c_Player[input]->Initialize( input, type, pos );
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	PlayerManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ ){
			//	各プレイヤー更新
			c_Player[i]->Update();
		}

		//	当たり判定
		HitCheck();
	}

	//	描画
	void	PlayerManager::Render( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			c_Player[i]->Render();
			DrawSphere( c_Player[i]->GetPos(), 2.0f, 0xFF000000 );
		}
	}

	//	シェーダー付描画
	void	PlayerManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			c_Player[i]->Render( shader, technique );
			DrawSphere( c_Player[i]->GetPos(), 2.0f, 0xFF000000 );
		}
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------

	//	当たり判定
	void	PlayerManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			for ( int n = 0; n < PLAYER_NUM; n++ )
			{
				if ( c_Player[i]->GetAttackParam() != 0 )
				{
					//	自分だったら次へ
					if ( i == n )	continue;

					//	当たり判定(今はとりあえず球で取る)
					static float	dist = 2.0f;
					if ( Collision::DistCheck( c_Player[i]->GetPos(), c_Player[n]->GetPos(), dist ) )
					{
						//	とりあえずエフェクト出す
						Vector3	p_pos = c_Player[n]->GetPos();
						float	effectScale = 2.0f;
						Effect1( p_pos.x, p_pos.y, p_pos.z, effectScale );
					}
				}
			}
		}
	}

//------------------------------------------------------------------------------
//	情報取得・設定
//------------------------------------------------------------------------------

	//	情報取得
	Vector3	PlayerManager::GetPos( int player ){	return	c_Player[player]->GetPos();	}
	Matrix	PlayerManager::GetMatrix( int player ){ return	c_Player[player]->GetMatrix(); }
	float		PlayerManager::GetAngle( int player ){ return		c_Player[player]->GetAngle(); }

	//	情報設定
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }