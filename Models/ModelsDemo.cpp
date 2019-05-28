/*
Beginning DirectX 11 Game Programming
By Allen Sherrod and Wendy Jones

3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#include"ModelsDemo.h"
#include<xnamath.h>
#include<stdio.h>
#include"objLoader.h"

char* TERRAIN_TEXTURE_NAME = "GameObjects/Menu_Stuff/MENU_SCREEN3.jpg";
char* QUAD_TEXTURE_NAME = "GameObjects/DomeTexture.png";
struct VertexPos
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
	XMFLOAT3 norm;
};

struct TextVertexPos
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};


ModelsDemo::ModelsDemo() : textureMapVS_(0), textureMapPS_(0), textTextureMapVS_(0), textTextureMapPS_(0),
inputLayout_(0), textInputLayout_(0), textVertexBuffer_(0), textColorMapSampler_(0),
vertexBuffer1_(0), colorMap1_(0), textColorMap_(0), colorMapSampler_(0),
viewCB_(0), projCB_(0), worldCB_(0), camPosCB_(0), totalVerts1_(0),
gameState_(PLAY_INTRO), pauseMenuSelection(RETURN), displayFPS(true)
{
	ZeroMemory(&controller1State_, sizeof(XINPUT_STATE));
	ZeroMemory(&prevController1State_, sizeof(XINPUT_STATE));
	ZeroMemory(&mouseCurrState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mousePrevState, sizeof(DIMOUSESTATE));

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	frameTime_ = 0;
	fps_ = 0;

}


ModelsDemo::~ModelsDemo()
{

}


bool ModelsDemo::LoadContent()
{

	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader("TextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (compileResult == false)
	{
		DXTRACE_MSG("Error compiling the vertex shader!");
		return false;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &textureMapVS_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating the vertex shader!");

		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout_);

	//vsBuffer->Release( );

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating the input layout!");
		return false;
	}

	ID3DBlob* psBuffer = 0;

	compileResult = CompileD3DShader("TextureMap.fx", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		DXTRACE_MSG("Error compiling pixel shader!");
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &textureMapPS_);

	//psBuffer->Release( );

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating pixel shader!");
		return false;
	}

	////////////////////////////////////////////text 2d shaders////////////////////////////////////////////////////////////

	//ID3DBlob* vsBuffer = 0;
	vsBuffer = 0;

	compileResult = CompileD3DShader("TextTextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer);

	if (compileResult == false)
	{
		DXTRACE_MSG("Error compiling the vertex shader!");
		return false;
	}

	d3dResult = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &textTextureMapVS_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating the vertex shader!");

		if (vsBuffer)
			vsBuffer->Release();

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC textSolidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	totalLayoutElements = ARRAYSIZE(textSolidColorLayout);

	d3dResult = d3dDevice_->CreateInputLayout(textSolidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &textInputLayout_);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating the input layout!");
		return false;
	}

	//ID3DBlob* psBuffer = 0;
	psBuffer = 0;

	compileResult = CompileD3DShader("TextTextureMap.fx", "PS_Main", "ps_4_0", &psBuffer);

	if (compileResult == false)
	{
		DXTRACE_MSG("Error compiling pixel shader!");
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &textTextureMapPS_);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Error creating pixel shader!");
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_, "fontEX.png", 0, 0, &textColorMap_, 0);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}

	D3D11_SAMPLER_DESC textColorMapDesc;
	ZeroMemory(&textColorMapDesc, sizeof(textColorMapDesc));
	textColorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	textColorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	textColorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textColorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	textColorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	textColorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = d3dDevice_->CreateSamplerState(&textColorMapDesc, &textColorMapSampler_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create color map sampler state!");
		return false;
	}

	D3D11_BUFFER_DESC textVertexDesc;
	ZeroMemory(&textVertexDesc, sizeof(textVertexDesc));
	textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	const int sizeOfSprite = sizeof(TextVertexPos) * 6;
	const int maxLetters = 24;

	textVertexDesc.ByteWidth = sizeOfSprite * maxLetters;

	d3dResult = d3dDevice_->CreateBuffer(&textVertexDesc, 0, &textVertexBuffer_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// ------------------------------ LOADING CHARACTER MODELS ------------------------------

	// ---------- LOADING PLAYERS ----------
	Player1.setPosition({ -10.0f, 0.0f, 0.0f });
	Player2.setPosition({ 10.0f, 0.0f, 0.0f });

	camera_.snapPosition(&Player1, &Player2);

	//============ WOLF ============
	if (!Wolf_M.Init("PlayerModels/Antonina_Wolf/Idle/Wolf_IdleF1.obj", d3dResult, d3dDevice_)) return false;
	if (!Wolf_T.Init("PlayerModels/Antonina_Wolf/Wolf_Diff_Eye.png", d3dResult, d3dDevice_)) return false;

	if (!WolfIdle.Load("PlayerModels/Antonina_Wolf/Idle/Wolf_IdleF", 8, d3dResult, d3dDevice_)) return false; // working
	if (!WolfWalk.Load("PlayerModels/Antonina_Wolf/Walk/Wolf_WalkF", 8, d3dResult, d3dDevice_)) return false; // working
	if (!WolfDamaged.Load("PlayerModels/Antonina_Wolf/Damage/Wolf_HitF", 8, d3dResult, d3dDevice_)) return false; // working
	if (!WolfDeath.Load("PlayerModels/Antonina_Wolf/Death/Wolf_DyingF", 8, d3dResult, d3dDevice_)) return false; // working - fast
	if (!WolfAttack.Load("PlayerModels/Antonina_Wolf/Attack/Wolf_AttackF", 8, d3dResult, d3dDevice_)) return false; // working - fast


	//============ ROBOT ============
	if (!Robot_M.Init("PlayerModels/Cameron_Robot/Idle/RobotIdle1.obj", d3dResult, d3dDevice_)) return false;
	if (!Robot_T.Init("PlayerModels/Cameron_Robot/RobotUVforModel.png", d3dResult, d3dDevice_)) return false;

	if (!RobotWalk.Load("PlayerModels/Cameron_Robot/Walk/RobotWalk", 8, d3dResult, d3dDevice_)) return false; // working
	if (!RobotDeath.Load("PlayerModels/Cameron_Robot/Death/RobotDeath", 8, d3dResult, d3dDevice_)) return false; // working
	if (!RobotAttack.Load("PlayerModels/Cameron_Robot/Attack/RobotAttack", 8, d3dResult, d3dDevice_)) return false; // working
	if (!RobotDamaged.Load("PlayerModels/Cameron_Robot/Damage/RobotDamage", 8, d3dResult, d3dDevice_)) return false; // working
	if (!RobotIdle.Load("PlayerModels/Cameron_Robot/Idle/RobotIdle", 8, d3dResult, d3dDevice_)) return false; // normals not working

	//============ KREMIT ============
	if (!Kremit_M.Init("PlayerModels/Matthew_Kremit/Idle/Kremit_idle_1.obj", d3dResult, d3dDevice_)) return false;
	if (!Kremit_T.Init("PlayerModels/Matthew_Kremit/Kremit_Texture.png", d3dResult, d3dDevice_)) return false;

	if (!KremitIdle.Load("PlayerModels/Matthew_Kremit/Idle/Kremit_idle_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!KremitWalk.Load("PlayerModels/Matthew_Kremit/Walk/Kremit_Walking_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!KremitDeath.Load("PlayerModels/Matthew_Kremit/Death/Kremit_Death_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!KremitAttack.Load("PlayerModels/Matthew_Kremit/Attack/Kremit_Attack_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!KremitDamaged.Load("PlayerModels/Matthew_Kremit/Damage/Kremit_Damaged_", 8, d3dResult, d3dDevice_)) return false; // working
	
	//============ ZOMBIE ============
	if (!Zombie_M.Init("PlayerModels/Nathan_ExoSuit/Idle/Exo_Suit_Idle_01.obj", d3dResult, d3dDevice_)) return false;
	if (!Zombie_T.Init("PlayerModels/Nathan_ExoSuit/CorpseBodTexture2.png", d3dResult, d3dDevice_)) return false;

	if (!ZombieIdle.Load("PlayerModels/Nathan_ExoSuit/Idle/Exo_Suit_Idle_0", 8, d3dResult, d3dDevice_)) return false; //working
	if (!ZombieWalk.Load("PlayerModels/Nathan_ExoSuit/Walk/Exo_Suit_Walk_0", 8, d3dResult, d3dDevice_)) return false; // working
	if (!ZombieDeath.Load("PlayerModels/Nathan_ExoSuit/Death/Exo_Suit_Death_0", 8, d3dResult, d3dDevice_)) return false; // working
	if (!ZombieDamaged.Load("PlayerModels/Nathan_ExoSuit/Damage/Exo_suit_Hit_0", 8, d3dResult, d3dDevice_)) return false; // working
	if (!ZombieAttack.Load("PlayerModels/Nathan_Exosuit/Attack/Exo_Suit_Punching_0", 8, d3dResult, d3dDevice_)) return false; // working

	//============ ALIEN ============
	if (!Alien_M.Init("PlayerModels/Lucy_Alien/Idle/Alien_Idle_1.obj", d3dResult, d3dDevice_)) return false;
	if (!Alien_T.Init("PlayerModels/Lucy_Alien/CharacterTexture.png", d3dResult, d3dDevice_)) return false;

	if (!AlienIdle.Load("PlayerModels/Lucy_Alien/Idle/Alien_Idle_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!AlienWalk.Load("PlayerModels/Lucy_Alien/Walk/Alien_Walk_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!AlienDeath.Load("PlayerModels/Lucy_Alien/Death/Alien_Dying_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!AlienAttack.Load("PlayerModels/Lucy_Alien/Attack/Alien_Attack_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!AlienDamaged.Load("PlayerModels/Lucy_Alien/Damage/Alien_GettingHit_", 8, d3dResult, d3dDevice_)) return false; // working

	//============ SKINNY ============
	if (!Skinny_M.Init("PlayerModels/Valdas_Skinny/Skinny_Idle_1.obj", d3dResult, d3dDevice_)) return false;
	if (!Skinny_T.Init("PlayerModels/Valdas_Skinny/UV17.png", d3dResult, d3dDevice_)) return false;

	if (!SkinnyIdle.Load("PlayerModels/Valdas_Skinny/Idle/Skinny_Idle_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!SkinnyWalk.Load("PlayerModels/Valdas_Skinny/Walk/Skinny_Walking_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!SkinnyDeath.Load("PlayerModels/Valdas_Skinny/Death/Skinny_Death_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!SkinnyAttack.Load("PlayerModels/Valdas_Skinny/Attack/Skinny_Punch_", 8, d3dResult, d3dDevice_)) return false; // working
	if (!SkinnyDamaged.Load("PlayerModels/Valdas_Skinny/Damage/Skinny_Gethit_", 8, d3dResult, d3dDevice_)) return false; // working

	if (Player1.IsPaused())
	{
		Player1.TogglePause();
	}
	if (Player2.IsPaused())
	{
		Player2.TogglePause();
	}

	// SETTING THE ANIMATIONS!!!!

	Player1.setIsAnimated(true);
	Player1.setAnimation("idle");
	Player1.setFPS(8 / 1.0f); // number of frames / the length of animation in seconds

	/*if (!Player1.IsReversed())
	{
		Player1.ToggleReverse();
	}*/

	Player2.setIsAnimated(true);
	Player2.setAnimation("idle");
	Player2.setFPS(8 / 1.0f);

	float mult = 140.0f;
	// ---------- LOADING OBJECTS ----------

	if (!SkyBoxMesh.Init("GameObjects/Skybox1.obj", d3dResult, d3dDevice_)) return false;
	if (!SkyBoxTexture.Init("GameObjects/SkyBoxTextureSpace.png", d3dResult, d3dDevice_)) return false;
	SkyBox.setMesh(&SkyBoxMesh);
	SkyBox.setTexture(&SkyBoxTexture);
	SkyBox.setPosition({ 0.0f, 0.0f, 0.0f });

	if (!DomeMesh.Init("GameObjects/Dome/Dome.obj", d3dResult, d3dDevice_)) return false;
	if (!DomeTexture.Init("GameObjects/Dome/DomeTexture.png", d3dResult, d3dDevice_)) return false;
	DomeObj.setMesh(&DomeMesh);
	DomeObj.setTexture(&DomeTexture);
	DomeObj.setPosition({ 0.0f, 0.1f, 0.0f });

	if (!DoorMesh.Init("GameObjects/Door/DoorModel.obj", d3dResult, d3dDevice_)) return false;
	if (!DoorTexture.Init("GameObjects/Door/DoorUVClean.png", d3dResult, d3dDevice_)) return false;
	Door.setMesh(&DoorMesh);
	Door.setTexture(&DoorTexture);

	XMVECTOR temp = XMVectorSet(100.0f, 0.0f, 100.0f, 0.0f);
	temp = XMVector3Normalize(temp) * 160.0f;
	Door.setPosition({ XMVectorGetX(temp),  XMVectorGetY(temp), XMVectorGetZ(temp) });
	Door.setDirection(temp * -1.0f);

	if (!CabinetMesh.Init("GameObjects/Props/Cabinet01.obj", d3dResult, d3dDevice_)) return false;
	if (!CabinetTexture.Init("GameObjects/Props/Cabinet.png", d3dResult, d3dDevice_)) return false;
	Cabinet.setMesh(&CabinetMesh);
	Cabinet.setTexture(&CabinetTexture);

	temp = XMVectorSet(70.0f, 0.0f, 100.0f, 0.0f);
	temp = XMVector3Normalize(temp) * mult;
	Cabinet.setPosition({ XMVectorGetX(temp),  XMVectorGetY(temp), XMVectorGetZ(temp) });
	Cabinet.setDirection(temp * -1.0f);

	if (!ContainerMesh.Init("GameObjects/Props/Container.obj", d3dResult, d3dDevice_)) return false;
	if (!ContainerTexture.Init("GameObjects/Props/ContainerUVColour.png", d3dResult, d3dDevice_)) return false;
	Container.setMesh(&ContainerMesh);
	Container.setTexture(&ContainerTexture);

	temp = XMVectorSet(80.0f, 0.0f, 100.0f, 0.0f);
	temp = XMVector3Normalize(temp) * mult;

	Container.setPosition({ XMVectorGetX(temp),  XMVectorGetY(temp), XMVectorGetZ(temp) });
	Container.setDirection(temp * -1.0f);

	if (!CanisterMesh.Init("GameObjects/Props/Canister_Props.obj", d3dResult, d3dDevice_)) return false;
	if (!CanisterTexture.Init("GameObjects/Props/Canisters_UV.png", d3dResult, d3dDevice_)) return false;
	Canister.setMesh(&CanisterMesh);
	Canister.setTexture(&CanisterTexture);

	temp = XMVectorSet(130.0f, 0.0f, 100.0f, 0.0f);
	temp = XMVector3Normalize(temp) * mult;

	Canister.setPosition({ XMVectorGetX(temp),  XMVectorGetY(temp), XMVectorGetZ(temp) });
	Canister.setDirection(temp * -1.0f);

	if (!HangarMesh.Init("GameObjects/Props/Exo_Hanger_Prop.obj", d3dResult, d3dDevice_)) return false;
	if (!HangarTexture.Init("GameObjects/Props/Hanger_Colours_Flipped.png", d3dResult, d3dDevice_)) return false;
	Hangar.setMesh(&HangarMesh);
	Hangar.setTexture(&HangarTexture);

	temp = XMVectorSet(150.0f, 0.0f, 100.0f, 0.0f);
	temp = XMVector3Normalize(temp) * mult;

	Hangar.setPosition({ XMVectorGetX(temp),  XMVectorGetY(temp), XMVectorGetZ(temp) });
	Hangar.setDirection(temp * -1.0f);

	/*if (enter pressed on bob)
	{
		Player1.setMesh(& Player1Mesh)
			set texture
			set animations*/
	//Player1.SetCharacter(WOLF);
	/*}*/
	// ------------------------------ END ------------------------------

	D3D11_BUFFER_DESC vertexDesc;

	D3D11_SUBRESOURCE_DATA resourceData;

	/*VertexPos terrainVertices[] =
	{
		{ XMFLOAT3(100.0f, 0.0f , 100.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f , 0.0f) },
		{ XMFLOAT3(100.0f, 0.0f , -100.0f), XMFLOAT2(1.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
		{ XMFLOAT3(-100.0f , -0.0f , -100.0f), XMFLOAT2(0.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },

		{ XMFLOAT3(-100.0f , -0.0f , -100.0f), XMFLOAT2(0.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
		{ XMFLOAT3(-100.0f ,  0.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
		{ XMFLOAT3(100.0f,  0.0f, 100.0f), XMFLOAT2(1.0f, 0.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
	};*/
	TextVertexPos terrainVertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, 0.5f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.5f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.5f),	XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 1.0f, 0.5f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f ,-1.0f, 0.5f),	XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.5f),	XMFLOAT2(0.0f, 1.0f) },
	};

	//D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 6;

	//D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = terrainVertices;

	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBufferTerrain_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}


	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_,
		//"decal.dds", 0, 0, &colorMap_, 0 );
		//"41.jpg", 0, 0, &colorMap_, 0 );
		TERRAIN_TEXTURE_NAME, 0, 0, &terrainColorMap_, 0);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}

	//===================== BLOODY QUAD!!! ================

	D3D11_BUFFER_DESC vertexDesc1;

	D3D11_SUBRESOURCE_DATA resourceData1;

	VertexPos quadVertices[] =
	{

		{ XMFLOAT3(10.0f, 0.0f , 10.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f , 0.0f) },
	{ XMFLOAT3(10.0f, 0.0f , -10.0f), XMFLOAT2(1.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
	{ XMFLOAT3(-10.0f,0.0f , -10.0f), XMFLOAT2(0.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },

	{ XMFLOAT3(-10.0f ,0.0f , -10.0f), XMFLOAT2(0.0f, 1.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
	{ XMFLOAT3(-10.0f , 0.0f, 10.0f), XMFLOAT2(0.0f, 0.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
	{ XMFLOAT3(10.0f,  0.0f, 10.0f), XMFLOAT2(1.0f, 0.0f) , XMFLOAT3(0.0f, 1.0f , 0.0f) },
	};

	//D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc1, sizeof(vertexDesc1));
	vertexDesc1.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc1.ByteWidth = sizeof(VertexPos) * 6;

	//D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData1, sizeof(resourceData1));
	resourceData1.pSysMem = quadVertices;

	d3dResult = d3dDevice_->CreateBuffer(&vertexDesc1, &resourceData1, &vertexBufferQuad_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}


	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_, QUAD_TEXTURE_NAME, 0, 0, &quadColorMap_, 0);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}


	//=====================================================

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//colorMapDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = d3dDevice_->CreateSamplerState(&colorMapDesc, &colorMapSampler_);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create color map sampler state!");
		return false;
	}


	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &viewCB_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &projCB_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &worldCB_);

	if (FAILED(d3dResult))
	{
		return false;
	}

	constDesc.ByteWidth = sizeof(XMFLOAT4);

	d3dResult = d3dDevice_->CreateBuffer(&constDesc, 0, &camPosCB_);

	if (FAILED(d3dResult))
	{
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*		D3D11_BLEND_DESC blendDesc2;
	ZeroMemory( &blendDesc2, sizeof( blendDesc2 ) );
	blendDesc2.RenderTarget[0].BlendEnable = TRUE;

	blendDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	d3dDevice_->CreateBlendState( &blendDesc2, &alphaBlendState_ );
	//d3dDevice_->CreateBlendState( &BlendState, &alphaBlendState_ );
	d3dContext_->OMSetBlendState( alphaBlendState_, blendFactor, 0xFFFFFFFF );*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(d3dResult))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(d3dResult))
	{
		return false;
	}

	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.01f, 10000.0f);
	projMatrix_ = XMMatrixTranspose(projMatrix_);

	camera_.SetDistance(12.0f, 4.0f, 20.0f);

	return true;
}


