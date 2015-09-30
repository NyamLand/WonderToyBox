
#ifndef __RANDOM_H__
#define	__RANDOM_H__

//*******************************************************************************
//
//	RandomƒNƒ‰ƒX
//
//*******************************************************************************

//	include
#include	<random>
using namespace std;

//	class
class Random
{
private:
	static	std::mt19937* rnd;

public:
	//	‰Šú‰»E‰ğ•ú
	static	bool	Initialize();
	static	void	Release();

	//	“®ìŠÖ”
	static	int		GetInt( int min, int max );
	static	float	GetFloat( float min, float max );
	static	bool	PercentageRandom( float probability );
};

//*******************************************************************************
#endif // !__RANDOM_H__
