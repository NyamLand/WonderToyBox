
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
	Object();
	~Object();
	virtual	bool	Initialize();
	virtual	void	Release();

	//	更新・描画
	virtual	void	Update();
	virtual	void	Render( iexShader* shader = nullptr, LPSTR technique = "" );

	//	動作関数
};

//*******************************************************************************
#endif // !__OBJECT_H__
