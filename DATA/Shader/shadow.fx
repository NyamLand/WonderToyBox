
//************************************************************************
//
//	Shadow
//
//************************************************************************

//-----------------------------------------------------------------------------------
//	���֘A
//-----------------------------------------------------------------------------------
	float4x4		ShadowProjection;

//-----------------------------------------------------------------------------------
//	�e�N�X�`���T���v���[
//-----------------------------------------------------------------------------------

	//	�V���h�E�}�b�v
	texture	ShadowMap;
	sampler	ShadowSamp = sampler_state
	{
		Texture = <ShadowMap>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		BorderColor = 0xFFFFFFFF;
		AddressU = BORDER;
		AddressV = BORDER;
	};

//-----------------------------------------------------------------------------------
//	���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------

	struct VS_SHADOW
	{
		float4	Pos	:	POSITION;
		float4	Color	:	TEXCOORD0;		//	���_�F
	};

	struct V_FULL_S
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;
		float4	Ambient	:	COLOR1;
		float4	light		:	COLOR2;
		float3	vLight	:	TEXCOORD1;
		float3	vE			:	TEXCOORD2;
		float3	pLight	:	TEXCOORD3;

		float3	vShadow	:	TEXCOORD4;
	};

//-----------------------------------------------------------------------------------
//	�V���h�E�}�b�v
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float		adjustValue = -0.004f;
	float		Shadow = 0.4f;


	//	�e�e�N�X�`�����擾
	inline		float3	GetShadowTex( float3 Pos )
	{
		float3	Tex;

		//	�e�N�X�`�����W�v�Z
		float4	ppp;
		ppp.xyz = Pos;
		ppp.w = 1;
		Tex = mul( ppp, ShadowProjection );

		Tex.y = -Tex.y;
		Tex.xy = 0.5f * Tex.xy + 0.5f;

		return	Tex;
	}

	//	�e���擾
	inline		float		GetShadow( float3	 Tex )
	{
		float	d = tex2D( ShadowSamp, Tex.xy ).r;	//	�V���h�E�}�b�v����[�x�����o��
		float	l = ( d < Tex.z + adjustValue ) ? Shadow : 1;

		return	l;
	}

//************************************************************************
//
//	�V���h�E�}�b�v
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_SHADOW	VS_ShadowBuf( float4 Pos	: POSITION )
		{
			VS_SHADOW	Out;

			//	���W�ϊ�
			float4x4		mat = mul( TransMatrix, ShadowProjection );
			Out.Pos = mul( Pos, mat );
			Out.Color = Out.Pos.z;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_ShadowBuf( VS_SHADOW In )	:	COLOR
		{			
			return	In.Color;
		}

//**************************************************************************************************
//
//		�V���h�E�}�b�v�̗p
//
//**************************************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		V_FULL_S	VS_Full_S( VS_INPUT	In )
		{
			V_FULL_S	Out = ( V_FULL_S )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = 1;
			Out.Tex = In.Tex;

			float4	P = mul( In.Pos, TransMatrix );
			float3x3	mat = TransMatrix;
			float3 N = mul( In.Normal, mat );
			N = normalize( N );

			//	�������C�e�B���O
			Out.Ambient.rgb = HemiSphereLight( N );

			//	���_���[�J�����W�n�Z�o
			float3	vx;
			float3	vy = { 0.0f, 1.0f, 0.001f };
			vx = cross( vy, N );
			vx = normalize( vx );
			vy = cross( vx, N );
			vy = normalize( vy );

			//	���C�g�x�N�g���␳
			Out.vLight.x = dot( vx, DirLightVec );
			Out.vLight.y = dot( vy, DirLightVec );
			Out.vLight.z = dot( N, DirLightVec );

			//	�_�����x�N�g���␳
			float3	pvec = ( P - plight_pos ) / plight_range;

			//	���C�g�x�N�g���␳
			Out.pLight.x = dot( vx, pvec );
			Out.pLight.y = dot( vy, pvec );
			Out.pLight.z = dot( N, pvec );

			//	�����x�N�g���␳
			float3	E = P - ViewPos;	//	�����x�N�g��
			Out.vE.x = dot( vx, E );
			Out.vE.y = dot( vy, E );
			Out.vE.z = dot( N, E );

			//	�V���h�E�}�b�v
			Out.vShadow = GetShadowTex( P );
			return	Out;
		}

		//-----------------------------------------------------------------------------------
		//	�s�N�Z���V�F�[�_�[
		//-----------------------------------------------------------------------------------
		float4	PS_Full_S( V_FULL_S In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x += 0.04f * h * E.x;
			Tex.y -= 0.04f * h * E.y;

			//	�@���擾
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	�_����
			float	plight = max( 0, 1 - length( In.pLight ) );
			In.pLight = normalize( In.pLight );
			plight *= max( 0, dot( -In.pLight, N ) );

			light += plight_color * plight;

			//	�������˃x�N�g��
			float3	R = reflect( -E, N );

			//	�X�y�L�����}�b�v�擾
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	���}�b�v
			float3	env = Environment( R ) * ( 1 - sp_tex.a );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * ( light + In.Ambient.rgb ) );
			OUT.rgb = OUT.rgb * env + OUT.rgb;

			//	�X�y�L����
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			//	�V���h�E�}�b�v�K�p
			OUT.rgb *= GetShadow(In.vShadow);

			return	OUT;
		}

		//-----------------------------------------------------------------------------------
		//	�s�N�Z���V�F�[�_�[
		//-----------------------------------------------------------------------------------
		float4	PS_NoLight_S( V_FULL_S In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D(HeightSamp, Tex).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x += 0.04f * h * E.x;
			Tex.y -= 0.04f * h * E.y;

			//	�@���擾
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D(DecaleSamp, Tex);

			//	�V���h�E�}�b�v�K�p
			OUT.rgb *= GetShadow(In.vShadow);

			return	OUT;
		}

//************************************************************************
//
//	�e�N�j�b�N
//
//************************************************************************

	//	�V���h�E�}�b�v
	technique	ShadowBuf
	{
		pass	Pass0
		{
			AlphaBlendEnable		=		false;
			ZWriteEnable				=		true;
			CullMode					=		none;

			VertexShader		=		compile vs_2_0	VS_ShadowBuf();
			PixelShader		=		compile ps_2_0 PS_ShadowBuf();
		}
	}

