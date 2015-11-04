
#ifndef __THIEF_H__
#define	__THIEF_H__

//*********************************************************************************
//
//	ThiefƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Thief : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		STAND = 1,			//	—§‚¿
		POSTURE,				//	\‚¦
		RUN = 4,				//	‘–‚è
		ATTACK1,				//	UŒ‚‚P’iŠK–Ú
		ATTACK2,				//	UŒ‚‚Q’iŠK–Ú
		ATTACK3,				//	UŒ‚‚R’iŠK–Ú
		JUMP,
		GUARD,
	};
	const float DEFAULT_ABSORB_LENGTH = 3.0f;

private:
	int		stayTime;	//‹ZŠÖ˜A‚Ìd’¼Œo‰ßŠÔ
	float	absorb_length;		//ƒRƒCƒ“‚ğ‹z‚¢‚ß‚é‹——£
	void	MotionManagement(int motion)override;

public:
	//	‰Šú‰»E‰ğ•ú
	Thief(void);
	~Thief(void);
	bool	Initialize(int playerNum, Vector3 pos)override;

	//	XVE•`‰æ
	void	Render(iexShader* shader = nullptr, LPSTR technique = nullptr)override;

	//	“®ìŠÖ”
	bool	QuickArts(void)override;
	bool	PowerArts(void)override;
	bool	HyperArts(void)override;

	//	î•ñİ’è
	void	SetAttackParam(int attackKind)override;
};

//*********************************************************************************
#endif // !__MAID_H__