void ModelsDemo::UnloadContent()
{
	if (colorMapSampler_) colorMapSampler_->Release();
	if (textColorMapSampler_) textColorMapSampler_->Release();

	//QUAD
	if (quadColorMap_) quadColorMap_->Release();

	if (terrainColorMap_) terrainColorMap_->Release();
	if (textColorMap_) textColorMap_->Release();
	if (textureMapVS_) textureMapVS_->Release();
	if (textTextureMapPS_) textTextureMapPS_->Release();
	if (textTextureMapVS_) textTextureMapVS_->Release();
	if (textureMapPS_) textureMapPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (textInputLayout_) textInputLayout_->Release();



	if (textVertexBuffer_) textVertexBuffer_->Release();
	if (viewCB_) viewCB_->Release();
	if (projCB_) projCB_->Release();
	if (worldCB_) worldCB_->Release();
	if (camPosCB_) camPosCB_->Release();

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	// unloading models
	Player1Mesh.Unload();
	Player1Texture.unloadTexture();

	Player2Mesh.Unload();
	Player2Texture.unloadTexture();

	Zombie_T.unloadTexture();
	Zombie_M.Unload();

	Wolf_T.unloadTexture();
	Wolf_M.Unload();

	Kremit_T.unloadTexture();
	Kremit_M.Unload();

	Robot_T.unloadTexture();
	Robot_M.Unload();

	Alien_T.unloadTexture();
	Alien_M.Unload();

	SkyBoxMesh.Unload();
	SkyBoxTexture.unloadTexture();

	DomeMesh.Unload();
	DomeTexture.unloadTexture();

	DoorMesh.Unload();
	DoorTexture.unloadTexture();

	CabinetMesh.Unload();
	CabinetTexture.unloadTexture();

	ContainerMesh.Unload();
	ContainerTexture.unloadTexture();

	CanisterMesh.Unload();
	CanisterTexture.unloadTexture();

	HangarMesh.Unload();
	HangarTexture.unloadTexture();

	colorMapSampler_ = 0;
	textColorMapSampler_ = 0;
	//colorMap1_ = 0;
	//colorMap2_ = 0;

	//QUAD
	quadColorMap_ = 0;

	terrainColorMap_ = 0;
	textColorMap_ = 0;
	textureMapVS_ = 0;
	textureMapPS_ = 0;
	textTextureMapVS_ = 0;
	textTextureMapPS_ = 0;
	inputLayout_ = 0;
	textInputLayout_ = 0;

	//vertexBuffer1_ = 0;
	//vertexBuffer2_ = 0;

	textVertexBuffer_ = 0;
	viewCB_ = 0;
	projCB_ = 0;
	worldCB_ = 0;
}

