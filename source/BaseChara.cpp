
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"EventManager.h"
#include	"Effect.h"
#include	"Stage.h"
#include	"Random.h"
#include	"CoinManager.h"
#include	"ItemManager.h"
#include	"Sound.h"
#include	"UI.h"
#include	"BaseChara.h"
#include	"CharacterManager.h"

//*******************************************************************************
//
//	BaseCharaクラス
//
//*******************************************************************************

//----------------------------------------------------------------------------
//	グローバル
//----------------------------------------------------------------------------

#define	MIN_INPUT_STATE	300	//	スティック判定最小値
#define	MIN_SLIP_LENGTH	0.01f	//	滑り長さ最小値
#define	SLIP_TIMER_MAX		300	
#define	JUMP_POWER		0.08f	//	ジャンプ力
#define	WALL_DIST			2.5f		//	壁との距離
#define	HYPER_RATE		0.01f	//　CPUがハイパーを発動する確率
namespace
{

	//	被ダメージ用色
	namespace
	{
		//	被ダメージ用各色
		const Vector3	DAMAGE_COLOR[] =
		{
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(1.0f, 1.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
		};
	}
}

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	BaseChara::BaseChara( void ) : obj(nullptr), input(nullptr),		//	pointer
		pos(0.0f, 0.0f, 0.0f), move(0.0f, 0.0f, 0.0f), angle(0.0f, 0.0f, 0.0f), objectMove(0.0f, 0.0f, 0.0f),	//	Vector3
		scale(0.0f), speed(0.0f), totalSpeed(0.0f), drag(0.0f), force(0.0f), moveVec(0.0f), jumpPower(0.0f), dt(0.0f), param(0.0f),	//	float
		isGround(false), isPlayer(false), jumpState(true), checkWall(false), renderflag(true), coinUnrivaled(false), initflag(false),//	bool
		mode(0), playerNum(0), totalPower(0),jumpStep(0), damageStep(0), rank(0), life(0), checkWallCount(0)		//	int
	{
	
	}

	//	デストラクタ
	BaseChara::~BaseChara( void )
	{
		Release();
	}

	//	初期化
	bool	BaseChara::Initialize( int playerNum, Vector3 pos, bool isPlayer )
	{
		//	プレイヤーならコントローラ登録
		this->isPlayer = isPlayer;
		if ( this->isPlayer )		this->input = ::input[playerNum];

		//	パラメータ初期化
		Initialize( playerNum, pos );

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();

		if ( obj == nullptr )	return false;
		return	true;
	}

	//	初期化
	bool	BaseChara::Initialize( int playerNum, Vector3 pos )
	{
		//	モデル設定
 		obj = nullptr;
		
		//	プレイヤー番号登録
		this->playerNum = playerNum;

		//	パラメータ初期化
		mode = MODE_STATE::WAIT;
		this->pos = pos;
		angle = Vector3( 0.0f, 0.0f, 0.0f );
		totalPower = 0/*power*/;
		totalSpeed = speed;

		//	構造体初期化
		{
			//	攻撃情報初期化
			{
				attackInfo.type = 0;
				attackInfo.bottom = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.top = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.pos = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.Interval = 0;
				attackInfo.power = 0;
				attackInfo.dropPower = 0;
				attackInfo.addParam = -1;
				attackInfo.r = 0.0f;
				attackInfo.t = 0.0f;
				attackInfo.coinDropType = DROP_TYPE::DROP;
			}

			//	ノックバック情報初期化
			{
				knockBackInfo.type = 0;
				knockBackInfo.vec = Vector3(0.0f, 0.0f, 0.0f);
				knockBackInfo.interval = 0;
		}

			//	ダメージ時色情報初期化
			{
				damageColor.catchColor = Vector3( 0.0f, 0.0f, 0.0f );
				damageColor.passColor = DAMAGE_COLOR[playerNum];
			}

			//	パラメータ状態初期化
			ParameterInfoInitialize();

			//	AI情報初期化
			{
				aiInfo.mode = AI_MODE_STATE::STOP;
				aiInfo.param = 0;
				aiInfo.act_flag = false;
				aiInfo.runStraightCount = 0;
				aiInfo.AdjustingCount = 10;
				aiInfo.step_autorun = 0;
				aiInfo.count_wait		= 30;
				aiInfo.count_run		= 3 * SECOND;
				aiInfo.count_runaway	= 3 * SECOND;
				aiInfo.count_attack		= 1 * SECOND;
				aiInfo.count_guard		= 1 * SECOND;
			}

			//	slip情報初期化
			{
				slipInfo.speed = 0.003f;
				slipInfo.drag = 0.99f;
			}

			//	パラメータ加算情報構造体初期化
			{
				plusStatusInfo.power = 1;
				plusStatusInfo.speed = 0.1f;
				
				/* 仮 */
				plusStatusInfo.boostPower = 2;
				plusStatusInfo.boostSpeed = 0.2f;
			}

			
		}

		//	影初期化
		ShadowInitialize();

		if ( obj == nullptr )	return	false;
		return	true;
	}

	//	解放
	void	BaseChara::Release( void )
	{
		SafeDelete( obj );
		SafeDelete( shadow.obj );
	}

	//	パラメータ状態初期化
	void	BaseChara::ParameterInfoInitialize( void )
	{
		ParameterInfoInitialize(unrivaled);
		ParameterInfoInitialize( slip );
		ParameterInfoInitialize( boost );
		ParameterInfoInitialize( outrage );
		ParameterInfoInitialize( attackUp );
		ParameterInfoInitialize( speedUp );
		ParameterInfoInitialize( bomb );
		ParameterInfoInitialize( jump );
		ParameterInfoInitialize( magnet );
		ParameterInfoInitialize( confusion );
		ParameterInfoInitialize( respawn );
		ParameterInfoInitialize(unrivaledItem);
	}

	//	パラメータ状態初期化
	void	BaseChara::ParameterInfoInitialize( PARAMETER_INFO& ps )
	{
		ps.state = false;
		ps.timer = 0;
	}

