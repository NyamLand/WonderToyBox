
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Tiger_CPU.h"

//*********************************************************************************
//
//	Tiger_CPU�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Tiger_CPU::Tiger_CPU( void ) : Tiger()
	{
		//	�p�����[�^������
		attackInfo.r = 0.5f;
		attackInfo.t = 0.0f;
		speed = 0.2f;
		scale = 0.02f;
		diffence = -1;
		isGround = true;
		//attackCount = 0;
		//attackFlag = false;
	}

	//	�f�X�g���N�^
	Tiger_CPU::~Tiger_CPU( void )
	{

	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

	//	AI����
	//void	Tiger_CPU::ControlAI( void )
	//{
	//	particle->BlueFlame( GetPos(), 1.0f );
	//}