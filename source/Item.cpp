
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
//	Item�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

//	�R���X�g���N�^
Item::Item(void)
{

}

//	�f�X�g���N�^
Item::~Item(void)
{

}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

//	����
void	Item::Move(void)
{
	//	�d�͉��Z
	move.y += GRAVITY;

	//	��]
	angle += 0.05f;

}

void	Item::Hitduringtheoperation(const Vector3& pos, const int& Num)
{
	state = false;
	float	effectScale = 0.2f;
	Particle::Spark(pos, effectScale);
	
	IEX_PlaySound(SoundInfo::COIN_SE, false);
}