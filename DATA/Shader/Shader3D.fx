
//-----------------------------------------------------------------------------------
//	���֘A
//-----------------------------------------------------------------------------------

	float4x4		Projection;	//	���e�ϊ��s��
	float4x4		TransMatrix;	//	���[���h�ϊ��s��
	float4x4		matView;
	float3		ViewPos = { 0.0f, 0.0f, 0.0f };	//	�����ϐ�

//-----------------------------------------------------------------------------------
//	�e�N�X�`���T���v���[
//-----------------------------------------------------------------------------------

	//	�f�J�[���T���v���[
	texture	Texture;
	sampler	DecaleSamp = sampler_state
	{
		Texture		=	<Texture>;
		MinFilter	=	LINEAR;
		MagFilter	=	LINEAR;
		MipFilter	=	NONE;

		AddressU	=	Wrap;
		AddressV	=	Wrap;
	};

	//	���p�e�N�X�`���T���v���[
	texture	EnvTex;
	sampler	EnvironmentSamp = sampler_state
	{
		Texture = <EnvTex>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	�@���e�N�X�`���T���v���[
	texture	NormalMap;
	sampler	NormalSamp = sampler_state
	{
		Texture = <NormalMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	�����e�N�X�`���T���v���[
	texture	HeightMap;
	sampler	HeightSamp = sampler_state
	{
		Texture = <HeightMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	�X�y�L�����T���v���[
	texture SpecularMap;
	sampler SpecularSamp = sampler_state
	{
		Texture = <SpecularMap>;

		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	���˃}�b�v
	texture	RefMap;
	sampler	RefSamp = sampler_state
	{
		Texture = <RefMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	�t�F�C�X�T���v���[
	texture SSSMap;
	sampler FaceSamp = sampler_state
	{
		Texture = <SSSMap>;

		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

//-----------------------------------------------------------------------------------
//	���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------
	
	struct VS_OUTPUT
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;

		float4	Ambient	:	COLOR1;
		//float4	light			:	COLOR2;
		float3	vLight	:	TEXCOORD1;		//	���C�g�x�N�g��(���_���)
		float3	vE			:	TEXCOORD2;		//	�����x�N�g��(���_���)

		float3	pLight	:	TEXCOORD3;	//	�_�����p�����[�^
	};

	struct VS_INPUT
	{
		float4	Pos		:	POSITION;
		float3	Normal	:	NORMAL;
		float2	Tex		:	TEXCOORD0;

		float4	Color		:	COLOR0;	//	���_�J���[
	};

	struct VS_OUTPUT_FOG
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;

		float		Fog	:	TEXCOORD1;

		float4	worldPos		:	TEXCOORD2;		//	���[���h�̍��W
		float3	worldNormal	:	TEXCOORD3;		//	���[���h�̖@��
	};

	struct VS_OUTPUT_TOON
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;

		float		Level		:	TEXCOORD1;
	};

	struct VS_HDR
	{
		float4	Pos		:	POSITION;
		float3	wPos		:	TEXCOORD2;
		float3	Normal	:	TEXCOORD1;
		float2	Tex		:	TEXCOORD0;
	};

	struct V_FULL
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;

		float4	Ambient	:	COLOR1;
		float4	light			:	COLOR2;
		float3	vLight		:	TEXCOORD1;
		float3	vE				:	TEXCOORD2;

		float3	pLight		:	TEXCOORD3;	//	�_�����p�����[�^
	};

	struct V_EFFECT
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;
	};

	struct VS_DEPTH
	{
		float4	Pos		:	POSITION;
		float		Depth	:	TEXCOORD0;
	};

	struct V_FULL_SR
	{
		float4	Pos			:	POSITION;
		float4	Color			:	COLOR0;
		float2	Tex			:	TEXCOORD0;
		float4	Ambient	:	COLOR1;
		float4	light			:	COLOR2;
		float3	vLight		:	TEXCOORD1;
		float3	vE				:	TEXCOORD2;
		float3	pLight		:	TEXCOORD3;
		
		float3	vShadow	:	TEXCOORD4;
		float4	wPos			:	TEXCOORD5;
	};
	
//-----------------------------------------------------------------------------------
//	����
//-----------------------------------------------------------------------------------
	
	//	�F
	float3	AmbientColor = { 0.9f, 0.9f, 0.9f };

//-----------------------------------------------------------------------------------
//	���s��
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float3	DirLightVec = { 1.0f, 0.0f, 1.0f };
	float3	DirLightColor = { 1.0f, 0.0f, 0.0f };

	//	���s��
	float3	DirLight( float3 normal )
	{
		float3	vec = normalize( DirLightVec );
		float		intensity = max( 0, dot( vec, -normal ) );

		return	DirLightColor * intensity;
	}

	//	���s���Q
	float3	DirLight2( float3 light, float3 normal )
	{
		float3	vec = normalize( light );
		float	intensity = max( 0, dot( vec, -normal ) );
		
		return	DirLightColor * intensity;
	}

//-----------------------------------------------------------------------------------
//	�_����
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float3	plight_color = { 1.0f, 0.0f, 0.5f };
	float3	plight_pos = { 0.0f, 0.5f, 0.0f };
	float		plight_range = 5.0f;

//-----------------------------------------------------------------------------------
//	�������C�e�B���O
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float3	SkyColor = { 0.3f, 0.4f, 0.5f };
	float3	GroundColor = { 0.4f, 0.3f, 0.1f };

	//	�������C�e�B���O
	float3	HemiSphereLight( float3 normal )
	{
		float	rate = normal.y * 0.5f + 0.5f;
		float3 color = GroundColor * ( 1 - rate ) + SkyColor * rate;

		return	color;
	}

//-----------------------------------------------------------------------------------
//	�X�L���V�F�[�_�[
//-----------------------------------------------------------------------------------

	//	���ːF
	float3	epiScatter = { 1.0f, 0.85f, 0.8f };	//	1�����ːF
	float3	subScatter = { 0.95f, 0.5f, 0.4f };	//	�Q�����ːF

	//	�e�J���F
	float3	sp_primary = { 0.75f, 0.9f, 1.0f };
	float3	sp_secondary = { 0.9f, 0.95f, 1.0f };

	//	���ߐF
	float3	backScatter = { 0.7f, 0.1f, 0.1f };

//-----------------------------------------------------------------------------------
//	�t�H�O
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float		FogNear		=		10.0f;
	float		FogFar			=		25.0f;
	float3	FogColor		=		{ 0.9f, 0.95f, 1.0f };

	//	�t�H�O
	float	Fog( float3 pos )
	{
		float rate;
		rate = ( FogFar - pos.z ) / ( FogFar - FogNear );
		rate = saturate( rate );

		return	rate;
	}

//-----------------------------------------------------------------------------------
//	�X�y�L�����`��
//-----------------------------------------------------------------------------------

	//	�X�y�L����
	float	Specular( float3 pos, float3 N )
	{
		float	rate;

		float3	vE = normalize( pos - ViewPos );	//	�����x�N�g��
		float3	vL = normalize( DirLightVec );		//	���C�g�x�N�g��
		float3	H = -normalize( vE + vL );				//	�����ƃ��C�g�̃n�[�t

		rate = max( 0, dot( H, N ) );	//	���邳�Z�o
		rate = pow( rate, 30 );			//	����(��)

		return	rate;
	}

	//	�X�y�L�����v�Z( �ʏ���L�c�C )
	float	HDRSpecular( float3 pos, float3 normal )
	{
		float	sp;
		float3 H = normalize( ViewPos - pos );
		H = normalize( H - DirLightVec );

		sp = dot( normal, H );
		sp = max( 0, sp );
		sp = pow( sp, 50 );

		return	sp;
	}

//-----------------------------------------------------------------------------------
//	���}�b�v
//-----------------------------------------------------------------------------------

	//	�f�荞�݂̋
	float	EnvParam = 0.5f;

	//	���}�b�v
	float4	Environment( float3 normal )
	{
		float4	color;

		float2	uv;
		uv = normal.xy * 0.5f + 0.5f;
		uv.y = 1 - uv.y;
		color = tex2D( EnvironmentSamp, uv );
		
		return	color;
	}

//-----------------------------------------------------------------------------------
//	�g�D�[��
//-----------------------------------------------------------------------------------

	//	�֊s���̑���
	float	OutlineSize = 1.0f;	

	//	�e�p
	float	ToonLevel = 0.3f;		//	�e�̃��x��
	float	ToonShadow = 0.4f;	//	�e�̔Z��

//-----------------------------------------------------------------------------------
//	�G�t�F�N�g�p
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float	adjustU = 0;
	float	adjustV = 0;

	//	���̑�
	float3		colorParam = { 0.0f, 0.0f, 0.0f };

//************************************************************************
//
//	�ʏ�`��
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Basic( VS_INPUT	In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1.0f;

			//	�����K�p
			Out.Color.rgb = AmbientColor;

			//	�@���ϊ�
			float3x3		mat = ( float3x3 )TransMatrix;
			float3		N = mul( In.Normal, mat );

			//	���s���K�p
			Out.Color.rgb += DirLight( N );
			
			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Basic( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//************************************************************************
//
//	�������C�e�B���O�ƃt�H�O
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_FOG	VS_Lighting( VS_INPUT In )
		{
			VS_OUTPUT_FOG	Out = ( VS_OUTPUT_FOG )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.a = 1;

			//	�@���ϊ�
			float3x3	 mat = ( float3x3 )TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	���s���K�p
			Out.Color.rgb += DirLight( N );

			//	�������C�e�B���O�K�p
			Out.Color.rgb += HemiSphereLight( N );

			//	�t�H�O
			Out.Fog = Fog( Out.Pos );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Lighting( VS_OUTPUT_FOG In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	�t�H�O�̗p
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	�X�y�L����
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_FOG	VS_Lighting2( VS_INPUT In )
		{
			VS_OUTPUT_FOG	Out = ( VS_OUTPUT_FOG )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.a = 1;

			//	�@���ϊ�
			float3x3	 mat = ( float3x3 )TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	���s���K�p
			Out.Color.rgb += DirLight( N );

			//	�������C�e�B���O�K�p
			Out.Color.rgb += HemiSphereLight( N );

			//	�t�H�O
			Out.Fog = Fog( Out.Pos );

			//	�O���[�o���|�W�V�������@��
			Out.worldPos = mul( In.Pos, TransMatrix );
			float3x3		m = ( float3x3 )TransMatrix;
			Out.worldNormal = mul( In.Normal, m );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Lighting2( VS_OUTPUT_FOG In ) : COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			In.worldNormal = normalize( In.worldNormal );

			//	�X�y�L����
			OUT.rgb += Specular( In.worldPos, In.worldNormal );

			//	�t�H�O�̗p
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	���}�b�v
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Env( VS_OUTPUT_FOG In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );
			In.worldNormal = normalize( In.worldNormal );

			//	���}�b�vUV�v�Z
			float2	euv = ( In.worldNormal.xy * 0.5f ) + 0.5f;
			euv.y = 1 - euv;

			//	���}�b�v
			float4	Env = tex2D( EnvironmentSamp, euv );

			//	���}�b�v�u�����h
			OUT = ( OUT * ( 1 - EnvParam ) ) + ( Env * EnvParam );

			//	�X�y�L����
			OUT.rgb += Specular( In.worldPos, In.worldNormal );

			//	�t�H�O�̗p
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	�g�D�[���V�F�[�_�[
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�A�E�g���C�����_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Outline( VS_INPUT In )
		{
			VS_OUTPUT Out = ( VS_OUTPUT )0;

			//	�@�������Ɋg��
			In.Normal = normalize( In.Normal );
			In.Pos.xyz += In.Normal * OutlineSize;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.rgb = 0;
			Out.Color.a = 1;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�g�D�[���e�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_TOON	VS_Toon( VS_INPUT In )
		{
			VS_OUTPUT_TOON	Out = ( VS_OUTPUT_TOON )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1;

			//	�@���ϊ�
			float3x3		mat = TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	���C�g�v�Z
			float3	vec = normalize( DirLightVec );
			Out.Level = dot( vec, -N );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		
		//	�֊s��
		float4	PS_Outline( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color;

			OUT.r += colorParam.r;
			OUT.g += colorParam.g;
			OUT.b += colorParam.b;

			return	OUT;
		}

		//	�g�D�[���e�V�F�[�_�[
		float4	PS_Toon( VS_OUTPUT_TOON In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = tex2D( DecaleSamp, In.Tex );

			//	���x���������e�ɂ���
			if ( In.Level < ToonLevel )	OUT.rgb *= ToonShadow;
			
			OUT.r += colorParam.r;
			OUT.g += colorParam.g;
			OUT.b += colorParam.b;

			return	OUT;
		}

//************************************************************************
//
//	�@���}�b�v,�����}�b�v
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_FullFX( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			float4		P			=	mul(In.Pos, TransMatrix);
			float3x3		mat		=	TransMatrix;
			float3		N			=	mul( In.Normal, mat );

			N = normalize( N );

			Out.Color.rgb = AmbientColor;
			Out.Color.rgb += HemiSphereLight( N );

			//	���_���[�J�����W�n�Z�o
			float3	vx;
			float3	vy = { 0.0f, 1.0f, 0.001f };
			vx = cross( vy, N );
			vx = -normalize( vx );
			vy = cross( N, vx );
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

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�@���}�b�v)
	//-----------------------------------------------------------------------------------
		float4	PS_Level1( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�@���擾
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light );

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�����}�b�v�K�p)
	//-----------------------------------------------------------------------------------
		float4	PS_Level2( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float		h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	�@���擾
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * light );

			return	OUT;
		}

//************************************************************************
//
//	�X�y�L�����}�b�v,���}�b�v
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�X�y�L�����}�b�v�K�p)
	//-----------------------------------------------------------------------------------
		float4	PS_Level3( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	�@���擾
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	�������˃x�N�g��
			float3	R = reflect( -E, N );

			//	�X�y�L�����}�b�v�擾
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * light );

			//	�X�y�L����
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(���}�b�v�K�p)
	//-----------------------------------------------------------------------------------
		float4	PS_Level4( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	�@���擾
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	�������˃x�N�g��
			float3	R = reflect( -E, N );

			//	�X�y�L�����}�b�v�擾
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	���}�b�v
			float3	env = ( 1 - sp_tex.a ) * Environment( R );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light + env );

			//	�X�y�L����
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�_�����K�p)
	//-----------------------------------------------------------------------------------
		float4	PS_Level5( VS_OUTPUT In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

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
			float3	env = ( 1 - sp_tex.a ) * Environment( R );

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light + env );

			//	�X�y�L����
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

//************************************************************************
//
//	�[��HDR�\��
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_HDR	VS_Specular( VS_INPUT In )
		{
			VS_HDR	Out = ( VS_HDR )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;

			//	�X�y�L�����v�Z�p���W
			Out.wPos = mul( In.Pos, TransMatrix );

			//	�X�y�L�����v�Z�p�@��
			float3x3		mat		=		TransMatrix;
			float3		N			=		mul( In.Normal, mat );
			Out.Normal			=		normalize( N );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Specular( VS_HDR In )	:	COLOR
		{
			float4	OUT;

			//	�X�y�L�����v�Z
			float	s = HDRSpecular( In.wPos, In.Normal );
			float4	sp = tex2D( SpecularSamp, In.Tex ) * s;

			//	�s�N�Z���F����
			OUT.rgb = sp * 0.5f;
			OUT.a = 1;
			
			return	OUT;
		}

//************************************************************************
//
//	�G�t�F�N�g�p�V�F�[�_�[
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		V_EFFECT	VS_Effect( VS_INPUT In )
		{
			V_EFFECT	Out = ( V_EFFECT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			Out.Tex.x += adjustU;
			Out.Tex.y += adjustV;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Effect( VS_INPUT In ) : COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//**************************************************************************************************
//
//		�_�����t���t���G�t�F�N�g
//
//**************************************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		V_FULL	VS_Full( VS_INPUT	In )
		{
			V_FULL	Out = ( V_FULL )0;

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

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Full( V_FULL In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�p�����b�N�X�f�B�X�v���[�X�����g
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
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

			return	OUT;
		}

//************************************************************************
//
//	�[�x�`��
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_DEPTH		VS_Depth( float4 Pos : POSITION )
		{
			VS_DEPTH		Out;

			Out.Pos = mul( Pos, Projection );
			Out.Depth = Out.Pos.z;		//	�[�x

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Depth( VS_DEPTH In )	:	COLOR
		{
			float4	OUT;
			OUT.rgb = In.Depth;	//	�[�x��F�Ƃ��ĕ`��
			OUT.a = 1;
			return	OUT;
		}

//	shadow.fx�̃C���N���[�h
#include	"shadow.fx"

//************************************************************************
//
//	���˃}�b�v�쐬
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		V_FULL	VS_Reflect( VS_INPUT In )
		{
			V_FULL	Out = ( V_FULL )0;

			In.Pos.y = -In.Pos.y;

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

			return	Out;
		}

//************************************************************************
//
//	���˃}�b�v�t��
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		V_FULL_SR	VS_FullFX_SR( VS_INPUT	In )
		{
			V_FULL_SR	Out = ( V_FULL_SR )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = 1;
			Out.Tex = In.Tex;

			Out.wPos = mul( In.Pos, Projection );

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
		float4	PS_FullFX_SR( V_FULL_SR In ) : COLOR
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
			OUT.rgb *= GetShadow( In.vShadow );

			float2	ref = ( In.wPos.xy / In.wPos.w ) * 0.5f + 0.5f;
			ref.y = -ref.y;
			float3	Env = tex2D( RefSamp, ref ) * ( OUT.rgb + 0.4f );
			OUT.rgb = lerp( Env, OUT.rgb, 0.3f );

			return	OUT;
		}

//************************************************************************
//
//	�X�L���V�F�[�_�[
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_FullVX( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			float4	P = mul( In.Pos, TransMatrix );

				float3x3	mat = ( float3x3 )TransMatrix;
				float3	N = mul( In.Normal, mat );
				N = normalize( N );

			//	�������C�e�B���O
			Out.Ambient.rgb = HemiSphereLight( N ).rgb;

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

			//	�����x�N�g���␳
			float3	E = P.xyz - ViewPos;	//	�����x�N�g��
			Out.vE.x = dot( vx, E );
			Out.vE.y = dot( vy, E );
			Out.vE.z = dot( N, E );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�L��)
	//-----------------------------------------------------------------------------------
		float4	PS_Skin( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT = (float4)0;
			float2	Tex = In.Tex;
			float4	sss_tex = tex2D(FaceSamp, Tex);

			//	�@���擾
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	���C�g�̋��x
			float	l = dot( In.vLight, -N );
			float3	l1 = (l + 1.0f) * 0.5f;
			float3	l2 = max(0, l);
			float3	l3 = dot(In.vLight, N);

			//	�\��J���[�v�Z
			float3	dif = l1 * 0.3f;
			float3	eps = epiScatter * l1 * sss_tex.r;
			float3	sps = subScatter * l2 * sss_tex.g;
			float3	back = backScatter * l3 * sss_tex.b;

			//	�J���[����
			float3	col = dif * eps * sps * back;

			//	�s�N�Z���F����
			OUT = tex2D( DecaleSamp, Tex );
			OUT.rgb *= col * DirLightColor + In.Ambient.rgb;

			//	�������˃x�N�g��
			float3	E = normalize(In.vE);
			float3	R = reflect(-E, N);

			//	�X�y�L����
			float	ssss = ( 1 - sss_tex.a ) * DirLightColor;
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 5 ) * sp_primary * ssss;
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 35 ) * sp_secondary * ssss;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[(�Ȃ�)
	//-----------------------------------------------------------------------------------
		float4	PS_NoSkin( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	�@���擾
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	���C�g�v�Z
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2(In.vLight, N) + In.Ambient.rgb;

			//	�������˃x�N�g��
			float3	E = normalize(In.vE);
			float3	R = reflect(-E, N);

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb *= light;


			return	OUT;
		}

//************************************************************************
//
//	�e�N�j�b�N
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�ʏ�
	//-----------------------------------------------------------------------------------
		
		//	�ʏ�
		technique	copy
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0	VS_Basic();
				PixelShader		=		compile ps_2_0 PS_Basic();
			}
		}

		//	�ʏ�(�����������蔲��)
		technique	alpha
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				CullMode = CCW;
				ZEnable = true;
				ZWriteEnable = false;

				VertexShader = compile vs_2_0	VS_Basic();
				PixelShader = compile ps_2_0 PS_Basic();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�������C�e�B���O�{�t�H�O+�X�y�L����
	//-----------------------------------------------------------------------------------

		//	�������C�e�B���O+�t�H�O
		technique	lighting
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile	 vs_2_0 VS_Lighting();
				PixelShader		=		compile ps_2_0 PS_Lighting();
			}
		}

		//	�������C�e�B���O�{�t�H�O�{�X�y�L����
		technique	lighting2
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=		compile		vs_2_0 VS_Lighting2();
				PixelShader		=		compile		ps_2_0 PS_Lighting2();
			}
		}

	//-----------------------------------------------------------------------------------
	//	���}�b�v
	//-----------------------------------------------------------------------------------

		//	���}�b�v
		technique	environment
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0		VS_Lighting2();
				PixelShader		=		compile ps_2_0		PS_Env();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�g�D�[��
	//-----------------------------------------------------------------------------------

		//	�g�D�[��
		technique	toon
		{
			//	�֊s��
			pass	Outline
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=	compile vs_2_0 VS_Outline();
				PixelShader		=	compile ps_2_0 PS_Outline();
			}

			//	�g�D�[��
			pass	Toon
			{
				CullMode			=		CCW;
				ZWriteEnable		=		true;

				VertexShader		=		compile vs_2_0 VS_Toon();
				PixelShader		=		compile ps_2_0 PS_Toon();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�@���}�b�v�A�����}�b�v�A�X�y�L�����}�b�v�A���}�b�v
	//-----------------------------------------------------------------------------------

		//	�@���}�b�v�K�p
		technique	level1
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0		VS_FullFX();
				PixelShader		=		compile ps_2_0		PS_Level1();
			}
		};

		//	�����}�b�v�K�p
		technique	level2
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader			=		compile		vs_2_0		VS_FullFX();
				PixelShader			=		compile		ps_2_0		PS_Level2();
			}
		};

		//	�X�y�L�����}�b�v�K�p
		technique	level3
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader			= compile		vs_2_0		VS_FullFX();
				PixelShader			= compile		ps_2_0		PS_Level3();
			}
		};

		//	���}�b�v�K�p
		technique	level4
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader			=		compile		vs_2_0		VS_FullFX();
				PixelShader			=		compile		ps_2_0		PS_Level4();
			}
		};

		//	�_�����K�p
		technique	level5
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZWriteEnable			=		true;

				VertexShader		=		compile		vs_2_0		VS_FullFX();
				PixelShader		=		compile		ps_2_0		PS_Level5();
			}
		};

	//-----------------------------------------------------------------------------------
	//	�[��HDR�\��
	//-----------------------------------------------------------------------------------

		//	�X�y�L����
		technique	specular
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile	vs_2_0	VS_Specular();
				PixelShader		=		compile ps_2_0	PS_Specular();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�G�t�F�N�g�p
	//-----------------------------------------------------------------------------------
		
		//	�ʏ�
		technique	effect_copy
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=		compile	vs_2_0 VS_Effect();
				PixelShader		=		compile ps_2_0 PS_Effect();
			}
		}

		//	���Z
		technique	effect_add
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				srcBlend				=		SrcAlpha;
				DestBlend				=		One;
				CullMode				=		None;
				ZEnable					=		false;

				VertexShader		=		compile	vs_2_0 VS_Effect();
				PixelShader		=		compile ps_2_0 PS_Effect();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�_�����t���t���G�t�F�N�g
	//-----------------------------------------------------------------------------------

		//	�����Ȃ�
		technique full
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;

				VertexShader			=		compile		vs_3_0 VS_Full();
				PixelShader			=		compile		ps_3_0 PS_Full();
			}
		}

		//	�V���h�E�}�b�v�̗p
		technique full_s
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				ZWriteEnable			=		true;

				VertexShader			=		compile		vs_3_0 VS_Full_S();
				PixelShader			=		compile		ps_3_0 PS_Full_S();
			}
		}

		//	�V���h�E�}�b�v�̗p(���C�e�B���O�Ȃ�)
		technique nolight_s
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile		vs_3_0 VS_Full_S();
				PixelShader = compile		ps_3_0 PS_NoLight_S();
			}
		}
	//-----------------------------------------------------------------------------------
	//	��ʊE�[�x
	//-----------------------------------------------------------------------------------
		
		//	�[�x�`��
		technique	depth
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0 VS_Depth();
				PixelShader		=		compile ps_2_0 PS_Depth();
			}
		}

	//-----------------------------------------------------------------------------------
	//	���˃}�b�v
	//-----------------------------------------------------------------------------------

		//	���˃}�b�v�쐬
		technique	Reflect
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;
				CullMode				=	cw;
			
				VertexShader		=		compile vs_3_0 VS_Reflect();
				PixelShader		=		compile ps_3_0 PS_Full();
			}

		}

		//	���˃}�b�v�t���t���G�t�F�N�g
		technique	full_sr
		{
			pass	P0
			{
				AlphaBlendEnable	= true;
				BlendOp				= Add;
				SrcBlend				= SrcAlpha;
				DestBlend				= InvSrcAlpha;
				ZWriteEnable			= true;

				VertexShader		=		compile vs_3_0 VS_FullFX_SR();
				PixelShader		=		compile ps_3_0 PS_FullFX_SR();
			}
		}

	//------------------------------------------------------
	//	�X�L���V�F�[�_�[	
	//------------------------------------------------------
		
		//	�L��
		technique skin
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;
				CullMode = none;
				VertexShader = compile vs_3_0 VS_FullFX();
				PixelShader = compile ps_3_0 PS_Skin();
			}
		}

		//	�Ȃ�
		technique no_skin
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;
				CullMode = none;
				VertexShader = compile vs_3_0 VS_FullFX();
				PixelShader = compile ps_3_0 PS_NoSkin();
			}
		}

	//------------------------------------------------------
	//	�X�N���[���p	
	//------------------------------------------------------

		float effect_size = 0.0f;
		float screen_width = 1280.0f;
		float screen_height = 720.0f;

		float4 PS_WipeEffect(float2 ScreenPos : VPOS) : COLOR
		{
			float p1 = ScreenPos.x - screen_width  * 0.5f;
			float p2 = ScreenPos.y - screen_height * 0.5f;

			return ( p1*p1 + p2*p2 >= effect_size*effect_size ) ? float4( 0.0f, 0.0f, 0.0f, 1.0f ) : float4( 0.0f, 0.0f, 0.0f, 0.0f );
		}

		technique WipeEffect
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				PixelShader = compile ps_3_0 PS_WipeEffect();
			}
		}

