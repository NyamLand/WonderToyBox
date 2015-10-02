
#ifndef		__PLAYERMANAGER_H__
#define		__PLAYERMANAGER_H__

#include	"Player.h"

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

public:
	//	�ϐ�
	bool	CanHyper;	//�@�n�C�p�[�A�[�c�������������p

private:
	Player*		c_Player[PLAYER_NUM];
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
	void	DonketsuBoost( void );
	void	RaiseStatus( int worst, int type );

	//	�����蔻��
	void	HitCheck( void );
	void	HitCheckSphereVSCapsule( Player* p1, Player* p2 );
	void	HitCheckCapsuleVSCapsule( Player* p1, Player* p2 );

	//	���擾
	Vector3	GetPos( int player )const;
	float		GetAngle( int player )const;
	Matrix	GetMatrix( int player )const;
	bool		GetUnrivaled( int player )const;
	int			GetAttackParam( int player )const;
	int			GetCoinNum( int player )const;
	int			GetType( int player )const;
	int			GetP_Num( int player )const;
	bool		GetCanHyper( int player )const;
	int			GetPower( int player )const;
	float		GetSpeed( int player )const;

	//	���ݒ�
	void		SetPos( int player, Vector3 pos );
	void		SetPos( int player, float x, float y, float z );
	void		SetAngle( int player, float angle );
	void		SetScale( int player, float scale );
	void		SetType( int player, int type );
	void		SetKnockBackVec( int player, Vector3	knockBackVec );
	void		SetMode( int player, int state );
	void		SetPower( int player, int power );
	void		SetSpeed( int player, float speed );
	void		SetBoosting(int player, bool boosting);
};
extern	PlayerManager*		m_Player;
//****************************************************************************************
#endif // !__PLAYERMANAGER_H__