	//	影情報初期化
	void	BaseChara::ShadowInitialize( void )
	{
		//	影構造体初期化
		{
			shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
			shadow.pos = pos;
			shadow.scale = 1.0f;
			SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, 0xFFFFFFFF );
		}
	}

	//攻撃情報初期化
	void	BaseChara::AttackParamInitialize(void)
	{
		attackInfo.t = 0.0f;
		attackInfo.r = 0.0f;
		attackInfo.type = 0;
		attackInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
		attackInfo.addParam = -1;
		attackInfo.top = Vector3(0.0f, 0.0f, 0.0f);
		attackInfo.bottom = Vector3(0.0f, 0.0f, 0.0f);
		initflag = false;
		knockBackInfo.type = 0;
	}

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	BaseChara::Update( void )
	{
		if ( mode != MODE_STATE::GUARD && !GetParameterState( PARAMETER_STATE::UNRIVALEDITEM ) ) 	m_Effect->SetShield( GetPlayerNum(), false );

		// 攻撃後再使用の準備
		if(attackInfo.Interval > 0 ) attackInfo.Interval--;

		//	モード管理
		(isPlayer) ? ModeManagement() : CPU_ModeManagement();

		//	死亡処理
		if ( GetLife() <= 0 )
		{
			life = 0;
			if (isPlayer)
			{
				if (mode != MODE_STATE::DEATH)
				{
					SetMode(MODE_STATE::DEATH);
					sound->PlaySE(SE::DEATH_SE);
					particle->BlueFlame(pos, 1.5f);
				}
			}
			else
			{
				if (aiInfo.mode != AI_MODE_STATE::DEATH)
				{
					aiInfo.mode = AI_MODE_STATE::DEATH;
					sound->PlaySE(SE::DEATH_SE);
					particle->BlueFlame(pos, 1.5f);
				}
			}
		}

		//	パラメータ情報更新
		ParameterInfoUpdate();
		ParameterAdjust();

		//	重力加算
		move.y += GRAVITY;

		//	影座標設定
		shadow.pos = pos;

		//	ステージ当たり判定
		StageCollisionCheck();
		
		//	移動値加算
		AddMove();

		//	抗力計算
		CalcDrag();

		//	落下チェック
		FallCheck();

		//	影更新
		ShadowUpdate();

		//	情報更新
		obj->Animation();
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();

		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		dt = elapse / 1000.0f;
		last += elapse;
	}

	//	描画
	void	BaseChara::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			//	ダメージ色調整
			CalcColorParameter();
			
			//	影描画
			iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );
			
			//	トゥーン輪郭線色設定
			shader3D->SetValue( "OutlineColor", this->damageColor.passColor );

			if ( renderflag )
			obj->Render( shader, technique );
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	モード管理
	void	BaseChara::ModeManagement( void )
	{
		switch ( mode )
		{
		case MODE_STATE::WAIT:
			AttackParamInitialize();
			Wait();
			break;

		case MODE_STATE::MOVE:
			AttackParamInitialize();
			Move();
			break;

		case MODE_STATE::POWERARTS:
		case MODE_STATE::HYPERARTS:
		case MODE_STATE::QUICKARTS:
			SetParameterState(PARAMETER_STATE::UNRIVALED);
			Attack( mode );
			break;

		case MODE_STATE::GUARD:
			AttackParamInitialize();
			Guard();
			break;

		case MODE_STATE::DAMAGE:
			AttackParamInitialize();
			Damage();
			break;

		case MODE_STATE::KNOCKBACK:
			AttackParamInitialize();
			KnockBack();
			break;

		case MODE_STATE::DEATH:
			AttackParamInitialize();
			Death();
			break;
		}
	}

	void	BaseChara::CPU_ModeManagement()
	{
		switch ( aiInfo.mode )
		{
		case AI_MODE_STATE::WAIT:
			AutoWait();
			break;

		case AI_MODE_STATE::MOVE:		//　コインを取りに行く
			AutoMove();
			break;

		case AI_MODE_STATE::POWERARTS:
		case AI_MODE_STATE::HYPERARTS:
		case AI_MODE_STATE::QUICKARTS:
			SetParameterState(PARAMETER_STATE::UNRIVALED);
			AutoAttack(aiInfo.mode);
			break;

		case AI_MODE_STATE::GUARD:
			AutoGuard();
			break;

		case AI_MODE_STATE::DAMAGE:
			AutoDamage();
			break;

		case AI_MODE_STATE::KNOCKBACK:
			AutoKnockBack();
			break;

		case AI_MODE_STATE::DEATH:
			AutoDeath();
			break;

		case AI_MODE_STATE::RUNAWAY:
			RunAway();
			break;

		case AI_MODE_STATE::STOP:
			Stop();
			break;
		}
	}

	//	抗力加算
	void	BaseChara::CalcDrag( void )
	{
		if ( !slip.state )
		{
			move.x *= drag;
			move.z *= drag;
		}
		else
		{
			move.x *= slipInfo.drag;
			move.z *= slipInfo.drag;
		}
	}

	//	移動値加算
	void	BaseChara::AddMove( void )
	{
		pos += move;
	}

	//	ダメージ時カラー計算
	void	BaseChara::CalcColorParameter( void )
	{
		damageColor.param -= Vector3( 0.035f, 0.035f, 0.035f );
		if ( damageColor.param.x <= 0.0f )	damageColor.param.x = 0.0f;
		if ( damageColor.param.y <= 0.0f )	damageColor.param.y = 0.0f;
		if ( damageColor.param.z <= 0.0f )	damageColor.param.z = 0.0f;

		shader3D->SetValue( "colorParam", damageColor.param );
	}

	//	ステージ当たり判定
	void	BaseChara::StageCollisionCheck( void )
	{
		int	outId;
		float		height = 0.0f;
		float		work = 0.0f;
		float		objectWork = 0.0f;
		objectMove = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	tempPos = Vector3( 0.0f, 0.0f, 0.0f );

		//	壁判定
		checkWall = stage->CheckWall( pos, move );

		Vector3 localPos = GetPos();
		Vector3 localAngle = Vector3(0.0f, GetAngle(), 0.0f);
		//乗り物
		bool ret = stage->GetLocal(localPos, localAngle );	//ボックスのレイが返っているか
		stage->ObjUpdate();
		//carrier->Update(mat);

		if (ret)
		{
			//carrier->GetWorld(localPos);
			stage->GetWorld(localPos, localAngle);
			/*player->SetMode(Player::MODE_MOVE);*/

			Vector3 m = GetMove();
			SetMove(Vector3(m.x, 0, m.z));


			SetPos(localPos);
			obj->SetPos(localPos);
			SetAngle(localAngle);
			obj->SetAngle(localAngle);
			obj->Update();
		}


		//	下方レイ判定
		work = stage->GetHeight( pos );
		objectWork = stage->GetHeightToObject( pos, objectMove, outId );
		
		//	影設定
		if ( work < objectWork )	height = objectWork;
		else									height = work;

		//	接地判定
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = height = objectWork;
				pos += objectMove;
				isGround = true;
			}
			if ( pos.y < work )
			{
				isGround = true;
				pos.y = height = work;
			}
			move.y = 0.0f;
			isGround = true;
			if ( jumpPower <= 0.0f )
			{
				jumpState = true;
			}
		}
		//	前方レイ判定
		objectWork = stage->GetFrontToObject( pos, tempPos, outId );
		if ( pos.z >= objectWork - WALL_DIST )
		{
			if ( pos.z >= objectWork - WALL_DIST )	pos.z = objectWork - WALL_DIST;
			move.z = 0.0f;
			checkWall = true;
		} 

		//	後方レイ判定
		objectWork = stage->GetBackToObject( pos, tempPos, outId );
		if ( pos.z <= objectWork + WALL_DIST )
		{
			if ( pos.z <= objectWork + WALL_DIST )	pos.z = objectWork + WALL_DIST;
			move.z = 0.0f;
			checkWall = true;
		}

		//	右レイ判定
		objectWork = stage->GetRightToObject( pos, tempPos, outId );
		if ( pos.x >= objectWork - WALL_DIST )
		{
			if ( pos.x >= objectWork - WALL_DIST )	pos.x = objectWork - WALL_DIST;
			move.x = 0.0f;
			checkWall = true;
		}

		//	左レイ判定
		objectWork = stage->GetLeftToObject( pos, tempPos, outId );
		if ( pos.x <= objectWork + WALL_DIST )
		{
			if ( pos.x <= objectWork + WALL_DIST )	pos.x = objectWork + WALL_DIST;
			move.x = 0.0f;
			checkWall = true;
		}

		//	影高さ設定
		shadow.pos.y = height + 0.1f;
	}

	//	角度調整
	void	BaseChara::AngleAdjust( float speed )
	{
		if ( !( input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )	return;

		//	左スティックの入力
		float	axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get( KEY_AXISY ) * 0.001f;

		//	カメラの前方方向を求める
		Vector3	vEye( mainView->GetTarget() - mainView->GetPos() );
		float	cameraAngle = atan2f( vEye.x, vEye.z );

		//	入力方向を求める
		float inputAngle = atan2f( axisX, axisY );

		//	目標の角度を求める
		float	targetAngle = cameraAngle + inputAngle;

		//	親に投げる
		AngleAdjust( Vector3( sinf( targetAngle ), 0.0f, cosf( targetAngle ) ), speed );
	}

	//	角度調整
	void	BaseChara::AngleAdjust( const Vector3& direction, float speed )
	{
		//	現在の向きと目標の向きの差を求める
		float	targetAngle( atan2f( direction.x, direction.z ) );
		
		//	混乱状態の時操作を逆にしてみる
		if ( confusion.state ) 	targetAngle = -targetAngle;

		//	移動する方向をセット
		moveVec = targetAngle;

		//	角度差を求める
		float	dAngle( targetAngle - GetAngle() );

		//	差の正規化
		if ( dAngle > 1.0f * D3DX_PI )	dAngle -= 2.0f * D3DX_PI;
		if ( dAngle < -1.0f * D3DX_PI ) 	dAngle += 2.0f * D3DX_PI;

		//	差をspeed分縮める
		if ( dAngle > 0.0f ){
			dAngle -= speed;
			if ( dAngle < 0.0f )  dAngle = 0.0f;
		}
		else{
			dAngle += speed;
			if ( dAngle > 0.0f )	dAngle = 0.0f;
		}

		//	向きに反映
		SetAngle( targetAngle - dAngle );

		//	プレイヤーの向きがπ以上にならないように調整する
		if ( GetAngle() >= 1.0f * D3DX_PI )		angle.y -= 2.0f * D3DX_PI;
		if ( GetAngle() <= -1.0f * D3DX_PI )		angle.y += 2.0f * D3DX_PI;
	}

	//	ノックバック
	void	BaseChara::KnockBack( void )
	{
		knockBackInfo.interval--;
		KeepDamageFrame();
//		SetMotion(MOTION_NUM::POSTURE);

		//SetParameterState(PARAMETER_STATE::UNRIVALED);
		//SetDrag(1.0f);	//一時的に抗力なくす
		if (knockBackInfo.interval < 0 && isGround)
		{
			move = Vector3(0.0f, 0.0f, 0.0f);
			damageStep = 0;
			SetMode(MODE_STATE::MOVE);

			SetMotion(COMMON_MOTION::WAIT);
			//SetUnrivaled(false);
		}
	}

	//	ノックバック与力
	void	BaseChara::AddKnockBackForce( float force )
	{
		//switch (damageStep)
		//{
		//case 0:
			isGround = false;
			SetDamageColor(damageColor.catchColor);
			move = knockBackInfo.vec * force;
			move.y = force;

			knockBackInfo.interval = 5;
			SetMode( MODE_STATE::KNOCKBACK );
			jumpState = true;
			//damageStep++;
			//break;

		//case 1:
			//Damage();
			//break;
		//}
	}



	//	待機
	void	BaseChara::Wait( void )
	{
		SetCoinUnrivaled( true );
		SetDrag( 0.8f );
	}

	//	動作
	void	BaseChara::Move( void )
	{
		if ( GetCoinUnrivaled() ) 	SetCoinUnrivaled( false );
		initflag = false;
		param = false;
		attackInfo.type = 0;
		AttackParamInitialize();

		Control();
	}

	//	攻撃
	void	BaseChara::Attack( int attackKind )
	{
//		SetMotion( MOTION_NUM::ATTACK1 );

		bool	isEnd = false;

		if (attackInfo.Interval > 0)
		{
			isEnd = true;
		}
		else
		{
			switch (attackKind)
			{
			case	MODE_STATE::QUICKARTS:
				isEnd = QuickArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case MODE_STATE::POWERARTS:
				isEnd = PowerArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case MODE_STATE::HYPERARTS:
				isEnd = HyperArts();
				canHyper = isEnd;
				if (!isEnd)	SetAttackParam(attackKind);
				break;
			}
		}

		//	モーション終了時に
		if ( isEnd )
		{
			mode = MODE_STATE::MOVE;
			SetUnrivaled(false);
			AttackParamInitialize();
		}
	}

	//	ジャンプ
	void	BaseChara::Jump( void )
	{
		if ( jumpState )
		{
			if ( isPlayer )
			{
				if ( input->Get( KEY_A ) == 3 )
				{
					jumpPower = JUMP_POWER;
					jumpState = false;
				}
			}
		}
		else
		{
			if ( jumpPower > 0.0f )		move.y += jumpPower;
			jumpPower -= JUMP_POWER * 0.1f;
		}
	}

	//	ガード
	void	BaseChara::Guard( void )
	{
		move.x = move.z = 0.0f;
		//SetParameterState(PARAMETER_STATE::UNRIVALED);
//		SetMotion( MOTION_NUM::GUARD );

		//	ボタンをはなすと戻る
		if ( input->Get( KEY_B6 ) != 1 )
		{
			SetMode( MODE_STATE::MOVE );
			m_Effect->SetShield( GetPlayerNum(), false );
			SetUnrivaled(false);
		}
	}

	//	ダメージ
	void	BaseChara::Damage( void )
	{
		// 攻撃中に食らったときにパラメーター初期化
		AttackParamInitialize();

		SetDamageMotion();
		
		AddKnockBackForce(force);
	}

	//	死亡
	void	BaseChara::Death( void )
	{		
		//	全効果初期化
		ParameterInfoInitialize(attackUp);
		ParameterInfoInitialize(speedUp);
		ParameterInfoInitialize(magnet);
		ParameterInfoInitialize(confusion);
		ParameterInfoInitialize(respawn);
		ParameterInfoInitialize(unrivaledItem);
		//	死亡中無敵
		SetParameterState(PARAMETER_STATE::UNRIVALED);
		SetCoinUnrivaled( true );
		particle->Death(GetPos(), 0.2f, GetDamageColor());
//		SetMotion( MOTION_NUM::DEATH );

		//	コイン半分ばらまき
		// 2.4応急処置
		if (param <= 0)
		{
			if (!initflag)
			{
				moveAngle = 0.0f;
				int	coinNum = gameManager->GetCoinNum(this->playerNum);
				FOR(0, coinNum / 2)
				{
					//	コイン半分ばらまき
					if (coinNum > 0)
					{
						coinManager->Append(GetPos(), Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(0.0f, 1.0f)), Random::GetFloat(-1.0f, 1.0f), Coin::COIN);
						gameManager->SubCoin(playerNum);
					}

				}
				initflag = true;
			}
		}

		//	待ち時間加算（仮）モーション出来次第変更
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		CubicFunctionInterpolation( moveAngle, 0.0f, D3DX_PI * 0.5f, param );
		SetAngle( Vector3( moveAngle, GetAngle(), 0.0f ) );

		//	今はモーションないので２秒待ち時間を設定
		if ( param >= 1.0f )
		{
			//	リスポーン
			pos = gameManager->InitPos[this->playerNum];
			SetLife( gameManager->GetStartLife( this->playerNum ) );	//	ライフを満タンにする
			SetMode( MODE_STATE::WAIT );											//	待機にして動けなくする
			SetParameterState( PARAMETER_STATE::RESPAWN );			//	点滅＆無敵処理開始
			SetAngle( Vector3( 0.0f, 0.0f, 0.0f ) );
			param = 0.0f;
			initflag = false;
		}
	}

	//	走る
	void	BaseChara::Run( void )
	{
		//	左スティックの入力チェック
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );

		if ( length > MIN_INPUT_STATE )
		{
			SetMotion( COMMON_MOTION::RUN );
			//SetMotion( MOTION_NUM::RUN );
			static	float adjustSpeed = 0.3f;
			AngleAdjust( adjustSpeed );
			if ( !slip.state )
			{
				move.x = sinf( moveVec ) * totalSpeed;
				move.z = cosf( moveVec ) * totalSpeed;
			}
			else
			{
				if ( move.Length() < speed )
				{
					move.x += sinf( moveVec ) * slipInfo.speed;
					move.z += cosf( moveVec ) * slipInfo.speed;
				}				
			}
		}
		else
		{
			SetMotion(COMMON_MOTION::WAIT);
//			SetMotion( MOTION_NUM::POSTURE );
			SetDrag( 0.8f );
		}
	}

	//	落下チェック
	void	BaseChara::FallCheck( void )
	{
		if (pos.y < -3.0f)
		{
			//	コインを半分減らす
			int		coinNum = gameManager->GetCoinNum(playerNum) / 2;
			FOR(0, coinNum)
			{
				if (coinNum > 0)
				{
					gameManager->SubCoin(this->playerNum);
					coinManager->Append(GetPos(), Vector3(Random::GetFloat(-0.7f, 0.7f), 1.0f, Random::GetFloat(0.3f, 0.7f)), Random::GetFloat(-0.7f, 0.7f), Coin::COIN);
				}
			}

			//	初期位置に配置→待ち状態にして行動不能にする→リスポーン状態を設定
			SubLife();
			pos = gameManager->InitPos[this->playerNum];
			if (life <= 0)
			{
				(isPlayer)?
					SetMode(MODE_STATE::MOVE) :
					aiInfo.mode = AI_MODE_STATE::MOVE;
			}
			else
			{
				(isPlayer) ?
					SetMode(MODE_STATE::WAIT) :
					aiInfo.mode = AI_MODE_STATE::WAIT;
				SetParameterState( PARAMETER_STATE::RESPAWN );
			}
		}
	} 

	//	パラメータ調整
	void	BaseChara::ParameterAdjust( void )
	{
		totalPower = attackInfo.dropPower;
		totalSpeed = speed;

		if (attackUp.state)	totalPower = attackInfo.dropPower + plusStatusInfo.power;
		if ( speedUp.state )	totalSpeed = speed + plusStatusInfo.speed;
		
		//　ブースト中
		if ( boost.state )
		{
			totalPower = attackInfo.dropPower + plusStatusInfo.boostPower;
			totalSpeed = speed + plusStatusInfo.boostSpeed;
			if ( attackUp.state )	totalPower += plusStatusInfo.power;
			if ( speedUp.state )	totalSpeed += plusStatusInfo.speed;
		}
	}

	//	影更新
	void	BaseChara::ShadowUpdate( void )
	{
		//	スケール計算( 高さに応じて影のスケールを調整、影の大きさの最大値はモデルの大きさの2.5倍に設定 )
		float	t = pos.y / 50.0f;
		static	float	maxScale = 2.5f;
		shadow.scale = maxScale - ( maxScale * t );

		//	頂点セット
		static	DWORD	vertexColor = 0xFFFFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

	//	ライフ加算
	void	BaseChara::AddLife( void )
	{
		life++;
		int	maxLife = gameManager->GetStartLife( this->playerNum );
		if ( life >= maxLife )
		{
			life = maxLife;
		}
	}

	//	ライフ減算
	void	BaseChara::SubLife( void )
	{
		//	演出
		ui->SetDamageFlag( playerNum, true, life );

		//	ライフ減算
		life--;
		
		//	ライフが０以下になると死亡モードに移行
		if ( life <= 0 )
		{
			life = 0;
			
			( isPlayer ) ?
				SetMode(MODE_STATE::DEATH) :
				aiInfo.mode = AI_MODE_STATE::DEATH;
		}
	}


	void	BaseChara::KeepDamageFrame(void)
	{
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::SCAVENGER)
		{
			if (obj->GetFrame() >= SCAVENGER::MOTION_FRAME::DAMAGE_END)
				obj->SetFrame(SCAVENGER::MOTION_FRAME::DAMAGE_END);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PIRATE)
		{
			if (obj->GetFrame() >= PIRATE::MOTION_FRAME::DAMAGE_END)
				obj->SetFrame(PIRATE::MOTION_FRAME::DAMAGE_END);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::THIEF)
		{
			if (obj->GetFrame() >= THIEF::MOTION_FRAME::DAMAGE_END)
				obj->SetFrame(THIEF::MOTION_FRAME::DAMAGE_END);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PRINCESS)
		{
			if (obj->GetFrame() >= PRINCESS::MOTION_FRAME::DAMAGE_END)
				obj->SetFrame(PRINCESS::MOTION_FRAME::DAMAGE_END);
		}
	}

