
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"Player.h"
#include	"Princess.h"
#include	"Squirrel.h"
#include	"Knight.h"
#include	"Tiger.h"
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
	PlayerManager::PlayerManager( void ) : CanHyper( true )
	{

	}
	
	//	デストラクタ
	PlayerManager::~PlayerManager( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )	SafeDelete( c_Player[i] );
		for ( int i = 0; i < OBJ_MAX; i++ )			SafeDelete( org[i] );
	}

	//	初期化
	void	PlayerManager::Initialize( int input, int type, Vector3 pos )
	{
		switch ( type )
		{
		case PlayerData::PRINCESS:
			c_Player[input] = new Princess();
			break;

		case PlayerData::KNIGHT:
			c_Player[input] = new Knight();
			break;

		case PlayerData::SQUIRREL:
			c_Player[input] = new Squirrel();
			break;

		case PlayerData::TIGER:
			c_Player[input] = new Tiger();
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
		org[PlayerData::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::KNIGHT] = new iex3DObj( "DATA/CHR/ミノタウルス/minotaurus.IEM" );
		org[PlayerData::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );
		org[PlayerData::TIGER] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
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
			CanHyper = GetCanHyper( i );
		}

		//	当たり判定
		HitCheck();

		//　どんけつブースト
		DonketsuBoost();
	}

	//	描画
	void	PlayerManager::Render( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			c_Player[i]->Update();
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
			
			if ( !debug )continue;
			DrawCapsule(p_pos, Vector3(p_pos.x, p_pos.y + 3.0f, p_pos.z), 1.0f);

			char str[256];
			wsprintf(str, "p%dの攻撃力：%d", i + 1, GetPower(i));
			DrawString(str, 200, 200 + i * 20);
		}
	}

//------------------------------------------------------------------------------
//	動作関数
//------------------------------------------------------------------------------
	//　どんけつブースト
	void	PlayerManager::DonketsuBoost()
	{
		if (GameManager::GetDonketsuBoostState())
		{
			//　（決定された）ビリが誰かを取得・どんけつモードセット
			int worst = GameManager::GetWorst();
			SetBoosting(worst, true);

			
			//　ビリが何のキャラかを識別してそれぞれに合ったステータス上昇
			RaiseStatus(worst, GetType(worst));
			
			//　オーラ
			//　顔「怒り」
		}
	}

	//　ステータス上昇（どんけつ）
	void		PlayerManager::RaiseStatus( int worst, int type )
	{
		
	}

