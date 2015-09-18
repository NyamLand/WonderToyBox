
#ifndef __ITEM_H__
#define	__ITEM_H__

//*****************************************************************************
//
//	Item�N���X
//
//*****************************************************************************
class Item
{
private:
	const float GRAVITY = -0.01f;

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
	Item( void );
	~Item( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	void	Move( void );

	//	���擾�E�ݒ�
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float scale );
	Vector3	GetPos( void );
	float		GetAngle( void );

private:
	friend	class ItemManager;
};

//*****************************************************************************
#endif // !__ITEM_H__
