#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"
#include	"CoinManager.h"
#include	"BaseBullet.h"
#include	"BulletManager.h"
#include	"BasePirateBullet.h"
#include	"Sound.h"
#include	"Stage.h"

BasePirateBullet::BasePirateBullet() :explosion(false), power(1)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		isPlayerCheck[i] = false;
	}
}

bool BasePirateBullet::Initialize()
{
	return true;
}
void	BasePirateBullet::Explode(void)
{
	radius += 1.0f;
	move = Vector3(0, 0, 0);
	particle->Bomb(pos, radius * 0.05f, Vector3(0.7f, 0.2f, 0.1f));
	//particle->Bom(this->pos, radius, 1.0f);
	//�����͈͂����ȏ�ɂȂ��������
	if (radius > 5.0f) state = false;
}

//	�v���C���[�Ƃ̂�����`�F�b�N
bool	BasePirateBullet::PlayerCollisionCheck(void)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == playerNum) continue;	//�������v���C���[�Ȃ珜�O
		if (isPlayerCheck[i]) continue;	//����̃v���C���[�Ɠ�x�ȏ�G���̋֎~
		if (!activate)	continue;
		if (characterManager->GetUnrivaled(i))	continue;

		//	�v���C���[���ݒ�
		Vector3	p_pos_bottom = characterManager->GetPos(i);
		Vector3	p_pos_top = Vector3(p_pos_bottom.x, p_pos_bottom.y + 3.0f, p_pos_bottom.z);
		float		p_r = 1.0f;

		//	�o���b�g���ݒ�
		Vector3	bulletPos = GetPos();
		bulletPos.y += 0.5f;
		float		bullet_r = radius;

		bool isHit = Collision::CapsuleVSSphere(p_pos_bottom, p_pos_top, p_r, bulletPos, bullet_r);

		if (isHit)
		{
			isPlayerCheck[i] = true;
			int bcMode = characterManager->GetMode(i);
			if (bcMode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				continue;
			}
			//	�G�t�F�N�g����
			//state = false;
			float	effectScale = 0.2f;
			particle->Spark(p_pos_top, effectScale);
			//�T�E���h�Đ�
			sound->PlaySE(SE::QUICK_HIT_SE);
			//	���C�t���炷
			FOR(0, power) characterManager->SubLife(i);

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = bulletPos - p_pos_bottom;
			knockBackVec.y = p_pos_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = characterManager->GetDamageColor(i);
			characterManager->SetPassColor(i, color);
			characterManager->SetKnockBackVec(i, -knockBackVec);
			characterManager->SetLeanFrame(i, leanpower);
			characterManager->SetForce(i, 2.0f);
			(characterManager->GetIsPlayer(i)) ?
				characterManager->SetMode(i, MODE_STATE::DAMAGE) :
				characterManager->SetAIMode(i, AI_MODE_STATE::DAMAGE);
			//characterManager->SetMode(i, MODE_STATE::DAMAGE);




			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.2f;
			int		p2_Num = characterManager->GetPlayerNum(playerNum);
			int		dropCoin = characterManager->GetTotalPower(i);
			int		p2_coinNum = gameManager->GetCoinNum(p2_Num);

			for (int i = 0; i < dropCoin; i++)
			{
				//	�R�C��������΂΂�܂�
				if (p2_coinNum > 0)
				{
					//	�R�C���΂�܂������ݒ�
					std::uniform_real_distribution<float>	vecrand(-1.0f, 1.0f);
					Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
					vec.Normalize();
					coinManager->Append(p_pos_top, vec, power, Coin::COIN);
					gameManager->SubCoin(p2_Num);
				}
			}
			return true;
		}
	}
	return false;
}