
#ifndef		__BULLET_MANAGER_H__
#define		__BULLET_MANAGER_H__

#include	"BaseBullet.h"
#include	"Squirrel_Bullet01.h"
#include	"Thief_Bullet01.h"
//******************************************************************************
//
//	BulletManagerクラス
//
//******************************************************************************
namespace BULLET_MODEL
{
	enum
	{
		SQUIRREL = 0,
		THIEF_01,
		THIEF_02
	};
}

class BulletManager
{
public:
	//	定数
	static const int BULLET_MAX = 201;		//	弾最大数
	static const int BULLET_KIND_MAX = 10;
	const int LIMIT = (const int)(60 * 1.5f);
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
	void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed,int playerNum);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, const float& scale, float speed);
	//void	Set(int type, BaseBullet* obj, Vector3 pos, const Vector3 vec, float speed, int leanpower);
	void	DistCheck(int n);
};

extern	BulletManager*	m_BulletManager;

//******************************************************************************
#endif // !__BULLET_MANAGER_H__
