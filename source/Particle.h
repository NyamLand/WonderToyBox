
#ifndef		__PARTICLE_H__
#define	__PARTICLE_H__

//****************************************************************************************
//
//	Particleä÷òA
//
//****************************************************************************************

//	ä÷êî
void		Effect1( float x, float y, float z, float scale );
void		Effect2( float x, float y, float z );
void		Effect3( float x, float y, float z );
void		Effect4( Vector3& pos, Vector3& target, float &out );
void		Effect5( Vector3& pos );
void		Moon(Vector3& pos, float scale);
void		Shine(Vector3& pos, float scale);

//	externêÈåæ
extern	iexParticle*		particle;

//****************************************************************************************
#endif // !__PARTICLE_H__