//------------------------------------------------------------------------------
//	当たり判定関数
//------------------------------------------------------------------------------

	//	当たり判定
	void	PlayerManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			for ( int n = 0; n < PLAYER_NUM; n++ )
			{
				//	自分か相手が無敵状態だとつぎへ
				if ( i == n )	continue;
				if ( c_Player[n]->GetUnrivaled() )	continue;

				//	攻撃タイプを取得
				int		attackParam = c_Player[i]->GetAttackParam();

				//	タイプ別当たり判定
				switch ( attackParam )
				{
				case PlayerData::COLLISION_TYPE::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( c_Player[i], c_Player[n] );
					break;

				case PlayerData::COLLISION_TYPE::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule( c_Player[i], c_Player[n] );
					break;

				case PlayerData::COLLISION_TYPE::SPHEREVSCYRINDER:
					break;

				case PlayerData::COLLISION_TYPE::SPHEREVSPOINT:
					break;

				case PlayerData::COLLISION_TYPE::SPHEREVSSPHERE:
					break;
				}
			}
		}
	}

	//	球とカプセルの当たり判定
	void	PlayerManager::HitCheckSphereVSCapsule( Player* p1, Player* p2 )
	{		
		//	攻撃する方
		Vector3	p1_attackPos		=		p1->GetAttackPos();
		float		p1_attack_r		=		p1->GetAttack_R();

		//	攻撃される方
		Vector3	p2_bottom = p2->GetPos();
		Vector3	p2_top = Vector3( p2_bottom.x, p2_bottom.y + 3.0f, p2_bottom.z );
		float		p2_r = 1.0f;

		//	攻撃判定
		bool	isHit = Collision::CapsuleVSSphere( p2_bottom, p2_top, p2_r, p1_attackPos, p1_attack_r );
	
		//	当たっていたら
		if ( isHit )
		{		
			//	エフェクトだす
			float	effectScale = 1.0f;
			Particle::Spark( p2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = p1_attackPos - p2_bottom;
			knockBackVec.y = p2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = p1->GetDamageColor();
			p2->SetReceiveColor( color );
			p2->SetKnockBackVec( -knockBackVec );
			p2->SetMode( PlayerData::DAMAGE_STRENGTH );

			//	色設定

			//	コインばらまき方向設定
			std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
			Vector3	vec = Vector3( vecrand( ran ), 1.0f, vecrand( ran ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float	power = 0.2f;
			int		p2_Num = p2->GetP_Num();
			int		p2_coinNum = GameManager::GetCoinNum( p2_Num );

			//	コインがあればばらまき
			if ( p2_coinNum > 0 )
			{
				m_CoinManager->Set( p2_top, vec, power );
				GameManager::SubCoin( p2_Num );
			}
		}
	}

	//	カプセルとカプセルの当たり判定
	void	PlayerManager::HitCheckCapsuleVSCapsule( Player* p1, Player* p2 )
	{
		//	攻撃する方
		Vector3	p1_attack_bottom = p1->GetAttackPos_Bottom();
		Vector3	p1_attack_top = p1->GetAttackPos_Top();
		float		p1_attack_r = p1->GetAttack_R();

		//	攻撃される方
		Vector3	p2_bottom = p2->GetPos();
		Vector3	p2_top = Vector3( p2_bottom.x, p2_bottom.y + 3.0f, p2_bottom.z );
		float		p2_r = 1.0f;

		//	攻撃判定
		bool	isHit = Collision::CapsuleVSCapsule( p1_attack_bottom, p1_attack_top, p1_attack_r, p2_bottom, p2_top, p2_r );

		//	当たっていたら
		if ( isHit )
		{
			//	エフェクトだす
			float	effectScale = 1.0f;
			Particle::Spark( p2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = p1_attack_top - p2_bottom;
			knockBackVec.y = p2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = p1->GetDamageColor();
			p2->SetReceiveColor(color);
			p2->SetKnockBackVec( -knockBackVec );
			p2->SetMode( PlayerData::DAMAGE_STRENGTH );
			
			//	コインばらまき方向設定
			std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
			Vector3	vec = Vector3( vecrand( ran ), 1.0f, vecrand( ran ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float	power = 0.15f;
			int		p2_Num = p2->GetP_Num();
			int		p2_coinNum = GameManager::GetCoinNum( p2_Num );

			//	コインがあればばらまき
			if ( p2_coinNum > 0 )
			{
				m_CoinManager->Set( p2_top, vec, power );
				GameManager::SubCoin( p2_Num );
			}
		}
	}

//------------------------------------------------------------------------------
//	情報取得・設定
//------------------------------------------------------------------------------

	//	情報取得
	Vector3		PlayerManager::GetPos( int player ){ return	c_Player[player]->GetPos();	}
	float		PlayerManager::GetAngle( int player ){ return	c_Player[player]->GetAngle(); }
	Matrix		PlayerManager::GetMatrix( int player ){ return	c_Player[player]->GetMatrix(); }
	bool		PlayerManager::GetUnrivaled( int player ){ return c_Player[player]->GetUnrivaled(); }
	int			PlayerManager::GetType( int player ){ return c_Player[player]->GetType(); }
	int			PlayerManager::GetP_Num( int player ){ return c_Player[player]->GetP_Num(); }
	bool		PlayerManager::GetCanHyper(int player){ return c_Player[player]->GetCanHyper(); }
	int			PlayerManager::GetPower(int player){ return c_Player[player]->GetPower(); }
	float		PlayerManager::GetSpeed(int player){ return c_Player[player]->GetSpeed(); }

	//	情報設定
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }
	void		PlayerManager::SetType( int player, int type ){ c_Player[player]->SetType(type); }
	void		PlayerManager::SetKnockBackVec( int player, Vector3 knockBackVec ){ c_Player[player]->SetKnockBackVec( knockBackVec ); }
	void		PlayerManager::SetMode( int player, PlayerData::STATE state ){ c_Player[player]->SetMode( state ); }
	void		PlayerManager::SetPower(int player, int power){ c_Player[player]->SetPower(power); }
	void		PlayerManager::SetSpeed(int player, float speed){ c_Player[player]->SetSpeed(speed); }
	void		PlayerManager::SetBoosting(int player, bool boosting){ c_Player[player]->SetBoosting(boosting); }