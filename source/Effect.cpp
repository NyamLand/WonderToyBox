
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Particle.h"

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

		}
	}

//-------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------

	//	�R���X�g���N�^
	Effect::Effect( void ) : aura( nullptr ), isAura( false ), _adjustV( 0 )
	{
	
	}

	//	�f�X�g���N�^
	Effect::~Effect( void )
	{
		SafeDelete( aura );
		SafeDelete( pow_up.obj );
		
		for ( int i = 0; i < 4; i++ )
		{
			SafeDelete( circle_pic[i] );
		}
	}

	//	������
	void	Effect::Initialize()
	{
		aura = new iexMesh("DATA\\Effect\\aura.IMO");

		circle_pic[0] = new iex2DObj("DATA/Effect/PL1.png");
		circle_pic[1] = new iex2DObj("DATA/Effect/PL2.png");
		circle_pic[2] = new iex2DObj("DATA/Effect/PL3.png");
		circle_pic[3] = new iex2DObj("DATA/Effect/PL4.png");

		pow_up.obj = new iex2DObj("DATA/Effect/ol-r.png");
		ImageInitialize(pow_up, 0, 0, 90, 90, 0, 0, 0, 0);

		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				//	���_�ݒ�
				SetVertex(circle[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
				SetVertex(circle_out[i].poligon[j], 0, 0, 0, 0, 0, 0xFFFFFF);
			}
			circle[i].c_pos = Vector3( 0.0f, 10.0f, 0.0f );	
			circle_out[i].c_pos = Vector3( 0.0f, 10.0f, 0.0f );
		}

		c_angle = 0.0f;
		pow_pos = Vector3(0, 0, 0);
		pow_time = 0;
		

		ShieldInitialize();
	}

	//	�V�[���h������
	void	Effect::ShieldInitialize( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			shield.v[i].angle = ( PI / 2 ) * i;
			shield.pos[i] = Vector3( 0.0f, 0.0f, 0.0f );
		}

		shield.scale = 2.0f;
		shield.r = 2.0f;
	}

//-------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------

	//	�X�V
	void	Effect::Update(void)
	{
		if (gameManager->GetMode() == GAME_MODE::DONKETSU_DIRECTION)
		{
			return;
		}
		if (gameManager->GetMode() == GAME_MODE::CLIMAX)	isAura = true;
		if (isAura)
		{
			Vector3 p = characterManager->GetPos(gameManager->GetWorst()) + Vector3(0, 1.5f, 0);
			aura->SetPos(p);
			aura->SetScale(0.02f);
			aura->SetAngle(0.0f);
			aura->Update();
		}
		_adjustV += 0.02f;
		shader3D->SetValue("adjustV", _adjustV);

		//	���X�V
		for (int i = 0; i < 4; i++){
			PoligonSet(&circle[i]);
			PoligonSet(&circle_out[i]);
			CirclePosSet(&circle[i], i);
			CirclePosSet(&circle_out[i], i);
		}
		//	��]
		Spin();
		c_angle += 0.02f;

		//	�p���[�X�V
		pow_time++;
		if (pow_up.waveState == false){
			SetWave(pow_up, 2.0f);
		}
		WaveUpdate(pow_up, 30, 0.3f);
		//particle->Arrow_DOWN(circle_out[1].c_pos, 1.5f);
		//particle->Arrow_UP(circle_out[0].c_pos, 1.5f);

		Shield( characterManager->GetPos( 0 ) );


	}

	//	��]
	void	Effect::Spin( void )
	{
		//	���_��]
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				circle_out[i].poligon[n].x = cosf(c_angle) * (circle[i].poligon[n].x - circle[i].c_pos.x) - sinf(c_angle) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.x;
				circle_out[i].poligon[n].z = sinf(c_angle) * (circle[i].poligon[n].x - circle[i].c_pos.x) + cosf(c_angle) * (circle[i].poligon[n].z - circle[i].c_pos.z) + circle[i].c_pos.z;
			}
		}
	}

	//	�`��
	void	Effect::Render( void )
	{
		if(isAura)	aura->Render(shader3D,"effect_add");
		for (int i = 0; i < 4; i++){
			iexPolygon::Render3D(circle_out[i].poligon, 2, circle_pic[i], shader3D, "alpha" );
		}

		//	�V�[���h�`��
		for ( int i = 0; i < 4; i++ )
		{
			iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			iexPolygon::Render3D( shield.v[i].v, 2, circle_pic[0], RS_COPY );
			iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
		}

		for (int i = 0; i < 4; i++){
			//	�v���C���[�̃|�W�V�������2�����ʒu�����
			WorldToClient(circle_out[i].c_pos + Vector3(0, 2.0f, 0), pow_pos, matView* matProjection);
			pow_up.x = (int)pow_pos.x;	pow_up.y = (int)pow_pos.y;
			
			//	�x�[�X�iNORMAL�j�̃p�����[�^�p
			pow_up.renderflag = true;	pow_up.alpha = 0.3f;

		//	RenderImage(pow_up, 0, 0, 512, 512, IMAGE_MODE::ADOPTPARAM);
		//	RenderImage(pow_up, 0, 0, 512, 512, IMAGE_MODE::WAVE);
		}
	}



