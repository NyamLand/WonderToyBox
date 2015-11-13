
#ifndef __BULLET_H__
#define	__BULLET_H__

//*****************************************************************************
//
//	Bulletクラス
//
//*****************************************************************************


class BaseBullet
{
protected:
	int				limitTimer;
protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	判定有効時間
	int				number;		//	番号割り当て用
	int				leanpower; //	仰け反り時間用
	int				playerNum;	//何番目のプレイヤーの弾か
	int time;	//生成されてからの時間
	int step;

public:
	//	初期化・解放
	BaseBullet(void);
	~BaseBullet(void);
	virtual bool	Initialize(void) = 0;

	//	更新・描画
	virtual void	Update(void) = 0;
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	動作関数
	bool	StageCollisionCheck(void);
	bool	PlayerCollisionCheck(void);

	//	情報取得・設定
	void	SetPos(Vector3 pos);
	void	SetAngle(float angle);
	void	SetScale(float angle);
	Vector3	GetPos(void);
	float		GetAngle(void);

protected:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
