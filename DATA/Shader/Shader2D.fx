
//--------------------------------------------------------------------------------
//	�e�N�X�`���T���v���[
//--------------------------------------------------------------------------------

	//	�f�J�[���T���v���[
	texture	Texture;
	sampler	DecaleSamp = sampler_state
	{
		Texture = <Texture>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	��ʊE�[�x
	texture	DepthTex;
	sampler	DepthSamp = sampler_state
	{
		Texture = <DepthTex>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

//--------------------------------------------------------------------------------
//	���_�t�H�[�}�b�g
//--------------------------------------------------------------------------------
	
	//	�QD�p���_�t�H�[�}�b�g
	struct VS_2D
	{
		float4	Pos		:	POSITION0;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;
	};

//-----------------------------------------------------------------------------------
//	��ʊE�[�x
//-----------------------------------------------------------------------------------

	//	�t�H�[�J�X
	float	FocusDist = 10.0f;		//	�œ_����
	float	FocusRange = 5.0f;	//	�͈�


//************************************************************************
//
//	2D�`��ƍ���
//
//************************************************************************

	//--------------------------------------------------------------------------------
	//	���_�V�F�[�_�[
	//--------------------------------------------------------------------------------

		//	�Ȃɂ����Ȃ�
		VS_2D	VS_Basic( VS_2D In )
		{
			return	In;
		}

	//--------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//--------------------------------------------------------------------------------

		//	���_�V�F�[�_�[����󂯎�����s�N�Z���������ƂɃs�N�Z���F������
		float4	PS_Basic( VS_2D In )		:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//************************************************************************
//
//	�X�N���[���t�B���^
//
//************************************************************************
	
	//--------------------------------------------------------------------------------
	//	�t�B���^���̒�`
	//--------------------------------------------------------------------------------

		float3	screenColor = { 1.2f, 1.1f, 0.8f };
		float		contrast = 1.8f;
		float		saturate = 0.6f;
		float		brightness = 0.0f;
	
	//--------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//--------------------------------------------------------------------------------

		float4	PS_Filter( VS_2D In )		:	COLOR
		{
			float4	OUT;

			float3	col = In.Color.rgb * tex2D( DecaleSamp, In.Tex );

			//	�P�x����
			col += brightness;

			//	�R���g���X�g����
			col = ( ( col - 0.5f ) * contrast ) + 0.5f;
			
			//	�ʓx����
			float	avr = ( col.r + col.g + col.b ) / 3;

			//	�J���[�o�����X����
			col *= screenColor;

			//	�s�N�Z���F����
			OUT.rgb = col;
			OUT.w = In.Color.a;

			return	OUT;
		}

//************************************************************************
//
//	��ʊE�[�x
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//-----------------------------------------------------------------------------------
		float4	PS_Focus( VS_2D In )	:	COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	�[�x�E�œ_�E�͈͂����Ƃɓ����x�v�Z
			float	a = tex2D( DepthSamp, In.Tex ).r;
			a = abs( a - FocusDist ) / FocusRange;
			OUT.a *= a > 1 ? 1 : a;

			return	OUT;
		}

//************************************************************************
//
//	���C�v�p�G�t�F�N�g
//
//************************************************************************

	//--------------------------------------------------------------------------------
	//	�p�����[�^
	//--------------------------------------------------------------------------------
		float		wipe_size = 100.0f;
		float		frame_size = 10.0f;
		int			center_posX = 0;
		int			center_posY = 0;
		int			picture_width = 0;
		int			picture_height = 0;
		float3	frame_color = float3( 0.0f, 0.0f, 1.0f );

	//--------------------------------------------------------------------------------
	//	�s�N�Z���V�F�[�_�[
	//--------------------------------------------------------------------------------

		//	�s�N�Z���V�F�[�_�[
		float4	PS_Wipe( VS_2D In, float2 pPos : VPOS ) : COLOR
		{
			float4	OUT;

			//	�s�N�Z���F����
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	�s�N�Z���̒��S����̈ʒu���v�Z
			float p1 = pPos.x - ( center_posX + ( picture_width * 0.5f ) );
			float p2 = pPos.y - ( center_posY + ( picture_height * 0.5f ) );

			//	�t���[���̃T�C�Y���܂ރT�C�Y��ݒ�
			float	renderSize = ( wipe_size + frame_size ) * ( wipe_size + frame_size );
			float	wipeSize = p1 * p1 + p2 * p2;

			//	�ݒ肵���T�C�Y�ȓ��͕\���A�O�͓����ɂ���
			if ( wipeSize >= renderSize )		OUT.a = 0.0f;
			else	OUT.a = 1.0f;

			//	�t���[�������Ȃ����T�C�Y��ݒ�
			renderSize = wipe_size * wipe_size;

			//	�t���[��������ݒ肵���F�ɂ�����
			if ( wipeSize >= renderSize )		OUT.rgb = frame_color;

			return	OUT;
		}

//************************************************************************
//
//	�e�N�j�b�N
//
//************************************************************************
	
	//--------------------------------------------------------------------------------
	//	�Q�c�`��ƍ���
	//--------------------------------------------------------------------------------

		//	�����Ȃ�
		technique	copy
		{
			pass	P0
			{
				AlphaBlendEnable		=		true;
				BlendOp					=		Add;
				SrcBlend					=		SrcAlpha;
				DestBlend					=		InvSrcAlpha;
				CullMode					=		None;
				ZEnable						=		false;

				VertexShader		=	compile		vs_2_0	VS_Basic();
				PixelShader		=	compile		ps_2_0	PS_Basic();
			}
		}

		//	���Z����
		technique	add
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		One;
				CullMode				=		None;
				ZEnable					=		false;

				VertexShader		=		compile		vs_2_0	VS_Basic();
				PixelShader		=		compile		ps_2_0	PS_Basic();

			}
		}

		//	���Z����
		technique	sub
		{
			pass	P0
			{
				AlphaBlendEnable		=		true;
				BlendOp					=		RevSubtract;
				SrcBlend					=		SrcAlpha;
				DestBlend					=		One;
				CullMode					=		None;
				ZEnable						=		false;

				VertexShader		=		compile		vs_2_0	VS_Basic();
				PixelShader		=		compile		ps_2_0	PS_Basic();
			}
		}

	//--------------------------------------------------------------------------------
	//	�X�N���[���t�B���^
	//--------------------------------------------------------------------------------

		//	�t�B���^�[
		technique	filter
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				CullMode = None;
				ZEnable = false;

				VertexShader		=	compile		vs_2_0 VS_Basic();
				PixelShader		=	compile		ps_2_0	PS_Filter();
			}
		}

	//--------------------------------------------------------------------------------
	//	��ʊE�[�x
	//--------------------------------------------------------------------------------

		//	��ʊE�[�x
		technique	focus
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		None;
				ZEnable					=		false;

				PixelShader = compile ps_2_0 PS_Focus();
			}
		}

		//--------------------------------------------------------------------------------
		//	���C�v����
		//--------------------------------------------------------------------------------

		//	���C�v�G�t�F�N�g
		technique WipeEffect
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile		vs_3_0 VS_Basic();
				PixelShader = compile		ps_3_0 PS_Wipe();
			}
		}