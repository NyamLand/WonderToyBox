
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"Stage.h"
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


	//	コンストラクタ
	CharacterManager::CharacterManager( void )
	{

	}

	//	デストラクタ
	CharacterManager::~CharacterManager( void )
	{

	}

	//	初期化
	bool	CharacterManager::Initialize( int playerNum, int type, Vector3 pos, bool isPlayer )
	{
		switch ( type )
		{
		case CHARACTER_TYPE::PRINCESS:
			if ( isPlayer )		character[playerNum] = new Princess();
			else					character[playerNum] = new Princess_CPU();
			break;

		case CHARACTER_TYPE::SCAVENGER:
			if ( isPlayer )		character[playerNum] = new Scavenger();
			else					character[playerNum] = new Scavenger_CPU();
			break;

		case CHARACTER_TYPE::THIEF:
			if ( isPlayer )		character[playerNum] = new Thief();
			else					character[playerNum] = new Thief_CPU();
			break;

		case CHARACTER_TYPE::PIRATE:
			if ( isPlayer )		character[playerNum] = new Pirate();
			else					character[playerNum] = new Pirate_CPU();
			break;
		}

		//	初期設定
		character[playerNum]->SetLife( gameManager->GetStartLife( playerNum ) );
		character[playerNum]->Initialize( playerNum, pos, isPlayer );
		return	true;
	}

	//	解放
	void	CharacterManager::Release( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			SafeDelete( character[i] );
		}
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
			SetRank(i, gameManager->GetRank(i));
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
		}

		//　デバッグ表示
		DrawDebug();
	}

	//　デバッグ
	void	CharacterManager::DrawDebug( void )
	{
		if (debug)
		{
			char str[256];

			//--------------------------------------------
			//　表題
			//--------------------------------------------
			//　AI一覧表
			DrawString("rank", 1020, 30);
			DrawString("AImode", 1060, 30);
			DrawString("Aimode一覧", 1130, 30);
			DrawString("０：ATTACK", 1120, 50);
			DrawString("１：RUN（コイン）", 1120, 70);
			DrawString("２：GETAWAY（逃げる）", 1120, 90);
			DrawString("３：GUARD", 1120, 110);
			DrawString("４：JUMP", 1120, 130);
			DrawString("５：WAIT", 1120, 150);
			//	power・speed
			DrawString("tPower    tSpeed", 50, 50);

			//--------------------------------------------
			//　４人分のパラメータ
			//--------------------------------------------
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				//　AImode・rank
				sprintf_s(str, "%dＰ：%d    %d", i + 1, GetRank(i), GetAIMode(i));
				DrawString(str, 1000, 50 + i * 20);
				//　totalPower・totalSpeed
				sprintf_s(str, "%dＰ：%d    %f", i + 1, GetTotalPower(i), GetTotalSpeed(i));
				DrawString(str, 600, 70 + i * 20);
			}
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
			SetParameterInfo( worst, PARAMETER_STATE::BOOST );

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
			//	攻撃タイプ取得( 攻撃中でなかったらつぎへ )
			int	 attackParam = character[i]->GetAttackParam();
			if ( attackParam == 0 )	continue;

			//	オブジェクトへの当たり判定
			switch ( attackParam )
			{
			case	Collision::SPHEREVSCAPSULE:
				HitCheckObjectSphere( character[i] );
				break;

			case Collision::CAPSULEVSCAPSULE:
				HitCheckObjectCapsule( character[i] );
				break;
			}

			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	自分か相手が無敵状態だとつぎへ
				if ( i == n )	continue;
				if ( character[n]->GetParameterState(PARAMETER_STATE::UNRIVALED) )	continue;

				//	タイプ別当たり判定
				switch ( attackParam )
				{
				case Collision::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( character[i], character[n] );
					break;

				case Collision::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule( character[i], character[n] );
					break;

				case Collision::SPHEREVSCYRINDER:
					break;

				case Collision::SPHEREVSPOINT:
					break;

				case Collision::SPHEREVSSPHERE:
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
		if ( isHit )
		{
			if (bc1->GetMode() == MODE_STATE::HYPERARTS)
			{
				gameManager->SetShakeCamera( 1.0f, 30 );
			}
			
			//	無敵にする
			if ( bc2->GetParameterState( PARAMETER_STATE::UNRIVALED ) )	return;
			bc2->SetParameterState( PARAMETER_STATE::UNRIVALED );

			int addParam = bc1->GetAttack_addParam();
			bc2->SetParameterState( addParam );

			//	ライフ減らす
			int power = bc1->GetPower();
			FOR(0, power) bc2->SubLife();

			//	エフェクトだす
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = bc1_attackPos - bc2_top;
			knockBackVec.y = bc2_top.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );

			SetKnockBackParam(bc1, bc2);

			//	コインばらまき方向設定
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			//float	power = 0.2f;
			float	totalpower = bc1->GetTotalPower()*0.1f;
			int		dropCoin = bc1->GetTotalPower();
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );

			//	攻撃側のtotalPower枚のコインをばらまき
			for (int i = 0; i < dropCoin; i++)
			{
				if (bc2_coinNum <= 0) break;
				coinManager->Append(bc2_top, vec, totalpower);
				gameManager->SubCoin(bc2_Num);

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
			if (bc1->GetMode() == MODE_STATE::HYPERARTS)
			{
				gameManager->SetShakeCamera( 1.0f, 30 );
			}
			if (bc1->GetMode() == MODE_STATE::HYPERARTS){};
			
			//	無敵状態取得・設定
			if (bc2->GetParameterState(PARAMETER_STATE::UNRIVALED))	return;
			bc2->SetUnrivaled( true );

			//	ライフ減らす
			int power = bc1->GetPower();
			FOR(0, power) bc2->SubLife();

			//	エフェクトだす
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	ノックバック
			Vector3	knockBackVec = bc1_attack_top - bc2_top;
			knockBackVec.y = bc2_top.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );


			SetKnockBackParam(bc1, bc2);

			//	コインばらまき方向設定
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	プレイヤー番号取得とばらまきパワー設定
			//float	power = 0.15f;
			float	totalpower = bc1->GetTotalPower()*0.1f;
			int		dropCoin = bc1->GetTotalPower();
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum(bc2_Num);

			//	攻撃側のtotalPower枚のコインをばらまき
			for (int i = 0; i < dropCoin; i++)
			{
				if (bc2_coinNum <= 0) break;
				coinManager->Append(bc2_top, vec, totalpower);
				gameManager->SubCoin( bc2_Num );

			}
		}
	}

	//	オブジェクトへの攻撃
	void	CharacterManager::HitCheckObjectCapsule( BaseChara* bc )
	{
		//	攻撃する方
		Vector3	bc_pos = bc->GetPos();
		Vector3	bc_attack_bottom = bc->GetAttackPos_Bottom();
		Vector3	bc_attack_top = bc->GetAttackPos_Top();
		float			bc_attack_r = bc->GetAttack_R();

		//	判定用
		float			workFront = 0.0f;
		float			workBack = 0.0f;
		float			workRight = 0.0f;
		float			workLeft = 0.0f;
		Vector3	hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		int			outId = 0;
		
		//	全方向取得
		workFront = stage->GetFrontToObject( bc_pos, hitPos, outId );
		
		bool	isHit = false;
		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workBack = stage->GetBackToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workRight = stage->GetRightToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workLeft = stage->GetLeftToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}
	}

	//	オブジェクトへの攻撃
	void	CharacterManager::HitCheckObjectSphere( BaseChara* bc )
	{
		//	攻撃する方
		Vector3	bc_pos = bc->GetPos();
		Vector3	bc_attackPos = bc->GetAttackPos();
		float			bc_attack_r = bc->GetAttack_R();


		//	判定用
		float			workFront = 0.0f;
		float			workBack = 0.0f;
		float			workRight = 0.0f;
		float			workLeft = 0.0f;
		Vector3	hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		int			outId = 0;

		//	全方向取得
		workFront = stage->GetFrontToObject( bc_pos, hitPos, outId );

		bool	isHit = false;
		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	エフェクト
				particle->BlueFlame( hitPos, 1.0f );

				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workBack = stage->GetBackToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	エフェクト
				particle->BlueFlame( hitPos, 1.0f );

				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workRight = stage->GetRightToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	エフェクト
				particle->BlueFlame( hitPos, 1.0f );

				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
			}
		}

		workLeft = stage->GetLeftToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	エフェクト
				particle->BlueFlame( hitPos, 1.0f );

				//	耐久値減少
				stage->SubDurableValue( outId );

				//	無敵にする
				stage->SetUnrivaled( outId, true );
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

	//	前方取得
	Vector3	CharacterManager::GetFront( int player )const
	{
		return	character[player]->GetFront();
	}

	//	上方取得
	Vector3	CharacterManager::GetUp( int player )const
	{
		return	character[player]->GetUp();
	}

	//	右方取得
	Vector3	CharacterManager::GetRight( int player )const
	{
		return	character[player]->GetRight();
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
		return	character[player]->GetParameterState(PARAMETER_STATE::UNRIVALED);
	}

	//	スリップ状態取得
	bool		CharacterManager::GetParameterState( int player, int type )const
	{
		return	character[player]->GetParameterState( type );
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

	//	攻撃力合計取得
	int			CharacterManager::GetTotalPower( int player )const
	{
		return	character[player]->GetTotalPower();
	}

	//　スピード取得
	float		CharacterManager::GetTotalSpeed( int player )const
	{
		return	character[player]->GetTotalSpeed();
	}

	//　順位取得
	int			CharacterManager::GetRank( int player )const
	{
		return character[player]->GetRank();
	}

	//　モード取得
	int			CharacterManager::GetMode( int player )const
	{
		return character[player]->GetMode();
	}
	
	//	AI用モード取得
	int			CharacterManager::GetAIMode( int player )const
	{
		return character[player]->GetAIMode();
	}

	//	ライフ取得
	int			CharacterManager::GetLife( int player )const
	{
		return	character[player]->GetLife();
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

	//	AI用モード設定
	void		CharacterManager::SetAIMode( int player, int mode )
	{
		character[player]->SetAIMode(mode);
	}

	//	ブースト状態取得
	/*void		CharacterManager::SetBoosting( int player, bool boosting )
	{
		character[player]->SetBoosting( boosting );
	}*/

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

	//	パラメータ情報設定
	void		CharacterManager::SetParameterInfo( int player, int parameterInfo )
	{
		character[player]->SetParameterState( parameterInfo );
	}

	//    順位設定
	void        CharacterManager::SetRank(int player, int rank)
	{
		character[player]->SetRank(rank);
	}

	//	ノックバック力設定
	void		CharacterManager::SetForce(int player, float force)
	{
		character[player]->SetForce(force);
	}

	//ノックバック情報設定
	void		CharacterManager::SetKnockBackParam( BaseChara* bc1, BaseChara* bc2)
	{
		switch (bc1->GetKnockBackType())
		{
		case KNOCKBACK_TYPE::NONE:
			bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::STRENGTH:
			bc2->SetForce(1.5f);
			bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::MIDDLE:
			bc2->SetForce(1.0f);
			bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::WEAK:
			bc2->SetForce(0.5f);
			bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::LEANBACKWARD:
			bc2->SetLeanFrame(bc1->GetLeanFrame());
			bc2->SetMode(MODE_STATE::DAMAGE_LEANBACKWARD);
			break;
		}
	}

	//	体力設定
	void		CharacterManager::SetLife( int player, int life )
	{
		character[player]->SetLife( life );
	}