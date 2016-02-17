
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"

#include	"item.h"
#include	"Effect.h"

//*******************************************************************************
//
//	Effect�N���X
//
//*******************************************************************************

	//�@���̐錾
	Effect*		m_Effect = nullptr;
	namespace{
		namespace{
			const float POS[3] =
			{
				3.0f,		//	�ő�
				-3.0f,		//	�ŏ�
				0.5f,		//	����
			};
			
			const float CON_POS[3] =
			{
				2.0f,		//	�ő�
				-2.0f,		//	�ŏ�
				0.5f,		//	����
			};
		}
	}

//-------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------

	//	�R���X�g���N�^
	Effect::Effect( void )
	{
	
	}

	//	�f�X�g���N�^
	Effect::~Effect( void )
	{
		SafeDelete( aura.mesh );
		SafeDelete( shieldInfo.obj );
		SafeDelete( confusionInfo.pic );
		SafeDelete( stateInfo.pic );
		
	FOR( 0, PLAYER_MAX )
		{
			SafeDelete( circleInfo.pic[value] );
		}
	}

	//	������
	void	Effect::Initialize( void )
	{
		aura.mesh		= new iexMesh( "DATA/Effect/state/aura.IMO" );
		aura.state		= false;
		storm.mesh		= new iexMesh( "DATA/Effect/cyclone/cyclone.IMO" );
		storm.pos		= Vector3(0.0f, 0.0f, 0.0f);
		storm.time		= 0;
		storm._adjustV	= 0.0f;
		storm.state		= false;

		circleInfo.pic[0] = new iex2DObj("DATA/Effect/circle/PL1.png");
		circleInfo.pic[1] = new iex2DObj("DATA/Effect/circle/PL2.png");
		circleInfo.pic[2] = new iex2DObj("DATA/Effect/circle/PL3.png");
		circleInfo.pic[3] = new iex2DObj("DATA/Effect/circle/PL4.png");
		confusionInfo.pic	= new iex2DObj( "DATA/Effect/confusion.png" );

		shieldInfo.obj		= new iex2DObj( "DATA/Effect/state/Guard-ef.png" );
		stateInfo.pic		= new iex2DObj("DATA/UI/item-state.png");

		FOR( 0, PLAYER_MAX ){
			for (int j = 0; j < 4; j++){
				//	���_�ݒ�
				SetVertex( circleInfo.circle[value].poligon[j]			, 0, 0, 0, 0, 0, 0xFFFFFF );
				SetVertex( circleInfo.circle_out[value].poligon[j]		, 0, 0, 0, 0, 0, 0xFFFFFF );
				SetVertex( confusionInfo.confusion[value].poligon[j]		, 0, 0, 0, 0, 0, 0xFFFFFF );
				SetVertex( confusionInfo.confusion_out[value].poligon[j]	, 0, 0, 0, 0, 0, 0xFFFFFF );
			}
			circleInfo.circle[value].c_pos				= Vector3( 0.0f, 10.0f, 0.0f );	
			circleInfo.circle_out[value].c_pos			= Vector3( 0.0f, 10.0f, 0.0f );
			circleInfo.speed							= 0.0f;
			confusionInfo.confusion[value].c_pos		= Vector3( 0.0f, 10.0f, 0.0f );
			confusionInfo.confusion_out[value].c_pos	= Vector3( 0.0f, 10.0f, 0.0f );
			confusionInfo.state[value]					= false;
			confusionInfo.speed							= 0.0f;

			ImageInitialize( stateInfo.stateEffect[value].image, 0, 0, 90, 90, 0, 0, 256, 256 );
			stateInfo.stateEffect[value].image.obj		= stateInfo.pic;
			stateInfo.stateEffect[value].pos			= Vector3( 0, 0, 0 );
		}
		
		//	�V�[���h������
		ShieldInitialize();
	}

	//	�V�[���h������
	void	Effect::ShieldInitialize( void )
	{
		for ( int n = 0; n < 4; n++ )
		{
			//	��ԏ�����
			shieldInfo.state[n] = false;

			FOR( 0, PLAYER_MAX )
			{
				//	�p�x�A���W������
				shieldInfo.shield[n].v[value].angle = ( D3DX_PI / 2 ) * value;
				shieldInfo.shield[n].pos[value]		= Vector3( 0.0f, 0.0f, 0.0f );
			}
		}

		shieldInfo.scale	= 4.0f;
		shieldInfo.r		= 2.0f;
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	Effect::Update(void)
	{
		if ( gameManager->GetMode() == GAME_MODE::DONKETSU_DIRECTION )
		{
			return;
		}
		//	�I�[��
		//AuraUpdate();

		//	�����^�C�}�[
		StormTimer();
		
		storm._adjustV += 0.02f;

		//	���X�V
		FOR( 0, PLAYER_MAX ){
			PoligonSet( &circleInfo.circle[value],				POS[0],		POS[1],		POS[2] );
			PoligonSet( &circleInfo.circle_out[value],			POS[0],		POS[1],		POS[2] );
			PoligonSet( &confusionInfo.confusion[value],		CON_POS[0], CON_POS[1], CON_POS[2] );
			PoligonSet( &confusionInfo.confusion_out[value],	CON_POS[0], CON_POS[1], CON_POS[2] );
			CirclePosSet( &circleInfo.circle[value],			value,	Vector3( 0.0f, 0.4f, 0.0f ) );
			CirclePosSet( &circleInfo.circle_out[value],		value,	Vector3( 0.0f, 0.4f, 0.0f ) );
			CirclePosSet( &confusionInfo.confusion[value],		value,	Vector3( 0.0f, 6.0f, 0.0f ) );
			CirclePosSet( &confusionInfo.confusion_out[value],	value,	Vector3( 0.0f, 6.0f, 0.0f ) );
		}
			confusionInfo.speed += 0.08f;	//	�����G�t�F�N�g�X�s�[�h
			circleInfo.speed	+= 0.02f;		//	�v���C���[�ԍ��T�[�N���X�s�[�h
		//	��]
		Spin();

		//	�V�[���h�X�V
		Shield();

		//	�A�C�e���G�t�F�N�g�X�V
		FOR( 0, PLAYER_MAX ){
			StateEffectUpdate( value );
		}

	}

	//	�I�[��
	void	Effect::AuraUpdate( void )
	{
		if ( gameManager->GetMode() == GAME_MODE::CLIMAX )	aura.state = true;
		if ( aura.state )
		{
			Vector3 p = characterManager->GetPos( gameManager->GetWorst() ) + Vector3( 0, 1.5f, 0 );
			aura.mesh->SetPos( p );
			aura.mesh->SetScale( 0.02f );
			aura.mesh->SetAngle( 0.0f );

			aura.mesh->Update();
		}
	}

	//	�A�C�e���G�t�F�N�g
	void	Effect::StateEffectUpdate( int num )
	{
		ScalingAlphaUpdate( stateInfo.stateEffect[num].image, 100 );	
		Lerp( stateInfo.stateEffect[num].pos, stateInfo.stateEffect[num].start, stateInfo.stateEffect[num].finish, stateInfo.stateEffect[num].image.t );
		stateInfo.stateEffect[num].image.x = ( int )stateInfo.stateEffect[num].pos.x;	
		stateInfo.stateEffect[num].image.y = ( int )stateInfo.stateEffect[num].pos.y;
	}

	//	�v���C���[�ԍ��ł��̏ꏊ�ɃG�t�F�N�g(�A�C�e���p)
	void	Effect::StateEffectSet( int num ,int state )
	{
		WorldToClient( characterManager->GetPos( num ), stateInfo.stateEffect[num].pos, matView * matProjection );
		stateInfo.stateEffect[num].start	= stateInfo.stateEffect[num].pos + Vector3( 0.0f, 0.0f, 0.0f );
		stateInfo.stateEffect[num].finish	= stateInfo.stateEffect[num].pos + Vector3( 0.0f, -100.0f, 0.0f );
		SetScaling( stateInfo.stateEffect[num].image, 1.0f, true );

		switch ( state )
		{
		case ITEM_TYPE::ATTACK_UP:
			stateInfo.stateEffect[num].image.sx = 0;
			stateInfo.stateEffect[num].image.sy = 0;
			break;

		case ITEM_TYPE::UNRIVALED:
			stateInfo.stateEffect[num].image.sx = 256;
			stateInfo.stateEffect[num].image.sy = 0;
			break;

		case ITEM_TYPE::SPEED_UP:
			stateInfo.stateEffect[num].image.sx = 0;
			stateInfo.stateEffect[num].image.sy = 256;
			break;

		case ITEM_TYPE::MAGNET:
			stateInfo.stateEffect[num].image.sx = 256;
			stateInfo.stateEffect[num].image.sy = 256;
			break;

		case ITEM_TYPE::LIFE:
			stateInfo.stateEffect[num].image.sx = 256 * 2;
			stateInfo.stateEffect[num].image.sy = 0;
			break;

		case PARAMETER_STATE::CONFUSION:
			stateInfo.stateEffect[num].image.sx = 256 * 3;
			stateInfo.stateEffect[num].image.sy = 0;
			break;

		}

	}

	//	����
	void	Effect::StormSet( Vector3 pos, int time)
	{
		if ( storm.state ) return;


		storm.state		= true;
		storm.pos		= pos;
		storm.time		= time;

		storm.mesh->SetPos( storm.pos );
		storm.mesh->SetScale( 0.06f, 0.02f, 0.06f );
		storm.mesh->SetAngle( 0.0f );
		storm.mesh->Update();

	}

	void	Effect::StormTimer( void )
	{
		if ( storm.time > 0 )
		{
			storm.time--;
		}
		else
		{
			storm.state = false;
		}
	}

	//	��]
	void	Effect::Spin( void )
	{
		//	���_��]
		FOR( 0, PLAYER_MAX )
		{
			for (int n = 0; n < 4; n++)
			{
				//	�T�[�N��
				circleInfo.circle_out[value].poligon[n].x = cosf( circleInfo.speed ) * ( circleInfo.circle[value].poligon[n].x - circleInfo.circle[value].c_pos.x ) - sinf( circleInfo.speed ) * ( circleInfo.circle[value].poligon[n].z - circleInfo.circle[value].c_pos.z ) + circleInfo.circle[value].c_pos.x;
				circleInfo.circle_out[value].poligon[n].z = sinf( circleInfo.speed ) * ( circleInfo.circle[value].poligon[n].x - circleInfo.circle[value].c_pos.x ) + cosf( circleInfo.speed ) * ( circleInfo.circle[value].poligon[n].z - circleInfo.circle[value].c_pos.z ) + circleInfo.circle[value].c_pos.z;

				//	����
				confusionInfo.confusion_out[value].poligon[n].x = cosf( confusionInfo.speed ) * ( confusionInfo.confusion[value].poligon[n].x - confusionInfo.confusion[value].c_pos.x ) - sinf( confusionInfo.speed ) * ( confusionInfo.confusion[value].poligon[n].z - confusionInfo.confusion[value].c_pos.z ) + confusionInfo.confusion[value].c_pos.x;
				confusionInfo.confusion_out[value].poligon[n].z = sinf( confusionInfo.speed ) * ( confusionInfo.confusion[value].poligon[n].x - confusionInfo.confusion[value].c_pos.x ) + cosf( confusionInfo.speed ) * ( confusionInfo.confusion[value].poligon[n].z - confusionInfo.confusion[value].c_pos.z ) + confusionInfo.confusion[value].c_pos.z;
			}
		}
	}

	//	�`��
	void	Effect::Render( void )
	{

		shader3D->SetValue( "adjustV", storm._adjustV );
		if( aura.state )	aura.mesh->Render( shader3D,"effect_add" );
		if ( storm.state )		storm.mesh->Render( shader3D, "effect_add" );

		//	�T�[�N���`��
		FOR( 0, PLAYER_MAX ){
			iexPolygon::Render3D( circleInfo.circle_out[value].poligon, 2, circleInfo.pic[value], shader3D, "alpha"  );

			//	�����`��
			if ( confusionInfo.state[value] )
			{
				iexPolygon::Render3D( confusionInfo.confusion_out[value].poligon, 2, confusionInfo.pic, shader3D, "alpha" );
			}
		}

		//	�V�[���h�`��
		RenderShield();

		//	�A�C�e���G�t�F�N�g�����`��
		FOR( 0, PLAYER_MAX ){
			RenderImage( stateInfo.stateEffect[value].image, stateInfo.stateEffect[value].image.sx, stateInfo.stateEffect[value].image.sy, stateInfo.stateEffect[value].image.sw, stateInfo.stateEffect[value].image.sh, IMAGE_MODE::SCALING );
		}
	}

	//	�V�[���h�`��

	void	Effect::RenderShield( void )
	{
		//	�V�[���h�`��
		FOR( 0, PLAYER_MAX )
		{
			if ( shieldInfo.state[value] )
			{
				for ( int n = 0; n < 4; n++ )
				{
					iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
					iexPolygon::Render3D( shieldInfo.shield[value].v[n].v, 2, shieldInfo.obj, RS_COPY );
					iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
				}
			}
		}
	}

//-------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------
	void	Effect::PoligonSet( Circle* c, float max, float min, float height )
	{
			//	�ۑ��p

			c->poligon[0].x		= min		+ c->c_pos.x;
			c->poligon[0].y		= height	+ c->c_pos.y;
			c->poligon[0].z		= max		+ c->c_pos.z;
			c->poligon[0].tu	= 0.0f;
			c->poligon[0].tv	= 0.0f;
			c->poligon[0].color = 0xFFFFFFFF;

			c->poligon[1].x		= max		+ c->c_pos.x;
			c->poligon[1].y		= height	+ c->c_pos.y;
			c->poligon[1].z		= max		+ c->c_pos.z;
			c->poligon[1].tu	= 1.0f;
			c->poligon[1].tv	= 0.0f;
			c->poligon[1].color = 0xFFFFFFFF;


			c->poligon[2].x		= min		+ c->c_pos.x;
			c->poligon[2].y		= height	+ c->c_pos.y;
			c->poligon[2].z		= min		+ c->c_pos.z;
			c->poligon[2].tu	= 0.0f;
			c->poligon[2].tv	= 1.0f;
			c->poligon[2].color = 0xFFFFFFFF;


			c->poligon[3].x		= max		+ c->c_pos.x;
			c->poligon[3].y		= height	+ c->c_pos.y;
			c->poligon[3].z		= min		+ c->c_pos.z;
			c->poligon[3].tu	= 1.0f;
			c->poligon[3].tv	= 1.0f;
			c->poligon[3].color	= 0xFFFFFFFF;

	}

	void	Effect::CirclePosSet( Circle* c, int i, Vector3 add )
	{
			c->c_pos = characterManager->GetPos( i ) + add;
	}


	void	LVChange( LVERTEX &l,Vector3 &p )
	{
		p.x = l.x;
		p.y = l.y;
		p.z = l.z;
	}


//-------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------

	//	�V�[���h
	void	Effect::Shield( void )
	{
		//	�v���C���[�S�l��
		FOR( 0, PLAYER_MAX ){
			//	�K�[�h������Ȃ�������Ƃ΂�
			if ( !shieldInfo.state[value] )	continue;

			//	���W�ݒ�
			shieldInfo.pos[value] = characterManager->GetPos( value );

			//	�|���S��4�����X�V
			for ( int p = 0; p < 4; p++ )
			{
				//	�e�|���S�����W�ݒ�
				shieldInfo.shield[value].pos[p] = Vector3( shieldInfo.pos[value].x + shieldInfo.r * sinf( shieldInfo.shield[value].v[p].angle ),
														   shieldInfo.pos[value].y + 2.0f,
														   shieldInfo.pos[value].z + shieldInfo.r * cosf( shieldInfo.shield[value].v[p].angle ) );

				//	��]
				shieldInfo.shield[value].v[p].angle += 0.01f;

				//	4���_�X�V
				for ( int n = 0; n < 4; n++ )
				{
					//	�e�N�X�`��UV�ݒ�
					float tu = static_cast<float>( n % 2 );
					float tv = static_cast<float>( n / 2 );

					//	���_���W�Z�b�g
					float x = shieldInfo.shield[value].pos[p].x - ( -1.0f * tu ) * ( shieldInfo.scale / 2.0f ) * cosf( shieldInfo.shield[value].v[p].angle );
					float y = shieldInfo.shield[value].pos[p].y + cosf( D3DX_PI * tv ) * shieldInfo.scale / 2;
					float z = shieldInfo.shield[value].pos[p].z - ( -1.0f * tu ) * ( shieldInfo.scale / 2.0f ) * -sinf( shieldInfo.shield[value].v[p].angle );

					//	���_���Z�b�g
					SetVertex( shieldInfo.shield[value].v[p].v[n], x, y, z, tu, tv, 0xFFFFFFFF );
				}
			}
		}
	}

	//	�V�[���h���ݒ�
	void	Effect::SetShield( int player, bool state )
	{
		shieldInfo.state[player] = state;
	}