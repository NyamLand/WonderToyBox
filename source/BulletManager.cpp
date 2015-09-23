
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"
#include	"Bullet.h"

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

	namespace
	{
		//	�萔
		const int BULLET_MAX = 201;		//	�e�ő吔
		const int LIMIT = ( const int )( 60 * 1.5f );
	}

//---------------------------------------------------------------------------------
//	�������E���
//---------------------------------------------------------------------------------

	//	�R���X�g���N�^
	BulletManager::BulletManager( void )
	{

	}

	//	�f�X�g���N�^
	BulletManager::~BulletManager( void )
	{

	}

	//	������
	bool	BulletManager::Initialize( void )
	{
		org = new iexMesh( "DATA/BG/Bullet/bulletEx.imo" );
		c_Bullet = new Bullet[BULLET_MAX];
		bullet_num = 0;

		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			c_Bullet[i].Initialize();
			c_Bullet[i].state = false;
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
		//	�����J�E���g������
		bullet_num = 0;

		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( !c_Bullet[i].state )	continue;
			bullet_num++;

			if ( c_Bullet[i].limitTimer > LIMIT ) c_Bullet[i].state = false;

			//	�ʒu����
			//DistCheck(i);
			c_Bullet[i].Update();
		}
	}

	//	�`��
	void	BulletManager::Render( void )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( !c_Bullet[i].state )	continue;
			c_Bullet[i].Render();
		}

		char	str[256];
		sprintf_s( str, "bullet_num = %d", bullet_num );
		DrawString( str, 20, 120 );
	}

	//	�V�F�[�_�[�t���`��
	void	BulletManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( !c_Bullet[i].state )	continue;
			c_Bullet[i].Render( shader, technique );
		}
	}

//---------------------------------------------------------------------------------
//	����֐�
//---------------------------------------------------------------------------------

	//	����
	void	BulletManager::Set( Vector3 pos, Vector3 vec, float speed )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( c_Bullet[i].state )	continue;

			c_Bullet[i].judgeTimer = 10;
			c_Bullet[i].limitTimer = 0;
			c_Bullet[i].activate = false;
			c_Bullet[i].state = true;
			c_Bullet[i].obj = org->Clone();
			c_Bullet[i].pos = pos;
			Vector3	v = vec;
			v.Normalize();
			c_Bullet[i].move = v * speed;
			c_Bullet[i].scale = 0.05f;
			break;
		}
	}

	void	BulletManager::Set( Vector3 pos, Vector3 vec, const float& scale, float speed )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( c_Bullet[i].state )	continue;

			c_Bullet[i].judgeTimer = 10;
			c_Bullet[i].limitTimer = 0;
			c_Bullet[i].activate = false;
			c_Bullet[i].state = true;
			c_Bullet[i].obj = org->Clone();
			c_Bullet[i].pos = pos;
			Vector3	v = vec;
			v.Normalize();
			c_Bullet[i].move = v * speed;
			c_Bullet[i].scale = scale;
			break;
		}
	}

	//	�ʒu����
	void	BulletManager::DistCheck( int n )
	{
		//	�e���m�̈ʒu�`�F�b�N		
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			//	����vs�����͏��O		
			if ( i == n ) continue;

			//	����������ւ̃x�N�g��
			Vector3	bullet_pos1 = c_Bullet[n].GetPos();
			Vector3	bullet_pos2 = c_Bullet[i].GetPos();
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
				c_Bullet[n].SetPos( bullet_pos1 );
			}

		}
	}