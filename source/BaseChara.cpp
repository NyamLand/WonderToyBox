
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"CoinManager.h"
#include	"ItemManager.h"
#include	"EventManager.h"
#include	"Effect.h"
#include	"Sound.h"
#include	"Random.h"

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

namespace
{
	namespace AI_MODE_STATE
	{
		enum
		{
			ATTACK,
			RUN,
			RUNAWAY,
			GUARD,
			JUMP,
			WAIT,
		};
	}

	//	被ダメージ用色
	namespace
	{
		//	被ダメージ用各色
		const Vector3	DAMAGE_COLOR[] =
		{
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
		};
	}
}

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	BaseChara::BaseChara( void ) : obj( nullptr ), input( nullptr ),		//	pointer
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ),	//	Vector3
		angle(0.0f), scale(0.0f), speed(0.0f),	totalSpeed(0.0f), drag(0.0f), force( 0.0f ), moveVec( 0.0f ),	//	float
		unrivaled(false), isGround(false), isPlayer(false), jumpState(false), checkWall(false),//	bool
		mode(0), playerNum(0), power(0), totalPower(0), leanFrame(0), jumpStep(0),damageStep(0),rank(0)		//	int
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

		SetMotion( MOTION_NUM::POSTURE );
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
		angle = 0.0f;
		totalPower = power;
		totalSpeed = speed;

		//	構造体初期化
		{
			//	攻撃情報初期化
			{
				attackInfo.type = 0;
				attackInfo.bottom = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.top = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.pos = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.r = 0.0f;
				attackInfo.t = 0.0f;
			}

			//	ノックバック情報初期化
			{
				knockBackInfo.type = 0;
				knockBackInfo.vec = Vector3(0.0f, 0.0f, 0.0f);
				knockBackInfo.isUp = false;
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
				aiInfo.mode = AI_MODE_STATE::WAIT;
				aiInfo.param = 0;
				aiInfo.act_flag = false;
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
		ParameterInfoInitialize( slip );
		ParameterInfoInitialize( boost );
		ParameterInfoInitialize( outrage );
		ParameterInfoInitialize( attackUp );
		ParameterInfoInitialize( speedUp );
		ParameterInfoInitialize( bomb );
		ParameterInfoInitialize( jump );
		ParameterInfoInitialize( magnet );
		ParameterInfoInitialize( confusion );
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

//----------------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------------

	//	更新
	void	BaseChara::Update( void )
	{
		//	モード管理
		ModeManagement();

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
			
			obj->Render( shader, technique );
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//	モーション管理
	void	BaseChara::MotionManagement( int motion )
	{
		switch ( motion )
		{
		case MOTION_NUM::STAND:
			obj->SetMotion( STAND );
			break;
			
		case MOTION_NUM::POSTURE:
			obj->SetMotion( POSTURE );
			break;

		case MOTION_NUM::JUMP:
			obj->SetMotion( JUMP );
			break;

		case MOTION_NUM::GUARD:
			obj->SetMotion( GUARD );
			break;

		case MOTION_NUM::LANDING:
			obj->SetMotion( LANDING );
			break;

		case MOTION_NUM::RUN:
			obj->SetMotion( RUN );
			break;

		case MOTION_NUM::ATTACK1:
			obj->SetMotion( ATTACK1 );
			break;

		case MOTION_NUM::ATTACK2:
			obj->SetMotion( ATTACK2 );
			break;

		case MOTION_NUM::ATTACK3:
			obj->SetMotion( ATTACK3 );
			break;
		}
	}

	//	モード管理
	void	BaseChara::ModeManagement( void )
	{
		switch ( mode )
		{
		case MODE_STATE::WAIT:
			Wait();
			break;

		case MODE_STATE::MOVE:
			Move();
			break;

		case MODE_STATE::POWERARTS:
		case MODE_STATE::HYPERARTS:
		case MODE_STATE::QUICKARTS:
			unrivaled = true;
			Attack( mode );
			break;

		case MODE_STATE::JUMP:
			Jump();
			break;

		case MODE_STATE::GUARD:
			Guard();
			break;

		case MODE_STATE::DAMAGE:
		case MODE_STATE::DAMAGE_FLYUP:
			AddKnockBackForce(force);
			break;

		case MODE_STATE::DAMAGE_LEANBACKWARD:
			KnockBackLeanBackWard();
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
		//	壁判定
		checkWall = Collision::CheckWall( pos, move );

		static	int fallTimer = 0;
		Vector3	height = Vector3( 0.0f, 0.0f, 0.0f );

		//　床判定
		if ( Collision::CheckDown( pos, move, height ) )
		{
			isGround = true;
			jumpState = true;
			fallTimer = 0;
		}
		else
		{
			fallTimer++;
			isGround = false;
		}

		//	影高さ設定
		shadow.pos.y = height.y + 0.1f;

		if ( fallTimer >= 5 )	jumpState = false;
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
		if ( GetAngle() >= 1.0f * D3DX_PI )		angle -= 2.0f * D3DX_PI;
		if ( GetAngle() <= -1.0f * D3DX_PI )	angle += 2.0f * D3DX_PI;
	}

	//	ノックバック
	void	BaseChara::KnockBack( void )
	{

		SetMotion(MOTION_NUM::POSTURE);
		switch (mode)
		{
		case MODE_STATE::DAMAGE:
			unrivaled = true;
			SetDrag(0.9f);
			if (move.Length() <= 0.001f)
			{
				damageStep = 0;
				SetMode(MODE_STATE::MOVE);
				unrivaled = false;
			}
			break;

		case MODE_STATE::DAMAGE_FLYUP:
			unrivaled = true;
			SetDrag(1.0f);
			if (isGround)
			{
				move = Vector3(0.0f, 0.0f, 0.0f);
				damageStep = 0;
				SetMode(MODE_STATE::MOVE);
				unrivaled = false;
			}
			break;
		}

	}

	//	ノックバック与力
	void	BaseChara::AddKnockBackForce( float force )
	{
		if (mode == MODE_STATE::DAMAGE_FLYUP) force /= 4;
		switch (damageStep)
		{
		case 0:
			SetDamageColor(damageColor.catchColor);
			move = knockBackInfo.vec * force;

			if (mode == MODE_STATE::DAMAGE) move.y = 0;
			if (mode == MODE_STATE::DAMAGE_FLYUP) move.y = force / 2;

			damageStep++;
			break;

		case 1:
			Damage();
			break;
		}
	}

	//	ノックバック	仰け反りのみ
	void	BaseChara::KnockBackLeanBackWard( void )
	{
		static int branktime = 0;	//仮の仰け反り時間　後でモーションフレームからとる可能性大

		unrivaled = true;
		if ( branktime == 0 )  SetDamageColor( damageColor.catchColor );
		branktime++;
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );
		SetMotion( MOTION_NUM::POSTURE );
		if ( branktime >= leanFrame )
		{
			branktime = 0;
			SetMode( MODE_STATE::MOVE );
			unrivaled = false;
		}
	}

	//	待機
	void	BaseChara::Wait( void )
	{
		SetMotion( MOTION_NUM::POSTURE );
		SetDrag( 0.8f );
	}

	//	動作
	void	BaseChara::Move( void )
	{
		//	プレイヤーかそうでないかで処理を分ける
		if ( isPlayer )	Control();
		else
		{
			ControlAI();
		}
	}

	//	攻撃
	void	BaseChara::Attack( int attackKind )
	{
		SetMotion( MOTION_NUM::ATTACK1 );

		bool	isEnd = false;

		switch ( attackKind )
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

		//	モーション終了時に
		if ( isEnd )
		{
			mode = MODE_STATE::MOVE;
			attackInfo.t = 0.0f;
			attackInfo.r = 0.0f;
			attackInfo.type = 0;
			attackInfo.pos = GetPos();
			knockBackInfo.type = 0;
			unrivaled = false;
		}
	}

	//	ジャンプ
	void	BaseChara::Jump( void )
	{
		static	float toY = 11.0f;
		switch ( jumpStep )
		{
		case 0:
			jumpState = false;
			toY = pos.y + 0.3f;
			jumpStep++;
			break;

		case 1:
			//	プレイヤーかCPUかで処理を分ける
			if ( isPlayer )	Control();
			else				ControlAI();

			if ( pos.y <= toY )
			{
				move.y += 0.1f;
			}
			else
			{
				jumpStep++;
			}
			break;

		case 2:
			//	プレイヤーかCPUかで処理を分ける
			if ( isPlayer )	Control();
			else				ControlAI();

			if ( isGround )
			{
				jumpStep = 0;
				jumpState = true;
				toY = 0;
				SetMode( MODE_STATE::MOVE );
			}
			break;
		}

	}

	//	ガード
	void	BaseChara::Guard( void )
	{
		move.x = move.z = 0.0f;
		unrivaled = true;
		SetMotion( MOTION_NUM::GUARD );

		//	ボタンをはなすと戻る
		if ( input->Get( KEY_B6 ) == 2 )
		{
			SetMode( MODE_STATE::MOVE );
			m_Effect->SetShield( GetPlayerNum(), false );
			unrivaled = false;
		}
	}

	//	ダメージ
	void	BaseChara::Damage( void )
	{
		KnockBack();
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
			SetMotion( MOTION_NUM::RUN );
			static	float adjustSpeed = 0.3f;
			AngleAdjust( adjustSpeed );
			if ( !slip.state )
			{
				move.x = sinf( moveVec ) * speed;
				move.z = cosf( moveVec ) * speed;
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
			SetMotion( MOTION_NUM::POSTURE );
			SetDrag( 0.8f );
		}
	}

	//	落下チェック
	void	BaseChara::FallCheck( void )
	{
		if ( pos.y < -3.0f )
		{
			//	コインを３枚減らす
			FOR( 0, 3 )
			{
				int		coinNum = gameManager->GetCoinNum( playerNum );
				if ( coinNum > 0 )
				{
					gameManager->SubCoin( this->playerNum );
				}
			}
			pos = gameManager->InitPos[this->playerNum];
		}
	} 

	//	パラメータ調整
	void	BaseChara::ParameterAdjust( void )
	{
		totalPower = power;
		totalSpeed = speed;

		if ( attackUp.state )	totalPower = power + plusStatusInfo.power;
		if ( speedUp.state )	totalSpeed = speed + plusStatusInfo.speed;
		
		//　ブースト中
		if (boost.state)
		{
			totalPower = power + plusStatusInfo.boostPower;
			totalSpeed = speed + plusStatusInfo.boostSpeed;
			if (attackUp.state)	totalPower += plusStatusInfo.power;
			if (speedUp.state)	totalSpeed += plusStatusInfo.speed;
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

//-------------------------------------------------------------------------------------
//	パラメータ情報動作関数
//-------------------------------------------------------------------------------------

	//	ステート管理
	void	BaseChara::ParameterInfoUpdate( void )
	{
		//--------各イベント・アイテム効果処理を書く--------//

		//	攻撃力アップアイテム効果動作
		AttackUp();

		//	スリップイベント
		EventSlip();

		//	アイテム・マグネット
		ItemMagnet();

		//	混乱
		Confusion();

	}

	//	攻撃力Upアイテム効果動作
	void	BaseChara::AttackUp( void )
	{
		if ( !attackUp.state )	return;

		particle->Arrow_UP( pos );

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

	//	ジャンプアイテム効果動作

	//	爆発アイテム効果動作

	//	混乱
	void	BaseChara::Confusion( void )
	{
		if ( !confusion.state )	return;

		//	タイマー減算
		confusion.timer--;

		//	時間が来たら効果取り消し
		if ( confusion.timer <= 0 )
		{
			confusion.timer = 0;
			confusion.state = false;
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
			if ( input->Get( KEY_A ) == 3 )	mode = MODE_STATE::HYPERARTS;
		}
	
		if ( input->Get( KEY_B ) == 3 )
		{
			if ( jumpState )		mode = MODE_STATE::JUMP;
		}
		
		if ( input->Get( KEY_B6 ) == 3 )
		{
			m_Effect->SetShield( GetPlayerNum(), true );
			mode = MODE_STATE::GUARD;
		}
	}

	//	AI操作
	void	BaseChara::ControlAI( void )
	{
		//switch (aiInfo.mode)
		//{
		//case AI_MODE_STATE::ATTACK:
		//	AutoAttack();
		//	break;

		//case AI_MODE_STATE::RUN:		//　コインを取りに行く
		//	AutoRun();
		//	break;

		//case AI_MODE_STATE::RUNAWAY:
		//	RunAway();
		//	break;

		//case AI_MODE_STATE::GUARD:
		//	AutoGuard();
		//	break;

		//case AI_MODE_STATE::WAIT:
		//	AutoWait();
		//	break;
		//}

		//--------------------------------------------
		//　ここでは各モードになるための条件を実装
		//--------------------------------------------

		/*
			・コインがある時はコインを取りに行く。（1,2歩歩く→ちょっと止まる）、（コイン取る→次を探す）
			　→ 確率で適当に攻撃出す（キャラによって挙動を変える）
			・段差を見分けてジャンプも出来るようにしたい。
			・コインがない時は１位もしくは距離が近い相手を攻撃。
			・誰かが近くで攻撃行為をしていたら確率でガード。
			・もしどんけつになったら８割ぐらいの確率でハイパーアーツを使う。
		*/

		//　フィールドにコインが○○枚以上　→　コイン優先
		if (m_CoinManager->GetFreeCoinNum() > 50)
		{
			aiInfo.mode = AI_MODE_STATE::RUN;
		}
		//　コイン○○以下
		/*
			１位  ：逃げる(80%)	＞	ガード(20%)
			２位　：攻撃(50%)	＞	逃げ(30%)	＞　コイン(20%)
			３位　：攻撃(60%)	＞	コイン(40%)
			４位　：攻撃(80%)	＞  コイン(20%)
		*/
		else
		{
			//　順位別にそれぞれ確率で行動分岐
			static int randi;
			const int randi_MAX = 11;
			if (!aiInfo.act_flag) randi = Random::GetInt(0, randi_MAX);
			switch (rank)
			{
			case 1:
				// 逃げる：ガード（８：２）
				if		(randi < 8)				aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				else if (randi > randi_MAX - 2)	aiInfo.mode = AI_MODE_STATE::GUARD;
				else							aiInfo.mode = AI_MODE_STATE::WAIT;
				break;

			case 2:
				//　攻撃：逃げる：コイン（５：３：２）
				if		(randi < 4)					aiInfo.mode = AI_MODE_STATE::ATTACK;
				else if (randi > randi_MAX - 3)		aiInfo.mode = AI_MODE_STATE::RUNAWAY;
				else if (randi == 4 || randi == 5)	aiInfo.mode = AI_MODE_STATE::RUN;
				else								aiInfo.mode = AI_MODE_STATE::WAIT;
				break;

			case 3:
				//　攻撃：コイン（６：４）
				if		(randi < 6)				aiInfo.mode = AI_MODE_STATE::ATTACK;
				else if (randi > randi_MAX - 4)	aiInfo.mode = AI_MODE_STATE::RUN;
				else							aiInfo.mode = AI_MODE_STATE::WAIT;
				break;

			case 4:
				//　攻撃：コイン（８：２）
				if		(randi < 8)				aiInfo.mode = AI_MODE_STATE::ATTACK;
				else if (randi > randi_MAX - 2)	aiInfo.mode = AI_MODE_STATE::RUN;
				else							aiInfo.mode = AI_MODE_STATE::WAIT;
				break;
			}
		}

		//	壁を感知したらジャンプ
		if ( checkWall )
		{
			if ( jumpState )		mode = MODE_STATE::JUMP;
		}

	}

//----------------------------------------------------------------------------
//	AI動作関数
//----------------------------------------------------------------------------
	
	//　コインを取りに行く
	void	BaseChara::AutoRun( void )
	{
		Vector3		target = Vector3( 0.0f, 0.0f, 0.0f );
		static	float adjustSpeed = 0.2f;
		bool			existence = false;
		enum
		{
			AUTORUN_WALK = 0,
			AUTORUN_STAND
		};

		//　targetに向けて1～3歩歩く
		existence = m_CoinManager->GetMinPos(target, pos);

		//	対象が存在していたら対象に向かって走る
		if (existence)
		{
			particle->BlueFlame(target, 1.0f);
			SetMotion(MOTION_NUM::RUN);
			AutoAngleAdjust(adjustSpeed, target);
			if (!slip.state)
			{
				move.x = sinf(angle) * speed;
				move.z = cosf(angle) * speed;
			}
			else
			{
				if (move.Length() < speed)
				{
					move.x += sinf(angle) * slipInfo.speed;
					move.z += cosf(angle) * slipInfo.speed;
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

	void	BaseChara::AutoAttack( void )
	{
		/*
			キャラによって 「攻撃条件」「攻撃時間」を変える　※オーバーライド
			（主に１位に近づいてパワーアーツかクイックアーツ？）
		*/

		aiInfo.act_flag = true;

		if (aiInfo.count_attack <= 0)
		{
			aiInfo.count_attack = 1 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_attack--;
	}

	//　逃げる
	void	BaseChara::RunAway( void )
	{
		SetMotion( MOTION_NUM::RUN );

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
		target = pos - vec_add;

		//　角度調整
		static	float	adjustSpeed = 0.2f;
		AutoAngleAdjust( adjustSpeed, target );

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
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_runaway--;
	}

	//　オートガード(引数：フレーム数)
	void	BaseChara::AutoGuard()
	{
		SetMotion(MOTION_NUM::GUARD);
		move.x = move.z = 0.0f;
		unrivaled = true;
		aiInfo.act_flag = true;
		
		if (aiInfo.count_guard <= 0)
		{
			unrivaled = false;
			aiInfo.count_guard = 1 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_guard--;
	}

	//　立ち止まり
	void	BaseChara::AutoWait()
	{
		SetMotion(MOTION_NUM::STAND);
		SetDrag(0.8f);
		move.x = move.z = 0.0f;
		aiInfo.act_flag = true;

		if (aiInfo.count_wait <= 0)
		{
			aiInfo.count_wait = 45;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_wait--;
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
		return	angle;
	}

	//	スケール取得
	float		BaseChara::GetScale( void )const
	{
		return	scale;
	}

	//	攻撃力取得
	int			BaseChara::GetPower( void )const
	{
		return	power;
	}
	int			BaseChara::GetTotalPower( void )const
	{
		return	totalPower;
	}

	float		BaseChara::GetTotalSpeed( void )const
	{
		return	totalSpeed;
	}

	//	無敵状態取得
	bool		BaseChara::GetUnrivaled( void )const
	{
		return	unrivaled;
	}

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
			break;
		}

		return	out;
	}

	//	モード取得
	int			BaseChara::GetMode( void )const
	{
		return	mode;
	}
	int			BaseChara::GetAIMode(void)const
	{
		return	aiInfo.mode;
	}

	//	プレイヤー番号取得
	int			BaseChara::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	仰け反りフレーム取得
	int			BaseChara::GetLeanFrame( void )const
	{
		return	leanFrame;
	}

	//	攻撃タイプ取得
	int			BaseChara::GetAttackParam( void )const
	{
		return	attackInfo.type;
	}

	//	ノックバックタイプ取得
	int			BaseChara::GetKnockBackType( void )const
	{
		return	knockBackInfo.type;
	}

	int			BaseChara::GetKnockBackIsUp(void)const
	{
		return knockBackInfo.isUp;
	}
	//　順位取得
	int			BaseChara::GetRank( void )const
	{
		return	rank;
	}

//----------------------------------------------------------------------------
//	情報設定
//----------------------------------------------------------------------------

	//	モーション設定
	void	BaseChara::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			MotionManagement( motion );
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
	void	BaseChara::SetAngle( float angle )
	{
		this->angle = angle;
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

	//	仰け反り時間設定
	void	BaseChara::SetLeanFrame( int frame )
	{
		leanFrame = frame;
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
		this->unrivaled = state;
	}

	//	パラメータ状態設定
	void	BaseChara::SetParameterState( int parameterState )
	{
		switch ( parameterState )
		{
		case PARAMETER_STATE::SLIP:
			SetParameterState( slip, 11 * SECOND );
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
			SetParameterState( confusion, 3 * SECOND );
			break;
		}
	}

	//　順位設定
	void	BaseChara::SetRank(int rank)
	{
		this->rank = rank;
	}

	//	パラメータ状態設定
	void	BaseChara::SetParameterState( PARAMETER_INFO& parameterState, int time )
	{
		parameterState.state = true;
		parameterState.timer = time;
	}


	void BaseChara::SetForce(float force)
	{
		this->force = force;
	}