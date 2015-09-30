
#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Object�N���X
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
	int				judgeTimer;	//	����L������
public:
	//	�������E���
	Object();
	~Object();
	virtual	bool	Initialize();
	virtual	void	Release();

	//	�X�V�E�`��
	virtual	void	Update();
	virtual	void	Render( iexShader* shader = nullptr, LPSTR technique = "" );

	//	����֐�
};

//*******************************************************************************
#endif // !__OBJECT_H__
