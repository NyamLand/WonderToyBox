
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"

#include	"BulletManager.h"

//******************************************************************************
//
//	BulletManager�N���X
//
//******************************************************************************

//---------------------------------------------------------------------------------
//	�O���[�o���ϐ�
//---------------------------------------------------------------------------------

	//	����
	BulletManager*	m_BulletManager;


//---------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------

	//	�R���X�g���N�^
	BulletManager::BulletManager( void )
	{
		ZeroMemory(org, sizeof(org));
		ZeroMemory(c_Bullet, sizeof(c_Bullet));
	}

	//	�f�X�g���N�^
	BulletManager::~BulletManager( void )
	{

	}

	//	������
	bool	BulletManager::Initialize( void )
	{
		org[BULLET_TYPE::SQUIRREL_01] = new iexMesh("DATA/BG/Bullet/bulletEx.imo");
		org[BULLET_TYPE::THIEF_01] = new iexMesh("DATA/Effect/kq_tramp/tramp.imo");
		org[BULLET_TYPE::THIEF_02] = new iexMesh("DATA/Effect/kq_hat/k_arm.imo");
		org[BULLET_TYPE::THIEF_03] = new iexMesh("DATA/Effect/k_arm/k_arm2.imo");
		//org[BULLET_TYPE::THIEF_03] = new iexMesh("DATA/Effect/kq_arm/kq_arm.imo");
		org[BULLET_TYPE::THIEF_04] = new iexMesh("DATA/Effect/kq_hand(��)/hand.imo");
		org[BULLET_TYPE::PIRATE_01] = new iexMesh("DATA/Effect/bomb/bomb.imo");
		org[BULLET_TYPE::PIRATE_02] = new iexMesh("DATA/Effect/bomb/bomb.imo");
		bullet_num = 0;

		for ( int i = 0; i < BULLET_MAX; i++ )
		{
		}

		if ( org != NULL ) 	return	false;
		return	true;
	}

//---------------------------------------------------------------------------------
//	�X�V�E�`��
//---------------------------------------------------------------------------------

	//	�X�V
	void	BulletManager::Update( void )
	{
		//	�J�E���g������
		bullet_num = 0;

		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( c_Bullet[i] == NULL )	continue;
			bullet_num++;

			if ( c_Bullet[i]->limitTimer < 0 ) c_Bullet[i]->state = false;

			//	�ʒu����
			//DistCheck(i);
			c_Bullet[i]->Update();

			if (c_Bullet[i]->state == false)
			{
				delete c_Bullet[i];
				c_Bullet[i] = NULL;
			}
		}
	}

	//	�`��
	void	BulletManager::Render( void )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if (c_Bullet[i] == NULL)	continue;
			c_Bullet[i]->Render();
		}

		//char	str[256];
		//sprintf_s( str, "bullet_num = %d", bullet_num );
		//DrawString( str, 20, 120 );
	}

	//	�V�F�[�_�[�t���`��
	void	BulletManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if (c_Bullet[i] == NULL)	continue;
			c_Bullet[i]->Render( shader, technique );
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------



	//	����
	void	BulletManager::Set(int type, BaseBullet* obj, Vector3 pos, Vector3 vec, float speed, int playerNum)
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if (c_Bullet[i] != NULL)	continue;

			c_Bullet[i] = obj;
			c_Bullet[i]->obj = org[type]->Clone();
			c_Bullet[i]->pos = pos;
			Vector3	v = vec;
			v.Normalize();
			c_Bullet[i]->move = v * speed;
			c_Bullet[i]->playerNum = playerNum;
			c_Bullet[i]->Initialize();
			break;
		}
	}

	void	BulletManager::Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const Vector3 angle, float speed, int playerNum)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (c_Bullet[i] != NULL)	continue;

			c_Bullet[i] = obj;
			c_Bullet[i]->obj = org[type]->Clone();
			c_Bullet[i]->pos = pos;
			Vector3	v = vec;
			v.Normalize();
			c_Bullet[i]->move = v * speed;
			c_Bullet[i]->angle = angle;
			c_Bullet[i]->playerNum = playerNum;
			c_Bullet[i]->Initialize();
			break;
		}
	}

	//void	BulletManager::Set(int type, BaseBullet* obj, Vector3 pos, Vector3 vec, const float& scale, float speed)
	//{
	//	for ( int i = 0; i < BULLET_MAX; i++ )
	//	{
	//		if (c_Bullet[i] != NULL)	continue;

	//		c_Bullet[i] = obj;
	//		c_Bullet[i]->obj = org[type]->Clone();
	//		c_Bullet[i]->judgeTimer = 10;
	//		c_Bullet[i]->limitTimer = 0;
	//		c_Bullet[i]->activate = false;
	//		c_Bullet[i]->state = true;
	//		c_Bullet[i]->pos = pos;
	//		Vector3	v = vec;
	//		v.Normalize();
	//		c_Bullet[i]->move = v * speed;	
	//		break;
	//	}
	//}


	//void	BulletManager::Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int leanpower)
	//{
	//	for (int i = 0; i < BULLET_MAX; i++)
	//	{
	//		if (c_Bullet[i]!= NULL)	continue;
	//		c_Bullet[i] = obj;
	//		c_Bullet[i]->obj = org[type]->Clone();
	//		c_Bullet[i]->judgeTimer = 10;
	//		c_Bullet[i]->limitTimer = 0;
	//		c_Bullet[i]->activate = false;
	//		c_Bullet[i]->state = true;
	//		c_Bullet[i]->pos = pos;
	//		Vector3	v = vec;
	//		v.Normalize();
	//		c_Bullet[i]->move = v * speed;
	//		c_Bullet[i]->scale = 0.05f;
	//		break;
	//	}
	//}

	//	�ʒu����
	void	BulletManager::DistCheck( int n )
	{
		//	�e���m�̈ʒu�`�F�b�N		
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			//	����vs�����͏��O		
			if ( i == n ) continue;

			//	����������ւ̃x�N�g��
			Vector3	bullet_pos1 = c_Bullet[n]->GetPos();
			Vector3	bullet_pos2 = c_Bullet[i]->GetPos();
			Vector3	vec = bullet_pos2 - bullet_pos1;


			//	�����v��			
			float length;
			length = vec.Length();

			//	�߂��ꍇ�͗���			
			if ( length < 0.5f )
			{
				//	�x�N�g�����K��		
				vec.Normalize();

				//	����
				bullet_pos1 = bullet_pos1 - vec * 0.5f;

				//	�ʒu���ݒ�
				c_Bullet[n]->SetPos( bullet_pos1 );
			}

		}
	}