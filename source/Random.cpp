
#include	"iextreme.h"
#include	"Random.h"

//*******************************************************************************
//
//	Random�N���X
//
//*******************************************************************************

//--------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------

	std::mt19937* Random::rnd = nullptr;

//--------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------

	//	������
	bool	Random::Initialize()
	{
		rnd = new std::mt19937( std::random_device()() );
		return	true;
	}

	//	���
	void	Random::Release()
	{
		if (rnd != nullptr)
		{
			delete	rnd;
			rnd = nullptr;
		}
	}

//--------------------------------------------------------------------------------
//	���擾
//--------------------------------------------------------------------------------

	//	�����^�����擾
	int		Random::GetInt( int min, int max )
	{
		std::uniform_int_distribution<int>	_dist( min, max );
		return	_dist( *rnd );
	}

	//	�����^�����擾
	float	Random::GetFloat( float min, float max )
	{
		std::uniform_real_distribution<float>	_dist( min, max );
		return	_dist( *rnd );
	}

	//	�S�����ł̃����_��
	bool	Random::PercentageRandom( float probability )
	{
		std::bernoulli_distribution _dist( probability );

		return _dist( *rnd );
	}