//-------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------
	void	Effect::PoligonSet( Circle* c )
	{
			//	�ۑ��p

			c->poligon[0].x = POS[1]	+ c->c_pos.x;
			c->poligon[0].y = POS[2]	+ c->c_pos.y;
			c->poligon[0].z = POS[0]	+ c->c_pos.z;
			c->poligon[0].tu = 0.0f;
			c->poligon[0].tv = 0.0f;
			c->poligon[0].color = 0xFFFFFFFF;

			c->poligon[1].x = POS[0]	+ c->c_pos.x;
			c->poligon[1].y = POS[2]	+ c->c_pos.y;
			c->poligon[1].z = POS[0]	+ c->c_pos.z;
			c->poligon[1].tu = 1.0f;
			c->poligon[1].tv = 0.0f;
			c->poligon[1].color = 0xFFFFFFFF;


			c->poligon[2].x = POS[1] + c->c_pos.x;
			c->poligon[2].y = POS[2] + c->c_pos.y;
			c->poligon[2].z = POS[1] + c->c_pos.z;
			c->poligon[2].tu = 0.0f;
			c->poligon[2].tv = 1.0f;
			c->poligon[2].color = 0xFFFFFFFF;


			c->poligon[3].x = POS[0] + c->c_pos.x;
			c->poligon[3].y = POS[2] + c->c_pos.y;
			c->poligon[3].z = POS[1] + c->c_pos.z;
			c->poligon[3].tu = 1.0f;
			c->poligon[3].tv = 1.0f;
			c->poligon[3].color = 0xFFFFFFFF;

	}

	void	Effect::CirclePosSet( Circle* c, int i )
	{
			c->c_pos = characterManager->GetPos(i) + Vector3( 0.0f, 0.4f, 0.0f );
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
	void	Effect::Shield( const Vector3& pos )
	{
		shield.r = 2.0f;
		shield.scale = 2.0f;

		//	�|���S��4�����X�V
		for ( int i = 0; i < 4; i++ )
		{
			//	�e�|���S�����W�ݒ�
			shield.pos[i] = Vector3( pos.x + shield.r * sinf( shield.v[i].angle ), pos.y + 2.0f, pos.z + shield.r * cosf( shield.v[i].angle ) );

			//	��]
			shield.v[i].angle += 0.01f;

			//	4���_�X�V
			for ( int n = 0; n < 4; n++ )
			{
				//	�e�N�X�`��UV�ݒ�
				float tu = static_cast<float>( n % 2 );
				float tv = static_cast<float>( n / 2 );

				//	���_���W�Z�b�g
				float x = shield.pos[i].x - ( -1.0f * tu ) * ( shield.scale / 2.0f ) * cosf( shield.v[i].angle );
				float	y = shield.pos[i].y + cosf( D3DX_PI * tv ) * shield.scale;
				float	z = shield.pos[i].z + ( shield.scale / 2.0f ) * (  ( -1.0f * tu )* sinf( shield.v[i].angle ) );

				//	���_���Z�b�g
				SetVertex( shield.v[i].v[n], x, y, z, tu, tv, 0xFFFFFFFF );
			}
		}
	}