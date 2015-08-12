
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Knight.h"

//*********************************************************************************
//
//	Knightクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------------

//	コンストラクタ
Knight::Knight(void)
{
	//	パラメータ初期化
	attack_r = 0.0f;
	attack_t = 0.0f;
	lance_r = 0.0f;
	attack_topPos = Vector3(0.0f, 0.0f, 0.0f);
	speed = 0.2f;
	scale = 0.02f;
	SetMotionData();
	isGround = true;
}

//	デストラクタ
Knight::~Knight(void)
{

}

//	モーションデータ登録
void	Knight::SetMotionData(void)
{
	motionData.STAND = MotionNum::STAND;
	motionData.POSTURE = MotionNum::POSTURE;
	motionData.RUN = MotionNum::RUN;
	motionData.ATTACK1 = MotionNum::ATTACK1;
	motionData.JUMP = MotionNum::JUMP;
	motionData.ATTACK2 = MotionNum::ATTACK2;
	motionData.ATTACK3 = MotionNum::ATTACK3;
	motionData.GUARD = MotionNum::GUARD;
	motionData.POSTURE = MotionNum::POSTURE;
}

//-----------------------------------------------------------------------------------
//	更新・描画
//-----------------------------------------------------------------------------------

//	更新
void	Knight::Update(void)
{
	ModeManagement();
	BaseObj::Update();
}

//	描画
void	Knight::Render(iexShader* shader, LPSTR technique)
{
	BaseObj::Render(shader, technique);

	//	デバッグ用
	//char	str[256];
	//sprintf_s(str, "timelimit = %f",GetAngle() / (PI / 180));
	//DrawString( str, 550, 100 );

	//DrawSphere(attackPos, attack_r, 0xFFFFFFFF);
	DrawCapsule(attackPos, attack_topPos, attack_r, 0xFFFFFFFF);
}

//-----------------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------------

//	モード管理
void	Knight::ModeManagement(void)
{
	switch (mode)
	{
	case MOVE:
		Move();
		break;

	case ATTACK:
	case	POWERARTS:
	case HYPERARTS:
	case QUICKARTS:
		move = Vector3(0.0f, 0.0f, 0.0f);
		Attack(mode);
		break;

	case JUMP:
		Jump();
		break;

	case GUARD:
		break;
	}
}

//	モードMove
void	Knight::Move(void)
{
	CommonMove();

	if (input->Get(KEY_A) == 3)	mode = QUICKARTS;
	if (input->Get(KEY_B) == 3)	mode = POWERARTS;
	if (input->Get(KEY_C) == 3)	mode = HYPERARTS;
	if (input->Get(KEY_D) == 3)	mode = JUMP;
}

//	クイックアーツ
bool	Knight::QuickArts(void)
{
	//	行列から前方取得
	Matrix	mat = obj->TransMatrix;
	Vector3	front = Vector3(mat._31, mat._32, mat._33);
	front.Normalize();

	Vector3	startPos = Vector3(pos.x, pos.y + 1.5f, pos.z);
	Vector3	finPos = startPos + front * 5.0f;

	//	当たり判定位置移動&範囲拡大
	float t = GetBezier(ePrm_t::eRapid_Lv1, ePrm_t::eSlow_Lv3, attack_t);
	if (t < 0.5f){
		Lerp(attackPos, startPos, finPos, t);
	}
	else{
		Lerp(attackPos, finPos, startPos, t);
	}
	//	あたり判定のパラメータを与える
	attack_r = 0.5f;
	attack_topPos = attackPos + front * 2.0f;
	
	//	パラメータ加算
	attack_t += 0.015f;

	if (attack_t >= 1.0f)	return	true;
	return	false;
}

