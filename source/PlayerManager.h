
#ifndef		__PLAYERMANAGER_H__
#define		__PLAYERMANAGER_H__

//****************************************************************************************
//
//	PlayerManager�N���X
//
//****************************************************************************************
class PlayerManager
{
private:
	static const int PLAYER_NUM = 4;

private:
	Player*	c_Player[PLAYER_NUM];

public:
	//	�������E���
	PlayerManager( void );
	~PlayerManager( void );
	void	Initialize( int input, int type, Vector3 pos );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	HitCheck( void );

	//	���擾
	Vector3	GetPos( int player );
	float		GetAngle( int player );
	Matrix	GetMatrix( int player );
	int			GetAttackParam( int player );

	//	���ݒ�
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
};

extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
