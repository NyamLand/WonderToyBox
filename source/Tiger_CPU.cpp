
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Tiger_CPU.h"

//*********************************************************************************
//
//	Tiger_CPUクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Tiger_CPU::Tiger_CPU( void ) : Tiger()
	{

	}

	//	デストラクタ
	Tiger_CPU::~Tiger_CPU( void )
	{

	}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------

	//	AI動作
	//void	Tiger_CPU::ControlAI( void )
	//{
	//	particle->BlueFlame( GetPos(), 1.0f );
	//}