//	パワーアーツ
bool	Knight::PowerArts(void)
{
		//行列から前方取得
	Matrix	mat = obj->TransMatrix;
	Vector3	right = Vector3(mat._11, mat._12, mat._13);
	Vector3	front = Vector3(mat._31, mat._32, mat._33);
	right.Normalize();
	front.Normalize();
	static int step = 0;
	//	当たり判定位置移動&範囲拡大
	float t = GetBezier(ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t);

	switch (step)
	{
	case 0:
		//	あたり判定のパラメータを与える
		attack_r = 0.5f;
		step++;
		break;
	case 1:
		Vector3 f = front * (2.0f * sinf(PI * t));
		Vector3 r = -right * (2.0f * cosf(PI * t));
		attackPos = pos + f + r;
		attack_topPos = attackPos + f + r;
		//	パラメータ加算
		attack_t += 0.02f;
		break;
	}

	if (attack_t >= 1.0f){
		step = 0;
		return	true;
	}
	return	false;
}

//	ハイパーアーツ
bool	Knight::HyperArts(void)
{
	static	int		num = 0;	//	回数
	//行列から前方取得
	Matrix	mat = obj->TransMatrix;
	Vector3	right = Vector3(mat._11, mat._12, mat._13);
	Vector3	front = Vector3(mat._31, mat._32, mat._33);
	right.Normalize();
	front.Normalize();
	static int step = 0;
	Vector3 f, r;

	//	当たり判定位置移動&範囲拡大
	float t = GetBezier(ePrm_t::eRapid_Lv5, ePrm_t::eSlow_Lv1, attack_t);

	switch (step)
	{
	case 0:
		//	あたり判定のパラメータを与える
		attack_r = 0.5f;
		step++;
		break;
		//	右から左へ薙ぎ払い
	case 1:
		f = front * (2.0f * sinf(PI * t));
		r = right * (2.0f * cosf(PI * t));
		attackPos = pos + f + r;
		attack_topPos = attackPos + f + r;
		//	パラメータ加算
		attack_t += 0.02f;
		//	薙ぎ払い終えたら次へ
		if (attack_t >= 1.0f){
			attack_t = 0.0f;
			step++;
		}
		break;

		//	左から右へ薙ぎ払い
	case 2:
		f = front * (2.0f * sinf(PI * t));
		r = -right * (2.0f * cosf(PI * t));
		attackPos = pos + f + r;
		attack_topPos = attackPos + f + r;
		//	パラメータ加算
		attack_t += 0.02f;
		//	薙ぎ払い終えたら次へ
		if (attack_t >= 1.0f){
			attack_t = 0.0f;
			step++;
		}
		break;

		//	回転切り
	case 3:
		attackPos.x = pos.x + 2.0f * cosf(PI / 180 * lance_r);
		attackPos.z = pos.z + 2.0f * sinf(PI / 180 * lance_r);

		attack_topPos.x = pos.x + 4.0f * cosf(PI / 180 * lance_r);
		attack_topPos.z = pos.z + 4.0f * sinf(PI / 180 * lance_r);
		lance_r += 10.0f;
		if (lance_r >= 360 * 5)
		{
			step = 0;
			return true;
		}
	}

	return false;
}

//	モードAttack
void	Knight::Attack(int attackKind)
{
	SetMotion(motionData.ATTACK1);
	int		frame = obj->GetFrame();

	bool	isEnd = false;

	switch (attackKind)
	{
	case QUICKARTS:
		isEnd = QuickArts();
		if (!isEnd)	attackParam = 1;
		break;

	case POWERARTS:
		isEnd = PowerArts();
		if (!isEnd)	attackParam = 2;
		break;

	case HYPERARTS:
		isEnd = HyperArts();
		if (!isEnd)	attackParam = 3;
		break;
	}

	//	モーション終了時に
	if (isEnd)
	{
		mode = MOVE;
		attack_t = 0.0f;
		attack_r = 0.0f;
		lance_r = 0.0f;
	}
}

//	モードJump
void	Knight::Jump(void)
{
	CommonJump();
}