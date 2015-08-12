
#ifndef __KNIGHT_H__
#define	__KNIGHT_H__

//*********************************************************************************
//
//	KnightNX
//
//*********************************************************************************
class Knight : public		Player
{
private:
	//	è
	enum MotionNum
	{
		STAND = 1,			//	§¿
		POSTURE,				//	\¦
		RUN = 4,				//	è
		ATTACK1,				//	UPiKÚ
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		JUMP,
		GUARD,
	};

private:
	float	attack_r;
	float	attack_t;
	float	lance_r;
	Vector3 attack_topPos;

public:
	//	ú»Eðú
	Knight(void);
	~Knight(void);
	void	SetMotionData(void);

	//	XVE`æ
	void	Update(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	®ìÖ
	void	ModeManagement(void);
	void	Move(void);
	void	Jump(void);
	void	Attack(int attackKind);
	bool	QuickArts(void);
	bool	PowerArts(void);
	bool	HyperArts(void);

};

//*********************************************************************************
#endif // !__KNIGHT_H__
