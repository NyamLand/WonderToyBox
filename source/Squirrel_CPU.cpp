
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"

#include	"Squirrel_CPU.h"

//*********************************************************************************
//
//	Squirrel_CPU�N���X
//
//*********************************************************************************

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Squirrel_CPU::Squirrel_CPU( void ) : Squirrel()
	{
		//	�p�����[�^������
		attackInfo.r = 0.0f;
		attackInfo.t = 0.0f;
		speed = 0.3f;
		scale = 0.02f;
		diffence = -5;
		isGround = true;
	}

	//	�f�X�g���N�^
	Squirrel_CPU::~Squirrel_CPU( void )
	{

	}

//-----------------------------------------------------------------------------
//	����֐�
//-----------------------------------------------------------------------------

	//	AI����
	//void	Squirrel_CPU::ControlAI( void )
	//{
	//	particle->BlueFlame( GetPos(), 1.0f );
	//}