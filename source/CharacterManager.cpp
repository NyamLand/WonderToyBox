
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"

//****************************************************************************************
//
//	CharacterManagerクラス
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------

	//	初期化
	bool	CharacterManager::Initialize( int playerNum, int type, Vector3 pos, bool isPlayer )
	{
		if ( !loadflag )	Load();

		switch ( type )
		{
		case CHARACTER_TYPE::PRINCESS:
			character[playerNum] = new Princess();
			break;

		case CHARACTER_TYPE::KNIGHT:
			character[playerNum] = new Knight();
			break;

		case CHARACTER_TYPE::SQUIRREL:
			character[playerNum] = new Squirrel();
			break;

		case CHARACTER_TYPE::TIGER:
			character[playerNum] = new Tiger();
			break;

		default:
			return	false;
		}

		//	初期設定
		character[playerNum]->Initialize( playerNum, org[type]->Clone(), pos, isPlayer );
		return	true;
	}

	//	解放
	void	CharacterManager::Release( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )	SafeDelete( character[i] );
		for ( int i = 0; i < OBJ_MAX; i++ )		SafeDelete( org[i] );
		loadflag = false;
	}

	//	読み込み
	void	CharacterManager::Load( void )
	{
		loadflag = true;
		org[CHARACTER_TYPE::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/ミノタウルス/minotaurus.IEM" );
		org[CHARACTER_TYPE::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );
		org[CHARACTER_TYPE::TIGER] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	CharacterManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	各キャラクター更新
			character[i]->Update();
			canHyper = GetCanHyper( i );
		}

		//	当たり判定
		HitCheck();

		//	どんけつブースト
		DonketsuBoost();
	}

	//	描画
	void	CharacterManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			character[i]->Render( shader, technique );
			Vector3	p_pos = character[i]->GetPos();

			if ( !debug )continue;
			DrawCapsule( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 1.0f );

			char str[256];
			wsprintf( str, "p%dの攻撃力：%d", i + 1, GetPower(i) );
			DrawString( str, 200, 200 + i * 20 );
		}
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------

	//	どんけつブースト
	void	CharacterManager::DonketsuBoost( void )
	{
		if ( gameManager->GetDonketsuBoostState() )
		{
			//	(決定された)ビリが誰かを取得・どんけつモードセット
			int worst = gameManager->GetWorst();
			SetBoosting( worst, true );

			//	ビリがなんのキャラかを識別してそれぞれに合ったステータス上昇
			RaiseStatus( worst, gameManager->GetCharacterType( worst ) );
		}
	}

	//	ステータス上昇
	void	CharacterManager::RaiseStatus( int worst, int type )
	{

	}

//-------------------------------------------------------------------------------------
//	当たり判定関数
//-------------------------------------------------------------------------------------

	//	当たり判定
	void	CharacterManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	自分か相手が無敵状態だとつぎへ
				if ( i == n )	continue;
				if ( character[n]->GetUnrivaled() )	continue;

				//	攻撃タイプを取得
				int		attackParam = character[i]->GetAttackParam();

				//	タイプ別当たり判定
				switch ( attackParam )
				{
				case COLLISION_TYPE::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule(character[i], character[n]);
					break;

				case COLLISION_TYPE::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule(character[i], character[n]);
					break;

				case COLLISION_TYPE::SPHEREVSCYRINDER:
					break;

				case COLLISION_TYPE::SPHEREVSPOINT:
					break;

				case COLLISION_TYPE::SPHEREVSSPHERE:
					break;
				}
			}
		}
	}

	//	球とカプセルの当たり判定
	void	CharacterManager::HitCheckSphereVSCapsule( BaseChara* bc1, BaseChara* bc2 )
	{
		//	攻撃する方
		Vector3	bc1_attackPos = bc1->GetAttackPos();
		float		bc1_attack_r = bc1->GetAttack_R();

		//	攻撃される方
		Vector3	bc2_bottom = bc2->GetPos();
		Vector3	bc2_top = Vector3( bc2_bottom.x, bc2_bottom.y + 3.0f, bc2_bottom.z );
		float		bc2_r = 1.0f;

		//	攻撃判定
		bool	isHit = Collision::CapsuleVSSphere( bc2_bottom, bc2_top, bc2_r, bc1_attackPos, bc1_attack_r );

		//	当たっていたら
		if (isHit)
		{
			//	エフェクトだす
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = bc1_attackPos - bc2_bottom;
			knockBackVec.y = bc2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );

			switch ( bc1->GetKnockBackType() )
			{
			case KNOCKBACK_TYPE::STRENGTH:
				bc2->SetMode( MODE_STATE::DAMAGE_STRENGTH );
				break;
			case KNOCKBACK_TYPE::MIDDLE:
				bc2->SetMode( MODE_STATE::DAMAGE_MIDDLE );
				break;
			case KNOCKBACK_TYPE::WEAK:
				bc2->SetMode( MODE_STATE::DAMAGE_WEAK );
				break;
			case KNOCKBACK_TYPE::LEANBACKWARD:
				bc2->SetLeanFrame( bc1->GetLeanFrame() );
				bc2->SetMode( MODE_STATE::DAMAGE_LEANBACKWARD );
				break;
			}

			//	色設定

			//	コインばらまき方向設定
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float	power = 0.2f;
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );

			//	コインがあればばらまき
			if ( bc2_coinNum > 0 )
			{
				m_CoinManager->Set( bc2_top, vec, power );
				gameManager->SubCoin( bc2_Num );
			}
		}
	}

	//	カプセルとカプセルの当たり判定
	void	CharacterManager::HitCheckCapsuleVSCapsule( BaseChara* bc1, BaseChara* bc2 )
	{
		//	攻撃する方
		Vector3	bc1_attack_bottom = bc1->GetAttackPos_Bottom();
		Vector3	bc1_attack_top = bc1->GetAttackPos_Top();
		float		bc1_attack_r = bc1->GetAttack_R();

		//	攻撃される方
		Vector3	bc2_bottom = bc2->GetPos();
		Vector3	bc2_top = Vector3( bc2_bottom.x, bc2_bottom.y + 3.0f, bc2_bottom.z );
		float		bc2_r = 1.0f;

		//	攻撃判定
		bool	isHit = Collision::CapsuleVSCapsule( bc1_attack_bottom, bc1_attack_top, bc1_attack_r, bc2_bottom, bc2_top, bc2_r );

		//	当たっていたら
		if ( isHit )
		{
			//	エフェクトだす
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = bc1_attack_top - bc2_bottom;
			knockBackVec.y = bc2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );

			switch ( bc1->GetKnockBackType() )
			{
			case KNOCKBACK_TYPE::STRENGTH:
				bc2->SetMode( MODE_STATE::DAMAGE_STRENGTH );
				break;
			case KNOCKBACK_TYPE::MIDDLE:
				bc2->SetMode( MODE_STATE::DAMAGE_MIDDLE );
				break;
			case KNOCKBACK_TYPE::WEAK:
				bc2->SetMode( MODE_STATE::DAMAGE_WEAK );
				break;
			case KNOCKBACK_TYPE::LEANBACKWARD:
				bc2->SetLeanFrame( bc1->GetLeanFrame() );
				bc2->SetMode( MODE_STATE::DAMAGE_LEANBACKWARD );
				break;
			}

			//	コインばらまき方向設定
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			float	power = 0.15f;
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );

			//	コインがあればばらまき
			if ( bc2_coinNum > 0 )
			{
				m_CoinManager->Set( bc2_top, vec, power );
				gameManager->SubCoin( bc2_Num );
			}
		}
	}

