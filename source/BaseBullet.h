
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
	Vector3			scale;
	Vector3			angle;
	bool			state;		//存在するかどうか
	bool			activate;	//あたり判定の有無
	float			radius;		//物体の半径(あたり判定で使用))
	int				judgeTimer;	//	判定有効時間
	int				number;		//	番号割り当て用
	int				leanpower; //	仰け反り時間用
	int				playerNum;	//何番目のプレイヤーの弾か
	int liveTime;	//生成されてからの時間
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
	virtual bool	PlayerCollisionCheck(void);

	//	情報取得・設定
	void	SetPos(Vector3 pos);
	void	SetAngle(Vector3 angle);
	void	SetScale(Vector3 scale);
	Vector3	GetPos(void);
	Vector3		GetAngle(void);
	Vector3		GetRight();
	Vector3		GetUp();
	Vector3		GetFront();
	float		GetDegreeAngle(Vector3 vec1, Vector3 vec2);	//2ベクトル間のベクトル取得(度数)

protected:
	friend	class BulletManager;
};

//*****************************************************************************
#endif // !__BULLET_H__
