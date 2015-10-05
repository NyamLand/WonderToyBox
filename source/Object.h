
#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

class Object
{
private:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	判定有効時間
public:
	//	初期化・解放
	Object( void );
	~Object( void );
	virtual	bool	Initialize( void );
	virtual	void	Release( void );

	//	更新・描画
	virtual	void	Update( void );
	virtual	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	virtual	void	Move( void );
};

//*******************************************************************************
#endif // !__OBJECT_H__
