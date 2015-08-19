
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Y2009.h"
#include	"ECCMAN.h"
#include	"Princess.h"
#include	"Squirrel.h"
#include	"Knight.h"
#include	"GameManager.h"

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
		for ( int i = 0; i < PLAYER_NUM; i++ ){
			SafeDelete( c_Player[i] );
		}

		for ( int i = 0; i < OBJ_MAX; i++ )
		{
			SafeDelete( org[i] );
		}
	}

	//	初期化
	void	PlayerManager::Initialize( int input, int type, Vector3 pos )
	{
		switch ( type )
		{
		case	PlayerData::Y2009:
			c_Player[input] = new Y2009();
			break;

		case PlayerData::ECCMAN:
			c_Player[input] = new ECCMAN();
			break;

		case PlayerData::PRINCESS:
			c_Player[input] = new Princess();
			break;

		case PlayerData::KNIGHT:
			c_Player[input] = new Knight();
			break;

		case PlayerData::SQUIRREL:
			c_Player[input] = new Squirrel();
			break;
		default:
			c_Player[input] = new Y2009();
			break;
		}

		//	モデル読み込み
		Load();

		//	初期設定
		c_Player[input]->Initialize( input, org[type]->Clone(), pos );
	}

	//	モデル読み込み
	void	PlayerManager::Load( void )
	{
		org[PlayerData::Y2009] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::ECCMAN] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
		org[PlayerData::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::KNIGHT] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::KING] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );
		org[PlayerData::TIGER] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::ANIMA] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::CROWS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::BEAR] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
	}

//------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------

	//	更新
	void	PlayerManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
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
			Vector3	p_pos = c_Player[i]->GetPos();
			DrawCapsule( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 1.0f, 0xFFFFFFFF );

			//	デバッグ用
			//char	str[256];
			//sprintf_s( str, "p%d_coin = %d", i + 1, c_Player[i]->GetCoinNum() );
			//DrawString( str, 20, 150 + i * 30 );
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
					static float	dist = 1.0f;
					Vector3	p_pos1 = c_Player[i]->GetPos();
					Vector3	p_pos2 = c_Player[n]->GetPos();
					p_pos1.y += 3.0f;
					p_pos2.y += 3.0f;
					if ( Collision::CapsuleVSCapsule( c_Player[i]->GetPos(), p_pos1, 1.0f, c_Player[n]->GetPos(), p_pos2, 1.0f ) )
					{
						//	とりあえずエフェクトとコイン出す
						Vector3	p_pos = c_Player[n]->GetPos();
						float	effectScale = 1.0f;
						Particle::Spark( p_pos, effectScale );
						Vector3	vec = p_pos2 - p_pos1;
						vec.y = 0.5f;
						vec.Normalize();
						
						//if ( c_Player[n]->GetCoinNum() > 0 )
						if ( GameManager::GetCoinNum( n ) > 0 ) 
						{
							m_CoinManager->Set( p_pos2, vec, 0.5f );
							//c_Player[n]->SubCoin();
							GameManager::SubCoin( n );
						}
					}
				}
			}
		}
	}

	//	カプセル同士の当たり判定
	void	PlayerManager::HitCheckSphereVSCapsule( Player* p1, Player* p2 )
	{

	}

	//	コイン加算
	void	PlayerManager::AddCoin( int player )
	{
		c_Player[player]->AddCoin();
	}

	//	コイン減算
	void	PlayerManager::SubCoin( int player )
	{
		c_Player[player]->SubCoin();
	}

//------------------------------------------------------------------------------
//	情報取得・設定
//------------------------------------------------------------------------------

	//	情報取得
	Vector3	PlayerManager::GetPos( int player ){	return	c_Player[player]->GetPos();	}
	Matrix	PlayerManager::GetMatrix( int player ){ return	c_Player[player]->GetMatrix(); }
	float		PlayerManager::GetAngle( int player ){ return		c_Player[player]->GetAngle(); }
	int			PlayerManager::GetCoinNum( int player ){ return	c_Player[player]->GetCoinNum(); }
	int			PlayerManager::GetType( int player ){ return c_Player[player]->GetType(); }
	bool		PlayerManager::GetUnrivaled( int player ){ return c_Player[player]->GetUnrivaled(); }

	//	情報設定
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }
	void		PlayerManager::SetType( int player, int type ){ c_Player[player]->SetType(type); }
