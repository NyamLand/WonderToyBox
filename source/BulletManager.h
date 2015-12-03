
#ifndef		__BULLET_MANAGER_H__
#define		__BULLET_MANAGER_H__

#include	"GameManager.h"
#include	"BaseBullet.h"
#include	"Squirrel_Bullet01.h"
#include	"Thief_Bullet01.h"
#include	"Thief_Bullet02.h"
#include	"Thief_Bullet03.h"
#include	"Thief_Bullet04.h"
#include	"Pirate_Bullet01.h"
#include	"Pirate_Bullet02.h"
//******************************************************************************
//
//	BulletManagerクラス
//
//******************************************************************************
namespace BULLET_TYPE
{
	enum
	{
		SQUIRREL_01 = 0,
		THIEF_01,
		THIEF_02,
		THIEF_03,
		THIEF_04,
		PIRATE_01,
		PIRATE_02,
	};
}

namespace
{
	//------------------------------------------------
	//判定有効時間や消滅時間が特に決まっていない場合は
	//とりあえず100秒にしておきます
	//-------------------------------------------------

	////判定有効時間
	//int BULLET_JUDGETIMER[] =
	//{
	//	100 * SECOND,		//リス1
	//	100 * SECOND,		//怪盗1
	//	100 * SECOND,		//怪盗2
	//	100 * SECOND,		//怪盗3のアーム
	//	100 * SECOND,		//怪盗3の手部分
	//	100 * SECOND			//海賊1
	//};

	//消滅するまでの時間
	int BULLET_LIMITTIMER[] =
	{
		100 * SECOND,		//リス1
		3  * SECOND,		//怪盗1
		100 * SECOND,		//怪盗2
		100 * SECOND,		//怪盗3のアーム
		100 * SECOND,		//怪盗3の手部分
		3  * SECOND,		//海賊1
		100 * SECOND,		//海賊2
	};

	//あたり判定が円状でないものに関してはNULL
	const float BULLET_RADIUS[] =
	{
		10.0f,		//リス1
		10.0f,		//怪盗1
		10.0f,		//怪盗2
		NULL,		//怪盗3のアーム
		100.0f,		//怪盗3の手部分
		0.5f,		//海賊1
		0.5f		//海賊2
	};
}

class BulletManager
{
public:
	//	定数
	static const int BULLET_MAX = 201;		//	弾最大数
	static const int BULLET_KIND_MAX = 10;
private:


	//	オブジェクト
	iexMesh*	org[BULLET_KIND_MAX];			//	オリジナルモデル
	BaseBullet*	c_Bullet[BULLET_MAX];

	//	変数
	int				bullet_num;	//	弾数
	int				limitTimer;

public:
	//	初期化・解放
	BulletManager(void);
	~BulletManager(void);
	bool	Initialize(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	動作関数
	void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int playerNum);
	void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const Vector3 angle, float speed, int playerNum);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const float& scale, float speed);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int leanpower);
	void	DistCheck(int n);
};

extern	BulletManager*	m_BulletManager;

//******************************************************************************
#endif // !__BULLET_MANAGER_H__
