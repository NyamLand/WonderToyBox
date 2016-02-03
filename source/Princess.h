
#ifndef		__PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace PRINCESS
{
	namespace MOTION_FRAME
	{

	}

	enum MOTION_DATA
	{
		WAIT,
		RUN,
		JUMP,
		QUICK,
		POWER,
		GUARD,
		DAMAGE,
		KNOCKBACK,
		FALL,
		WAKEUP,
		WIN,
		LOSE
	};
}

//	class
class Princess : public		BaseChara
{
private:
	int hyperNum;	//	�n�C�p�[�E�F�[�u��

private:
//	void	MotionManagement( int motion )override;

public:
	//	�������E���
	Princess( void );
	~Princess( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	�X�V�E�`��
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	����֐�
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	���ݒ�
	void	SetAttackParam( int attackKind )override;
};

//*********************************************************************************
#endif // !__PRINCESS_H__
