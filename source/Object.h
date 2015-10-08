
#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Object�N���X
//
//*******************************************************************************

class Object
{
protected:
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
	virtual	void	StageCollisionCheck( void );
	virtual	void	PlayerCollisionCheck( void );
	virtual	void	Move( void );
	virtual	void	HitDuaringTheOperation( const Vector3& pos, const int& num );	//	�q�b�g������

	//	���ݒ�
	void	SetObj( iexMesh* obj );
	void	SetPos( Vector3 pos );
	void	SetScale( float scale );
	void	SetAngle( float angle );

	//	���擾
	Vector3	GetPos( void )const;
	float		GetAngle( void )const;
	bool		GetState( void )const;
};

//*******************************************************************************
#endif // !__OBJECT_H__
