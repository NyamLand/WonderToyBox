
#ifndef __BLOCK_H__
#define	__BLOCK_H__

//*********************************************************************************
//
//	Blockクラス
//
//*********************************************************************************

class Block
{
private:
	iexMesh*	box;

	//	パラメータ
	Vector3		pos;
	float			scale;
	float			angle;
	//	コントローラ
	iexInput*		input;

	//	パラメータ
	Vector3		attackPos;
	Vector3		move;
	float			speed;
	int				mode;
	int				attackParam;

public:
	//	初期化・解放
	Block( void );
	~Block( void );
	bool	Initialize( int input, int type, Vector3 pos, float scale );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	Move( void );
	void	Move( float speed );

	//	角度補正関数
	void	AngleAdjust(float speed);
	void	AngleAdjust(const Vector3& direction, float speed);

	//	情報設定
	void	SetPos(Vector3 pos);
	void	SetPos(float x, float y, float z);
	void	SetAngle(float angle);
	void	SetScale(float scale);

	//	情報取得
	Vector3	GetPos(void);
	Vector3	GetAttackPos(void);
	Matrix	GetMatrix(void);
	float		GetAngle(void);
	int			GetAttackParam(void);

};
extern	Block*	m_Block;

//*********************************************************************************
#endif // !__BLOCK_H__
