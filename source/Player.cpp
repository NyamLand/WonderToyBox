
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Camera.h"
#include	"PlayerManager.h"
#include	"Player.h"

//****************************************************************************************
//
//	Playerクラス
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------

	//	コンストラクタ
	Player::Player( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), power( 0 ), bPower(power), diffence( 0 ), knockBackVec( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ),bSpeed(speed), mode( 0 ), unrivaled( false ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), attackPos_top( 0.0f, 0.0f, 0.0f ), attackPos_bottom( 0.0f, 0.0f, 0.0f ), attack_r( 0.0f ), attack_t( 0.0f ), knockBackType( 0 ),
		isGround(true), force(0.0f), type(0), p_num(0), CanHyper(true), boosting(false)
	{

	}

	//	デストラクタ
	Player::~Player( void )
	{
		SafeDelete( obj );
	}

	//	初期化
	bool	Player::Initialize( int input, iex3DObj* org, Vector3 pos )
	{
		this->obj = org;
		this->input = ::input[input];
		this->p_num = input;
		this->pos = pos;
		this->mode = PlayerData::WAIT;
		this->passDamageColor = PlayerData::DAMAGE_COLOR[input];
		
		bPower = power * 2;
		bSpeed = speed * 2;

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	Player::Update( void )
	{
		ModeManagement();
		CommonUpdate();
	}
	
	//	描画
	void	Player::Render( void )
	{
		CommonRender();
	}

	//	共通更新
	void	Player::CommonUpdate( void )
	{
		//	重力加算
		move.y += GRAVITY;

		//	ステージ当たり判定
		StageCollisionCheck();

		//	移動値加算
		pos += move;

		//　どんけつかどうかでパラメータ更新
		power = (boosting) ? bPower : power;
		speed = (boosting) ? bSpeed : speed;

		//	情報更新
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	共通描画
	void	Player::CommonRender( void )
	{
		obj->Render();
	}

	//	共通シェーダー付き描画
	void	Player::CommonRender( iexShader* shader, LPSTR technique )
	{

		//	ダメージ時白化計算
		colorParam -= Vector3( 0.035f, 0.035f, 0.035f );
		if ( colorParam.x <= 0.0f ){
			colorParam.x = 0.0f;
		}
		if ( colorParam.y <= 0.0f ){
			colorParam.y = 0.0f;
		}
		if ( colorParam.z <= 0.0f ){
			colorParam.z = 0.0f;
		}

		shader3D->SetValue( "colorParam", colorParam );
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------

	//	モード管理
	void	Player::ModeManagement( void )
	{
		switch ( mode )
		{
		case PlayerData::WAIT:
			Wait();
			break;

		case PlayerData::MOVE:
			Move();
			break;

		case PlayerData::ATTACK:
		case PlayerData::POWERARTS:
		case PlayerData::HYPERARTS:
		case PlayerData::QUICKARTS:
			unrivaled = true;
			Attack( mode );
			break;

		case PlayerData::JUMP:
			Jump();
			break;

		case PlayerData::GUARD:
			Guard();
			break;

		case PlayerData::DAMAGE_STRENGTH:
			CommonKnockBackStrength();
			SetDamageColor( receiveDamageColor );
			break;

		case PlayerData::DAMAGE:
			Damage();
			break;
		}
	}

	//	ステージ当たり判定チェック
	void	Player::StageCollisionCheck( void )
	{
		//　床判定
		float work = Collision::GetHeight( pos );
		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			isGround = true;
		}
		else
		{
			isGround = false;
		}

		//	壁判定
		Collision::CheckWall( pos, move );
	}

	//	角度調整
	void	Player::AngleAdjust( float speed )
	{
		if ( !( input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )	return;

		//	左スティックの入力
		float axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get( KEY_AXISY ) * 0.001f;

		//	カメラの前方方向を求める
		Vector3	vEye( m_Camera->GetTarget() - m_Camera->GetPos() );
		float	cameraAngle = atan2f( vEye.x, vEye.z );

		//	入力方向を求める
		float inputAngle = atan2f( axisX, axisY );

		//	目標の角度を求める
		float	targetAngle = cameraAngle + inputAngle;

		//	親に投げる
		AngleAdjust( Vector3( sinf( targetAngle ), 0.0f, cosf( targetAngle ) ), speed );
	}

	//	角度調整
	void	Player::AngleAdjust( const Vector3& direction, float speed )
	{
		//	現在の向きと目標の向きの差を求める
		float	targetAngle( atan2f( direction.x, direction.z ) );
		float	dAngle( targetAngle - GetAngle() );

		//	差の正規化
		if ( dAngle > 1.0f * PI )	dAngle -= 2.0f * PI;
		if ( dAngle < -1.0f * PI )	dAngle += 2.0f * PI;

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
		if ( GetAngle() >= 1.0f * PI )		angle -= 2.0f * PI;
		if ( GetAngle() <= -1.0f * PI )	angle += 2.0f * PI;
	}

	//	共通動作
	void	Player::CommonMove( void )
	{
		//	左スティックの入力チェック
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );
		if ( length > MIN_INPUT_STATE )
		{
			SetMotion( motionData.RUN );
			static float adjustSpeed = 0.2f;
			AngleAdjust( adjustSpeed );
			CommonMove( speed );
		}
		else
		{
			SetMotion( motionData.POSTURE );

			//	徐々に移動量と力減らす
			move.x *= 0.8f;
			move.z *= 0.8f;
		}
	}

	void	Player::CommonMove( float speed )
	{
		move.x = sinf( angle ) * speed;
		move.z = cosf( angle ) * speed;
	}

	//	ジャンプ
	void	Player::CommonJump( void )
	{
		mode = PlayerData::MOVE;
		if ( !isGround )	return;
		static	float	toY = pos.y + 20.0f;

		if ( pos.y <= toY )
		{
			move.y += 0.2f;
			pos += move;
		}

		//	移動
		CommonMove();

		//	接地してたら
		if ( isGround )	mode = PlayerData::MOVE;
	}

	//	ガード
	void	Player::CommonGuard( void )
	{
		unrivaled = true;
		SetMotion( motionData.GUARD );
		if ( input->Get( KEY_B7 ) == 2 )
		{
			mode = PlayerData::MOVE;
			unrivaled = false;
		}
	}

	//	ノックバック　強
	void	Player::CommonKnockBackStrength( void )
	{
		AddForce( 0.3f );

		move = knockBackVec * force;
		mode = PlayerData::DAMAGE;
	}

	//	ノックバック	共通
	void	Player::CommonKnockBack( void )
	{
		unrivaled = true;
		move.x *= 0.9f;
		move.z *= 0.9f;

		SetMotion( motionData.POSTURE );
		if ( move.Length() <= 0.01f )
		{
			mode = PlayerData::MOVE;
			unrivaled = false;
		}
	}

	//	力加算
	void	Player::AddForce( float force )
	{
		this->force = force;
	}

	//	モードWait
	void	Player::Wait( void )
	{
		SetMotion( motionData.POSTURE );
	}

	//	モードMove
	void	Player::Move( void )
	{
		CommonMove();

		if ( input->Get( KEY_A ) == 3 )		mode = PlayerData::QUICKARTS;
		if ( input->Get( KEY_B ) == 3 )		mode = PlayerData::POWERARTS;
		CanHyper = m_Player->CanHyper;
		if (CanHyper)
		{
			if (input->Get(KEY_C) == 3)		mode = PlayerData::HYPERARTS;
		}
		if ( input->Get( KEY_D ) == 3 )		mode = PlayerData::JUMP;
		if ( input->Get( KEY_B7 ) == 3 )	mode = PlayerData::GUARD;
		if ( input->Get( KEY_B10 ) == 3 )	mode = PlayerData::DAMAGE_STRENGTH;
	}

	//	モードAttack
	void	Player::Attack( int attackKind )
	{
		SetMotion( motionData.ATTACK1 );
		int		frame = obj->GetFrame();

		bool	isEnd = false;

		switch ( attackKind )
		{
		case PlayerData::QUICKARTS:
			isEnd = QuickArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case PlayerData::POWERARTS:
			isEnd = PowerArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;

		case PlayerData::HYPERARTS:
			isEnd = HyperArts();
			CanHyper = HyperArts();
			if ( !isEnd )	SetAttackParam( attackKind );
			break;
		}

		//	モーション終了時に
		if ( isEnd )
		{
			mode = PlayerData::MOVE;
			attack_t = 0.0f;
			attack_r = 0.0f;
			attackParam = 0;
			attackPos = GetPos();
			knockBackType = 0;
			unrivaled = false;
		}
	}

	//	モードJump
	void	Player::Jump( void )
	{
		CommonJump();
	}

	//	モードGuard
	void	Player::Guard( void )
	{
		move.x = move.z = 0.0f;
		SetMotion( motionData.GUARD );
		CommonGuard();
	}

	//	モードDamage
	void	Player::Damage( void )
	{
		CommonKnockBack();
	}

//-------------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------------

	//	モーション設定
	void	Player::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}

	//	モード設定
	void	Player::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}

	//	座標設定
	void	Player::SetPos( const Vector3& pos )
	{ 
		this->pos = pos;
	}

	//	座標設定
	void	Player::SetPos( const float& x, const float& y, const float& z )
	{
		this->pos = Vector3( x, y, z );
	}

	//	向き設定
	void	Player::SetAngle( const float& angle )
	{ 
		this->angle = angle;
	}

	//	スケール設定
	void	Player::SetScale( const float& scale )
	{ 
		this->scale = scale; 
	}

	//	ノックバック方向設定
	void	Player::SetKnockBackVec( const Vector3& knockBackVec )
	{ 
		this->knockBackVec = knockBackVec;
	}

	//	モード設定
	void	Player::SetMode( const PlayerData::STATE& state )
	{
		if ( GetMode() != state )		mode = state;
	}

	//	タイプ設定
	void	Player::SetType( const int& type )
	{
		this->type = type;
	}

	//	ダメージ時色設定
	void	Player::SetDamageColor( const Vector3& color )
	{ 
		this->colorParam = color;
	}

	//	渡す色設定
	void	Player::SetReceiveColor( const Vector3& color )
	{
		this->receiveDamageColor = color;
	}

	//	パワー設定
	void	Player::SetPower( const int& power )
	{
		this->power = power;
	}

	//	スピード設定
	void	Player::SetSpeed( const float& speed )
	{
		this->speed = speed;
	}

	//	ブースト設定
	void	Player::SetBoosting( const bool& boosting )
	{ 
		this->boosting = boosting; 
	}

