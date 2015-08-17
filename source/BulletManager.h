
#ifndef		__BULLET_MANAGER_H__
#define		__BULLET_MANAGER_H__

//******************************************************************************
//
//	BulletManagerクラス
//
//******************************************************************************
class BulletManager
{
private:

private:
	//	定数
	static const int BULLET_MAX = 201;		//	弾最大数
	const int LIMIT = ( const int )( 60 * 1.5f );

	//	オブジェクト
	iexMesh*	org;			//	オリジナルモデル
	Bullet*		c_Bullet;

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
	void	Set(const Vector3& pos, const Vector3& vec, float speed);
	void	Set(const Vector3& pos, const Vector3& vec, const float& scale, float speed);
	void	DistCheck(int n);
};

extern	BulletManager*	m_BulletManager;

//******************************************************************************
#endif // !__BULLET_MANAGER_H__