//-------------------------------------------------------------------------------------
//	パラメータ情報動作関数
//-------------------------------------------------------------------------------------

	//	ステート管理
	void	BaseChara::ParameterInfoUpdate( void )
	{
		//--------各イベント・アイテム効果処理を書く--------//

		//一定時間無敵
		Unrivaled();

		//	攻撃力アップアイテム効果動作
		AttackUp();

		//	スリップイベント
		EventSlip();

		//	アイテム・マグネット
		ItemMagnet();

		//	混乱
		Confusion();

		//	リスポーン
		Respawn();

		//	スピードアップ
		SpeedUp();

		//	一定時間無敵(アイテム)
		ItemUnrivaled();
	}

	//	一定時間無敵
	void	BaseChara::Unrivaled(void)
	{
		if (!unrivaled.state)
		{
			unrivaled.timer = 0;
			return;
		}

		//	無敵にする
		unrivaled.state = true;

		//	タイマー減算
		unrivaled.timer--;

		//	時間が来たら効果取り消し
		if (unrivaled.timer <= 0)
		{
			unrivaled.timer = 0;
			unrivaled.state = false;
		}
	}

	//	攻撃力Upアイテム効果動作
	void	BaseChara::AttackUp( void )
	{
		if ( !attackUp.state )	return;

		particle->PowerUp( pos );

		//	タイマー減算
		attackUp.timer--;

		//	時間が来たら効果取り消し
		if ( attackUp.timer <= 0 )
		{
			attackUp.timer = 0;
			attackUp.state = false;
		}
	}

	//	スリップ
	void	BaseChara::EventSlip( void )
	{
		if ( !slip.state )	return;

		//	タイマー減算
		slip.timer--;

		//	時間が来たら効果取り消し
		if ( slip.timer <= 0 )
		{
			slip.timer = 0;
			slip.state = false;
		}
	}

	//	マグネット
	void	BaseChara::ItemMagnet( void )
	{
		if ( !magnet.state )	return;

		particle->Magnet( pos, 6.0f, 0.25f, 5 );

		//	タイマー減算
		magnet.timer--;

		//	時間が来たら効果取り消し
		if ( magnet.timer <= 0 )
		{
			magnet.timer = 0;
			magnet.state = false;
		}
	}

	//	どんけつブースト
	void	BaseChara::BoostUp( void )
	{
		/*if (boost.state)
		{
			power = totalPower;
			speed = totalSpeed;
		}*/
	}

	//	暴走状態

	//	スピードUpアイテム効果動作
	void	BaseChara::SpeedUp( void )
	{
		if ( !speedUp.state )	return;

		particle->SpeedUp( pos );

		//	タイマー減算
		speedUp.timer--;

		//	時間が来たら効果取り消し
		if ( speedUp.timer <= 0 )
		{
			speedUp.timer = 0;
			speedUp.state = false;
		}
	}

	//	ジャンプアイテム効果動作

	//	爆発アイテム効果動作

	//	混乱
	void	BaseChara::Confusion( void )
	{
		if ( !confusion.state )
		{
			m_Effect->SetConfusion( false, playerNum );
			return;
		}
		m_Effect->SetConfusion(true, playerNum);
		//	タイマー減算
		confusion.timer--;

		//	時間が来たら効果取り消し
		if ( confusion.timer <= 0 )
		{
			confusion.timer = 0;
			m_Effect->SetConfusion(false, playerNum);
			confusion.state = false;
		}
	}

	//	リスポーン
	void	BaseChara::Respawn( void )
	{
		if ( !respawn.state )	return;

		//	タイマー減算
		respawn.timer--;

		//	無敵状態
		unrivaled.state = true;

		//	描画フラグ切り替え
		if ( respawn.timer % 10 == 0 )	renderflag = !renderflag;

		if (respawn.timer <= 3 * SECOND - 30)
		{
			(isPlayer) ?
				SetMode(MODE_STATE::MOVE) :
				aiInfo.mode = AI_MODE_STATE::MOVE;
		}


		//	時間が来たら効果取り消し
		if ( respawn.timer <= 0 )
		{
			unrivaled.state = false;
			renderflag = true;
			respawn.timer = 0;
			respawn.state = false;
			//SetMode( MODE_STATE::MOVE );
		}
	}

	//	一定時間無敵(アイテム)
	void	BaseChara::ItemUnrivaled(void)
	{
		if ( !unrivaledItem.state )	return;

		//	無敵にする
		unrivaled.state = true;

		//	シールドを常に設定
		m_Effect->SetShield( playerNum, true );

		//	タイマー減算
		unrivaledItem.timer--;

		//	時間が来たら効果取り消し
		if ( unrivaledItem.timer <= 0 )
		{
			m_Effect->SetShield( playerNum, false );
			unrivaledItem.timer = 0;
			unrivaledItem.state = false;
			unrivaled.state = false;
		}
	}

