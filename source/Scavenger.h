
#ifndef __SCAVENGER_H__
#define	__SCAVENGER_H__

//*********************************************************************************
//
//	ScavengerNX
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Scavenger : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		STAND,				//	§¿
		RUN,				//	è
		POSTURE,				//	\¦
		ATTACK1,				//	UPiKÚ
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		JUMP,
		GUARD,
	};
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//ZÖAÌd¼oßÔ
	float	absorb_length;		//RCðz¢ßé£
	void	MotionManagement(int motion)override;

public:
	//	ú»Eðú
	Scavenger(void);
	~Scavenger(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	XVE`æ
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	®ìÖ
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	void	ShiftMove(void);
	void	RollAngle(void);

	//	îñÝè
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