void ModelsDemo::TurnOnAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}


void ModelsDemo::TurnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnZBufferOn()
{
	d3dContext_->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}


void ModelsDemo::TurnZBufferOff()
{
	d3dContext_->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void ModelsDemo::SetGameState(GameStates state)
{
	gameState_ = state;
}
GameStates ModelsDemo::GetGameState()
{
	return gameState_;
}
void ModelsDemo::Update(float dt)
{
	//camera_.ApplyZoom(.01);
	//camera_.ApplyRotation(0.001,0.001);
	if (gameState_ == RUN)
	{
		XMVECTOR cameraPosition = XMLoadFloat3(&camera_.GetPosition());
		
		// following each other
		
		//models[0].moveForward(2 * dt);
		Tank3Speed = 2 * dt;

		// Updating Players
		Player1.update(dt, Player2.getPosition());
		Player2.update(dt, Player1.getPosition());
	}
	else
	{
		Tank3Speed = 0;
	}


	float moveSpeed = 0.01f;
	float moveSpeed2 = 20.0f;
	float zoom = 0.0;
	float xRotation = 0.0;
	float yRotation = 0.0;
	wait -= dt;
	attack_time1 -= dt;
	attack_time2 -= dt;
	
	//============================= STATES CONTROLS !!! ==================
	

	if (gameState_ == START_MENU )
	{

		if (!(keystate[DIK_RETURN] & 0x80) && ((keyPrevState[DIK_RETURN] & 0x80)) && (wait <= 0))
		{
			gameState_ = SELECTION;
			wait = 1.0;
		}
	}

	if (gameState_ == SELECTION)
	{

		if ((keystate[DIK_RETURN] & 0x80) && (wait <= 0))
		{
			gameState_ = RUN;
			wait = 0.1f;
		}
		//=========== Select Player 1 ===================
		if (
			(!(keystate[DIK_S] & 0x80) && (keyPrevState[DIK_S] & 0x80))
			)
		{
			P1charSelection++;
		}
		if (
			(!(keystate[DIK_W] & 0x80) && (keyPrevState[DIK_W] & 0x80))
			)

		{
			P1charSelection--;
		}
		//=========== Select Player 2 ================
		if (
			(!(keystate[DIK_DOWN] & 0x80) && (keyPrevState[DIK_DOWN] & 0x80))
			)
		{
			P2charSelection ++;
		}
		if (
			(!(keystate[DIK_UP] & 0x80) && (keyPrevState[DIK_UP] & 0x80))
			)

		{
			P2charSelection --;
		}
	}

	if (gameState_ == PAUSED)
	{

		if (!(keystate[DIK_ESCAPE] & 0x80) && (keyPrevState[DIK_ESCAPE] & 0x80) && (wait <= 0))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if ((keystate[DIK_RETURN] & 0x80) && (pauseMenuSelection == RETURN) && (wait <= 0))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if ((keystate[DIK_RETURN] & 0x80) && (pauseMenuSelection == PLAY_MOVIE) && (wait <= 0))
		{
			//PostQuitMessage(0);
			gameState_ = INTRO_MOVIE_REPLAY;
		}

		if ((keystate[DIK_RETURN] & 0x80) && (pauseMenuSelection == QUIT) && (wait <= 0))
		{
			gameState_ = START_MENU;
			wait = 0.1;
		}

		if ((!(keystate[DIK_RETURN] & 0x80) && (keyPrevState[DIK_RETURN] & 0x80) && (wait <= 0))
			&& (pauseMenuSelection == FPS))
		{
			displayFPS = !displayFPS;
		}

		if (
			(!(keystate[DIK_DOWN] & 0x80) && (keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80) && (keyPrevState[DIK_S] & 0x80))
			)
		{
			pauseMenuSelection++;
		}
		if (
			(!(keystate[DIK_UP] & 0x80) && (keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80) && (keyPrevState[DIK_W] & 0x80))
			)

		{
			pauseMenuSelection--;
		}

	}

	//========================== GETTING THE INPUTS / COLLISIONS  ==============================
	//moveSpeed=0.001f;
	float moveLeftRight = 0.0;
	float moveBackForward = 0.0;
	float moveUpDown = 0.0;

	float radius1 = 2.0f;
	Player1.setRadius(radius1);

	float radius2 = 2.0f;
	Player2.setRadius(radius2);

	
	float dist = Player1.getRadius() + Player2.getRadius() ;
	Player2.SetPlayer(false);


	if (gameState_ == RUN)
	{

		//movement = false;
		//// =========== MODEL MOVEMENTS 1 =================

		if ((keystate[DIK_W] & 0x80))
		{
			AnimatioState = 1;
			float tempx = m_x1;
			tempx -= moveSpeed2 * dt;
			bool right = false;
			Player1.moveRight(dt, right);
		}
		else if ((keystate[DIK_S] & 0x80))
		{
			AnimatioState = 1;
			float tempx = m_x1;
			tempx += moveSpeed2 * dt;
			//m_x1 += moveSpeed2 * dt;
			bool right = true;
			Player1.moveRight(dt, right);
		}
		else if ((keystate[DIK_A] & 0x80))
		{
			AnimatioState = 1;
			if (!Player1.IsReversed())
			{
				Player1.ToggleReverse();
			}
			float tempx = m_z1;
			tempx -= moveSpeed2 * dt;
			Player1.moveForward(dt, true);
				
		}
		else if ((keystate[DIK_D] & 0x80))
		{
			AnimatioState = 1;
			float tempx = m_z1 * (float)200.0f;
			tempx += moveSpeed2 * dt;
			if (collision.colliding(Player1.getPosition(), Player2.getPosition(), dt, dist) != 1)
			{
				//m_z1 += moveSpeed2 * dt;
				bool forward = false;
				Player1.moveForward(dt, forward);
					
			}
		}
		else 
		{
			AnimatioState = 0;
		}
		if ((keystate[DIK_F] & 0x80) && (attack_time1 <= 0))
		{
			attack = true;
			AnimatioState = 2;

		}

		//
		//if ((keystate[DIK_F] & 0x80) && (attack_time1 <= 0))
		//{
		//	attack = true;
		//	Player1.setAnimation("attack");
		//	Player1.setFPS(8 / 1.0f);
		//	Player1.SetCurrentFrame(0);

		//	attack_time1 = 1.0f;
		//}
		//if (attack == true) 
		//{

		//	if (Player1.GetCurremtFrame() == 7) 
		//	{
		//		attack == false;
	
		//	}
		//}
		//else if (attack == false) 
		//{
		//	Player1.setAnimation("idle");
		//	Player1.setFPS(8 / 1.0f);
		//}
		/*if ((keystate[DIK_F] & 0x80)&& collision.colliding(Player1.getPosition(), Player2.getPosition(), dt, dist) == 1 && (attack_time1 <= 0))
		{
			Player2.ApplyDamage(Player1.GetAttack());
			if (!Player1.GetAlive()) 
			{
				gameState_ = START_MENU;
			}

			attack_time1 = 0.1f;
		}*/
		switch (AnimatioState) 
		{
		case 0:
			Player1.setAnimation("idle");
			Player1.setFPS(8 / 1.0f);
			if (PrevAnimState != 0) 
			{
				Player1.SetCurrentFrame(0);
			}
			break;
		case 1:
			Player1.setAnimation("walk");
			Player1.setFPS(8 / 1.0f);
			if (PrevAnimState != 1)
			{
				Player1.SetCurrentFrame(0);
			}
			break;
		case 2:
			Player1.setAnimation("attack");
			Player1.setFPS(8 / 1.0f);
			if (PrevAnimState != 2)
			{
				Player1.SetCurrentFrame(0);
			}

			break;
		//default:
		//	Player1.setAnimation("idle");
		//	Player1.setFPS(8 / 1.0f);
		//	if (PrevAnimState != 0)
		//	{
		//		Player1.SetCurrentFrame(0);
		//	}

		}
		PrevAnimState = AnimatioState;
		// =========== MODEL MOVEMENTS 2 =================
		if ((keystate[DIK_DOWN] & 0x80))
		{
			float tempx = m_x2;
			tempx -= moveSpeed2 * dt;
			bool right = false;
			Player2.moveRight(dt, right);
		}
		if ((keystate[DIK_UP] & 0x80))
		{
			float tempx = m_x2;
			tempx += moveSpeed2 * dt;
			bool right = true;
			Player2.moveRight(dt, right);
		}

		if ((keystate[DIK_RIGHT] & 0x80))
		{
			float tempx = m_z2;
			tempx -= moveSpeed2 * dt;
			Player2.moveForward(dt, true);	  
		}
		if ((keystate[DIK_LEFT] & 0x80))
		{
			float tempx = m_z2;
			tempx += moveSpeed2 * dt;
			if (collision.colliding(Player2.getPosition(), Player1.getPosition(), dt, dist) != 1)
			{
				bool forward = false;
				Player2.moveForward(dt, forward);
				
			}
		}

		if ((keystate[DIK_NUMPAD4] & 0x80) && (attack_time2 <= 0))
		{

			Player2.setAnimation("attack");
			Player2.setFPS(8 / 1.0f);
			Player2.SetCurrentFrame(0);



			attack_time2 = 1.0f;
		}
		if (Player2.GetCurremtFrame() == 7)
		{
			Player2.setAnimation("idle");
			Player2.setFPS(8 / 1.0f);
		}
		if ((keystate[DIK_NUMPAD4] & 0x80)&& collision.colliding(Player2.getPosition(), Player1.getPosition(), dt, dist) == 1 )
		{
			Player1.ApplyDamage(Player2.GetAttack());
			attack_time2 = 0.1f;
		}
//=================== AM I STILL ALIVE??? ==================
		if (!Player1.GetAlive())
		{
			gameState_ = START_MENU;
		}

		else if (!Player2.GetAlive())
		{
			gameState_ = START_MENU;
		}
		//============================== CAMERA  =====================================

		if (camera_.getControllable())
		{
			if ((keystate[DIK_J] & 0x80))
			{
				//moveLeftRight -= moveSpeed;
				moveLeftRight -= moveSpeed2 * dt;

			}
			if ((keystate[DIK_L] & 0x80))
			{
				//moveLeftRight += moveSpeed;
				moveLeftRight += moveSpeed2 * dt;

			}

			if ((keystate[DIK_K] & 0x80))
			{
				//moveBackForward  -= moveSpeed;
				moveBackForward -= dt * moveSpeed2;
			}
			if ((keystate[DIK_I] & 0x80))
			{
				//moveBackForward  += moveSpeed;
				moveBackForward += dt * moveSpeed2;
			}

			if ((keystate[DIK_U] & 0x80))
			{
				//moveBackForward  -= moveSpeed;
				moveUpDown -= dt * moveSpeed2;
			}
			if ((keystate[DIK_O] & 0x80))
			{
				//moveBackForward  += moveSpeed;
				moveUpDown += dt * moveSpeed2;
			}
		}	

		if (keystate[DIK_SUBTRACT] & 0x80)
		{
			zoom += moveSpeed * 2;
		}
		if (keystate[DIK_ADD] & 0x80)
		{
			zoom -= moveSpeed * 2;
		}

		if (!(keystate[DIK_ESCAPE] & 0x80) && (keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
		}

		if (camera_.getControllable())
		{
			if ((mouseCurrState.lX != mousePrevState.lX) || (mouseCurrState.lY != mousePrevState.lY))
			{
				yRotation += mousePrevState.lX * 0.005f;
				//yRotation += mousePrevState.lX * dt*2.0f;//mouse movement already based on time

				xRotation += mouseCurrState.lY * 0.005f;
				//xRotation += mouseCurrState.lY * dt*2.0f;/mouse movement already based on time


				if (xRotation>XM_PI / 8)
				{
					xRotation = XM_PI / 8;
				}

				if (xRotation<-(XM_PI / 8))
				{
					xRotation = -(XM_PI / 8);
				}

				mousePrevState = mouseCurrState;
			}

			camera_.Move(moveLeftRight, moveBackForward, moveUpDown);
			//camera_.setControllable(false);
			camera_.ApplyRotation(xRotation, yRotation);

		}

		camera_.ApplyZoom(zoom);
		camera_.update(dt, &Player1, &Player2);
	}

	memcpy(&prevController1State_, &controller1State_, sizeof(XINPUT_STATE));
}

bool ModelsDemo::DrawString(char* message, float startX, float startY)
{
	//float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );


	// Size in bytes for a single sprite.
	const int sizeOfSprite = sizeof(TextVertexPos) * 6;

	// Demo's dynamic buffer setup for max of 24 letters.
	const int maxLetters = 24;

	int length = strlen(message);

	// Clamp for strings too long.
	if (length > maxLetters)
		length = maxLetters;

	// Char's width on screen.
	float charWidth = 32.0f / 800.0f;

	// Char's height on screen.
	float charHeight = 32.0f / 640.0f;

	// Char's texel width.
	//float texelWidth = 32.0f / 864.0f;
	float texelWidth = 32.0f / 3072.0f;

	// verts per-triangle (3) * total triangles (2) = 6.
	const int verticesPerLetter = 6;

	D3D11_MAPPED_SUBRESOURCE mapResource;
	HRESULT d3dResult = d3dContext_->Map(textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to map resource!");
		return false;
	}

	// Point to our vertex buffer's internal data.
	TextVertexPos *spritePtr = (TextVertexPos*)mapResource.pData;

	const int indexSpace = static_cast<char>(' ');
	const int indexA = static_cast<char>('A');
	const int indexZ = static_cast<char>('Z');
	const int indexSquare = static_cast<char>(127);

	for (int i = 0; i < length; ++i)
	{
		float thisStartX = startX + (charWidth * static_cast<float>(i));
		float thisEndX = thisStartX + charWidth;
		float thisEndY = startY + charHeight;

		spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
		spritePtr[1].pos = XMFLOAT3(thisEndX, startY, 1.0f);
		spritePtr[2].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[3].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
		spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

		int texLookup = 0;
		int letter = static_cast<char>(message[i]);

		//if( letter < indexA || letter > indexZ )
		if (letter < indexSpace || letter > indexSquare)
		{
			// Grab one index past Z, which is a blank space in the texture.
			// texLookup = ( indexZ - indexA ) + 1;
			texLookup = indexSquare;
		}
		else
		{
			// A = 0, B = 1, Z = 25, etc.
			//texLookup = ( letter - indexA );
			texLookup = (letter - indexSpace);
		}

		float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
		float tuEnd = tuStart + texelWidth;

		spritePtr[0].tex0 = XMFLOAT2(tuEnd, 0.0f);
		spritePtr[1].tex0 = XMFLOAT2(tuEnd, 1.0f);
		spritePtr[2].tex0 = XMFLOAT2(tuStart, 1.0f);
		spritePtr[3].tex0 = XMFLOAT2(tuStart, 1.0f);
		spritePtr[4].tex0 = XMFLOAT2(tuStart, 0.0f);
		spritePtr[5].tex0 = XMFLOAT2(tuEnd, 0.0f);

		spritePtr += 6;
	}

	d3dContext_->Unmap(textVertexBuffer_, 0);
	d3dContext_->Draw(6 * length, 0);

	return true;
}

void ModelsDemo::Render()
{
	if (d3dContext_ == 0)
		return;

	/////////////////////debug frames per second//////////////////
	/*int a=12348,b=409786;
	int x =0;
	while(x<10000000)
	{
	a=a/b;
	x++;
	}*/
	/////////////////////////////////////////////////

	//float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	float clearColor[4] = { 0.7f, 0.8f, 1.0f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	//======================= MENU !!! =======================
	if (gameState_ == START_MENU)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &terrainColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);

		d3dContext_->Draw(6, 0);
		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}

	//============= Character Selection =============

	if (gameState_ == SELECTION)
	{
		/*TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);*/
		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		Player1.SetAlive(true);
		Player2.SetAlive(true);

		DrawString("PRESS ENTER to SELECT CHARACTER", -0.4f, 0.0f);

		//=============== Player 1 ==============
		if (P1charSelection == WOLF)
		{
			DrawString("->WOLF<-", -0.63f, -0.1f);
			Player1.SetCharacter(WOLF);
			Player1.setMesh(&Wolf_M);
			Player1.setTexture(&Wolf_T);

			Player1.setIdleMesh(&WolfIdle);
			Player1.setWalkMesh(&WolfWalk);
			Player1.setDeathMesh(&WolfDeath);
			Player1.setAttackMesh(&WolfAttack);
			Player1.setDamagedMesh(&WolfDamaged);
		}
		else
		{
			DrawString("WOLF", -0.55f, -0.1f);
		}
		if (P1charSelection == ROBOT)
		{
			DrawString("->ROBOT<-", -0.63f, -0.2f);

			Player1.SetCharacter(ROBOT);
			Player1.setMesh(&Robot_M);
			Player1.setTexture(&Robot_T);
			Player1.setDamagedMesh(&RobotDamaged);

			Player1.setIdleMesh(&RobotIdle);
			Player1.setWalkMesh(&RobotWalk);
			Player1.setDeathMesh(&RobotDeath);
			Player1.setAttackMesh(&RobotAttack);
			Player1.setDamagedMesh(&RobotDamaged);
		}
		else
		{
			DrawString("ROBOT", -0.55f, -0.2f);
		}

		if (P1charSelection == KREMIT)
		{
			DrawString("->KREMIT<-", -0.63f, -0.3f);

			Player1.SetCharacter(KREMIT);
			Player1.setMesh(&Kremit_M);
			Player1.setTexture(&Kremit_T);

			Player1.setIdleMesh(&KremitIdle);
			Player1.setWalkMesh(&KremitWalk);
			Player1.setDeathMesh(&KremitDeath);
			Player1.setAttackMesh(&KremitAttack);
			Player1.setDamagedMesh(&KremitDamaged);
		}
		else
		{
			DrawString("KREMIT", -0.55f, -0.3f);
		}

		if (P1charSelection == ZOMBIE)
		{
			Player1.SetCharacter(ZOMBIE);
			DrawString("->ZOMBIE<-", -0.63f, -0.4f);
			Player1.setMesh(&Zombie_M);
			Player1.setTexture(&Zombie_T);

			Player1.setIdleMesh(&ZombieIdle);
			Player1.setWalkMesh(&ZombieWalk);
			Player1.setDeathMesh(&ZombieDeath);
			Player1.setAttackMesh(&ZombieAttack);
			Player1.setDamagedMesh(&ZombieDamaged);
		}
		else
		{
			DrawString("ZOMBIE", -0.55f, -0.4f);
		}
		if (P1charSelection == ALIEN)
		{
			DrawString("->ALIEN<-", -0.63f, -0.5f);

			Player1.SetCharacter(ALIEN);
			Player1.setMesh(&Alien_M);
			Player1.setTexture(&Alien_T);

			Player1.setIdleMesh(&AlienIdle);
			Player1.setWalkMesh(&AlienWalk);
			Player1.setDeathMesh(&AlienDeath);
			Player1.setAttackMesh(&AlienAttack);
			Player1.setDamagedMesh(&AlienDamaged);
		}
		else
		{
			DrawString("ALIEN", -0.55f, -0.5f);
		}
		if (P1charSelection == SKINNY)
		{
			DrawString("->SKINNY<-", -0.63f, -0.6f);

			Player1.SetCharacter(SKINNY);
			Player1.setMesh(&Skinny_M);
			Player1.setTexture(&Skinny_T);

			Player1.setIdleMesh(&SkinnyIdle);
			Player1.setWalkMesh(&SkinnyWalk);
			Player1.setDeathMesh(&SkinnyDeath);
			Player1.setAttackMesh(&SkinnyAttack);
			Player1.setDamagedMesh(&SkinnyDamaged);
			
		}
		else
		{
			DrawString("SKINNY", -0.55f, -0.6f);

		}
		if (P1charSelection == PRAVEZ)
		{
			DrawString("->PRAVEZ<-", -0.63f, -0.7f);

			Player1.SetCharacter(PRAVEZ);
			Player1.setMesh(&Player1Mesh);
			Player1.setTexture(&Player1Texture);
		}
		else
		{
			DrawString("PRAVEZ", -0.55f, -0.7f);
		}

		//=============== Player 2 ==============

		if (P2charSelection == WOLF)
		{
			DrawString("->WOLF<-", 0.33f, -0.1f);

			Player2.SetCharacter(WOLF);
			Player2.setMesh(&Wolf_M);
			Player2.setTexture(&Wolf_T);

			Player2.setIdleMesh(&WolfIdle);
			Player2.setWalkMesh(&WolfWalk);
			Player2.setDeathMesh(&WolfDeath);
			Player2.setAttackMesh(&WolfAttack);
			Player2.setDamagedMesh(&WolfDamaged);
		}
		else
		{
			DrawString("WOLF", 0.41f, -0.1f);
		}
		if (P2charSelection == ROBOT)
		{
			DrawString("->ROBOT<-", 0.33f, -0.2f);

			Player2.SetCharacter(ROBOT);
			Player2.setMesh(&Robot_M);
			Player2.setTexture(&Robot_T);

			Player2.setIdleMesh(&RobotIdle);
			Player2.setWalkMesh(&RobotWalk);
			Player2.setDeathMesh(&RobotDeath);
			Player2.setAttackMesh(&RobotAttack);
			Player2.setDamagedMesh(&RobotDamaged);
		}
		else
		{
			DrawString("ROBOT", 0.41f, -0.2f);
		}

		if (P2charSelection == KREMIT)
		{
			DrawString("->KREMIT<-", 0.33f, -0.3f);

			Player2.SetCharacter(KREMIT);
			Player2.setMesh(&Kremit_M);
			Player2.setTexture(&Kremit_T);
			
			Player2.setIdleMesh(&KremitIdle);
			Player2.setWalkMesh(&KremitWalk);
			Player2.setDeathMesh(&KremitDeath);
			Player2.setAttackMesh(&KremitAttack);
			Player2.setDamagedMesh(&KremitDamaged);
		}
		else
		{
			DrawString("KREMIT", 0.41f, -0.3f);
		}

		if (P2charSelection == ZOMBIE)
		{
			DrawString("->ZOMBIE<-", 0.33f, -0.4f);

			Player2.SetCharacter(ZOMBIE);
			Player2.setMesh(&Zombie_M);
			Player2.setTexture(&Zombie_T);

			Player2.setIdleMesh(&ZombieIdle);
			Player2.setWalkMesh(&ZombieWalk);
			Player2.setDeathMesh(&ZombieDeath);
			Player2.setAttackMesh(&ZombieAttack);
			Player2.setDamagedMesh(&ZombieDamaged);
		}
		else
		{
			DrawString("ZOMBIE", 0.41f, -0.4f);
		}
		if (P2charSelection == ALIEN)
		{
			DrawString("->ALIEN<-", 0.33f, -0.5f);

			Player2.SetCharacter(ALIEN);
			Player2.setMesh(&Alien_M);
			Player2.setTexture(&Alien_T);

			Player2.setIdleMesh(&AlienIdle);
			Player2.setWalkMesh(&AlienWalk);
			Player2.setDeathMesh(&AlienDeath);
			Player2.setAttackMesh(&AlienAttack);
			Player2.setDamagedMesh(&AlienDamaged);
		}
		else
		{
			DrawString("ALIEN", 0.41f, -0.5f);
		}
		if (P2charSelection == SKINNY)
		{
			DrawString("->SKINNY<-", 0.33f, -0.6f);

			Player2.SetCharacter(SKINNY);
			Player2.setMesh(&Skinny_M);
			Player2.setTexture(&Skinny_T);

			Player2.setIdleMesh(&SkinnyIdle);
			Player2.setWalkMesh(&SkinnyWalk);
			Player2.setDeathMesh(&SkinnyDeath);
			Player2.setAttackMesh(&SkinnyAttack);
			Player2.setDamagedMesh(&SkinnyDamaged);
		}
		else
		{
			DrawString("SKINNY", 0.41f, -0.6f);
		}
		if (P2charSelection == PRAVEZ)
		{
			DrawString("->PRAVEZ<-", 0.33f, -0.7f);

			Player2.SetCharacter(PRAVEZ);
			Player2.setMesh(&Player2Mesh);
			Player2.setTexture(&Player2Texture);
		}
		else
		{
			DrawString("PRAVEZ", 0.41f, -0.7f);
		}

		/*TurnOffAlphaBlending();
		TurnZBufferOn();*/
		TurnOffAlphaBlending();
		TurnZBufferOn();

	}


	if ((gameState_ == RUN) || (gameState_ == PAUSED))
	{

		/////////////////////////////////////////geometry settings//////////////////////////////


		d3dContext_->IASetInputLayout(inputLayout_);
		//d3dContext_->IASetVertexBuffers(0, 1, model2Mesh.getVertexBuffer(), &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textureMapPS_, 0, 0);


		d3dContext_->PSSetSamplers(0, 1, &colorMapSampler_);


		//TurnZBufferOff();
		// 1st objects

		//d3dContext_->PSSetShaderResources(0, 1, model2Texture.getColorMap());
		XMMATRIX worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);

		XMMATRIX viewMat = camera_.GetViewMatrix();
		viewMat = XMMatrixTranspose(viewMat);

		XMFLOAT3 cameraPos = camera_.GetPosition();

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->UpdateSubresource(viewCB_, 0, 0, &viewMat, 0, 0);
		d3dContext_->UpdateSubresource(projCB_, 0, 0, &projMatrix_, 0, 0);
		d3dContext_->UpdateSubresource(camPosCB_, 0, 0, &cameraPos, 0, 0);

		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->VSSetConstantBuffers(1, 1, &viewCB_);
		d3dContext_->VSSetConstantBuffers(2, 1, &projCB_);
		d3dContext_->VSSetConstantBuffers(3, 1, &camPosCB_);

		XMVECTOR cameraPosition = XMLoadFloat3(&camera_.GetPosition());

		////////////////////terrain////////////////////////////////
		//d3dContext_->PSSetShaderResources(0, 1, &terrainColorMap_);
		//worldMat = XMMatrixIdentity();
		//worldMat = XMMatrixTranspose(worldMat);
		//XMMATRIX scale = XMMatrixIdentity();
		//scale = XMMatrixScaling(250.0f, 250.0f, 250.0f);
		//worldMat = scale;

		//d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		//d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		//d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferTerrain_, &stride, &offset);

		//d3dContext_->Draw(6, 0);
		//================ QUAD =========================

		d3dContext_->PSSetShaderResources(0, 1, &quadColorMap_);
		//d3dContext_->PSSetShaderResources(0, 1, &terrainColorMap_);
		worldMat = XMMatrixIdentity();
		worldMat = XMMatrixTranspose(worldMat);
		XMMATRIX scale = XMMatrixIdentity();
		scale = XMMatrixScaling(250.0f, 250.0f, 250.0f);
		worldMat = scale;

		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &worldMat, 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->IASetVertexBuffers(0, 1, &vertexBufferQuad_, &stride, &offset);

		d3dContext_->Draw(6, 0);


		// ---------- DRAWING GAME OBJECTS ----------

		// Skybox
		d3dContext_->IASetVertexBuffers(0, 1, SkyBox.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, SkyBox.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &SkyBox.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(SkyBox.getMesh()->getTotalVerts(), 0);
		

		// Door
		d3dContext_->IASetVertexBuffers(0, 1, Door.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Door.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Door.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Door.getMesh()->getTotalVerts(), 0);
#
		// Cabinet
		d3dContext_->IASetVertexBuffers(0, 1, Cabinet.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Cabinet.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Cabinet.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Cabinet.getMesh()->getTotalVerts(), 0);

		//Container
		d3dContext_->IASetVertexBuffers(0, 1, Container.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Container.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Container.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Container.getMesh()->getTotalVerts(), 0);

		// Canister
		d3dContext_->IASetVertexBuffers(0, 1, Canister.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Canister.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Canister.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Canister.getMesh()->getTotalVerts(), 0);

		// Dome

		d3dContext_->IASetVertexBuffers(0, 1, DomeObj.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, DomeObj.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &DomeObj.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(DomeObj.getMesh()->getTotalVerts(), 0);

		// Hangar
		d3dContext_->IASetVertexBuffers(0, 1, Hangar.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Hangar.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Hangar.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Hangar.getMesh()->getTotalVerts(), 0);

		// ---------- DRAWING PLAYERS ----------
		d3dContext_->IASetVertexBuffers(0, 1, Player1.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Player1.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Player1.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Player1.getMesh()->getTotalVerts(), 0);

		d3dContext_->IASetVertexBuffers(0, 1, Player2.getMesh()->getVertexBuffer(), &stride, &offset);
		d3dContext_->PSSetShaderResources(0, 1, Player2.getTexture()->getColorMap());
		d3dContext_->UpdateSubresource(worldCB_, 0, 0, &Player2.getWorldMat(), 0, 0);
		d3dContext_->VSSetConstantBuffers(0, 1, &worldCB_);
		d3dContext_->Draw(Player2.getMesh()->getTotalVerts(), 0);
	}


	if ((gameState_ == RUN) && (displayFPS == true))
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		char output[64];
		sprintf_s(output, "FPS:%d", fps_);

		DrawString(output, -0.9f, 0.83f);

		sprintf_s(output, "Frame Time:%.6f", frameTime_);

		DrawString(output, -0.9f, 0.6f);

		sprintf_s(output, "Player1 HP:%.1f", Player1.GetHealth());

		DrawString(output, -1.0f, 0.7f);

		sprintf_s(output, "Player2 HP:%.1f", Player2.GetHealth());

		DrawString(output, 0.4f, 0.7f);

		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}


	if (gameState_ == PAUSED)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		TurnZBufferOff();
		TurnOnAlphaBlending();

		stride = sizeof(TextVertexPos);
		offset = 0;

		d3dContext_->IASetInputLayout(textInputLayout_);
		d3dContext_->IASetVertexBuffers(0, 1, &textVertexBuffer_, &stride, &offset);
		d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext_->VSSetShader(textTextureMapVS_, 0, 0);
		d3dContext_->PSSetShader(textTextureMapPS_, 0, 0);
		d3dContext_->PSSetShaderResources(0, 1, &textColorMap_);
		d3dContext_->PSSetSamplers(0, 1, &textColorMapSampler_);

		DrawString("GAME PAUSED", -0.2f, 0.0f);
		if (pauseMenuSelection == RETURN)
		{
			DrawString("->Return to Game<-", -0.33f, -0.1f);
		}
		else
		{
			DrawString("Return to Game", -0.25f, -0.1f);
		}

		if (pauseMenuSelection == FPS)
		{
			if (displayFPS == true)
			{
				DrawString("->Display FPS: ON<-", -0.35f, -0.2f);
			}
			else
			{
				DrawString("->Display FPS:OFF<-", -0.35f, -0.2f);
			}
		}
		else
		{
			if (displayFPS == true)
			{
				DrawString("Display FPS: ON", -0.27f, -0.2f);
			}
			else
			{
				DrawString("Display FPS:OFF", -0.27f, -0.2f);
			}
		}

		if (pauseMenuSelection == PLAY_MOVIE)
		{
			DrawString("->Play the Movie<-", -0.33f, -0.3f);
		}
		else
		{
			DrawString("Play the Movie", -0.25f, -0.3f);
		}

		if (pauseMenuSelection == QUIT)
		{
			DrawString("->Quit the Game<-", -0.33f, -0.4f);
		}
		else
		{
			DrawString("Quit the Game", -0.25f, -0.4f);
		}


		TurnOffAlphaBlending();
		TurnZBufferOn();

		////////////////////////////////////////////////////////////////////////////////////////////////
	}


	swapChain_->Present(0, 0);
}