//----------------------------------------------------------------------------
//	操作方法別動作
//----------------------------------------------------------------------------

	//	コントローラ操作
	void	BaseChara::Control( void )
	{
		//	スティックで移動
		Run();

		if ( input->Get( KEY_D ) == 3 )		mode = MODE_STATE::QUICKARTS;
		if ( input->Get( KEY_C ) == 3 )		mode = MODE_STATE::POWERARTS;
		if ( canHyper )
		{
			if (input->Get(KEY_B) == 3)
			{
				if ( GetLife() > 1 && !GetParameterState( PARAMETER_STATE::RESPAWN ) )
				{
					SubLife();
					mode = MODE_STATE::HYPERARTS;
				}
			}
		}
	
		//if ( input->Get( KEY_B ) == 3 )
		//{
		//	if ( jumpState )
		//	{
		//		mode = MODE_STATE::JUMP;
		//	}
		//}
		Jump();

		if ( input->Get( KEY_B6 ) == 3 )
		{
			m_Effect->SetShield( GetPlayerNum(), true );
			mode = MODE_STATE::GUARD;
		}


	}

	//	AI操作
	void	BaseChara::ControlAI( void )
	{
		
	}


	


//----------------------------------------------------------------------------
//	AI動作関数
//----------------------------------------------------------------------------
	
	//　基本行動（行動分岐）
	void	BaseChara::AutoMove( void )
	{
		if (GetCoinUnrivaled()) 	SetCoinUnrivaled(false);

		//　各モードに遷移
		ControlAI();
	}

	//　立ち止まり
	void	BaseChara::AutoWait()
	{
		SetDrag(0.8f);
		move.x = move.z = 0.0f;
		aiInfo.act_flag = true;

		if (aiInfo.count_wait <= 0)
		{
			aiInfo.count_wait = 30;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_wait--;
	}

	//　攻撃
	void	BaseChara::AutoAttack( int attackKind )
	{
		/*
			キャラによって 「攻撃条件」「攻撃時間」を変える　※オーバーライド
			（主に１位に近づいてパワーアーツかクイックアーツ？）
		*/

		bool	isEnd = false;
		if (attackInfo.Interval > 0)
		{
			isEnd = true;
		}
		else
		{
			switch (attackKind)
			{
			case AI_MODE_STATE::QUICKARTS:
				isEnd = QuickArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case AI_MODE_STATE::POWERARTS:
				isEnd = PowerArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case AI_MODE_STATE::HYPERARTS:
				isEnd = HyperArts();
				canHyper = isEnd;
				if (!isEnd)	SetAttackParam(attackKind);
				break;
			}

		}

		//	モーション終了時に
		if (isEnd)
		{
			aiInfo.mode = AI_MODE_STATE::MOVE;
			attackInfo.t = 0.0f;
			attackInfo.r = 0.0f;
			attackInfo.type = 0;
			attackInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.addParam = -1;
			attackInfo.top = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.bottom = Vector3(0.0f, 0.0f, 0.0f);
			knockBackInfo.type = 0;
			SetUnrivaled(false);
		}

		aiInfo.act_flag = true;

		if (aiInfo.count_attack <= 0)
		{
			aiInfo.count_attack = 1 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_attack--;
	}

	//　オートガード(引数：フレーム数)
	void	BaseChara::AutoGuard()
	{
//		SetMotion(MOTION_NUM::GUARD);
		move.x = move.z = 0.0f;
		SetParameterState(PARAMETER_STATE::UNRIVALED);
		aiInfo.act_flag = true;
		
		if (aiInfo.count_guard <= 0)
		{
			SetUnrivaled(false);
			aiInfo.count_guard = 1 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;

		}
		else aiInfo.count_guard--;
	}

	//　ダメージ
	void	BaseChara::AutoDamage(void)
	{
		// 攻撃中に食らったときにパラメーター初期化
		AttackParamInitialize();

		SetDamageMotion();

		AutoAddKnockBackForce(force);
	}

	//　ノックバック
	void	BaseChara::AutoKnockBack(void)
	{
		KeepDamageFrame();
		//		SetMotion(MOTION_NUM::POSTURE);

		//SetParameterState(PARAMETER_STATE::UNRIVALED);
		SetDrag(1.0f);	//一時的に抗力なくす
		if (/*move.y < 0 &&*/ isGround)
		{
			move = Vector3(0.0f, 0.0f, 0.0f);
			damageStep = 0;
			aiInfo.mode = AI_MODE_STATE::MOVE;

			SetMotion(COMMON_MOTION::WAIT);

			//SetUnrivaled(false);
		}
	}

	//　ノックバック与力
	void	BaseChara::AutoAddKnockBackForce(float force)
	{
		isGround = false;
		SetDamageColor(damageColor.catchColor);
		move = knockBackInfo.vec * force;
		move.y = force;

		aiInfo.mode = AI_MODE_STATE::KNOCKBACK;
		jumpState = true;
	}



	//　死亡
	void	BaseChara::AutoDeath(void)
	{
		//	死亡中無敵
		SetParameterState(PARAMETER_STATE::UNRIVALED);
		SetCoinUnrivaled(true);
		particle->Death(GetPos(), 0.2f, GetDamageColor());
		//		SetMotion( MOTION_NUM::DEATH );

		//	コイン半分ばらまき
		if (!initflag)
		{
			moveAngle = 0.0f;
			int	coinNum = gameManager->GetCoinNum(this->playerNum);
			FOR(0, coinNum / 2)
			{
				//	コイン半分ばらまき
				if (coinNum > 0)
				{
					coinManager->Append(GetPos(), Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(0.0f, 1.0f)), Random::GetFloat(-1.0f, 1.0f), Coin::COIN);
					gameManager->SubCoin(playerNum);
				}

			}
			initflag = true;
		}

		//	待ち時間加算（仮）モーション出来次第変更
		param += 0.01f;
		if (param >= 1.0f)	param = 1.0f;

		CubicFunctionInterpolation(moveAngle, 0.0f, D3DX_PI * 0.5f, param);
		SetAngle(Vector3(moveAngle, GetAngle(), 0.0f));

		//	今はモーションないので２秒待ち時間を設定
		if (param >= 1.0f)
		{
			//	リスポーン
			pos = gameManager->InitPos[this->playerNum];
			SetLife(gameManager->GetStartLife(this->playerNum));	//	ライフを満タンにする
			aiInfo.mode = AI_MODE_STATE::WAIT;											//	待機にして動けなくする
			SetParameterState(PARAMETER_STATE::RESPAWN);			//	点滅＆無敵処理開始
			SetAngle(Vector3(0.0f, 0.0f, 0.0f));
			param = 0.0f;
			initflag = false;
		}
	}

	//　コインを取りに行く
	void	BaseChara::AutoPickCoin( void )
	{	
		//if (coinManager->GetFreeCoinNum() < freeCoinMin) return;

		//-----------------------------------------
		//　targetに向けて1〜3歩歩く
		//-----------------------------------------
		Vector3			target = Vector3( 0.0f, 0.0f, 0.0f );
		static	float	adjustSpeed = 0.2f;
		bool			existence = false;
		enum
		{
			AUTORUN_WALK = 0,
			AUTORUN_STAND
		};
		existence = coinManager->GetMinPos(target, pos);

		//	対象が存在していたら対象に向かって走る
		if (existence)
		{
			AutoAngleAdjust(adjustSpeed, target);
			if (!slip.state)
			{
				move.x = sinf(angle.y) * speed;
				move.z = cosf(angle.y) * speed;
			}
			else
			{
				if (move.Length() < speed)
				{
					move.x += sinf(angle.y) * slipInfo.speed;
					move.z += cosf(angle.y) * slipInfo.speed;
				}
			}
		}
	}
	
	

	//	向き調整
	void	BaseChara::AutoAngleAdjust( float speed, Vector3 target )
	{
		//	カメラの前方方向を求める
		Vector3	vEye( mainView->GetTarget() - mainView->GetPos() );
		float	cameraAngle = atan2f(vEye.x, vEye.z);

		Vector3	vec = target - pos;
		vec.Normalize();

		//	入力方向を求める
		float inputAngle = atan2f( vec.x, vec.z );
		moveVec = inputAngle;

		//	目標の角度を求める
		float	targetAngle = cameraAngle + inputAngle;

		//	親に投げる
		AngleAdjust(Vector3(sinf(targetAngle), 0.0f, cosf(targetAngle)), speed);
	}

	//　逃げる
	void	BaseChara::RunAway( void )
	{
//		SetMotion( MOTION_NUM::RUN );

		aiInfo.act_flag = true;

		Vector3 vec_add( ZERO_VECTOR3 );
		Vector3 target( ZERO_VECTOR3 );

		
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			Vector3 vec[4];

			//	プレイヤーの番号を取得
			int	p_num = characterManager->GetPlayerNum( i );

			//	自分と同じ番号だったらスキップ
			if ( GetPlayerNum() == p_num )
			{
				vec[p_num] = Vector3( ZERO_VECTOR3 );
				continue;
			}
			
			//	以下は自分VS相手の処理
			vec[i] = characterManager->GetPos( characterManager->GetPlayerNum( i ) ) - pos;

			//　相手３人へのベクトルを合算
			vec_add += vec[i];
		}

		//　逃げる方向は相手３人に対して反対方向
		vec_add.Normalize();

		//　壁に向かってぶち当たるなら逆方向へ直進
		if (checkWall)
		{
			checkWallCount++;
		}
		if (aiInfo.runStraightCount <= 0)
		{
			target = pos - vec_add;
		}
		if (checkWallCount >= 10)
		{
			target *= -3.0f;
			aiInfo.runStraightCount = 1 * SECOND;
			checkWallCount = 0;
		}
		aiInfo.runStraightCount--;
		
		//　角度調整
		static	float	adjustSpeed = 0.2f;
		AutoAngleAdjust(adjustSpeed, target);

		//　移動
		if ( !slip.state )
		{
			move.x = sinf( moveVec ) * totalSpeed;
			move.z = cosf( moveVec ) * totalSpeed;
		}
		else
		{
			if ( move.Length() < totalSpeed )
			{
				move.x += sinf( moveVec ) * slipInfo.speed;
				move.z += cosf( moveVec ) * slipInfo.speed;
			}
		}

		//　行動続行是非
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_runaway--;
	}


	//　ジャンプ
	void	BaseChara::AutoJump( void )
	{
		//	壁を感知したらジャンプ
		if (checkWall)
		{
			if (jumpState)
			{
				//mode = MODE_STATE::JUMP;
				jumpPower = JUMP_POWER;
				jumpState = false;
			}
			else
			{
				if (jumpPower > 0.0f)		move.y += jumpPower;
				jumpPower -= JUMP_POWER * 0.1f;
			}
		}

	}

	//　完全停止（イントロ・終了時用）
	void	BaseChara::Stop( void )
	{
		SetMotion(0);
		SetCoinUnrivaled(true);
		SetDrag(0.8f);
	}