//-------------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------------

	//	行列取得
	Matrix	CharacterManager::GetMatrix( int player )const
	{
		return	character[player]->GetMatrix();
	}

	//	座標取得
	Vector3	CharacterManager::GetPos( int player )const
	{
		return	character[player]->GetPos();
	}

	//	ダメージ時色取得
	Vector3	CharacterManager::GetDamageColor( int player )const
	{
		return	character[player]->GetDamageColor();
	}

	//	向き取得
	float		CharacterManager::GetAngle( int player )const
	{
		return	character[player]->GetAngle();
	}

	//	ハイパーアーツ使用可否取得
	bool		CharacterManager::GetCanHyper( int player )const
	{
		return	character[player]->GetCanHyper();
	}

	//	無敵状態取得
	bool		CharacterManager::GetUnrivaled( int player )const
	{
		return	character[player]->GetUnrivaled();
	}

	//	攻撃種類取得
	int			CharacterManager::GetAttackParam( int player )const
	{
		return	character[player]->GetAttackParam();
	}

	//	プレイヤー番号取得
	int			CharacterManager::GetPlayerNum( int player )const
	{
		return	character[player]->GetPlayerNum();
	}

	//	攻撃力取得
	int			CharacterManager::GetPower( int player )const
	{
		return	character[player]->GetPower();
	}

	//	実体取得
	CharacterManager*	CharacterManager::GetInstance( void )
	{
		static	CharacterManager	out;
		return	&out;
	}

//-------------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------------

	//	座標設定
	void		CharacterManager::SetPos( int player, Vector3 pos )
	{
		character[player]->SetPos( pos );
	}

	//	向き設定
	void		CharacterManager::SetAngle( int player, float angle )
	{
		character[player]->SetAngle( angle );
	}

	//	スケール設定
	void		CharacterManager::SetScale( int player, float scale )
	{
		character[player]->SetScale( scale );
	}

	//	ノックバック方向設定
	void		CharacterManager::SetKnockBackVec( int player, Vector3 vec )
	{
		character[player]->SetKnockBackVec( vec );
	}

	//	モード設定
	void		CharacterManager::SetMode( int player, int mode )
	{
		character[player]->SetMode( mode );
	}

	//	ブースト状態取得
	void		CharacterManager::SetBoosting( int player, bool boosting )
	{
		character[player]->SetBoosting( boosting );
	}

	//	仰け反り時間取得
	void		CharacterManager::SetLeanFrame( int player, int leanframe )
	{
		character[player]->SetLeanFrame( leanframe );
	}

	//	渡す色設定
	void		CharacterManager::SetPassColor( int player, Vector3 color )
	{
		character[player]->SetPassColor( color );
	}