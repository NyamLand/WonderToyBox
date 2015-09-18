
#include	"iextreme.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"Particle.h"
#include	"Player.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

#include	"Item.h"

//******************************************************************************
//
//	Itemクラス
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

//	コンストラクタ
Item::Item(void)
{

}

//	デストラクタ
Item::~Item(void)
{

}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

//	動作
void	Item::Move(void)
{
	//	重力加算
	move.y += GRAVITY;

	//	回転
	angle += 0.05f;

}

void	Item::Hitduringtheoperation(const Vector3& pos, const int& Num)
{
	state = false;
	float	effectScale = 0.2f;
	Particle::Spark(pos, effectScale);
	
	IEX_PlaySound(SoundInfo::COIN_SE, false);
}