//----------------------------------------------------------------------------
//	情報取得
//----------------------------------------------------------------------------

	//	行列取得
	Matrix	BaseChara::GetMatrix( void )const
	{
		return	obj->TransMatrix;
	}

	//	ボーン行列取得
	Matrix	BaseChara::GetBoneMatrix( int num )const
	{
		//	ボーン行列取得
		Matrix	mat = *obj->GetBone( num );
		mat *= obj->TransMatrix;

		return	mat;
	}

	//	座標取得
	Vector3	BaseChara::GetPos( void )const
	{
		return	pos;
	}

	//	移動値取得
	Vector3	BaseChara::GetMove( void )const
	{
		return	move;
	}

	//	前方取得
	Vector3	BaseChara::GetFront( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._31, mat._32, mat._33 );
		out.Normalize();
		return	out;
	}

	//	右方取得
	Vector3	BaseChara::GetRight( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._11, mat._12, mat._13 );
		out.Normalize();
		return	out;
	}

	//	上方取得
	Vector3	BaseChara::GetUp( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._21, mat._22, mat._23 );
		out.Normalize();
		return	out;
	}

	//	ダメージ時色取得
	Vector3	BaseChara::GetDamageColor( void )const
	{
		return	damageColor.passColor;
	}

	//	攻撃判定位置取得
	Vector3	BaseChara::GetAttackPos( void )const
	{
		return	attackInfo.pos;
	}

	//	攻撃判定位置下取得
	Vector3	BaseChara::GetAttackPos_Bottom( void )const
	{
		return	attackInfo.bottom;
	}

	//	攻撃判定位置上取得
	Vector3	BaseChara::GetAttackPos_Top( void )const
	{
		return	attackInfo.top;
	}

	//	ボーン位置取得
	Vector3	BaseChara::GetBonePos( int num )const
	{
		Matrix	mat = GetBoneMatrix( num );
		
		//	行列から座標を取得
		Vector3	bonePos = Vector3( mat._41, mat._42, mat._43 );

		return	bonePos;		
	}

	//	ボーン前方取得
	Vector3	BaseChara::GetBoneFront( int num )const
	{
		//	行列取得
		Matrix	mat = GetBoneMatrix( num );

		//	行列から前方取得
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

		return	front;
	}

	//	ボーン上方取得
	Vector3	BaseChara::GetBoneUp( int num )const
	{
		//	行列取得
		Matrix	mat = GetBoneMatrix( num );

		//	行列から上方取得
		Vector3	up = Vector3( mat._21, mat._22, mat._23 );
		up.Normalize();

		return	up;
	}

	//	ボーン右方取得
	Vector3	BaseChara::GetBoneRight( int num )const
	{
		//	行列取得
		Matrix	mat = GetBoneMatrix( num );

		//	行列から右方取得
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		right.Normalize();

		return	right;
	}


	//	攻撃追加効果取得(状態異常)
	int		BaseChara::GetAttack_addParam(void)const
	{
		return	attackInfo.addParam;
	}

	//	攻撃判定半径取得
	float		BaseChara::GetAttack_R( void )const
	{
		return	attackInfo.r;
	}

	//	攻撃判定パラメータ取得
	float		BaseChara::GetAttack_T( void )const
	{
		return	attackInfo.t;
	}

	//	向き取得
	float		BaseChara::GetAngle( void )const
	{
		return	angle.y;
	}


	float		BaseChara::GetAngle(Vector3 vec1, Vector3 vec2)const
	{
		float out;
		out = Vector3Dot(vec1, vec2) / (vec1.Length() * vec2.Length());
		out = acos(out);
		return out;
	}

	//	スケール取得
	float		BaseChara::GetScale( void )const
	{
		return	scale;
	}

	//	攻撃力取得
	int			BaseChara::GetPower( void )const
	{
		return	attackInfo.power;
	}

	//	コイン落とす攻撃力取得
	int			BaseChara::GetDropPower(void)const
	{
		return	attackInfo.dropPower;
	}

	int			BaseChara::GetTotalPower( void )const
	{
		return	totalPower;
	}

	float		BaseChara::GetTotalSpeed( void )const
	{
		return	totalSpeed;
	}

	////	無敵状態取得
	//bool		BaseChara::GetUnrivaled( void )const
	//{
	//	return	unrivaled;
	//}

	//	ハイパー使用可否取得
	bool		BaseChara::GetCanHyper( void )const
	{
		return	canHyper;
	}
	
	//	パラメーター状態取得
	bool		BaseChara::GetParameterState( int type )const
	{
		bool	out = false;

		switch ( type )
		{
		case PARAMETER_STATE::UNRIVALED:
			out = unrivaled.state;
			break;

		case PARAMETER_STATE::SLIP:
			out = slip.state;
			break;

		case PARAMETER_STATE::BOOST:
			out = boost.state;
			break;

		case PARAMETER_STATE::OUTRAGE:
			out = outrage.state;
			break;

		case PARAMETER_STATE::ATTACKUP:
			out = attackUp.state;
			break;

		case PARAMETER_STATE::SPEEDUP:
			out = speedUp.state;
			break;

		case PARAMETER_STATE::BOMB:
			out = bomb.state;
			break;

		case PARAMETER_STATE::JUMP:
			out = jump.state;

		case	PARAMETER_STATE::MAGNET:
			out = magnet.state;
			break;

		case	PARAMETER_STATE::CONFUSION:
			out = confusion.state;
			break;

		case	PARAMETER_STATE::UNRIVALEDITEM:
			out = unrivaledItem.state;
			break;

		case PARAMETER_STATE::RESPAWN:
			out = respawn.state;
			break;
		}

		return	out;
	}

	//	ジャンプフラグ取得
	bool		BaseChara::GetJumpFlag( void )const
	{
		return	jumpState;
	}

	//	コイン判定状態取得
	bool		BaseChara::GetCoinUnrivaled( void )const
	{
		return	coinUnrivaled;
	}

	//　プレイヤーかどうかを取得
	bool		BaseChara::GetIsPlayer( void )const
	{
		return	isPlayer;
	}

	//	モード取得
	int		BaseChara::GetMode( void )const
	{
		return	mode;
	}

	//	
	int		BaseChara::GetAIMode( void )const
	{
		return	aiInfo.mode;
	}

	//	プレイヤー番号取得
	int		BaseChara::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	攻撃タイプ取得
	int		BaseChara::GetAttackParam( void )const
	{
		return	attackInfo.type;
	}

	//	ノックバックタイプ取得
	int		BaseChara::GetKnockBackType( void )const
	{
		return	knockBackInfo.type;
	}

	//　順位取得
	int		BaseChara::GetRank( void )const
	{
		return	rank;
	}

	//	ライフ取得
	int		BaseChara::GetLife( void )const
	{
		return	life;
	}

	//攻撃時のコインの落とし方取得
	int		BaseChara::GetDropType(void)const
	{
		return attackInfo.coinDropType;
	}

