
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Pirate_CPU.h"

//*********************************************************************************
//
//	Pirate_CPUクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

//	コンストラクタ
Pirate_CPU::Pirate_CPU(void) : Pirate()
{

}

//	デストラクタ
Pirate_CPU::~Pirate_CPU(void)
{

}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------

//	AI動作
//void	Knight_CPU::ControlAI( void )
//{
//	particle->BlueFlame( GetPos(), 1.0f );
//}