//-------------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------------
	
	//	座標取得
	Vector3		Player::GetPos( void )
	{
		Vector3	out = pos;
		return	out;
	}

	//	行列取得
	Matrix		Player::GetMatrix( void )
	{ 
		Matrix	out = obj->TransMatrix;
		return	out;
	}

	//	向き取得
	float		Player::GetAngle( void )
	{ 
		float	out = angle;
		return out;
	}

	//	無敵状態取得
	bool		Player::GetUnrivaled( void )
	{ 
		bool	out = unrivaled;
		return out;
	}

	//	モード取得
	int			Player::GetMode( void )
	{
		int		out = this->mode;
		return out;
	}

	//	タイプ取得
	int			Player::GetType( void )
	{
		int		out = this->type;
		return out; 
	}

	//	プレイヤー番号取得
	int			Player::GetP_Num( void )
	{
		int		out = this->p_num;
		return out; 
	}

	//	ダメージ時色取得
	Vector3		Player::GetDamageColor( void )
	{
		Vector3	out = this->passDamageColor;
		return	out; 
	}

	//	ハイパー使用状態取得
	bool		Player::GetCanHyper( void )
	{
		bool	out = CanHyper;
		return out; 
	}

	//	パワー取得
	int			Player::GetPower( void )
	{
		int		out = power;
		return out; 
	}

	//	スピード取得
	float		Player::GetSpeed( void )
	{
		float	out = speed;
		return out; 
	}

	//	攻撃パラメータ取得
	int			Player::GetAttackParam( void )
	{
		int		out = attackParam;
		return out; 
	}

	//	ノックバックタイプ取得
	int			Player::GetKnockBackType( void )
	{
		int		out = knockBackType;
		return out; 
	}

	//	当たり判定位置取得
	Vector3		Player::GetAttackPos( void )
	{
		Vector3	out = attackPos;
		return out;
	}

	//	当たり判定位置上端取得
	Vector3		Player::GetAttackPos_Top( void )
	{
		Vector3	out = attackPos_top;
		return out; 
	}

	//	当たり判定位置下端取得
	Vector3		Player::GetAttackPos_Bottom( void )
	{
		Vector3	out = attackPos_bottom;
		return out; 
	}

	//	攻撃中割合取得
	float		Player::GetAttack_T( void )
	{
		float	out = attack_t;
		return out; 
	}

	//	攻撃当たり判定半径取得
	float		Player::GetAttack_R( void )
	{
		float	out = attack_r;
		return out; 
	}

