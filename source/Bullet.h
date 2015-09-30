
#ifndef __BULLET_H__
#define	__BULLET_H__

//*****************************************************************************
//
//	Bulletクラス
//
//*****************************************************************************
class Bullet
{
private:
	int				limitTimer;
private:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	判定有効時間
	int				number;		//	番号割り当て用

public:
	//	初期化・解放
	Bullet(void);
	~Bullet(void);
	bool	Initialize(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	動作関数
	void	StageCollisionCheck(void);
	void	PlayerCollisionCheck(void);
	void	Move(void);

	//	情報取得・設定
	void	SetPos(Vector3 pos);
	void	SetAngle(float angle);
	void	SetScale(float angle);
	Vector3	GetPos(void);
	float		GetAngle(void);

private:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
