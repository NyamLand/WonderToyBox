

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
		Texture = <Texture>;
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
		float4 wPos	: TEXCOORD4;

		float3	N	:	TEXCOORD5;
		float3	T	:	TEXCOORD6;
		float3	B	:	TEXCOORD7;
	};

	struct VS_INPUT
	{
		float4	Pos		:	POSITION;
		float3	Normal	:	NORMAL;
		float2	Tex		:	TEXCOORD0;

		float4	Color		:	COLOR0;	//	���_�J���[
	};

	struct PS_OUTPUT
	{
		float4	color	:	COLOR0;
		float4	spec	:	COLOR1;
		float4	depth	:	COLOR2;
		float4	normal	:	COLOR3;
	};

//-----------------------------------------------------------------------------------
//	���s��
//-----------------------------------------------------------------------------------

	//	�p�����[�^
	float3	LightVec = { 0.7f, -0.7f, 0.0f };
	float3	LightColor = { 1.0f, 1.0f, 1.0f };

//************************************************************************
//
//	�f�B�t�@�[�h�@�A
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Differed( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1.0f;

			Out.wPos = Out.Pos;

			float3x3		mat = (float3x3)matView;

			//	�J������ԍ��W�n
			Out.N = mul( In.Normal, mat );
			Out.N = normalize( Out.N );
			float3	vy = { 0.0f, 1.0f, 0.001f };
			Out.T = cross( vy, Out.N );
			Out.T = normalize( Out.T );
			Out.B = cross( Out.N, Out.T );
			Out.B = normalize( Out.B );
			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		PS_OUTPUT	PS_Differed( VS_OUTPUT In )
		{
			PS_OUTPUT	OUT;

			float2	Tex = In.Tex;

			//	�f�B�t���[�Y�F
			OUT.color = In.Color * tex2D( DecaleSamp, Tex );

			//	�X�y�L����
			OUT.spec = tex2D( SpecularSamp, Tex );
			OUT.spec.a = 1;

			//	�[�x
			OUT.depth.rgb = In.wPos.z / In.wPos.w;
			OUT.depth.a = 1;

			//	���_��ԁ��J�����ϊ��s��
			float3x3		ToView;
			ToView[0] = normalize( In.T );
			ToView[1] = normalize( In.B );
			ToView[2] = normalize( In.N );

			//	�@���ϊ�
			float3	N = tex2D( NormalSamp, Tex ).xyz - 0.5f;
			float3	normal = normalize( mul( N, ToView ) );
			normal = normal * 0.5f + 0.5f;

			//	�J������Ԗ@��
			OUT.normal.rgb = normal;
			OUT.normal.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	���s��
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_DirLight( float2 Tex : TEXCOORD0 )	:	COLOR
		{
			float4	OUT;
			float4	n = tex2D(DecaleSamp, Tex);
			float3	normal = n.rgb * 2.0f - 1.0f;
			normal = normalize( normal );

			//	���C�g��
			float	r = max( 0, dot( normal, -LightVec ) );

			//	�s�N�Z���F����
			OUT.rgb = r * LightColor;
			OUT.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	�e�N�j�b�N
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�f�B�t�@�[�h�@�A
	//-----------------------------------------------------------------------------------

		//	MRT
		technique	differed
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile vs_3_0 VS_Differed();
				PixelShader = compile ps_3_0 PS_Differed();
			}
		}

	//-----------------------------------------------------------------------------------
	//	�f�B�t�@�[�h�B
	//-----------------------------------------------------------------------------------

		//	���s��
		technique	dirlight
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = One;
				CullMode = None;
				ZEnable = false;
				PixelShader = compile ps_2_0 PS_DirLight();
			}
		}
