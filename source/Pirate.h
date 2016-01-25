
#ifndef __PIRATE_H__
#define	__PIRATE_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

namespace PIRATE
{
	namespace MOTION_FRAME
	{
		const int SHOT = 113;
		const int SHOT_STOP = 131;
	}
	
	enum MOTION_DATA
	{
		WAIT,
		RUN,
		QUICK,
		POWER_START,
		POWER,
		POWER_END,
		HYPER,
		DAMAGE,
		WIN,
		LOSE
	};
}


//	class
class Pirate : public		BaseChara
{
private:
	

	enum QuickArts_DATA
	{
		NORMAL_SHOT = 0,
		TIMER_SHOT,
	};

private:
	//void	MotionManagement( int motion )override;
	iexMesh*	cannon;

public:
	//	�������E���
	Pirate( void );
	~Pirate( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	�X�V�E�`��
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	����֐�
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;
	void	SetCannonPos( void );
	//	���ݒ�
	void	SetAttackParam( int attackKind )override;


};

//*********************************************************************************
#endif // !__PRINCESS_H__
