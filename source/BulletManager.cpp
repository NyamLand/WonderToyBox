
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"

#include	"BulletManager.h"

//******************************************************************************
//
//	BulletManagerクラス
//
//******************************************************************************

//---------------------------------------------------------------------------------
//	グローバル変数
//---------------------------------------------------------------------------------

	//	実体
	BulletManager*	m_BulletManager;


//---------------------------------------------------------------------------------
//	初期化・解放
//---------------------------------------------------------------------------------

	//	コンストラクタ
	BulletManager::BulletManager( void )
	{
		ZeroMemory(org, sizeof(org));
		ZeroMemory(c_Bullet, sizeof(c_Bullet));
	}

	//	デストラクタ
	BulletManager::~BulletManager( void )
	{

	}

	//	初期化
	bool	BulletManager::Initialize( void )
	{
		org[BULLET_TYPE::SQUIRREL_01] = new iexMesh("DATA/BG/Bullet/bulletEx.imo");
		org[BULLET_TYPE::THIEF_01] = new iexMesh("DATA/Effect/kq_tramp/tramp.imo");
		org[BULLET_TYPE::THIEF_02] = new iexMesh("DATA/Effect/kq_hat/k_arm.imo");
		org[BULLET_TYPE::THIEF_03] = new iexMesh("DATA/Effect/k_arm/k_arm2.imo");
		//org[BULLET_TYPE::THIEF_03] = new iexMesh("DATA/Effect/kq_arm/kq_arm.imo");
		org[BULLET_TYPE::THIEF_04] = new iexMesh("DATA/Effect/kq_hand(仮)/hand.imo");
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
//	更新・描画
//---------------------------------------------------------------------------------

	//	更新
	void	BulletManager::Update( void )
	{
		//	カウント初期化
		bullet_num = 0;

		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if ( c_Bullet[i] == NULL )	continue;
			bullet_num++;

			if ( c_Bullet[i]->limitTimer < 0 ) c_Bullet[i]->state = false;

			//	位置調整
			//DistCheck(i);
			c_Bullet[i]->Update();

			if (c_Bullet[i]->state == false)
			{
				delete c_Bullet[i];
				c_Bullet[i] = NULL;
			}
		}
	}

	//	描画
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

	//	シェーダー付き描画
	void	BulletManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			if (c_Bullet[i] == NULL)	continue;
			c_Bullet[i]->Render( shader, technique );
		}
	}

//---------------------------------------------------------------------------------
//	動作関数
//---------------------------------------------------------------------------------



	//	生成
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

	//	位置調整
	void	BulletManager::DistCheck( int n )
	{
		//	弾同士の位置チェック		
		for ( int i = 0; i < BULLET_MAX; i++ )
		{
			//	自分vs自分は除外		
			if ( i == n ) continue;

			//	自分→相手へのベクトル
			Vector3	bullet_pos1 = c_Bullet[n]->GetPos();
			Vector3	bullet_pos2 = c_Bullet[i]->GetPos();
			Vector3	vec = bullet_pos2 - bullet_pos1;


			//	距離計測			
			float length;
			length = vec.Length();

			//	近い場合は離す			
			if ( length < 0.5f )
			{
				//	ベクトル正規化		
				vec.Normalize();

				//	離す
				bullet_pos1 = bullet_pos1 - vec * 0.5f;

				//	位置情報設定
				c_Bullet[n]->SetPos( bullet_pos1 );
			}

		}
	}