//----------------------------------------------------------------------------
//	情報設定
//----------------------------------------------------------------------------

	//	モーション設定
	void	BaseChara::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion(motion);
		}
	}

	//ダメージ設定
	void	BaseChara::SetDamageMotion()
	{
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::SCAVENGER)
		{
			SetMotion(SCAVENGER::MOTION_DATA::DAMAGE);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::THIEF)
		{
			SetMotion(THIEF::MOTION_DATA::DAMAGE);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PIRATE)
		{
			SetMotion(PIRATE::MOTION_DATA::DAMAGE);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PRINCESS)
		{
			SetMotion(PIRATE::MOTION_DATA::DAMAGE);
		}
	}

	//	座標設定
	void	BaseChara::SetPos( Vector3 pos )
	{
		this->pos = pos;
	}

	//	移動値設定
	void	BaseChara::SetMove( Vector3 move )
	{
		this->move = move;
	}
	
	//	向き設定
	void	BaseChara::SetAngle( Vector3 angle )
	{
		this->angle = angle;
	}

	//	向き設定
	void	BaseChara::SetAngle( float angle )
	{
		this->angle.y = angle;
	}

	//	スケール設定
	void	BaseChara::SetScale( float scale )
	{
		this->scale = scale;
	}

	//	モード設定
	void	BaseChara::SetMode( int mode )
	{
		if ( GetMode() != mode )
		{
			this->mode = mode;
		}
	}

	//	AIモード設定
	void	BaseChara::SetAIMode(int mode)
	{
		if (GetAIMode() != mode)
		{
			this->aiInfo.mode = mode;
		}
	}

	//	抗力設定
	void	BaseChara::SetDrag( float param )
	{
		drag = param;
	}

	//	ダメージ時色設定
	void	BaseChara::SetDamageColor( Vector3 color )
	{
		damageColor.param = color;
	}

	//	渡す色設定
	void	BaseChara::SetPassColor( Vector3 color )
	{
		damageColor.catchColor = color;
	}

	//	ブースト状態設定
	/*void	BaseChara::SetBoosting( bool boosting )
	{
	this->boosting = boosting;
	}*/

	//	ノックバック方向設定
	void	BaseChara::SetKnockBackVec( Vector3 vec )
	{
		knockBackInfo.vec = vec;
	}

	//	無敵状態設定
	void	BaseChara::SetUnrivaled( bool state )
	{
		unrivaled.state = state;
	}

	//	コイン判定設定
	void	BaseChara::SetCoinUnrivaled( bool state )
	{
		coinUnrivaled = state;
	}

	//	パラメータ状態設定
	void	BaseChara::SetParameterState( int parameterState )
	{
		switch ( parameterState )
		{
		case	PARAMETER_STATE::UNRIVALED:
			SetParameterState(unrivaled, 2 * SECOND);
			break;

		case PARAMETER_STATE::SLIP:
			SetParameterState( slip, 1 * SECOND );
			break;

		case PARAMETER_STATE::BOOST:
			SetParameterState( boost, 30 * SECOND );
			break;

		case PARAMETER_STATE::OUTRAGE:
			SetParameterState( outrage, 11 * SECOND );
			break;

		case PARAMETER_STATE::ATTACKUP:
			SetParameterState( attackUp, 11 * SECOND );
			break;

		case PARAMETER_STATE::SPEEDUP:
			SetParameterState( speedUp, 11 * SECOND );
			break;

		case PARAMETER_STATE::BOMB:
			SetParameterState( speedUp, 5 * SECOND );
			break;

		case PARAMETER_STATE::JUMP:
			SetParameterState( jump, 11 * SECOND );
			break;

		case PARAMETER_STATE::CONFUSION:
			m_Effect->StateEffectSet(playerNum, PARAMETER_STATE::CONFUSION);
			SetParameterState( confusion, 3 * SECOND );
			break;

		case PARAMETER_STATE::RESPAWN:
			SetParameterState( respawn, 3 * SECOND );
			break;

		case	PARAMETER_STATE::MAGNET:
			SetParameterState( magnet, 5 * SECOND );
			break;

		case	PARAMETER_STATE::UNRIVALEDITEM:
			m_Effect->SetShield( playerNum, true );
			SetParameterState( unrivaledItem, 5 * SECOND );
			break;
		}
	}

	//　順位設定
	void	BaseChara::SetRank( int rank )
	{
		this->rank = rank;
	}

	//	パラメータ状態設定
	void	BaseChara::SetParameterState( PARAMETER_INFO& parameterState, int time )
	{
		parameterState.state = true;
		parameterState.timer = time;
	}

	//	与力設定
	void BaseChara::SetForce( float force )
	{
		this->force = force;
	}

	//	ライフ設定
	void	BaseChara::SetLife( int life )
	{
		this->life = life;
	}