
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"CoinManager.h"

#include	"Scavenger_CPU.h"

//*********************************************************************************
//
//	Scavenger_CPUクラス
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	初期化・解放
//-----------------------------------------------------------------------------

	//	コンストラクタ
	Scavenger_CPU::Scavenger_CPU(void) : Scavenger()
	{
	
	}
	
	//	デストラクタ
	Scavenger_CPU::~Scavenger_CPU(void)
	{
	
	}

//-----------------------------------------------------------------------------
//	動作関数
//-----------------------------------------------------------------------------

//	AI動作
	//void	Scavenger_CPU::AutoAttack(void)
	//{
	//	
	//}

	// モード遷移（aiInfo.mode == MOVE）
	void	Scavenger_CPU::ControlAI(void)
	{
		AutoPickCoin(CPU_SERCH_COIN_MIN);

		bool isEnd = false;

		if (coinManager->GetFreeCoinNum() >= 30)
		{
			if (life >= 3)
			{
				if (Random::PercentageRandom(0.8f))
				{
					SubLife();
					aiInfo.mode = AI_MODE_STATE::HYPERARTS;
				}
			}
			else
			{
				if (Random::PercentageRandom(0.5f))
				{
					aiInfo.mode = AI_MODE_STATE::POWERARTS;
				}
			}
		}

		//	//　ハイパー：コイン回収（８：２）
		//	if (coinManager->GetFreeCoinNum() >= 50)
		//	{
		//		if (life >= 3)
		//		{
		//			if (Random::PercentageRandom(0.8f))
		//			{
		//				isEnd = HyperArts();
		//				canHyper = isEnd;
		//				if (!isEnd)
		//				{
		//					SetAttackParam(MODE_STATE::HYPERARTS);
		//					return;
		//				}
		//			}
		//			else
		//			{
		//				
		//			}

		//			AI_MODE_STATE::MOVE;
		//			return;

		//		}
		//		else	//　ライフ３未満
		//		{
		//			aiInfo.mode == AI_MODE_STATE::QUICKARTS;
		//			return;
		//		}
		//		
		//	}
		//	//　基本的にはコイン回収
		//	aiInfo.mode = AI_MODE_STATE::MOVE;
		//}
		//else
		//{
		//	if (rank == 1)
		//	{
		//		
		//	}
		//	else	//　２位以下
		//	{

		//	}
		//}

		//　更新
		//CPU_ModeManagement();
	}