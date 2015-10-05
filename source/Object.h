
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
	Object( void );
	~Object( void );
	virtual	bool	Initialize( void );
	virtual	void	Release( void );

	//	�X�V�E�`��
	virtual	void	Update( void );
	virtual	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	virtual	void	Move( void );
};

//*******************************************************************************
#endif // !__OBJECT_H__
