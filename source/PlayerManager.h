
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
	//	�萔
	static const int PLAYER_NUM = 4;
	static const int OBJ_MAX = 10;

	//	�ϐ�

private:
	Player*	c_Player[PLAYER_NUM];
	iex3DObj*	org[OBJ_MAX];

public:
	//	�������E���
	PlayerManager( void );
	~PlayerManager( void );
	void	Initialize( int input, int type, Vector3 pos );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	AddCoin( int player );
	void	SubCoin( int player );

	//	�����蔻��
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( Player* p1, Player* p2 );

	//	���擾
	Vector3	GetPos( int player );
	float		GetAngle( int player );
	Matrix	GetMatrix( int player );
	int			GetAttackParam( int player );
	int			GetCoinNum( int player );

	//	���ݒ�
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
};

extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
