
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"CoinManager.h"

#include	"Scavenger_CPU.h"

//*********************************************************************************
//
//	Scavenger_CPU�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Scavenger_CPU::Scavenger_CPU(void) : Scavenger()
	{
	
	}
	
	//	�f�X�g���N�^
	Scavenger_CPU::~Scavenger_CPU(void)
	{
	
	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

//	AI����
	//void	Scavenger_CPU::AutoAttack(void)
	//{
	//	
	//}

	// ���[�h�J�ځiaiInfo.mode == MOVE�j
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

		//	//�@�n�C�p�[�F�R�C������i�W�F�Q�j
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
		//		else	//�@���C�t�R����
		//		{
		//			aiInfo.mode == AI_MODE_STATE::QUICKARTS;
		//			return;
		//		}
		//		
		//	}
		//	//�@��{�I�ɂ̓R�C�����
		//	aiInfo.mode = AI_MODE_STATE::MOVE;
		//}
		//else
		//{
		//	if (rank == 1)
		//	{
		//		
		//	}
		//	else	//�@�Q�ʈȉ�
		//	{

		//	}
		//}

		//�@�X�V
		//CPU_ModeManagement();
	}