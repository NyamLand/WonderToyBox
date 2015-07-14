
#include	"iextreme.h"

#include	"Particle.h"

//****************************************************************************************
//
//	Particleä÷òA
//
//****************************************************************************************

iexParticle*	particle = NULL;

//	âä
void	Effect1( float x, float y, float z, float scale )
{
	Vector3	Pos, Move, Power;
	for ( int j = 0; j<5; j++ ){
		Pos.x = x + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );
		Pos.y = y + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );
		Pos.z = z + ( ( rand() % 1000 - 1000 ) * ( 0.001f * scale ) );

		Move.x = 0.0f;
		Move.y = 0.0f;
		Move.z = 0.0f;

		Power.x = 0.0f;
		Power.y = ( rand() % 100 )* ( scale * 0.00015f );
		Power.z = 0.0f;

		particle->Set( 6, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
	}
}

//	âä
void		Effect2( float x, float y, float z )
{
	Vector3	Pos, Move, Power;
	for (int i = 0; i<20; i++){
		Pos.x = x + (rand() % 200 - 100)*.03f;
		Pos.y = y + (rand() % 200 - 100)*.01f;
		Pos.z = z + (rand() % 200 - 100)*.03f;

		Move.x = (rand() % 200 - 100)*.00001f;
		Move.y = (rand() % 500 - 100)*.00006f;
		Move.z = (rand() % 200 - 100)*.00001f;

		Power.x = .0f;
		Power.y = 0.0001f + (rand() % 100)*0.00005f;
		Power.z = .0f;
		particle->Set(9, 0, .0f, 25, .0f, 10, 1.0f, &Pos, &Move, &Power, 1.0f, 0.6f, 0.8f, 1.5f, RS_ADD);

		Pos.x = x + (rand() % 200 - 100)*.03f;
		Pos.y = y + (rand() % 200 - 100)*.01f;
		Pos.z = z + (rand() % 200 - 100)*.03f;

		Move.x = (rand() % 200 - 100)*.00001f;
		Move.y = (rand() % 300 - 100)*.00005f;
		Move.z = (rand() % 200 - 100)*.00001f;

		Power.x = .0f;
		Power.y = (rand() % 100)*0.00005f;
		Power.z = .0f;
		IEX_SetParticle(9, 10, .0f, 60, .0f, 10, 1.0f, &Pos, &Move, &Power, .1f, .3f, .3f, 1.5f, RS_SUB);

	}

}

//	ê·
void		Effect3( float x, float y, float z )
{
	Vector3	Pos, Move, Power;
	for (int i = 0; i<2; i++){
		Pos.x = x + (rand() % 100 - 50);
		Pos.y = y + (rand() % 50);
		Pos.z = z + (rand() % 100 - 50);
		Move.x = .0f;
		Move.y = .01f;
		Move.z = .01f;
		Power.x = .0f;
		Power.y = -0.001f;
		Power.z = .0f;
		particle->Set(12, 0, 1.0f, 240, .0f, 200, 1.0f, &Pos, &Move, &Power, .6f, .6f, .6f, 0.5f, RS_ADD);
	}
}
